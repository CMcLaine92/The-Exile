/***********************************************
* Filename:  		Minotaur.h
* Date:      		09/02/2015
* Mod. Date: 		----------
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:   		Is in control of the Minotaur Agent behaviour
************************************************/
#ifndef CMINOTAUR_H
#define CMINOTAUR_H

#include "../../../../Engine/Asset Manager/AssetManager.h"
#include "../../../../Engine/Animation/Animation.h"
#include "../../../../Engine/Animation/Mesh.h"
#include "../../../../Engine/AI/PathFinding.h"
#include "../../../../Engine/AI/Node.h"
#include "../../Agents/AgentState.h"
#include "../../../../Engine/AI/Boid.h"
#include "../../Camera/Camera.h"
class CMinotaurZone;
class CObjectManager;
class CAnimationManager;
class CAttack;
class CParticleManager;
//class CSteeringManager;
#define SLOW_TIME 1.5f


class CMinotaur : public CBoid
{

public:
	// Enums for Animation State
	enum EMinotaurAnimation{ eIdleAnim, eRunAnim, eTellAnim, eChargeAnim, eStompAnim, eDeathAnim, eEnrageAnim, eStunnedAnim, eAnimCount };
	enum EMinotaurStates{  ePatrolState, eHostileState, eDeadState, eNumStates };
	enum EMinotaurParticles { eBloodParticle, eBloodString, eBloodMist, eBloodMist2, eStompDustOut, eStompDustUp,eStepDust,eChargeParticle, eParticleCount };


	

private:

	// Animations
	CAnimation m_cpAnimations[eAnimCount];
	//CMesh m_cMesh;


	IAgentState<CMinotaur>* m_pStates[eNumStates];

	CRenderMesh* m_cDropShadow = nullptr;

	//CSteeringManager* m_pSteering;
	CParticleManager* m_pParticleManager;
	// The player object
	IObject* m_pPlayer;
	// Object to handle path planing
	CPathFinding* m_pPathPlanner;

	// Object Manager
	CObjectManager* m_cpObjectManager;

	//  Temp asset manager
	CAssetManager* m_pAssetManager = nullptr;

	// The Animation Manager
	CAnimationManager* m_cpAnimationManager = nullptr;

	//Camera Object
	CCamera* m_pCamera = nullptr;

	// Path to take
	vector<XMFLOAT3> m_vPath;
	XMFLOAT3 m_tSpawnPosition; 
	// Place on the path
	int m_nCurrentNodeOnPath = 0;
	// Current animation
	 int m_unCurrAnim;
	// The current state
	int m_nCurrState;

	unsigned int m_unParticleEmitters[eParticleCount];
	// Skeletons Health
	float m_fCurrentHealth;
	float m_fMoveSpeed;
	// Skeletons Max health
	float m_fMaxHealth = 100;

	// Is active/alive bool
	bool m_bIsActive = false;
	bool m_bIsGrounded = false;

	bool m_bInvincible = 0.0f;
	float m_fInvincibilityTimer = 0.0f;
	float m_fSlowTimer = 0.0f;
	float m_fStompTimer = 0.0f;

	bool m_bChargeParticlePlaying = false;

	//Door Objects for use in Ray Casting
	vector<IObject*> m_vDoors;

	vector<int> m_vWaypoints;
	
	bool m_bStepBool = false;

	CMinotaurZone* m_pMinotaurZone = nullptr;
public:

	CMinotaur();
	CMinotaur(const XMFLOAT3& spawnPosition, CObjectManager* _manager, CAnimationManager* _animManager = nullptr);
	virtual ~CMinotaur();

	void Update();
	void CollisionResponse(IObject* _cCollidedObject);
	void SwitchAnimation(EMinotaurAnimation _animation);
	void PlayParticle(unsigned int _toPlay, XMFLOAT3 _relativePos);
	void ChangeState(int _newState);
	void UpdateState();
	void AdjustRotation();
	void Activate();
	void DeActivate();
	void TakeDamage(int damageAmount);
	int GetWaypoint(int index) { return m_vWaypoints[index]; }
	void Kill();
	bool RayCastToPlayer(float& fHitTime);
	bool RayCastInfinite(float& fHitTime,XMFLOAT3& f3CollisionPoint);
	bool RayCastForward(float& fHitTime);
	void AddWaypoint(int waypoint){ m_vWaypoints.push_back(waypoint); }

	/*********************************
	MUTATORS
	************************************/
	void SetPath(vector<XMFLOAT3>& _path) { m_vPath = _path; }
	void SetPlayer(IObject* _player) { m_pPlayer = _player; }
	//void SetCanAttack(bool _atk) { m_bCanAttack = _atk; }
	void SetNodeOnPath(int _node) { m_nCurrentNodeOnPath = _node; }
	void SetMoveSpeed(float _moveSpeed) { m_fMoveSpeed = _moveSpeed; }
	void SetStompTimer(float _timer) { m_fStompTimer = _timer; }
	
	/*********************************
	ACCESSORS
	************************************/
	IAgentState<CMinotaur>* GetState(EMinotaurStates state) { return m_pStates[state]; }

	CObjectManager* GetObjectManager() const { return m_cpObjectManager; }
	IAgentState<CMinotaur>* GetCurrState() const { return m_pStates[m_nCurrState]; }
	IObject* GetPlayer() const { return m_pPlayer; }
	const XMFLOAT3& GetSpawnPosition() const { return m_tSpawnPosition; }
	CPathFinding* GetPathPlanner() const { return m_pPathPlanner; }
	vector<XMFLOAT3>& GetPath() { return m_vPath; }
	bool GetIsActive() const { return m_bIsActive; }
	int GetCurNodeOnPath() const { return m_nCurrentNodeOnPath; }
	float GetMoveSpeed() const { return m_fMoveSpeed; }
	float GetStompTimer() const { return m_fStompTimer; }
	vector<IObject*>& GetDoors() { return m_vDoors; }
	CCamera* GetCamera(){ return m_pCamera; }

	void PlayStepParticle();
	void PlayChargeParticle();
	void PlayStopParticle();
	void PlayHurtParticle();
	void StopChargeParticle();
	const CAnimation& GetAnimation(EMinotaurAnimation anim) const { return m_cpAnimations[anim]; }
	const CAnimation& GetCurrAnimation() const { return m_cpAnimations[m_unCurrAnim]; }
	 CAnimation& GetCurrAnimation() { return m_cpAnimations[m_unCurrAnim]; }

	void AddToRenderer();

};

#endif