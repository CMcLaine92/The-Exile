/***********************************************
* Filename:  		Wood.cpp
* Date:      		9/29/2015
* Mod. Date:
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			On ground loot for Wood.
************************************************/

#include "Wood.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Object Manager/ObjectManager.h"
#include "../../../Engine/Core/Collision/AABB.h"
#include "../../../Engine/Particles/ParticleManager.h"
#include "../../../Engine/Particles/ParticleManager.h"
#include "../../../Engine/Particles/Emitter.h"
#include "../../../Engine/Renderer/Lights.h"
#include "../../../Engine/Core/Messages/AddObjectMessage.h"
#include "../../../Engine/Core/Messages/RemoveObjectMessage.h"


CWood::CWood( CObjectManager* _objManager, IObject* _parent ) : IObject("Wood")
{
	m_cpParent = _parent;
	m_bActive = false;
	m_fDespawnTime = ACTIVE_TIME;

	XMFLOAT3 newPos = { m_cpParent->GetPosition()->x, 50, m_cpParent->GetPosition()->z };
	SetPosition( newPos );

	CMesh* theMesh = CAssetManager::GetInstance()->GetPrefabMesh("WoodPile");
	m_cpRenderMesh = new CRenderMesh( theMesh,
		GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(),
		nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Failbot_Wood.dds" );

	m_cpRenderMesh->GetTransparent() = true;
	m_cpRenderMesh->GetTransparencyOverride() = 0.0f;
	GRAPHICS->AddRenderMesh( m_cpRenderMesh );

	XMFLOAT3 extents = XMFLOAT3{ 50, 60, 50 };
	AddCollider( new CCollider( false, new CAABB( *GetPosition(), extents ), true, false ) );

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

void CWood::SetWorldMatrix(const XMFLOAT4X4 world)
{
	m_mWorld = world;
	m_cpParticleManager->GetEmitter(m_unParticle)->GetPosition(0) = world._41;
	m_cpParticleManager->GetEmitter(m_unParticle)->GetPosition(1) = world._42;
	m_cpParticleManager->GetEmitter(m_unParticle)->GetPosition(2) = world._43;
	((CAABB*)m_pvColliders[0]->GetBounds())->SetCenter(XMFLOAT3(world._41, world._42 + 30.0f, world._43));
}

CWood::~CWood()
{
	delete m_cpParticleManager;

	m_cpParticleManager = nullptr;

	m_cpParent = nullptr;

}

void CWood::Update()
{
	if( m_bActive )
	{
		RotateLocalY( m_mWorld, 1.0f );

		m_fDespawnTime -= DELTA_TIME();
		if( m_fDespawnTime <= 0.0f )
		{
			m_fDespawnTime = ACTIVE_TIME;
			DeActivate();
		}
		m_cpParticleManager->Update();

		/*CAABB* bounds = ((CAABB*)GetColliders()[0]->GetBounds());
		DEBUG::DrawAABBCollider(bounds->Min(), bounds->Max(), { 1.0f, 0.0f, 0.0f });*/
	}

}

void CWood::Activate()
{
	m_bActive = true;
	GRAPHICS->AddLight(&m_vtPointLights[0]);
	m_pvColliders[0]->SetActive( true );
	
	m_cpRenderMesh->SetEdgeGlow(true);
	m_cpRenderMesh->GetTransparencyOverride() = 1.0f;
	m_cpParticleManager->GetEmitter( m_unParticle )->ePlay();
	CAddObjectMessage* msg = new CAddObjectMessage(this, CObjectManager::Static);
	msg->Send();
}

void CWood::DeActivate()
{
	m_bActive = false;
	GRAPHICS->RemoveLight(&m_vtPointLights[0]);
	m_pvColliders[0]->SetActive( false );
	m_cpRenderMesh->SetEdgeGlow(false);
	m_cpRenderMesh->GetTransparencyOverride() = 0.0f;
	m_cpParticleManager->GetEmitter( m_unParticle )->Stop();
	CRemoveObjectMessage* msg = new CRemoveObjectMessage(this, CObjectManager::Static);
	msg->Send();
}

void CWood::AddToRenderer()
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
