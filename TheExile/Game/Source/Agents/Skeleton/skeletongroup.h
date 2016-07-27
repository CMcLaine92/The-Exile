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
#define REPATH_TIME 1.5f
#define ATTACK_TIME 1.0f

#define NUM_SKELETONS 2

#define DESPAWN_RANGE 2000

#define PATH_OFFSET_VALUE 50
#define SPAWN_OFFSET_VALUE 250

#define ALIGNMENT_STRENGTH 1

#define SEPARATION_DISTANCE 100
#define SEPARATION_STRENGTH 1

class CPathPlanner;
class CSkeleton;
class CNode;
class CObjectManager;
class IObject;
class CAnimationManager;
class CDayNight;
class CSpawnControl;

class CSkeletonGroup
{
	// Path planner
	CPathPlanner* m_cpPathPlanner;

	// Object manager
	CObjectManager* m_cpObjManager;

	// Spawning location
	XMFLOAT3 m_vecSpawnLocation;
	CDayNight* m_pDayNight;

	// The path
	vector<CNode*> m_vPath;

	// Vector of Skeletons
	vector<IObject*> m_vSkeletons;

	// The player pointer
	IObject* m_cpPlayer;

	CSpawnControl* m_pSpawnController;

	int m_nActiveSkeletons;

	// Is the group active
	bool m_bIsActive = false;

	// Bool to allow attack
	bool m_bAllowAttack = true;

	bool m_bRepath = true;
	bool m_bSkeletonsSpawned = false;

	// Allow attack timer
	float m_fAttackTimer = 1.0f;

	// Find a new path every x seconds
	float m_fSpawnCoolDown = 0.0f;
	unsigned int m_unSpawnIndex = 0;


public:
	CSkeletonGroup(CObjectManager* _objManager, CAnimationManager* _animManager = nullptr, CDayNight* _dayNight = nullptr,CSpawnControl* spawnControl = nullptr, unsigned int spawnIndex = 0);
	~CSkeletonGroup();

	void BuildPath();
	void Update();
	void MakeActive();
	void SetPlayer(IObject* _thePlayer) { m_cpPlayer = _thePlayer; }
	void SetSpawnLocation(XMFLOAT3 _location) { m_vecSpawnLocation = _location; }

	void SetActive(bool _isActive) { m_bIsActive = _isActive; }
	void SetAllowAttack(bool _allow) { m_bAllowAttack = _allow; }
	void KillSkeleton() { m_nActiveSkeletons--; }

	bool IsActive() const { return m_bIsActive; }
	bool AllowAttack() const { return m_bAllowAttack; }

	XMVECTOR CalculateSeparation(CSkeleton* _current);
	XMVECTOR CalculateAlignment();

	void SpawnSkeletons(XMFLOAT3 pos, unsigned int spawnIndex);

	void Repath();

	void AddToRenderer();

private:


};



#endif