/***********************************************
* Filename:  		SkeletonController.h
* Date:      		9/7/2015
* Mod. Date: 		9/7/2015
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:			Handles the activation and deactivation
*					of Skeletons
************************************************/

#ifndef _SKELETON_CONTROLLER_H
#define _SKELETON_CONTROLLER_H

#define SPAWN_OFFSET_VALUE 250
#define NUM_SKELETONS 21
#define SKELETONS_PER_GROUP 3

#include <queue>


class CSkeleton;
class CDayNight;
class CObjectManager;
class CAnimationManager;
class CSkeletonSpawner;

class CSkeletonController
{


public:

	CSkeletonController(CObjectManager* _objectManager, CAnimationManager* animManager, std::vector<XMFLOAT4X4>& spawners, CDayNight* _dayNight = nullptr);

	virtual ~CSkeletonController();

	void Update();
	void SetDayNight(CDayNight* dayNight) { m_cpDayNight = dayNight; }
	void AddToRenderer();
	bool SpawnSkeletons(XMFLOAT3 position);
	void KillSkeleton() { m_nNumActiveSkeletons--; }

private:

	std::vector<CSkeleton*> m_vSkeletonPool;
	std::vector<CSkeletonSpawner*> m_vSpawnPoints;

	CDayNight* m_cpDayNight;
	int m_nNumActiveSkeletons;
	bool m_bNightTime;

	



};

#endif