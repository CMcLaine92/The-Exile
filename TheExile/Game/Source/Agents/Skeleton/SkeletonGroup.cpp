/***********************************************
* Filename:  		SkeletonGroup.cpp
* Date:      		08/21/2015
* Mod. Date: 		----------
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:   		Is in control of the Skeleton Group behaviours
************************************************/
#include "SkeletonGroup.h"
#include "Skeleton.h"
#include "../../../../Engine/Object Manager/ObjectManager.h"
#include "../../../../Engine/Core/Collision/AABB.h"
#include "../../../../Engine/Core/Collision/Capsule.h"
#include "../../../../Engine/AI/PathFinding.h"
#include "../../../../Engine/Core/Messages/AddObjectMessage.h"
#include "../../Environment/DayNight.h"
#include "../SpawnControl.h"
// Defines


CSkeletonGroup::CSkeletonGroup(CObjectManager* _objManager, CAnimationManager* _animManager, CDayNight* _dayNight, CSpawnControl* spawnControl, unsigned int spawnIndex)
{
	m_unSpawnIndex = spawnIndex;
	m_pSpawnController = spawnControl;
	m_pDayNight = _dayNight;
	m_cpObjManager = _objManager;
	m_cpPlayer = _objManager->GetPlayer();
	m_nActiveSkeletons = 0;
	m_bSkeletonsSpawned = false;
	XMFLOAT3 vecOffsets[4] = { { -PATH_OFFSET_VALUE, 0, PATH_OFFSET_VALUE }, { PATH_OFFSET_VALUE, 0, PATH_OFFSET_VALUE },
	{ -PATH_OFFSET_VALUE, 0, -PATH_OFFSET_VALUE }, { PATH_OFFSET_VALUE, 0, -PATH_OFFSET_VALUE } };

	


}

CSkeletonGroup::~CSkeletonGroup()
{
	// Delete skeletons
	for (size_t skel = 0; skel < m_vSkeletons.size(); skel++)
	{
		delete m_vSkeletons[skel];
	}
}

void CSkeletonGroup::Repath()
{
	BuildPath();
	m_bRepath = false;

}

void CSkeletonGroup::Update()
{
	


	//Update the spawn timer if its good to update
	//if (m_fSpawnCoolDown > 0.0f)
	//{
	//	// get the distance to the player
	//	XMVECTOR playerPos = XMLoadFloat3(m_cpPlayer->GetPosition());
	//	XMVECTOR groupPos = XMLoadFloat3(&m_vecSpawnLocation);
	//	XMVECTOR dis = XMVector3Length(playerPos - groupPos);

	//	m_fSpawnCoolDown -= DELTA_TIME();

	//	if (dis.m128_f32[0] > 5000.0f) //if the players far enough away, reset the spawn timer 
	//		m_fSpawnCoolDown = -1.0f;

	//	if (m_fSpawnCoolDown < 0.0f)
	//	{
	//		m_pSpawnController->GetSkelSpawnStates()[m_unSpawnIndex] = true;
	//		m_bIsActive = false;
	//		return;
	//	}
	//}

	if (m_nActiveSkeletons == 0 )
	{
		m_fSpawnCoolDown = 60.0f;
	}



}

void CSkeletonGroup::SpawnSkeletons(XMFLOAT3 pos, unsigned int spawnIndex)
{
	m_unSpawnIndex = spawnIndex;
	m_vecSpawnLocation = pos;
	m_bIsActive = true;
	m_bSkeletonsSpawned = true;
	XMVECTOR vecOffsets[NUM_SKELETONS] = { { -SPAWN_OFFSET_VALUE, 0, SPAWN_OFFSET_VALUE }/*, { SPAWN_OFFSET_VALUE, 0, SPAWN_OFFSET_VALUE }*/
	/*,{ -SPAWN_OFFSET_VALUE, 0, -SPAWN_OFFSET_VALUE }, { SPAWN_OFFSET_VALUE, 0, -SPAWN_OFFSET_VALUE }*/ };

	//m_nActiveSkeletons = NUM_SKELETONS;
	//for (size_t i = 0; i < m_vSkeletons.size(); i++)
	//{
	//	// Convert for math
	//	XMVECTOR newPosition = XMLoadFloat3(&m_vecSpawnLocation);

	//	// Add the offset to the spawn location
	//	newPosition = newPosition + vecOffsets[i];


	//	// Convert back
	//	XMFLOAT3 realPosition; XMStoreFloat3(&realPosition, newPosition);

	//	reinterpret_cast<CSkeleton*>(m_vSkeletons[i])->SetupSpawn(realPosition);

	//	reinterpret_cast<CSkeleton*>(m_vSkeletons[i])->Activate();

	//}

}
void CSkeletonGroup::MakeActive()
{
	// For offset spawning, will break with more then 4
	/*,{ -SPAWN_OFFSET_VALUE, 0, -SPAWN_OFFSET_VALUE }, { SPAWN_OFFSET_VALUE, 0, -SPAWN_OFFSET_VALUE }*/

	for (size_t i = 0; i < m_vSkeletons.size(); i++)
	{
		//// Convert for math
		//XMVECTOR newPosition = XMLoadFloat3(&_location);

		//// Add the offset to the spawn location
		//newPosition = newPosition + vecOffsets[i];


		//// Convert back
		//XMFLOAT3 realPosition; XMStoreFloat3(&realPosition, newPosition);

		//XMFLOAT3 start = realPosition;
		//XMFLOAT3 end = realPosition;
		//start.y = 55.0f;
		//end.y = 240.0f;

		//realPosition.y = 0.0f;

		//// Set the skeletons position
		//m_vSkeletons[i]->SetPosition(realPosition);
		//reinterpret_cast<CSkeleton*>(m_vSkeletons[i])->SetSpawnPoint(realPosition);
		//reinterpret_cast<CAABB*>(m_vSkeletons[i]->GetColliders()[0]->GetBounds())->SetCenter(realPosition);

		//reinterpret_cast<CCapsule*>(m_vSkeletons[i]->GetColliders()[1]->GetBounds())->SetStart(start);
		//reinterpret_cast<CCapsule*>(m_vSkeletons[i]->GetColliders()[1]->GetBounds())->SetEnd(end);

		//// Create a AddObj Message

		// Activte the skeleton
		// Mark as activated
	}

	// Find a new path
	//	BuildPath();
}

