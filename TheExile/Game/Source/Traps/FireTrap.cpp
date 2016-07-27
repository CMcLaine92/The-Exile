/***********************************************
* Filename:  		FireTrap.cpp
* Date:      		09/5/2015
* Mod. Date:
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			Controls the trap that shoots fire when activated.
************************************************/

#include "FireTrap.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Core/Messages/AddObjectMessage.h"
#include "../../../Engine/Core/Messages/RemoveObjectMessage.h"
#include "../../../Engine/Animation/Mesh.h"
#include "../../../Engine/Core/Collision/AABB.h"
#include "../../../Engine/Particles/ParticleManager.h"
#include "../../../Engine/Particles/Emitter.h"
#include "../../../Engine/Particles/Particle.h"
#include "../../../Engine/Object Manager/PressurePlate.h"
#include "../../../Engine/Audio/Source/AudioSystemWwise.h"
#include "../../../Game/Source/Player/Player.h"

#define TRAP_DURATION 6.0f
#define TRAP_DELAY 0.3f
#define TRAP_HOTBOX_SIZE 300
#define TRAP_DMG 10

CFireTrap::CFireTrap(CObjectManager* _objManager, XMFLOAT3 _location, XMFLOAT3 _facing) : IObject("FireTrap")
{
	m_cpObjManager = _objManager;
	m_f3FireDirection = _facing;

	if (m_f3FireDirection.y != 0.0f)
	{
		_location.y = 0.0f;
	}
	SetPosition(_location);

	m_fActiveTimer = TRAP_DURATION;
	m_fDelayTimer = TRAP_DELAY;
	m_nTrapDamage = TRAP_DMG;
	// Setup the audio.
	AudioSystemWwise::Get()->RegisterEntity(this, "FireTrap");

	//// Add the object
	//CAddObjectMessage* addObj = new CAddObjectMessage(this, CObjectManager::Static);
	//addObj->Send();

	// Find the center of the AABB
	//XMVECTOR mathCenter = XMLoadFloat3(&_location);
	//XMVECTOR mathHeading = XMLoadFloat3(&m_f3FireDirection);
	//mathCenter += mathHeading * (TRAP_HOTBOX_SIZE / 2);

	//XMFLOAT3 extents = XMFLOAT3(1.0f, 1.0f, 1.0f);
	//// Create a vector for the extents of the AABB

	//XMVECTOR mathExtent = XMLoadFloat3(&extents);
	//mathExtent *= (TRAP_HOTBOX_SIZE / 2);

	//// Convert Back

	//XMStoreFloat3(&extents, mathExtent);
	//XMFLOAT3 boundsCenter; XMStoreFloat3(&boundsCenter, mathCenter);

	// Quad treee collider (Not detecting collision)

	// Render the mesh
	CMesh* cMesh = CAssetManager::GetInstance()->GetPrefabMesh("FireTrap");
	SetRenderMesh(new CRenderMesh(cMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader()));

	GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	// Create and set up the pressure plate
	//m_cpPressurePlate = new CPressurePlate( boundsCenter, true );
	//m_cpPressurePlate->SetParent( this );

	//InitializeParticles( _facing );

}

