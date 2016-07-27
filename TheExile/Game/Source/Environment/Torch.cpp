/***********************************************
* Filename:  		Torch.cpp
* Date:      		9/2/2015
* Mod. Date: 		9/10/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a torch
************************************************/

#include "Torch.h"
#include "../../../Engine/Animation/Mesh.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Core/Collision/Collider.h"
#include "../../../Engine/Core/Collision/AABB.h"
#include "../../../Engine/Particles/ParticleManager.h"
#include "../../../Engine/Particles/Emitter.h"
#include "../../../Engine/Particles/Particle.h"
#include "../../../Engine/Renderer/Lights.h"
#include "../../../Engine/Audio/Source/Wwise_IDs.h"

CTorch::CTorch(CObjectManager* objectManager, XMFLOAT3 position, XMFLOAT3 tiltDirection) : IObject("Torch")
{
	//set object manager
	m_cpObjectManager = objectManager;

	//flame is off
	m_bFlameON = false;
	tilt = tiltDirection;

	// Render the mesh
	CMesh* tempMesh = CAssetManager::GetInstance()->GetPrefabMesh("Torch");
	SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), NULL, NULL,NULL, L"../Game/Assets/Art/2D/Textures/TorchDiffuse.dds"));

	//set position
	m_mWorld =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		position.x, position.y, position.z, 1
	};

	// Particles
	m_cpParticleManager = new CParticleManager(m_cpObjectManager);
	m_vnFireParticle = m_cpParticleManager->LoadEmitter("TorchFire.xml");
	m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(0) = GetPosition()->x;
	m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(1) = GetPosition()->y + PARTICLE_Y_OFFSET;
	m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(2) = GetPosition()->z;


	//rotate based on direction
	if (tiltDirection.x != 0.0f)
	{
		if (tiltDirection.x < 0.0f)
		{
			TranslateLocalX(m_mWorld, -TORCH_XZ_TRANSLATE);
			RotateLocalZ(m_mWorld, TORCH_TILT_ANGLE);
			m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(0) = GetPosition()->x - PARTICLE_XZ_OFFSET;
			m_fXOffset = -PARTICLE_XZ_OFFSET;
			m_fZOffset = 0.0f;
		}
		else
		{
			TranslateLocalX(m_mWorld, TORCH_XZ_TRANSLATE);
			RotateLocalZ(m_mWorld, -TORCH_TILT_ANGLE);
			m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(0) = GetPosition()->x + PARTICLE_XZ_OFFSET;
			m_fXOffset = PARTICLE_XZ_OFFSET;
			m_fZOffset = 0.0f;
		}
	}
	else if (tiltDirection.z != 0.0f)
	{
		if (tiltDirection.z < 0.0f)
		{
			TranslateLocalZ(m_mWorld, -TORCH_XZ_TRANSLATE);
			RotateLocalX(m_mWorld, -TORCH_TILT_ANGLE);
			m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(2) = GetPosition()->z - PARTICLE_XZ_OFFSET;
			m_fXOffset = 0.0f;
			m_fZOffset = -PARTICLE_XZ_OFFSET;
		}
		else
		{
			TranslateLocalZ(m_mWorld, TORCH_XZ_TRANSLATE);
			RotateLocalX(m_mWorld, TORCH_TILT_ANGLE);
			m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(2) = GetPosition()->z + PARTICLE_XZ_OFFSET;
			m_fXOffset = 0.0f;
			m_fZOffset = PARTICLE_XZ_OFFSET;
		}
	}

	//collider set up
	XMVECTOR mathCenter;// = XMLoadFloat3(&position);
	XMVECTOR mathExtent;// = XMLoadFloat3(&XMFLOAT3(1.0f, 1.0f, 1.0f));

	if (tiltDirection.x < 0.0f && tiltDirection.y == 0.0f && tiltDirection.z == 0.0f)//neg X
	{
		XMFLOAT3 tempPos = position;
		tempPos.x -= 500.0f;
		mathCenter = XMLoadFloat3(&tempPos);
		mathExtent = XMLoadFloat3(&XMFLOAT3(500.0f, 300.0f, 400));
	}
	else if (tiltDirection.x > 0.0f && tiltDirection.y == 0.0f && tiltDirection.z == 0.0f)//pos X
	{
		XMFLOAT3 tempPos = position;
		tempPos.x += 500.0f;
		mathCenter = XMLoadFloat3(&tempPos);
		mathExtent = XMLoadFloat3(&XMFLOAT3(500.0f, 300.0f, 400));
	}
	else if (tiltDirection.x == 0.0f && tiltDirection.y > 0.0f && tiltDirection.z == 0.0f)//pos Y
	{
		mathCenter = XMLoadFloat3(&position);
		mathExtent = XMLoadFloat3(&XMFLOAT3(1800.0f, 500.0f, 1800.0f));
	}
	else if (tiltDirection.x == 0.0f && tiltDirection.y == 0.0f && tiltDirection.z < 0.0f)//neg Z
	{
		XMFLOAT3 tempPos = position;
		tempPos.z -= 500.0f;
		mathCenter = XMLoadFloat3(&tempPos);
		mathExtent = XMLoadFloat3(&XMFLOAT3(400, 300.0f, 500.0f));
	}
	else if (tiltDirection.x == 0.0f && tiltDirection.y == 0.0f && tiltDirection.z > 0.0f)//pos Z
	{
		XMFLOAT3 tempPos = position;
		tempPos.z += 500.0f;
		mathCenter = XMLoadFloat3(&tempPos);
		mathExtent = XMLoadFloat3(&XMFLOAT3(400, 300.0f, 500.0f));
	}
	else
	{
		mathCenter = XMLoadFloat3(&position);
		mathExtent = XMLoadFloat3(&XMFLOAT3(1.0f, 1.0f, 1.0f));
	}

	XMFLOAT3 extents;
	XMStoreFloat3(&extents, mathExtent);
	XMFLOAT3 boundsCenter;
	XMStoreFloat3(&boundsCenter, mathCenter);

	AddCollider(new CCollider(false, new CAABB(boundsCenter, extents), true, false));

	//add to renderer
	GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	//set up point light
	TPointLight tempLight;
	tempLight.m_fPosition[0] = m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(0);
	tempLight.m_fPosition[1] = m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(1);
	tempLight.m_fPosition[2] = m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(2);
	tempLight.m_fColor[0] = 1.0f;
	tempLight.m_fColor[1] = 0.25f;
	tempLight.m_fColor[2] = 0.0f;
	tempLight.m_fRadius = 3000.0f;
	tempLight.m_fBrightness = 5.0f;
	AddLight(tempLight);

	//stop particles
	m_cpParticleManager->GetEmitter(m_vnFireParticle)->Stop();

	//set up audio
	m_cpTorchAudio = m_cpTorchAudio->Get();

}

