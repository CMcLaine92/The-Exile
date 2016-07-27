/***********************************************
* Filename:  		SpawnControl.h
* Date:      		08/24/2015
* Mod. Date: 		----------
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:   		Controls when and where to spawn enemies
************************************************/

#ifndef _SPAWN_CONTROL_H
#define _SPAWN_CONTROL_H



#define SPAWN_RADIUS 2000.0f
#define ACTIVATE_DISTANCE 1000.0f
#define RESET_RADIUS 15000
#define NUM_SKELETON_GROUPS 4
#define MINOTAUR_COUNT 1


class CLevelLoader;
class CSpawnerObject;
class CObjectManager;
class CAnimationManager;
class COrcController;
class CMinotaur;
class IObject;
class CMinotaurController;
class CSkeletonController;

class CDayNight;
class CMaze;

class CSpawnControl
{
public:
	CSpawnControl(CMaze* _loader, CObjectManager* _objManager, CAnimationManager* _animManager = nullptr, CDayNight* _dayNight = nullptr);
	~CSpawnControl();

	void Update();

	void SetPlayer(IObject* _player) { m_ipPlayer = _player; }


	void AddToRenderer();

private:
	// Loader to access spawn points
	CMaze* m_cpLevel;
	CDayNight* m_cpDayNight;
	// Locations of spawn objects
	vector<XMFLOAT4X4> m_vSkeletonSpawnObjects;
	vector<XMFLOAT4X4> m_vNightSkeletonSpawnObjects;

	vector<XMFLOAT4X4> m_vMinotaurSpawnObjects;
	vector<XMFLOAT4X4> m_vOrcSpawnObjects;



	// PreAllocated Skeleton Groups
	//vector<CSkeletonGroup*> m_vSkeletonGroups;
	CSkeletonController* m_cpSkeletonController;
	CMinotaurController* m_cpMinotaurController;
	COrcController* m_cpOrcController;

	


	//// Bools for each spawnObject
	//vector<bool> m_vSkelSpawnAvailable;
	//vector<bool> m_vMinotaurSpawnAvailable;
	//vector<bool> m_vOrcSpawnAvailable;



	float m_fTimer = 0.0f;
	// The player
	IObject* m_ipPlayer;
};


#endif