void CFireTrap::InitializeParticles(XMFLOAT3 _heading)
{
	// Particles
	m_cpParticleManager = new CParticleManager(m_cpObjManager);

	//set up smoke particle
	m_vnSmokeParticle.push_back(m_cpParticleManager->LoadEmitter("FireTrapSmoke.xml"));
	m_vnSmokeParticle.push_back(m_cpParticleManager->LoadEmitter("FireTrapSmoke2.xml"));
	for (unsigned int i = 0; i < m_vnSmokeParticle.size(); i++)
	{
		m_cpParticleManager->GetEmitter(m_vnSmokeParticle[i])->GetPosition(0) = GetPosition()->x;
		m_cpParticleManager->GetEmitter(m_vnSmokeParticle[i])->GetPosition(1) = GetPosition()->y;
		m_cpParticleManager->GetEmitter(m_vnSmokeParticle[i])->GetPosition(2) = GetPosition()->z;
	}


	// Fire on X axis
	if (_heading.x != 0.0f)
	{
		m_vunParticle.push_back(m_cpParticleManager->LoadEmitter("FireX.xml"));
		m_vunParticle.push_back(m_cpParticleManager->LoadEmitter("RedFireX.xml"));

		for (unsigned int i = 0; i < m_vnSmokeParticle.size(); i++)
		{
			m_cpParticleManager->GetEmitter(m_vnSmokeParticle[i])->GetPosition(0) += (10.0f * _heading.x);
		}

		for (size_t i = 0; i < m_vunParticle.size(); i++)
		{
			m_cpParticleManager->GetEmitter(m_vunParticle[i])->GetFlyweight().f3MaxEndVelocity[0] *= _heading.x;
			m_cpParticleManager->GetEmitter(m_vunParticle[i])->GetFlyweight().f3MaxStartVelocity[0] *= _heading.x;
			m_cpParticleManager->GetEmitter(m_vunParticle[i])->GetFlyweight().f3MinEndVelocity[0] *= _heading.x;
			m_cpParticleManager->GetEmitter(m_vunParticle[i])->GetFlyweight().f3MinStartVelocity[0] *= _heading.x;
		}
	}
	// Fire on Y Axis
	else if (_heading.y != 0.0f)
	{
		m_vunParticle.push_back(m_cpParticleManager->LoadEmitter("FireY.xml"));
		m_vunParticle.push_back(m_cpParticleManager->LoadEmitter("RedFireY.xml"));

		for (unsigned int i = 0; i < m_vnSmokeParticle.size(); i++)
		{
			m_cpParticleManager->GetEmitter(m_vnSmokeParticle[i])->GetPosition(1) += (10.0f * _heading.y);
		}


		for (size_t i = 0; i < m_vunParticle.size(); i++)
		{
			m_cpParticleManager->GetEmitter(m_vunParticle[i])->GetFlyweight().f3MaxEndVelocity[1] *= _heading.y;
			m_cpParticleManager->GetEmitter(m_vunParticle[i])->GetFlyweight().f3MaxStartVelocity[1] *= _heading.y;
			m_cpParticleManager->GetEmitter(m_vunParticle[i])->GetFlyweight().f3MinEndVelocity[1] *= _heading.y;
			m_cpParticleManager->GetEmitter(m_vunParticle[i])->GetFlyweight().f3MinStartVelocity[1] *= _heading.y;
		}
	}
	// Fire on Z Axis
	else if (_heading.z != 0.0f)
	{
		m_vunParticle.push_back(m_cpParticleManager->LoadEmitter("FireZ.xml"));
		m_vunParticle.push_back(m_cpParticleManager->LoadEmitter("RedFireZ.xml"));

		for (unsigned int i = 0; i < m_vnSmokeParticle.size(); i++)
		{
			m_cpParticleManager->GetEmitter(m_vnSmokeParticle[i])->GetPosition(2) += (10.0f * _heading.z);
		}

		for (size_t i = 0; i < m_vunParticle.size(); i++)
		{
			m_cpParticleManager->GetEmitter(m_vunParticle[i])->GetFlyweight().f3MaxEndVelocity[2] *= _heading.z;
			m_cpParticleManager->GetEmitter(m_vunParticle[i])->GetFlyweight().f3MaxStartVelocity[2] *= _heading.z;
			m_cpParticleManager->GetEmitter(m_vunParticle[i])->GetFlyweight().f3MinEndVelocity[2] *= _heading.z;
			m_cpParticleManager->GetEmitter(m_vunParticle[i])->GetFlyweight().f3MinStartVelocity[2] *= _heading.z;
		}
	}

	for (size_t i = 0; i < m_vunParticle.size(); i++)
	{
		m_cpParticleManager->GetEmitter(m_vunParticle[i])->GetPosition(0) = GetPosition()->x;
		m_cpParticleManager->GetEmitter(m_vunParticle[i])->GetPosition(1) = GetPosition()->y;
		m_cpParticleManager->GetEmitter(m_vunParticle[i])->GetPosition(2) = GetPosition()->z;
		m_cpParticleManager->GetEmitter(m_vunParticle[i])->Stop();
	}

	for (unsigned int i = 0; i < m_vnSmokeParticle.size(); i++)
	{
		m_cpParticleManager->GetEmitter(m_vnSmokeParticle[i])->ePlay();
	}

}


CFireTrap::~CFireTrap()
{
	delete m_cpPressurePlate;
	delete m_cpParticleManager;

	AudioSystemWwise::Get()->UnRegisterEntity(this);

	
}

