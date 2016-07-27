#include "SkeletonController.h"
#include "SkeletonSpawner.h"
#include "Skeleton.h"

#include "../../../../Engine/Object Manager/ObjectManager.h"
CSkeletonController::CSkeletonController(CObjectManager* _objectManager, CAnimationManager* animManager, std::vector<XMFLOAT4X4>& spawners, CDayNight* _dayNight)
{
	

	m_cpDayNight = _dayNight;
	m_bNightTime = false;
	m_nNumActiveSkeletons = 0;
	for (size_t i = 0; i < NUM_SKELETONS; i++)
	{
		// Create a new skeleton as a Object
		CSkeleton* newSkeleton = new CSkeleton(_objectManager, animManager, _dayNight, this);

		// Cast to a Skeleton and set player pointer
		newSkeleton->SetPlayer(_objectManager->GetPlayer());
		// Set the parent
		// Push the skeleton into the vector
		m_vSkeletonPool.push_back(newSkeleton);
	}

	for (size_t currSpawner = 0; currSpawner < spawners.size(); currSpawner++)
	{
		float3 pos(spawners[currSpawner]._41, spawners[currSpawner]._42, spawners[currSpawner]._43);
		CSkeletonSpawner* spawner = new CSkeletonSpawner(this, pos);
		m_vSpawnPoints.push_back(spawner);
		_objectManager->AddObject(spawner, CObjectManager::eObjectType::Static);

	}
}

CSkeletonController::~CSkeletonController()
{
	VECTOR_DELETE_ALL(m_vSkeletonPool);
	VECTOR_DELETE_ALL(m_vSpawnPoints);

}

void CSkeletonController::Update()
{
	
}
void CSkeletonController::AddToRenderer()
{
	for (unsigned int i = 0; i < m_vSkeletonPool.size(); i++)
	{
		m_vSkeletonPool[i]->AddToRenderer();
	}
}
bool  CSkeletonController::SpawnSkeletons(XMFLOAT3 position)
{
	XMFLOAT3 vecOffsets[4] = { { -SPAWN_OFFSET_VALUE, 0, SPAWN_OFFSET_VALUE }, { SPAWN_OFFSET_VALUE, 0, SPAWN_OFFSET_VALUE },
	{ -SPAWN_OFFSET_VALUE, 0, -SPAWN_OFFSET_VALUE }, { SPAWN_OFFSET_VALUE, 0, -SPAWN_OFFSET_VALUE } };

	int skeletonsRemaining = NUM_SKELETONS - m_nNumActiveSkeletons;
	if (skeletonsRemaining >= SKELETONS_PER_GROUP)
	{
		int skeletonsFound = 0;
		for (unsigned int i = 0; i < NUM_SKELETONS; i++)
		{
			if (m_vSkeletonPool[i]->GetIsActive() == false)
			{
				XMFLOAT3 spawnPoint;
				XMVECTOR vPosition = XMLoadFloat3(&position);
				XMVECTOR vOffset = XMLoadFloat3(&vecOffsets[skeletonsFound]);
				XMStoreFloat3(&spawnPoint, (vPosition + vOffset));
				m_vSkeletonPool[i]->Activate(spawnPoint);
				m_nNumActiveSkeletons++;
				skeletonsFound++;
			}
			if (skeletonsFound == SKELETONS_PER_GROUP)
				return true;
		}
	}
	return false;

}
