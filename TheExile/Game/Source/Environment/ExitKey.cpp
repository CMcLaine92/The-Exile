
#include "ExitKey.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Object Manager/ObjectManager.h"
#include "../../../Engine/Core/Collision/AABB.h"
#include "../../../Engine/Particles/ParticleManager.h"
#include "../../../Engine/Particles/Emitter.h"
#include "../../../Engine/Particles/Particle.h"
#include "../../../Engine/Renderer/Lights.h"

CExitKey::CExitKey(XMFLOAT3 position, CObjectManager* objectManager) : IObject("ExitKey")
{
	m_bActive = false;
	m_bBounceUp = true;
	m_fBounceTime = 0.0f;
	m_f3SpawnPoint = position;

	//set position and scale
	XMStoreFloat4x4(&m_mWorld, XMMatrixScaling(5.0f, 5.0f, 5.0f) * XMMatrixTranslation(m_f3SpawnPoint.x, SPAWN_OFFSET_Y, m_f3SpawnPoint.z));

	//set rendermesh
	CMesh* theMesh = CAssetManager::GetInstance()->GetPrefabMesh("ExitKey");
	m_cpRenderMesh = new CRenderMesh(theMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Failbot_ExitKey_Diffuse.dds");
	m_cpRenderMesh->SetNormals(L"../Game/Assets/Art/2D/Textures/Failbot_ExitKey_Normal.dds");
	m_cpRenderMesh->GetSpecular() = 1.5f;
	m_cpRenderMesh->GetTransparent() = true;
	m_cpRenderMesh->GetRender() = false;
	//m_cpRenderMesh->SetEdgeGlow(true);//only if we can change the color from black
	GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	//flip the mesh 180*
	RotateLocalZ(m_mWorld, 180.0f);

	//set collider
	XMFLOAT3 extents = XMFLOAT3{ 100, 400, 100 };
	XMFLOAT3 tempPos = XMFLOAT3{ GetPosition()->x, SPAWN_OFFSET_Y, GetPosition()->z };
	AddCollider(new CCollider(false, new CAABB(tempPos, extents), false));

	TPointLight tempLight;
	tempLight.m_fPosition[0] = GetPosition()->x;
	tempLight.m_fPosition[1] = LIGHT_OFFSET_Y;
	tempLight.m_fPosition[2] = GetPosition()->z;
	tempLight.m_fColor[0] = 0.75f;
	tempLight.m_fColor[1] = 0.75f;
	tempLight.m_fColor[2] = 0.75f;
	tempLight.m_fRadius = 500.0f;
	tempLight.m_fBrightness = 10.0f;
	AddLight(tempLight);

	//set up paticles
	m_cpParticleManager = new CParticleManager(objectManager);
	m_nParticleID = m_cpParticleManager->LoadEmitter("ExitKeyParticles.xml");
	m_cpParticleManager->GetEmitter(m_nParticleID)->GetPosition(0) = m_f3SpawnPoint.x;
	m_cpParticleManager->GetEmitter(m_nParticleID)->GetPosition(1) = m_f3SpawnPoint.y + SPAWN_OFFSET_Y;
	m_cpParticleManager->GetEmitter(m_nParticleID)->GetPosition(2) = m_f3SpawnPoint.z;
	m_cpParticleManager->GetEmitter(m_nParticleID)->Stop();

	//add to object manager
	objectManager->AddObject(this, CObjectManager::Static);
}

CExitKey::~CExitKey()
{
	SAFE_DELETE(m_cpParticleManager);
}

void CExitKey::Update()
{
	if (m_bActive == true)
	{
		//rotate
		RotateLocalY(m_mWorld, 5.0f);
		
		//verticle movement
		BounceEffect();

		//update particles
		m_cpParticleManager->GetEmitter(m_nParticleID)->GetPosition(0) = GetPosition()->x;
		m_cpParticleManager->GetEmitter(m_nParticleID)->GetPosition(1) = GetPosition()->y - PARTICLE_OFFSET_Y;
		m_cpParticleManager->GetEmitter(m_nParticleID)->GetPosition(2) = GetPosition()->z;
		m_cpParticleManager->Update();
	}

	//CAABB* pAABB = (CAABB*)m_pvColliders[0]->GetBounds();
	//DEBUG_UTIL::DrawAABBCollider(pAABB->Min(), pAABB->Max());
}

void CExitKey::Activate()
{
	//activate
	m_bActive = true;
	m_cpRenderMesh->GetRender() = true;
	m_pvColliders[0]->SetActive(true);

	//add light
	GRAPHICS->AddLight(&m_vtPointLights[0]);

	//start particles
	m_cpParticleManager->GetEmitter(m_nParticleID)->ePlay();
}

void CExitKey::DeAactive()
{
	//deActivate
	m_bActive = false;
	m_cpRenderMesh->GetRender() = false;
	m_pvColliders[0]->SetActive(false);

	//remove light
	GRAPHICS->RemoveLight(&m_vtPointLights[0]);

	//stop particles
	m_cpParticleManager->GetEmitter(m_nParticleID)->Stop();
}

void CExitKey::BounceEffect()
{
	//m_fBounceTime += DELTA_TIME();
	//if (m_fBounceTime > BOUNCE_TIME)
	//{
	//	m_bBounceUp = !m_bBounceUp;
	//	m_fBounceTime = 0.0f;
	//}

	if (m_mWorld._42 < 125.0f)
	{
		m_bBounceUp = true;
		m_mWorld._42 = 125.0f;
	}
	if (m_mWorld._42 > 250.0f)
	{
		m_bBounceUp = false;
		m_mWorld._42 = 250.0f;
	}

	if (m_bBounceUp == true)
		m_mWorld._42 += 3.0f;
	else
		m_mWorld._42 -= 3.0f;

	//if (m_bBounceUp == true)//move up
	//	TranslateLocalY(m_mWorld, 1.0f);
	//else//move down
	//	TranslateLocalY(m_mWorld, -1.0f);
}

void CExitKey::AddToRenderer()
{
	if (m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	if (m_bActive == true)
	{
		if (&m_vtPointLights[0] != nullptr)
			GRAPHICS->AddLight(&m_vtPointLights[0]);
	}

	m_nParticleID = m_cpParticleManager->LoadEmitter("ExitKeyParticles.xml");
	m_cpParticleManager->GetEmitter(m_nParticleID)->GetPosition(0) = m_f3SpawnPoint.x;
	m_cpParticleManager->GetEmitter(m_nParticleID)->GetPosition(1) = m_f3SpawnPoint.y + SPAWN_OFFSET_Y;
	m_cpParticleManager->GetEmitter(m_nParticleID)->GetPosition(2) = m_f3SpawnPoint.z;
}
