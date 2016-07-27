/***********************************************
* Filename:  		Herb.cpp
* Date:      		9/13/2015
* Mod. Date: 		9/13/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a herb
************************************************/

#include "Herb.h"
#include "../../../Engine/Core/Collision/AABB.h"
#include "../../../Engine/Animation/Mesh.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Particles/ParticleManager.h"
#include "../../../Engine/Particles/Emitter.h"
#include "../../../Engine/Particles/Particle.h"
#include "../../../Engine/Renderer/Lights.h"
#include "../../../Engine/Core/Messages/AddObjectMessage.h"
#include "../../../Engine/Core/Messages/RemoveObjectMessage.h"

CHerb::CHerb( XMFLOAT3 position, CObjectManager* objectManager ) : IObject( "Herb" )
{
	//set object manager for particle use
	m_cpObjectManager = objectManager;

	//set inactive
	m_bActive = false;
	m_fActiveTime = ACTIVE_TIME;

	//set position
	m_mWorld =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		position.x, position.y - 250, position.z, 1
	};

	m_f3SpawnPoint = position;

	//set up rendermesh... Green health bar texture works just fine for now lol
	CMesh* tempMesh = CAssetManager::GetInstance()->GetPrefabMesh("Herb");
	SetRenderMesh( new CRenderMesh( tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/HealthBarGreen.dds" ) );
	GRAPHICS->AddRenderMesh( m_cpRenderMesh );
	m_cpRenderMesh->GetRender() = false;

	//set up colliders
	XMVECTOR mathCenter = XMLoadFloat3( &m_f3SpawnPoint );
	XMVECTOR mathExtent = XMLoadFloat3( &XMFLOAT3( 50.0f, 50.0f, 50.0f ) );

	XMFLOAT3 extents; XMStoreFloat3( &extents, mathExtent );
	XMFLOAT3 boundsCenter; XMStoreFloat3( &boundsCenter, mathCenter );

	AddCollider( new CCollider( false, new CAABB( boundsCenter, extents ), true ) );

	TPointLight tempLight;
	tempLight.m_fPosition[0] = GetPosition()->x;
	tempLight.m_fPosition[1] = GetPosition()->y;
	tempLight.m_fPosition[2] = GetPosition()->z;
	tempLight.m_fColor[0] = 0.3f;
	tempLight.m_fColor[1] = 0.3f;
	tempLight.m_fColor[2] = 0.3f;
	tempLight.m_fRadius = 500.0f;
	tempLight.m_fBrightness = 10.0f;
	AddLight(tempLight);


	//set up particles 
	m_cpParticleManager = new CParticleManager( m_cpObjectManager );
	m_nDroppedItemParticle = m_cpParticleManager->LoadEmitter( "DroppedItemParticle.xml" );
	m_cpParticleManager->GetEmitter( m_nDroppedItemParticle )->GetPosition( 0 ) = position.x;
	m_cpParticleManager->GetEmitter( m_nDroppedItemParticle )->GetPosition( 1 ) = position.y + 50;
	m_cpParticleManager->GetEmitter( m_nDroppedItemParticle )->GetPosition( 2 ) = position.z;

	m_cpParticleManager->GetEmitter( m_nDroppedItemParticle )->Stop();
	
}

void CHerb::SetSpawnPosition(XMFLOAT3 pos)
{
	m_f3SpawnPoint = pos;
	m_cpParticleManager->GetEmitter(m_nDroppedItemParticle)->GetPosition(0) = pos.x;
	m_cpParticleManager->GetEmitter(m_nDroppedItemParticle)->GetPosition(1) = pos.y + 50;
	m_cpParticleManager->GetEmitter(m_nDroppedItemParticle)->GetPosition(2) = pos.z;

}

CHerb::~CHerb()
{
	delete m_cpParticleManager;
}

void CHerb::Update()
{
	if( m_bActive == true )
	{
		RotateLocalY( m_mWorld, 1.0f );
		
		m_fActiveTime -= DELTA_TIME();
		if( m_fActiveTime <= 0.0f )//get rid of herb. player didnt pick it up. bush is about to respawn
		{
			m_fActiveTime = ACTIVE_TIME;
			DeActivate();
		}

		m_cpParticleManager->Update();
	}
}

void CHerb::Activate()
{
	m_fActiveTime = ACTIVE_TIME;

	if( m_bActive == true )
	{
		return;
	}

	m_bActive = true;
	m_cpRenderMesh->GetRender() = true;
	m_cpRenderMesh->SetEdgeGlow(true);

	m_vtPointLights[0].m_fPosition[1] += 200.0f;
	GRAPHICS->AddLight(&m_vtPointLights[0]);

	m_mWorld =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		m_f3SpawnPoint.x, m_f3SpawnPoint.y + 100.0f, m_f3SpawnPoint.z, 1
	};

	for( unsigned int i = 0; i < m_pvColliders.size(); i++ )
	{
		
		m_pvColliders[i]->SetActive( true );
		((CAABB*)m_pvColliders[i]->GetBounds())->SetCenter(XMFLOAT3(m_f3SpawnPoint.x, m_f3SpawnPoint.y + 100.0f, m_f3SpawnPoint.z));

	}

	m_cpParticleManager->GetEmitter( m_nDroppedItemParticle )->ePlay();

	CAddObjectMessage* msg = new CAddObjectMessage(this, CObjectManager::Static);
	msg->Send();
}

void CHerb::DeActivate()
{
	if( m_bActive == false )
	{
		return;
	}

	m_bActive = false;
	m_fActiveTime = ACTIVE_TIME;
	m_cpRenderMesh->GetRender() = false;
	m_cpRenderMesh->SetEdgeGlow(false);

	m_vtPointLights[0].m_fPosition[1] -= 250.0f;
	GRAPHICS->RemoveLight(&m_vtPointLights[0]);

	m_mWorld =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		m_f3SpawnPoint.x, m_f3SpawnPoint.y - 250.0f, m_f3SpawnPoint.z, 1
	};

	for( unsigned int i = 0; i < m_pvColliders.size(); i++ )
	{
		m_pvColliders[i]->SetActive( false );
	}

	m_cpRenderMesh->GetRender() = false;

	m_cpParticleManager->GetEmitter( m_nDroppedItemParticle )->Stop();
	CRemoveObjectMessage* msg = new CRemoveObjectMessage(this, CObjectManager::Static);
	msg->Send();
}

void CHerb::AddToRenderer()
{
	if (m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	if (m_bActive == true)
	{
		if (&m_vtPointLights[0] != nullptr)
			GRAPHICS->AddLight(&m_vtPointLights[0]);
	}

	m_nDroppedItemParticle = m_cpParticleManager->LoadEmitter("DroppedItemParticle.xml");
	m_cpParticleManager->GetEmitter(m_nDroppedItemParticle)->GetPosition(0) = m_f3SpawnPoint.x;
	m_cpParticleManager->GetEmitter(m_nDroppedItemParticle)->GetPosition(1) = m_f3SpawnPoint.y + 50;
	m_cpParticleManager->GetEmitter(m_nDroppedItemParticle)->GetPosition(2) = m_f3SpawnPoint.z;
}