void CFireTrap::Update()
{
	
#if _DEBUG
	CAABB* pAABB = (CAABB*)(m_pvColliders[0]->GetBounds());
	DEBUG_UTIL::DrawAABBCollider(pAABB->Min(), pAABB->Max());
#endif

	if (m_bActive)
	{
		m_fActiveTimer -= DELTA_TIME();
		if (m_fActiveTimer <= 0.0f)
		{
			//AudioSystemWwise::Get()->PostEvent(AK::EVENTS::STOPFIRETRAP);
			//AudioSystemWwise::Get()->PostEvent(AK::EVENTS::FIRETRAP);
			for (size_t i = 0; i < m_vunParticle.size(); i++)
				for (size_t i = 0; i < m_vunParticle.size(); i++)
					AudioSystemWwise::Get()->PostEvent(AK::EVENTS::FIRETRAP, *GetPosition());
			for (size_t i = 0; i < m_vunParticle.size(); i++)
			{
				m_cpParticleManager->GetEmitter(m_vunParticle[i])->Stop();
			}

			m_fActiveTimer = TRAP_DURATION;
			m_bActive = false;
		}
		else
		{
			//AudioSystemWwise::Get()->PostEvent( AK::EVENTS::FIRETRAP, *GetPosition() );
			//for( size_t i = 0; i < m_vunParticle.size(); i++ )
			//{
			//	if( !m_cpParticleManager->GetEmitter( m_vunParticle[i] )->GetIsActive() )
			//		m_cpParticleManager->GetEmitter( m_vunParticle[i] )->ePlay();
			//}
		}
	}

	if (m_cpParticleManager != nullptr)
	{
		m_cpParticleManager->Update();
	}


	/*for (int i = 0; i < m_pvColliders.size(); i++)
	{
		CAABB* aabb = ((CAABB*)m_pvColliders[i]->GetBounds());
		DEBUG::DrawAABBCollider(aabb->Min(), aabb->Max());
	}*/
}

void CFireTrap::MakeActive()
{
	m_fDelayTimer -= DELTA_TIME();
	if (m_fDelayTimer <= 0.0f)
	{
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::FIRETRAP, *GetPosition());
		for (size_t i = 0; i < m_vunParticle.size(); i++)
		{
			if (!m_cpParticleManager->GetEmitter(m_vunParticle[i])->GetIsActive())
				m_cpParticleManager->GetEmitter(m_vunParticle[i])->ePlay();
		}
		m_bActive = true;
		m_fDelayTimer = TRAP_DELAY;
	}
}

void CFireTrap::CollisionResponse(IObject * _cCollidedObject)
{
	DBG_UNREFERENCED_PARAMETER(_cCollidedObject);

	DBG_UNREFERENCED_PARAMETER(_cCollidedObject);

	DBG_UNREFERENCED_PARAMETER(_cCollidedObject);
	//if (_cCollidedObject->GetTag() == "Player")
	//{

	//	if (m_bActive)
	//	{
	//		reinterpret_cast<CPlayer*>(_cCollidedObject)->TakeDamage(10);
	//	}
	//}

}

void CFireTrap::SetWorldMatrix(const DirectX::XMFLOAT4X4 matrix)
{
	m_mWorld = matrix;

	InitializeParticles(m_f3FireDirection);

	// Find the center of the AABB
	XMVECTOR mathCenter = XMLoadFloat3(GetPosition());
	XMVECTOR mathHeading = XMLoadFloat3(&m_f3FireDirection);
	mathCenter += mathHeading * ((TRAP_HOTBOX_SIZE / 2) + 75.0f);
	XMFLOAT3 boundsCenter; 
	XMStoreFloat3(&boundsCenter, mathCenter);

	XMFLOAT3 extents = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMVECTOR mathExtent = XMLoadFloat3(&extents);
	mathExtent *= (TRAP_HOTBOX_SIZE / 2);
	XMStoreFloat3(&extents, mathExtent);


	// Quad treee collider (Not detecting collision)

	AddCollider(new CCollider(false, new CAABB(boundsCenter, extents), true, false));

		


	//pressure plate
	m_cpPressurePlate = new CPressurePlate(boundsCenter, true);
	m_cpPressurePlate->SetParent(this);

}

void CFireTrap::AddToRenderer()
{
	if (m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	InitializeParticles(m_f3FireDirection);
}
