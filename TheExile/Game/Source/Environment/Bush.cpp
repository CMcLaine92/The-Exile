/***********************************************
* Filename:  		Bush.cpp
* Date:      		9/2/2015
* Mod. Date: 		9/12/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a bush
************************************************/

#include "Bush.h"
#include "Herb.h"
#include "DayNight.h"
#include "../../../Engine/Animation/Mesh.h"
#include "../../../Engine/Particles/ParticleManager.h"
#include "../../../Engine/Particles/Emitter.h"
#include "../../../Engine/Particles/Particle.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Core/Collision/Collider.h"
#include "../../../Engine/Core/Collision/AABB.h"

CBush::CBush(CObjectManager* objectManager, XMFLOAT3 position) : IObject("Bush")
{
	//set active
	m_bActive = true;
	m_bDestroyedDay = false;
	m_bDestroyedNight = false;

	//set object manager
	m_cpObjectManager = objectManager;

	//set position
	m_mWorld =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		position.x, position.y, position.z, 1
	};

	m_f3SpawnPos = position;

	//set up rendermesh
	CMesh* tempMesh = CAssetManager::GetInstance()->GetPrefabMesh("Bush");
	SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Env_Bush.dds"));
	GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	//set up collider
	XMVECTOR mathCenter = XMLoadFloat3(&position);
	XMFLOAT3 tempExtents = XMFLOAT3(100.0f, 500.0f, 100.0f);
	XMVECTOR mathExtent = XMLoadFloat3(&tempExtents);

	XMFLOAT3 extents; XMStoreFloat3(&extents, mathExtent);
	XMFLOAT3 boundsCenter; XMStoreFloat3(&boundsCenter, mathCenter);

	AddCollider(new CCollider(false, new CAABB(boundsCenter, extents), true,false));

	//set up particles 
	m_cpParticleManager = new CParticleManager(m_cpObjectManager);
	m_vnBushParticle = m_cpParticleManager->LoadEmitter("BushDestroy.xml");
	m_cpParticleManager->GetEmitter(m_vnBushParticle)->GetPosition(0) = GetPosition()->x;
	m_cpParticleManager->GetEmitter(m_vnBushParticle)->GetPosition(1) = GetPosition()->y + 50;
	m_cpParticleManager->GetEmitter(m_vnBushParticle)->GetPosition(2) = GetPosition()->z;
	
	//set up audio 
	m_cpBushAudio = m_cpBushAudio->Get();

	//set up the herb
	m_cpHerb = new CHerb(m_f3SpawnPos, m_cpObjectManager);

}

CBush::~CBush()
{
	delete m_cpParticleManager;
	delete m_cpHerb;
}

void CBush::Update()
{
	//if the bush was destroyed
	if (m_bActive == false)
	{
		//if the next day/night has come
		if ((m_bDestroyedDay == true && m_cpDayNight->IsNight() == true) || (m_bDestroyedNight == true && m_cpDayNight->IsDay() == true))
		{
			RespawnBush();
		}
	}

	/*CAABB* bounds = ((CAABB*)GetColliders()[0]->GetBounds());
	DEBUG::DrawAABBCollider(bounds->Min(), bounds->Max(), { 0.0f, 1.0f, 0.0f });*/

	m_cpParticleManager->Update();
}

void CBush::DestroyBush()
{
	m_bActive = false;

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

	m_mWorld =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		m_f3SpawnPos.x, m_f3SpawnPos.y - 250.0f, m_f3SpawnPos.z, 1
	};

	for (unsigned int i = 0; i < m_pvColliders.size(); i++)
	{
		//((CAABB*)m_pvColliders[i]->GetBounds())->SetCenter({ m_mWorld._41, m_mWorld._42, m_mWorld._43 });
		m_pvColliders[i]->SetActive(false);
	}

	//set render bool
	m_cpRenderMesh->GetRender() = false;

	//play audio
	m_cpBushAudio->PostEvent(AK::EVENTS::BUSH, XMFLOAT3(m_mWorld._41, m_mWorld._42, m_mWorld._43));

	//play effect
	m_cpParticleManager->GetEmitter(m_vnBushParticle)->ePlay();

	//spawn herb
	m_cpHerb->Activate();
}

void CBush::SetWorldMatrix(const XMFLOAT4X4 newWorld)
{
	m_mWorld = newWorld;
	m_f3SpawnPos = XMFLOAT3(newWorld._41, newWorld._42, newWorld._43);
	m_cpHerb->SetWorldMatrix(newWorld);
	m_cpHerb->SetSpawnPosition(XMFLOAT3(newWorld._41, newWorld._42, newWorld._43));
	m_cpParticleManager->GetEmitter(m_vnBushParticle)->GetPosition(0) = m_mWorld._41;
	m_cpParticleManager->GetEmitter(m_vnBushParticle)->GetPosition(1) = m_mWorld._42 + 50;
	m_cpParticleManager->GetEmitter(m_vnBushParticle)->GetPosition(2) = m_mWorld._43;

}

void CBush::RespawnBush()
{
	m_bActive = true;
	m_bDestroyedDay = false;
	m_bDestroyedNight = false;

	m_mWorld =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		m_f3SpawnPos.x, m_f3SpawnPos.y, m_f3SpawnPos.z, 1
	};

	for (unsigned int i = 0; i < m_pvColliders.size(); i++)
	{
		//((CAABB*)m_pvColliders[i]->GetBounds())->SetCenter({ m_mWorld._41, m_mWorld._42, m_mWorld._43 });
		m_pvColliders[i]->SetActive(true);
	}

	//set render bool
	m_cpRenderMesh->GetRender() = true;

	//despawn herb 
	m_cpHerb->DeActivate();
}

void CBush::SetDayNight(CDayNight* dayNight)
{
	m_cpDayNight = dayNight;
}

void CBush::AddToRenderer()
{
	if (m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	m_vnBushParticle = m_cpParticleManager->LoadEmitter("BushDestroy.xml");
	m_cpParticleManager->GetEmitter(m_vnBushParticle)->GetPosition(0) = GetPosition()->x;
	m_cpParticleManager->GetEmitter(m_vnBushParticle)->GetPosition(1) = GetPosition()->y + 50;
	m_cpParticleManager->GetEmitter(m_vnBushParticle)->GetPosition(2) = GetPosition()->z;
}
