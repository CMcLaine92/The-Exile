
#ifndef CORC_H
#define CORC_H
#include "../../../../Engine/AI/Boid.h"
#include "../../../../Renderer/Lights.h"

#define THROW_COOLDOWN 10.0f
class CObjectManager;
class CAnimationManager;
class CPathFinding;
class CAnimation;
class CParticleManager;
class CDayNight;
class CAttack;
class COrcBomb;
class CGunpowder;
class COrc : public CBoid
{
public:
	enum EOrcAnimation{ eIdle1Anim, eIdle2Anim, eRunAnim, eMeleeAnim, eDeathAnim, eHurtAnim, eHurt2Anim, ePreThrowAnim, eThrowBomb, eAnimCount };
	enum EOrcStates{ eIdleState, eReposition, eMeleeState, eThrowBombState, eFollowState, eRetreatState, eHurtState, eSpawnState, eDeathState, eMoveIn, eNumStates };
	enum EOrcParticles { eBloodParticle, eBloodString, eBloodMist, eBloodMist2, eFadeAway, eCaveSmoke1, eCaveSmoke2, eCaveSmoke3, eParticleCount };
private:

	CParticleManager* m_pParticleManager;
	unsigned int m_unParticleEmitters[eParticleCount];
	// Animations
	CAnimation* m_cpAnimations;

	CRenderMesh* m_cDropShadow = nullptr;

	CDayNight* m_pDayNight;
	IObject* m_pPlayer = nullptr;
	CPathFinding* m_pPathPlanner = nullptr;
	CAnimationManager* m_cpAnimationManager = nullptr;
	vector<XMFLOAT3> m_vPath;
	XMFLOAT3 m_tSpawnPosition;
	XMFLOAT3 knockBackVelocity = { 0.0f, 0.0f, 0.0f };
	//TPointLight m_tPointLight;
	CAttack* m_cpAttack;
	int m_nCurrentNodeOnPath = 0;
	int  m_ipCurrentState;
	unsigned int m_unCurrAnim;
	EOrcStates m_eCurrState;
	EOrcStates m_eNewState;
	CRenderMesh * m_cWepMesh;
	COrcBomb* m_pBomb;
	CGunpowder* theLoot;
	int m_nMaxHealth;
	int m_nCurrHealth;
	float m_fInvincibilityTimer = 0.0f;
	float m_fExitTimer;
	float m_fThrowTimer;
	float m_fSpawnTimer = 0.0f;
	float m_fSwingTimer = 0.0f;
	bool m_bIsActive = false;
	bool m_bIsGrounded = true;
	bool m_bHit;
	bool m_bBuffed;


public:

	

	COrc(XMFLOAT3 position, COrcBomb* bomb, CObjectManager* _manager, CAnimationManager* _animManager = nullptr, CDayNight* _dayNight = nullptr);
	virtual ~COrc();
	void Update();
	void CollisionResponse(IObject* _collided);
	void SwitchAnimation(EOrcAnimation _animation);
	void ChangeState(int _newState);
	void UpdateState();
	void AdjustRotation();
	void Activate();
	void DeActivate();
	void TakeDamage(int damageAmount);
	void PlayHitParticles();
	void UpdateDayStatus();
	void PlayParticle(unsigned int _toPlay, XMFLOAT3 _relativePos);
	void Knockback(XMFLOAT3 _direction, float _force);
	XMFLOAT4X4 GetJoint(const char* jointName);
	virtual void UpdatePosition();


	//States
	EOrcStates Idle();
	EOrcStates Spawn();
	EOrcStates Reposition();
	EOrcStates Follow();
	EOrcStates Retreat();
	EOrcStates Hurt();
	EOrcStates Melee();
	EOrcStates ThrowBomb();
	EOrcStates Dead();
	EOrcStates MoveIn();

	//Mutators
	void SetPath(vector<XMFLOAT3>& _path) { m_vPath = _path; }
	void SetPlayer(IObject* _player) { m_pPlayer = _player; }
	void SetNodeOnPath(int _node) { m_nCurrentNodeOnPath = _node; }
	void SetSpawnPoint(XMFLOAT3& position) { m_tSpawnPosition = position; }
	void SetIsGrounded(bool _grounded) { m_bIsGrounded = _grounded; }

	/*********************************
	ACCESSORS
	************************************/
	IObject* GetPlayer() const { return m_pPlayer; }
	CPathFinding* GetPathPlanner() const { return m_pPathPlanner; }
	vector<XMFLOAT3>& GetPath() { return m_vPath; }
	bool GetIsActive() const { return m_bIsActive; }
	int GetCurNodeOnPath() const { return m_nCurrentNodeOnPath; }
	const XMFLOAT3& GetSpawnPoint() const { return m_tSpawnPosition; }

	const int& GetHealth() const { return m_nCurrHealth; }
	const CAnimation* GetAnimation(EOrcAnimation anim) const;
	const CAnimation* GetCurrAnimation() const;
	CAnimation* GetCurrAnimation();


	void Blocked();

	void AddToRenderer();

#pragma endregion

private:
	bool  NearlyEqual(float a, float b);
	float  DistanceToPlayer();
	void CheckState();

	void EnterSubState();
	void ExitSubState();
	void BuildPathHome();
	void BuildPathToPlayer();

	void MakeDayOrc();
	void MakeNightOrc();
	
};

#endif