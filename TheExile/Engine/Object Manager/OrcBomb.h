
#ifndef _ORC_BOMB_H_
#define _ORC_BOMB_H_



#include "IObject.h"
class CObjectManager;
class COrc;
class CParticleManager;
#define FUSE_TIMER 3.0f

enum EBombState { eTraveling, eHeld, eExploding, eInactive };
class COrcBomb : public IObject
{

public:
	COrcBomb(COrc* owner, CObjectManager* objManager);
	virtual ~COrcBomb();

	virtual void Update();
	void Activate();
	void DeActivate();
	void CollisionResponse(IObject * _cCollidedObject);
	void ChangeState(EBombState newState);
	void BombHeld();
	void BombThrow();
	void BombTravel();
	void BombExplode();

	void SetOwner(COrc* owner) { m_pOwner = owner; }
private:
	void SetStartingVelocity(XMFLOAT3 start, XMFLOAT3 targetPos);

	CParticleManager* m_cpParticleManager;
	COrc* m_pOwner;
	float gravityMag;
	float m_fExitTime;
	float m_fFuseTimer;
	EBombState m_eCurrState;

	float3 bombPosition;
	float3 bombTarget;
	float3 worldVelocity;
	float3 bombExtents;
	float3 explosionExtents;


	bool traveled;





	

};




#endif