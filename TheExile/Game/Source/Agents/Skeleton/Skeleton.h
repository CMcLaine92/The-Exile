/***********************************************
* Filename:  		Skeleton.h
* Date:      		08/16/2015
* Mod. Date: 		----------
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:   		Is in control of the Skeleton Agent behaviour
************************************************/
#ifndef CSKELETON_H
#define CSKELETON_H
#include "../../../../Renderer/Lights.h"
#include "../../../../Engine/AI/Boid.h"

class CObjectManager;
class CAnimationManager;
class CAttack;
class CSkeletonIdle;
class CSkeletonAttack;
class CSkeletonHurt;
class CSkeletonFollow;
class CSkeletonFlee;
class CSkeletonRetreat;
class CPathFinding;
class CWeapon;
class CAssetManager;
class CAnimation;
class CParticleManager;
class CDayNight;
class CBone; 
class CGunpowder;

#define ATTACK_RANGE 125
#define LEFT_HAND "sword_joint"
#define GRAVITY -1500.0f
#define DAY_DAMAGE 15
#define DAY_BLOCK_CHANCE 35
#define DAY_MAX_HEALTH 100.0f
#define DAY_MOVE_SPEED 500.0f
#define NIGHT_DAMAGE 25
#define NIGHT_BLOCK_CHANCE 70
#define NIGHT_MAX_HEALTH 200.0f
#define NIGHT_MOVE_SPEED 700.0f
#define MOVE_SPEED 500.0f

#define BLOCK_RIGHT 1
#define BLOCK_LEFT 0
#define BLOCK_CHANCE_UPDATE 1.0f
#define SCALE_FACTOR 1.5f

class CSkeletonController;

