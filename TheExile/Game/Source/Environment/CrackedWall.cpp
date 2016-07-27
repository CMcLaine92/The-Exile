/***********************************************
* Filename:  		CrackedWall.cpp
* Date:      		9/2/2015
* Mod. Date: 		9/2/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a cracked wall
************************************************/

#include "CrackedWall.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Particles/ParticleManager.h"
#include "../../../Engine/Core/Input/InputManager.h"
#include "../../../Engine/Particles/Flyweight.h"
#include "../../../Engine/Particles/Particle.h"
#include "../../../Engine/Particles/Emitter.h"
#include "../../../Engine/Core/Collision/Collider.h"
#include "../../../Engine/Core/Collision/AABB.h"
#include "../../../Engine/Renderer/HUDMesh.h"
#include "../../../Engine/Animation/Mesh.h"

#define OFFSET 200
#define NUM_BOMBS 4
#define FUSE_SECONDS 3.5f
CCrackedWall::CCrackedWall( string szTag, CObjectManager* objManager ) : CPrefabObject( "CrackedWall", "CrackedWall", objManager )
{
	m_bInitialized = false;
	m_bActive = false;
	m_fFuseTimer = 0.0f;

	m_cpParticleManager = new CParticleManager( objManager );
	
	CMesh* mesh = CAssetManager::GetInstance()->GetPrefabMesh("WallBomb");
	for(unsigned int i = 0; i < NUM_BOMBS; i++ )
	{
		
		m_cpIndicatorBomb.push_back( new CRenderMesh( mesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetStandardPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Failbot_Green_Bomb.dds" ) );

		m_cpIndicatorBomb[i]->GetTransparent() = true;
		m_cpIndicatorBomb[i]->GetTransparencyOverride() = 0.5f;
		GRAPHICS->AddRenderMesh( m_cpIndicatorBomb[i] );
	}

	BuildTextFrame();

#pragma region Controller Pop-Up

	vector<TVertex2D> tempVerts;
	vector<unsigned int> tempInd;

	tempInd.push_back(0);
	tempInd.push_back(1);
	tempInd.push_back(2);

	tempInd.push_back(0);
	tempInd.push_back(2);
	tempInd.push_back(3);

	tempVerts.push_back // Top Left
		({
			{ -0.4f, 0.2f, -1.0f, 1 },
			{ 0, 0 }
	});

	tempVerts.push_back // Top Right
		({
			{ 0.4f, 0.2f, -1.0f, 1 },
			{ 1, 0 }
	});

	tempVerts.push_back // Bot Right
		({
			{ 0.4f, -0.2f, -1.0f, 1 },
			{ 1, 1 }
	});

	tempVerts.push_back // Bot Left
		({
			{ -0.4f, -0.2f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_cpControllerPopUp = new CHUDMesh(tempInd, tempVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Build_Controller_PopUp.dds");
	m_cpControllerPopUp->GetRender() = false;
	m_cpControllerPopUp->GetTransparent() = true;
	TranslateLocalY(m_cpControllerPopUp->GetTransformMatrix(), 0.4f);
	GRAPHICS->AddRenderMesh(m_cpControllerPopUp);

#pragma endregion

}

void CCrackedWall::Initialize()
{
	for( size_t i = 0; i < m_cpIndicatorBomb.size(); i++ )
	{
		m_cpIndicatorBomb[i]->GetTransformMatrix() = GetWorldMatrix();
	}
	SetBombPosition();
	SetupParticle();
	m_bInitialized = true;
}

CCrackedWall::~CCrackedWall()
{
	for( size_t i = 0; i < m_cpIndicatorBomb.size(); i++ )
		delete m_cpIndicatorBomb[i];

	SAFE_DELETE(m_cpTextFrame);
	SAFE_DELETE(m_cpParticleManager);
	SAFE_DELETE(m_cpControllerPopUp);
}

void CCrackedWall::Update()
{

	if (m_cpTextFrame->GetRender())
	{
		m_cpTextFrame->GetRender() = false;
		m_cpControllerPopUp->GetRender() = false;
	}

	//for( size_t i = 0; i < m_pvColliders.size(); i++ )
	//{
	//	if( m_pvColliders[i]->GetType() == Bounds::AABB )
	//	{
	//		CAABB* pAABB = (CAABB*)m_pvColliders[i]->GetBounds();
	//		DEBUG::DrawAABBCollider( pAABB->Min(), pAABB->Max() );
	//	}
	//}

	if( !m_bInitialized )
		Initialize();

	if( m_bActive  && m_fFuseTimer > 0.0f )
	{
		m_fFuseTimer -= DELTA_TIME();
		if( m_fFuseTimer <= 0.0f )
		{
			for (size_t i = 0; i < m_pvColliders.size(); i++)
			{
				if (m_pvColliders[i]->GetType() == Bounds::AABB)
				{
					m_pvColliders[i]->SetActive(false);
				}
			}
			

			AudioSystemWwise::Get()->PostEvent( AK::EVENTS::WALLEXPLOSION, *GetPosition() );

			m_vRenderMeshes[0]->GetRender() = false;
			SetFrustumCull( false );

			for( size_t i = 0; i < m_cpIndicatorBomb.size(); i++ )
			{
				m_cpIndicatorBomb[i]->GetRender() = false;
			}

			if( m_unCurrParticleID != -1 )
				m_cpParticleManager->GetEmitter( m_vunParticleID[m_unCurrParticleID] )->ePlay();

		}
		else
		{
			string szCountDown = to_string( (int)m_fFuseTimer );
			wstring wszCount( szCountDown.begin(), szCountDown.end() );
			GRAPHICS->RenderText( wszCount.c_str(), { -250, 500 }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ .5f, .5f } );
			m_cpTextFrame->GetVertex2DArr()[1].m_fPosition[0] = .005f;
			m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[0] = .005f;

			m_cpTextFrame->GetRender() = true;
		}
	}
	m_cpParticleManager->Update();
}

void CCrackedWall::Activate( bool _audio, int curID )
{
	if( !m_bActive )
	{
		m_bActive = true;

		(curID == -1 || curID >= m_vunParticleID.size()) ? m_unCurrParticleID = -1 : m_unCurrParticleID = m_vunParticleID[curID];

		if( _audio )
		{
			m_fFuseTimer = FUSE_SECONDS;
			AudioSystemWwise::Get()->PostEvent( AK::EVENTS::BOMBFUSE, *GetPosition() );
		}
		else
		{
			m_vRenderMeshes[0]->GetRender() = false;
			SetFrustumCull( false );

			for( size_t i = 0; i < m_cpIndicatorBomb.size(); i++ )
			{
				m_cpIndicatorBomb[i]->GetRender() = false;
			}
		}

		for( size_t i = 0; i < m_cpIndicatorBomb.size(); i++ )
		{
			m_cpIndicatorBomb[i]->GetTransparencyOverride() = 1.0f;
			m_cpIndicatorBomb[i]->SetTexture( L"../Game/Assets/Art/2D/Textures/Failbot_WallBomb.dds" );
		}

		
	}

}

void CCrackedWall::DrawAvailable()
{
	if( CInputManager::GetInstance()->GetControllerBeingUsed() )
	{
		//GRAPHICS->RenderText( L"X to Build!", { -250, 500 }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ .5f, .5f } );
		m_cpControllerPopUp->GetRender() = true;
	}
	else
	{
		GRAPHICS->RenderText( L"F to Build!", { -250, 500 }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ .5f, .5f } );
	}

	for( size_t i = 0; i < m_cpIndicatorBomb.size(); i++ )
		m_cpIndicatorBomb[i]->SetTexture( L"../Game/Assets/Art/2D/Textures/Failbot_Green_Bomb.dds" );

	m_cpTextFrame->GetVertex2DArr()[1].m_fPosition[0] = .15f;
	m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[0] = .15f;
	m_cpTextFrame->GetRender() = true;
}

void CCrackedWall::DrawUnAvailable()
{
	for( size_t i = 0; i < m_cpIndicatorBomb.size(); i++ )
		m_cpIndicatorBomb[i]->SetTexture( L"../Game/Assets/Art/2D/Textures/Failbot_Red_Bomb.dds" );

	GRAPHICS->RenderText( L"Craft a Bomb in Inventory", { -250, 500 }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ 0.5f, 0.45f } );
	m_cpTextFrame->GetVertex2DArr()[1].m_fPosition[0] = 0.55f;
	m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[0] = 0.55f;
	m_cpTextFrame->GetRender() = true;
}

void CCrackedWall::BuildTextFrame()
{
	vector<TVertex2D> ScreenVerts;
	vector<unsigned int> ScreenIndicies;

	ScreenIndicies.push_back( 0 );
	ScreenIndicies.push_back( 1 );
	ScreenIndicies.push_back( 2 );

	ScreenIndicies.push_back( 0 );
	ScreenIndicies.push_back( 2 );
	ScreenIndicies.push_back( 3 );

	float fStartX = -0.30f;
	float fStartY = 0.55f;
	float fWidth = .7f;
	float fHeight = .2f;

	ScreenVerts.push_back // Top Left
		( {
				{ fStartX, fStartY, -1.0f, 1 },
				{ 0, 0 }
	} );

	ScreenVerts.push_back // Top Right
		( {
				{ fStartX + fWidth, fStartY, -1.0f, 1 },
				{ 1, 0 }
	} );

	ScreenVerts.push_back // Bot Right
		( {
				{ fStartX + fWidth, fStartY - fHeight, -1.0f, 1 },
				{ 1, 1 }
	} );

	ScreenVerts.push_back // Bot Left
		( {
				{ fStartX, fStartY - fHeight, -1.0f, 1 },
				{ 0, 1 }
	} );

	m_cpTextFrame = new CHUDMesh( ScreenIndicies, ScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/Failbot_TextBackground.dds" );
	m_cpTextFrame->GetRender() = false;
	m_cpTextFrame->GetTransparent() = true;
	GRAPHICS->AddRenderMesh( m_cpTextFrame );

}

void CCrackedWall::SetBombPosition()
{

	XMFLOAT3 vExtents = ( (CAABB*)( GetColliders()[0] )->GetBounds() )->GetExtents();

	float fNewOffsetPosX = GetPosition()->x + ( vExtents.x + OFFSET );
	float fNewOffsetNegX = GetPosition()->x - ( vExtents.x + OFFSET );
	float fNewOffsetPosZ = GetPosition()->z + ( vExtents.z + OFFSET );
	float fNewOffsetNegZ = GetPosition()->z - ( vExtents.z + OFFSET );

	m_cpIndicatorBomb[0]->GetTransformMatrix()._41 = fNewOffsetPosX;

	vExtents = XMFLOAT3{ 50.0f, 50.0f, 50.0f };
	XMFLOAT3 vCenter;

	vCenter.x = m_cpIndicatorBomb[0]->GetTransformMatrix()._41;
	vCenter.y = m_cpIndicatorBomb[0]->GetTransformMatrix()._42;
	vCenter.z = m_cpIndicatorBomb[0]->GetTransformMatrix()._43;

	CAABB* posX = new CAABB( vCenter, vExtents );
	AddCollider( new CCollider( false, posX, true, false ) );


	m_cpIndicatorBomb[1]->GetTransformMatrix()._41 = fNewOffsetNegX;
	vCenter.x = m_cpIndicatorBomb[1]->GetTransformMatrix()._41;
	vCenter.y = m_cpIndicatorBomb[1]->GetTransformMatrix()._42;
	vCenter.z = m_cpIndicatorBomb[1]->GetTransformMatrix()._43;

	CAABB* negX = new CAABB( vCenter, vExtents );
	AddCollider( new CCollider( false, negX, true, false ) );


	m_cpIndicatorBomb[2]->GetTransformMatrix()._43 = fNewOffsetPosZ;
	vCenter.x = m_cpIndicatorBomb[2]->GetTransformMatrix()._41;
	vCenter.y = m_cpIndicatorBomb[2]->GetTransformMatrix()._42;
	vCenter.z = m_cpIndicatorBomb[2]->GetTransformMatrix()._43;

	CAABB* posZ = new CAABB( vCenter, vExtents );
	AddCollider( new CCollider( false, posZ, true, false ) );


	m_cpIndicatorBomb[3]->GetTransformMatrix()._43 = fNewOffsetNegZ;
	vCenter.x = m_cpIndicatorBomb[3]->GetTransformMatrix()._41;
	vCenter.y = m_cpIndicatorBomb[3]->GetTransformMatrix()._42;
	vCenter.z = m_cpIndicatorBomb[3]->GetTransformMatrix()._43;

	CAABB* negZ = new CAABB( vCenter, vExtents );
	AddCollider( new CCollider( false, negZ, true, false ) );
}

void CCrackedWall::SetupParticle()
{
	m_vunParticleID.push_back( m_cpParticleManager->LoadEmitter( "WallExplosion.xml" ) );
	m_vunParticleID.push_back( m_cpParticleManager->LoadEmitter( "WallExplosion.xml" ) );
	m_vunParticleID.push_back( m_cpParticleManager->LoadEmitter( "WallExplosion.xml" ) );
	m_vunParticleID.push_back( m_cpParticleManager->LoadEmitter( "WallExplosion.xml" ) );

	for( size_t i = 0; i < m_vunParticleID.size(); i++ )
	{
		m_cpParticleManager->GetEmitter( m_vunParticleID[i] )->Stop();
		m_cpParticleManager->GetEmitter( m_vunParticleID[i] )->GetPosition( 0 ) = m_cpIndicatorBomb[i]->GetPositionMatrix()._41;
		m_cpParticleManager->GetEmitter( m_vunParticleID[i] )->GetPosition( 1 ) = m_cpIndicatorBomb[i]->GetPositionMatrix()._42;
		m_cpParticleManager->GetEmitter( m_vunParticleID[i] )->GetPosition( 2 ) = m_cpIndicatorBomb[i]->GetPositionMatrix()._43;
	}
}

void CCrackedWall::AddToRenderer()
{
	if (m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	for (unsigned int i = 0; i < m_vRenderMeshes.size(); i++)
	{
		if (m_vRenderMeshes[i] != nullptr)
		{
			GRAPHICS->AddRenderMesh(m_vRenderMeshes[i]);
		}
	}

	for (unsigned int i = 0; i < m_vObjects.size(); i++)
	{
		m_vObjects[i]->AddToRenderer();
	}

	if (m_cpControllerPopUp != nullptr)
		GRAPHICS->AddRenderMesh(m_cpControllerPopUp);

	if (m_cpTextFrame != nullptr)
		GRAPHICS->AddRenderMesh(m_cpTextFrame);

	CMesh* mesh = CAssetManager::GetInstance()->GetPrefabMesh("WallBomb");
	for (unsigned int i = 0; i < NUM_BOMBS; i++)
	{
		m_cpIndicatorBomb.push_back(new CRenderMesh(mesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetStandardPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Failbot_Green_Bomb.dds"));
		m_cpIndicatorBomb[i]->GetTransparent() = true;
		m_cpIndicatorBomb[i]->GetTransparencyOverride() = 0.5f;
		GRAPHICS->AddRenderMesh(m_cpIndicatorBomb[i]);
	}

	Initialize();
}
