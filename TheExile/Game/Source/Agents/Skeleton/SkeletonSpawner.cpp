#include "SkeletonSpawner.h"
#include "SkeletonController.h"
#include "../../../../Engine/Core/Collision/AABB.h"



CSkeletonSpawner::CSkeletonSpawner(CSkeletonController* controller, XMFLOAT3 position) : IObject("SkeletonSpawn")
{
	color = float3(0.0f, 1.0f, 0.0f);
	m_vPosition = position;
	//m_vPosition.y = 500.0f;
	m_cpRenderMesh = nullptr;
	m_cpSpawnController = controller;
	XMStoreFloat4x4(&m_mWorld, XMMatrixTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z));
	m_vPosition = position;
	XMFLOAT3 center = m_vPosition;
	center.y = 500.0f;
	XMFLOAT3 extents(1000.0f, 500.0f, 1000.0f);
	AddCollider(new CCollider(false, new CAABB(center, extents), true, false));

	
}
CSkeletonSpawner::~CSkeletonSpawner()
{
	m_cpSpawnController = nullptr;
}
void CSkeletonSpawner::Update()
{
	//CAABB* aabb = (CAABB*)(GetColliders()[0]->GetBounds());
	//DEBUG_UTIL::DrawAABBCollider(aabb->Min(), aabb->Max(), color);


	if (m_fSpawnCoolDown > 0.0f)
		m_fSpawnCoolDown -= DELTA_TIME();

	else
		color = float3(0.0f, 1.0f, 0.0f);
}
void CSkeletonSpawner::CollisionResponse(IObject * _cCollidedObject)
{
	if (m_fSpawnCoolDown <= 0.0f)
	{
		if (_cCollidedObject->GetTag() == "Player")
		{
			m_cpSpawnController->SpawnSkeletons(m_vPosition);
			m_fSpawnCoolDown = SPAWN_COOLDOWN;
			color = float3(1.0f, 0.0f, 0.0f);

		}
	}
	
}