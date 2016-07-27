/***********************************************
* Filename:  		CTree.cpp
* Date:      		09/28/2015
* Mod. Date:
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			A cuttable tree that is harvested for wood.
************************************************/
#include "Tree.h"
#include "Wood.h"
#include "DayNight.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Object Manager/ObjectManager.h"
#include "../../../Engine/Core/Collision/AABB.h"
#include "../../../Engine/Particles/ParticleManager.h"
#include "../../../Engine/Particles/ParticleManager.h"
#include "../../../Engine/Particles/Emitter.h"

#define TREE_WIDTH 30.0f
#define TREE_HEIGHT 200.0f
#define FALL_SPEED .5f

CTree::CTree( CObjectManager* objectManager, XMFLOAT3 position ) : IObject( "Tree" )
{
	SetPosition( position );
	m_f3SpawnPos = position;

	m_bActive = true;
	m_bFalling = false;
	m_fFallAngle = 0.0f;
	m_fFallSpeed = .5f;
	m_cpParticleManager = new CParticleManager( objectManager );

	m_bDestroyedDay = false;
	m_bDestroyedNight = false;

	CMesh* theMesh = CAssetManager::GetInstance()->GetPrefabMesh( "Tree" );
	m_cpRenderMesh = new CRenderMesh( theMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Failbot_Tree.dds" );
	m_cpRenderMesh->GetTransparent() = true;
	GRAPHICS->AddRenderMesh( m_cpRenderMesh );

	AddCollider( new CCollider( false, new CAABB( *GetPosition(), { TREE_WIDTH, TREE_HEIGHT, TREE_WIDTH } ), true ) );

	// Particles
	m_unParticle = m_cpParticleManager->LoadEmitter( "BushDestroy.xml" );
	m_cpParticleManager->GetEmitter( m_unParticle )->GetPosition( 0 ) = GetPosition()->x + 30;
	m_cpParticleManager->GetEmitter( m_unParticle )->GetPosition( 1 ) = GetPosition()->y + 20;
	m_cpParticleManager->GetEmitter( m_unParticle )->GetPosition( 2 ) = GetPosition()->z;

	m_cpTheLoot = new CWood( objectManager, this );

	AudioSystemWwise::Get()->RegisterEntity( this, "Tree" );
}

CTree::~CTree()
{
	AudioSystemWwise::Get()->UnRegisterEntity( this );

	if( m_cpParticleManager )
	{
		delete m_cpParticleManager;
		m_cpParticleManager = nullptr;
	}

	if( m_cpTheLoot )
	{
		delete m_cpTheLoot;
		m_cpTheLoot = nullptr;
	}
}

void CTree::SetWorldMatrix(const XMFLOAT4X4 world)
{
	m_mWorld = world;
	m_f3SpawnPos = XMFLOAT3(world._41, world._42, world._43);
	m_cpParticleManager->GetEmitter(m_unParticle)->GetPosition(0) = GetPosition()->x + 30;
	m_cpParticleManager->GetEmitter(m_unParticle)->GetPosition(1) = GetPosition()->y + 20;
	m_cpParticleManager->GetEmitter(m_unParticle)->GetPosition(2) = GetPosition()->z;
	//((CAABB*)m_pvColliders[0]->GetBounds())->SetCenter(XMFLOAT3(world._41, world._42, world._43));
	m_cpTheLoot->SetWorldMatrix(world);
	
}

void CTree::Update()
{
	if (m_bActive == true)
	{
		if ((m_bDestroyedDay == true && m_cpDayNight->IsNight() == true) || (m_bDestroyedNight == true && m_cpDayNight->IsDay() == true))
		{
			Respawn();
		}
	}

	if( m_bFalling )
	{
		AudioSystemWwise::Get()->PostEvent( AK::EVENTS::FALLINGTREE );
		m_fFallAngle += ( m_fFallSpeed * DELTA_TIME() );
		RotateLocalX( m_mWorld, ( m_fFallSpeed * DELTA_TIME() ) );

		if( m_fFallAngle > 90.0f )
			DeActivate();

		m_fFallSpeed += ( DELTA_TIME() * m_fFallSpeed ) * 5;
	}
	//CAABB* bounds = ((CAABB*)GetColliders()[0]->GetBounds());

	//DEBUG::DrawAABBCollider(bounds->Min(), bounds->Max(), { 1.0f, 1.0f, 0.0f });
	m_cpParticleManager->Update();
}

void CTree::DeActivate()
{
	m_fFallAngle = 0.0f;
	m_bFalling = false;
	m_bActive = true;
	m_pvColliders[0]->SetActive( false );
	m_cpRenderMesh->GetRender() = false;
	m_cpRenderMesh->GetTransparencyOverride() = 0.0f;
	m_cpParticleManager->GetEmitter( m_unParticle )->ePlay();

	// Drop loot
	m_cpTheLoot->Activate();

	if (m_cpDayNight->IsDay() == true)
	{
		m_bDestroyedDay = true;
		m_bDestroyedNight = false;
	}
	else if (m_cpDayNight->IsNight() == true)
	{
		m_bDestroyedNight = true;
		m_bDestroyedDay = false;
	}
}

void CTree::Respawn()
{
	m_mWorld = 
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		m_f3SpawnPos.x, m_f3SpawnPos.y, m_f3SpawnPos.z, 1
	};

	m_fFallAngle = 0.0f;
	m_fFallAngle = 0.0f;
	m_fFallSpeed = 0.5f;
	m_bFalling = false;
	m_bActive = false;
	m_pvColliders[0]->SetActive(true);
	m_cpRenderMesh->GetRender() = true;
	m_cpRenderMesh->GetTransparencyOverride() = 100.0f;

	m_bDestroyedDay = false;
	m_bDestroyedNight = false;
}

void CTree::SetDayNight(CDayNight* dayNight)
{
	m_cpDayNight = dayNight;
}

void CTree::AddToRenderer()
{
	if (m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	m_unParticle = m_cpParticleManager->LoadEmitter("BushDestroy.xml");
	m_cpParticleManager->GetEmitter(m_unParticle)->GetPosition(0) = m_f3SpawnPos.x + 30;
	m_cpParticleManager->GetEmitter(m_unParticle)->GetPosition(1) = m_f3SpawnPos.y + 20;
	m_cpParticleManager->GetEmitter(m_unParticle)->GetPosition(2) = m_f3SpawnPos.z;
}
