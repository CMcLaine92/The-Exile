/***********************************************
* Filename:  		Bone.cpp
* Date:      		9/29/2015
* Mod. Date:
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			On ground loot for Bone.
************************************************/

#include "Bone.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Object Manager/ObjectManager.h"
#include "../../../Engine/Core/Collision/AABB.h"
#include "../../../Engine/Particles/ParticleManager.h"
#include "../../../Engine/Particles/ParticleManager.h"
#include "../../../Engine/Particles/Emitter.h"
#include "../../../Engine/Renderer/Lights.h"


#include "../../../Engine/Core/Collision/Capsule.h"

#define DESPAWN_TIME 30.0f
CBone::CBone( CObjectManager* _objManager, IObject* _parent ) : IObject( "Bone" )
{
	m_cpParent = _parent;
	m_bActive = false;
	m_fDespawnTime = DESPAWN_TIME;

	XMFLOAT3 newPos = { m_cpParent->GetPosition()->x, 50, m_cpParent->GetPosition()->z };
	SetPosition( newPos );

	//SetPosition( {500,100,0} );

	CMesh* theMesh = CAssetManager::GetInstance()->GetPrefabMesh("BonePile"); 
	m_cpRenderMesh = new CRenderMesh( theMesh,
		GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(),
		nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Failbot_BoneLoot.dds" );
	m_cpRenderMesh->SetNormals( L"../Game/Assets/Art/2D/Normal Maps/Failbot_BoneLoot.dds" );

	m_cpRenderMesh->GetTransparent() = true;
	m_cpRenderMesh->GetTransparencyOverride() = 0.0f;
	GRAPHICS->AddRenderMesh( m_cpRenderMesh );

	XMFLOAT3 extents = XMFLOAT3{ 40, 100, 40 };
	AddCollider( new CCollider( true, new CAABB( *GetPosition(), extents ), false ) );

	_objManager->AddObject( this, CObjectManager::Dynamic );

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
	m_cpParticleManager = new CParticleManager( _objManager );
	m_unParticle = m_cpParticleManager->LoadEmitter( "DroppedItemParticle.xml" );
	m_cpParticleManager->GetEmitter( m_unParticle )->GetPosition( 0 ) = newPos.x;
	m_cpParticleManager->GetEmitter( m_unParticle )->GetPosition( 1 ) = newPos.y;
	m_cpParticleManager->GetEmitter( m_unParticle )->GetPosition( 2 ) = newPos.z;

	m_cpParticleManager->GetEmitter( m_unParticle )->Stop();
}

CBone::~CBone()
{
	delete m_cpParticleManager;

	m_cpParticleManager = nullptr;

	m_cpParent = nullptr;
}

void CBone::Update()
{
	if( m_bActive )
	{
		m_pvColliders[0]->SetActive( true );

		RotateLocalY( m_mWorld, 1.0f );

		m_fDespawnTime -= DELTA_TIME();
		if( m_fDespawnTime <= 0.0f )
		{
			m_fDespawnTime = DESPAWN_TIME;
			DeActivate();
		}
		m_cpParticleManager->Update();
	}
}

void CBone::Activate()
{
	XMFLOAT3 newPos = *m_cpParent->GetPosition();

	m_cpRenderMesh->GetTransparencyOverride() = 1.0f;
	m_cpRenderMesh->SetEdgeGlow( true );
	m_bActive = true;

	m_cpParticleManager->GetEmitter( m_unParticle )->GetPosition( 0 ) = newPos.x;
	m_cpParticleManager->GetEmitter( m_unParticle )->GetPosition( 1 ) = newPos.y;
	m_cpParticleManager->GetEmitter( m_unParticle )->GetPosition( 2 ) = newPos.z;
	m_cpParticleManager->GetEmitter( m_unParticle )->ePlay();


	( (CAABB*)m_pvColliders[0]->GetBounds() )->SetCenter( newPos );

	newPos.y += 50;
	SetPosition( newPos );

	m_vtPointLights[0].m_fPosition[0] = newPos.x;
	m_vtPointLights[0].m_fPosition[1] = newPos.y;
	m_vtPointLights[0].m_fPosition[2] = newPos.z;
	GRAPHICS->AddLight(&m_vtPointLights[0]);


	m_pvColliders[0]->SetActive( true );
}

void CBone::DeActivate()
{
	m_bActive = false;
	GRAPHICS->RemoveLight(&m_vtPointLights[0]);
	m_pvColliders[0]->SetActive( false );
	m_cpRenderMesh->GetTransparencyOverride() = 0.0f;
	m_cpRenderMesh->SetEdgeGlow( false );
	m_cpParticleManager->GetEmitter( m_unParticle )->Stop();
}

void CBone::AddToRenderer()
{
	if (m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	if (m_bActive == true)
	{
		if (&m_vtPointLights[0] != nullptr)
			GRAPHICS->AddLight(&m_vtPointLights[0]);
	}

	m_unParticle = m_cpParticleManager->LoadEmitter("DroppedItemParticle.xml");
	m_cpParticleManager->GetEmitter(m_unParticle)->GetPosition(0) = GetPosition()->x;
	m_cpParticleManager->GetEmitter(m_unParticle)->GetPosition(1) = GetPosition()->y;
	m_cpParticleManager->GetEmitter(m_unParticle)->GetPosition(2) = GetPosition()->z;
}
