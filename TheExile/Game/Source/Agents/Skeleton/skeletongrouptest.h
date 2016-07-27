/***********************************************
* Filename:  		SkeletonGroup.h
* Date:      		08/21/2015
* Mod. Date: 		----------
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:   		Is in control of the Skeleton Group behaviours
************************************************/
#ifndef _SKELETONGROUP_H
#define _SKELETONGROUP_H

#include "../../../../Engine/Core/stdafx.h"

class CPathPlanner;
class CSkeleton;
class CNode;
class CObjectManager;
class IObject;

class CSkeletonGroup
{
	// Path planner
	CPathPlanner* m_cpPathPlanner;

	// Object manager
	CObjectManager* m_cpObjManager;

	// Spawning location
	XMFLOAT3 m_vecSpawnLocation;

	// The path
	vector<CNode*> m_vPath;

	// Vector of Skeletons
	vector<IObject*> m_vSkeletons;

	// The player pointer
	IObject* m_cpPlayer;

public:
	CSkeletonGroup(CObjectManager* _objManager, XMFLOAT3 _location, IObject* _player);
	~CSkeletonGroup();

	void SetPlayer(IObject* _thePlayer) { m_cpPlayer = _thePlayer; }
private:

	void SetSpawnLocation(XMFLOAT3 _location) { m_vecSpawnLocation = _location; }

};



#endif