class CSkeleton :
	public CBoid
{
public:
	// Enums for Animation States
	enum ESkeletonAnimation{ eRunAnim, eAttackAnim, eDeathAnim, eHurtAnim, eHurt2Anim, eBlockWalkAnim, eSpawnAnim,eAnimCount };
	enum ESkeletonStates{ MOVING_IN, BACKING_OUT, MANUVERING, SWINGING, /*BLOCKING, */FOLLOW, RETREAT, HURT, SPAWN, DEATH, eNumStates };
	enum ESkeletonParticles { eBoneParticle, eDustParticle, eFadeAway, eParticleCount };


private:


	XMFLOAT4X4 m_f4x4ShieldMatrices[2];
	unsigned int m_unParticleEmitters[eParticleCount];
	//CRenderMesh * m_cShieldRender[2];
	// The attack instance
	CAttack* m_cpAttack;

	CRenderMesh* m_cDropShadow = nullptr;

	CParticleManager* m_pParticleManager;
	// Animations
	CAnimation* m_cpAnimations;
	float m_fSpawnTimer = 1.0f;
	float m_fHeight = -180.0f;
	XMFLOAT4X4 m_mSkeleLocal;
	CDayNight* m_pDayNight;




	// The player object
	IObject* m_pPlayer = nullptr;

	// Object to handle path planing
	CPathFinding* m_pPathPlanner = nullptr;

	// Object Manager
	CObjectManager* m_cpObjectManager = nullptr;

	//  Temp asset manager
	CAssetManager* m_pAssetManager = nullptr;

	// The Animation Manager
	CAnimationManager* m_cpAnimationManager = nullptr;
	// - Debug Input Manager
	//CInputManager* m_pInputManager = nullptr;

	CWeapon* m_cpSword;
	// Path to take
	vector<XMFLOAT3> m_vPath;
	XMFLOAT3 m_tCombatPosOffset;

	XMFLOAT3 m_tSpawnPoint;

	// Place on the path
	int m_nCurrentNodeOnPath = 0;

	// The current state
	int  m_ipCurrentState;
	/*int m_nRightBlockChance;
	int m_nLeftBlockChance;*/
	int m_nChance;

	// Current animation
	unsigned int m_unCurrAnim;


//	unsigned int m_unBlockChance = 0;

	ESkeletonStates m_eCurrState;
	ESkeletonStates m_eNewState;



	// Skeletons Health
	float m_fCurrentHealth;

	// Skeletons Max health
	float m_fMaxHealth = 100;

	float m_fInvincibilityTimer = 0.0f;
	float m_fExitTimer;
	//float m_fBlockScaleTimer = 0.0f;
	int m_nMaxManuver;
	int m_nManuverCount;
	//int m_nShieldIndex;

	// Is active/alive bool
	bool m_bIsActive = false;
	bool m_bIsGrounded = true;
	// Bool for manager to set
	bool m_bHit;
	bool m_bCanAttack = true;
	// Skeletons invincibility timer
	bool m_bInvincible = 0;
	bool m_bBuffed;
	bool m_bPoweringUp;

	XMFLOAT3 knockBackVelocity = { 0.0f, 0.0f, 0.0f };
	// Loading stuff
	//vector<CMesh> m_vMesh;
	
	//CAnimation* m_cpAnim;
	//IAgentState<CSkeleton>* m_vStates[eNumStates];

	//TPointLight m_tPointLight;


	CBone* m_cpTheLoot;
	CGunpowder* m_cpThePowder;
	CSkeletonController* m_cpSkeletonController;
	float m_fScale;
public:


	CSkeleton(CObjectManager* _manager, CAnimationManager* _animManager, CDayNight* _dayNight, CSkeletonController* skeletonController);
	virtual ~CSkeleton();

	void Update();

	void CollisionResponse(IObject * _cCollidedObject);

	void SwitchAnimation(ESkeletonAnimation _animation);
	void ChangeState(int _newState);
	void UpdateState();
	void AdjustRotation();
	void Activate(XMFLOAT3 spawnPosition);
	void DeActivate();
	void TakeDamage(int damageAmount);

	void PlayHitParticles();

	void MakeNightSkeleton();
	void MakeDaySkeleton();
	void UpdateDayStatus();

	void PlayParticle(unsigned int _toPlay, XMFLOAT3 _relativePos);

	void Knockback(XMFLOAT3 _direction, float _force);

	
	void AddToRenderer();


#pragma region AISTATES

	ESkeletonStates Move(bool _forward);
	ESkeletonStates Manuver();

	ESkeletonStates Swinging();
	ESkeletonStates Blocking();
	ESkeletonStates Retreating();
	ESkeletonStates Spawn();
	ESkeletonStates Die();
	ESkeletonStates Follow();

	ESkeletonStates Hurt();



#pragma endregion

#pragma region MUTATORS
	/*********************************
	MUTATORS
	************************************/
	void SetPath(vector<XMFLOAT3>& _path) { m_vPath = _path; }
	void SetPlayer(IObject* _player) { m_pPlayer = _player; }
	void SetCanAttack(bool _atk) { m_bCanAttack = _atk; }
	void SetNodeOnPath(int _node) { m_nCurrentNodeOnPath = _node; }
	void SetAttack(CAttack* _atk) { m_cpAttack = _atk; }
	void SetSpawnPoint(XMFLOAT3& position) { m_tSpawnPoint = position; }
	void SetCombatPosOffset(XMFLOAT3& position) { m_tCombatPosOffset = position; }
	void SetIsGrounded(bool _grounded) { m_bIsGrounded = _grounded; }

#pragma endregion

#pragma region ACCESSORS
	/*********************************
	ACCESSORS
	************************************/
	CObjectManager* GetObjectManager() const { return m_cpObjectManager; }
	IObject* GetPlayer() const { return m_pPlayer; }
	CPathFinding* GetPathPlanner() const { return m_pPathPlanner; }
	vector<XMFLOAT3>& GetPath() { return m_vPath; }
	CAttack* GetAttack()const { return m_cpAttack; }
	bool GetCanAttack() const { return m_bCanAttack; }
	bool GetIsActive() const { return m_bIsActive; }
	int GetCurNodeOnPath() const { return m_nCurrentNodeOnPath; }
	const XMFLOAT3& GetSpawnPoint() const { return m_tSpawnPoint; }
	const XMFLOAT3& GetCombatPosOffset() const { return m_tCombatPosOffset; }
//	const unsigned int GetBlockChance() const { return m_unBlockChance; }


	const CWeapon* GetWeapon(void) const { return m_cpSword; }
	const float& GetHealth() const { return m_fCurrentHealth; }
	const CAnimation* GetAnimation(ESkeletonAnimation anim) const;
	const CAnimation* GetCurrAnimation() const;
	CAnimation* GetCurrAnimation();

//	CRenderMesh * GetShieldRenderMesh(int _shield) { return m_cShieldRender[_shield]; }
	XMFLOAT4X4 GetShieldMatrix(int _shield) { return m_f4x4ShieldMatrices[_shield]; }

	void Blocked();
#pragma endregion

private:
	bool  NearlyEqual(float a, float b);
	float  DistanceToPlayer();
	void CheckState();
	void AddToBlockWeight(int side);
	int CheckBlockSide();
	void UpdateBlockSide();

	void ResetManuverVector(int chance, XMFLOAT3& mVector);
	void CSkeleton::EnterSubState();
	void CSkeleton::ExitSubState();
	void BuildPathHome();
	void BuildPathToPlayer();

	void UpdatePosition() override;

};

#endif