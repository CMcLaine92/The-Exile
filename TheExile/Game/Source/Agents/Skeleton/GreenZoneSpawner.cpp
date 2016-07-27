
#include "GreenZoneSpawner.h"
#include "Skeleton.h"

CGreenZoneSpawner::CGreenZoneSpawner(CObjectManager* objManager, CAnimationManager* animManager, CDayNight* dayNight, XMFLOAT3 position)
{
	/*XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());
	for (size_t i = 0; i < 8; i++)
	{
		CSkeleton* skel = new CSkeleton(objManager, animManager, dayNight);
		skel->SetSpawnPoint(position);
		m_vSkeletons.push_back(skel);
	}*/
	//m_bActive = false;
	//m_bMaxSpawnReached = false;
	//m_nNumSpawning = 1;
	//m_nNumSpawned = 0;
	//m_fIncreaseTimer = 0.0f;
}
CGreenZoneSpawner::~CGreenZoneSpawner()
{
	//VECTOR_DELETE_ALL(m_vSkeletons);
}
void CGreenZoneSpawner::Update()
{
	/*if (m_bActive)
	{
		if (m_fIncreaseTimer > 0.0f && m_bMaxSpawnReached == false)
		{
			m_fIncreaseTimer -= DELTA_TIME();
			if (m_fIncreaseTimer < 0.0f)
			{
				m_nNumSpawning *= 2;
				if (m_nNumSpawning == 8)
					m_bMaxSpawnReached = true;
			}
		}
		if (m_nNumSpawned < m_nNumSpawning)
		{
			m_nNumSpawned = 0;
			for (size_t i = 0; i < m_nNumSpawning; i++)
			{
				if (m_vSkeletons[i]->GetIsActive())
					m_nNumSpawned++;
				else
				{
					m_vSkeletons[i]->Activate();
					m_nNumSpawned++;
				}
					
			}
		}

		m_bActive = false;
	}
	else
	{
		m_nNumSpawning = 1;
	}*/
}
void CGreenZoneSpawner::CollisionResponse(IObject * _cCollidedObject)
{
	/*if (_cCollidedObject->GetTag() == "Player")
	{
		if (m_fIncreaseTimer <= 0.0f)
			m_fIncreaseTimer = 10.0f;
		m_bActive = true;
	}*/
}