void CSkeletonGroup::BuildPath()
{
	// For offset spawning, will break with more then 4

	XMVECTOR vecOffsets[NUM_SKELETONS] = { { -SPAWN_OFFSET_VALUE, 0, SPAWN_OFFSET_VALUE }, { SPAWN_OFFSET_VALUE, 0, SPAWN_OFFSET_VALUE }/*
	{ -SPAWN_OFFSET_VALUE, 0, -SPAWN_OFFSET_VALUE }, { SPAWN_OFFSET_VALUE, 0, -SPAWN_OFFSET_VALUE }*/ };

	// The new Path
	vector<XMFLOAT3> newPath;

	// Find an active skeleton
	for (size_t i = 0; i < m_vSkeletons.size(); i++)
	{
		CSkeleton* curSkeleton = reinterpret_cast<CSkeleton*>(m_vSkeletons[i]);
		if (curSkeleton->GetIsActive())
		{
			// Path to offset player position
			XMVECTOR mathOffsetPos = XMLoadFloat3(m_cpPlayer->GetPosition());
			mathOffsetPos += vecOffsets[i];
			XMFLOAT3 offsetPosition; XMStoreFloat3(&offsetPosition, mathOffsetPos);

			// Find the target node
			int nPlayerNodeIndex = curSkeleton->GetPathPlanner()->FindClosestNode(&offsetPosition);

			// Get the closest nodes
			int nSkeletonNodeIndex = curSkeleton->GetPathPlanner()->FindClosestNode(curSkeleton->GetPosition());

			// Build a path to the target
			newPath = curSkeleton->GetPathPlanner()->GeneratePath(nSkeletonNodeIndex, nPlayerNodeIndex, false);

			if (newPath.empty())
			{
				newPath.push_back(*curSkeleton->GetPlayer()->GetPosition());
				newPath.push_back(*curSkeleton->GetPosition());

			}
			else if (newPath.size() == 1)
			{
				newPath.push_back(*curSkeleton->GetPlayer()->GetPosition());

			}

			// Set the new path
			curSkeleton->SetPath(newPath);

			// Set the node on path
			curSkeleton->SetNodeOnPath(newPath.size() - 1);
		}
	}

	// Went back to path finding for each
	//// Set each skeleton's new path
	//for (size_t ske = 0; ske < m_vSkeletons.size(); ske++)
	//{
	//	reinterpret_cast<CSkeleton*>(m_vSkeletons[ske])->SetPath(newPath);
	//
	//	// Set the current node
	//	reinterpret_cast<CSkeleton*>(m_vSkeletons[ske])->SetNodeOnPath(newPath.size() - 1);
	//}
}

XMVECTOR CSkeletonGroup::CalculateSeparation(CSkeleton* _current)
{
	XMVECTOR mathOutput = XMVECTOR();

	// For all skeletons
	for (size_t i = 0; i < m_vSkeletons.size(); i++)
	{
		// Cast the skeleton for future use
		CSkeleton* other = reinterpret_cast<CSkeleton*>(m_vSkeletons[i]);

		// Convert the positions for math
		XMVECTOR mathOtherPos = XMLoadFloat3(other->GetPosition());
		XMVECTOR mathCurrentPos = XMLoadFloat3(_current->GetPosition());

		// Find the distance between them
		XMVECTOR mathFromVector = mathCurrentPos - mathOtherPos;
		float fDistance = XMVector3Length(mathFromVector).m128_f32[0];

		// If within safe distance
		if (fDistance < SEPARATION_DISTANCE)
		{
			mathFromVector = XMVector3Normalize(mathFromVector);

			mathFromVector *= (SEPARATION_DISTANCE - fDistance) / SEPARATION_DISTANCE;

			mathOutput += mathFromVector;
		}
	}

	// Rescale the velocity
	if (XMVector3Length(mathOutput).m128_f32[0] > 1.0f)
		mathOutput = XMVector3Normalize(mathOutput);

	// Scale by modifier
	return mathOutput * SEPARATION_STRENGTH;



}

XMVECTOR CSkeletonGroup::CalculateAlignment()
{

	// Calculate the avarage velocity
	XMVECTOR avgVelocity = XMVECTOR();
	for (size_t i = 0; i < m_vSkeletons.size(); i++)
	{
		CSkeleton* other = reinterpret_cast<CSkeleton*>(m_vSkeletons[i]);
		XMFLOAT3 vec = other->GetWorldVelocity();
		XMVECTOR otherVelocity = XMLoadFloat3(&vec);
		avgVelocity += otherVelocity;
	}
	avgVelocity /= (float)m_vSkeletons.size();
	avgVelocity.m128_f32[1] = 0.0f;

	if (XMVector3Length(avgVelocity).m128_f32[0] > 1.0f)
		avgVelocity = XMVector3Normalize(avgVelocity);

	return avgVelocity * ALIGNMENT_STRENGTH;

}

void CSkeletonGroup::AddToRenderer()
{
	for (unsigned int i = 0; i < m_vSkeletons.size(); i++)
	{
		m_vSkeletons[i]->AddToRenderer();
	}
}