void CTorch::SetWorldMatrix(const XMFLOAT4X4 world)
{
	IObject::SetWorldMatrix(world);
	m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(0) = GetPosition()->x;
	m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(1) = GetPosition()->y + PARTICLE_Y_OFFSET;
	m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(2) = GetPosition()->z;
	if (tilt.x != 0.0f)
	{
		if (tilt.x < 0.0f)
			m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(0) = GetPosition()->x - PARTICLE_XZ_OFFSET;
		else
			m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(0) = GetPosition()->x + PARTICLE_XZ_OFFSET;
	}
	else if (tilt.z != 0.0f)
	{
		if (tilt.z < 0.0f)
			m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(2) = GetPosition()->z - PARTICLE_XZ_OFFSET;
		else
			m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(2) = GetPosition()->z + PARTICLE_XZ_OFFSET;
	}


	m_vtPointLights[0].m_fPosition[0] = m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(0);
	m_vtPointLights[0].m_fPosition[1] = m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(1);
	m_vtPointLights[0].m_fPosition[2] = m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(2);

}

CTorch::~CTorch()
{
	delete m_cpParticleManager;

	if (m_bFlameON == true)
	{
		m_cpTorchAudio->PostEvent(AK::EVENTS::TORCHOFF, XMFLOAT3(GetPosition()->x, GetPosition()->y, GetPosition()->z));
		m_bFlameON = false;
	}

}

void CTorch::Update()
{
	if (m_bFlameON == true)
	{
		m_cpParticleManager->GetEmitter(m_vnFireParticle)->ePlay();
		m_cpParticleManager->Update();
	}

	//CAABB* pAABB = (CAABB*)m_pvColliders[0]->GetBounds();
	//DEBUG_UTIL::DrawAABBCollider(pAABB->Min(), pAABB->Max());
}

void CTorch::ActivateTorch()
{
	if (m_bFlameON == false)
	{
		m_bFlameON = true;
		GRAPHICS->AddLight(&m_vtPointLights[0]);
		m_cpTorchAudio->PostEvent(AK::EVENTS::TORCHON, XMFLOAT3(GetPosition()->x, GetPosition()->y, GetPosition()->z));
	}
}

void CTorch::AddToRenderer()
{
	if (m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	if (m_bFlameON == true)
	{
		if (&m_vtPointLights[0] != nullptr)
			GRAPHICS->AddLight(&m_vtPointLights[0]);
	}

	m_vnFireParticle = m_cpParticleManager->LoadEmitter("TorchFire.xml");
	m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(0) = GetPosition()->x + m_fXOffset;
	m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(1) = GetPosition()->y + PARTICLE_Y_OFFSET;
	m_cpParticleManager->GetEmitter(m_vnFireParticle)->GetPosition(2) = GetPosition()->z + m_fZOffset;
}
