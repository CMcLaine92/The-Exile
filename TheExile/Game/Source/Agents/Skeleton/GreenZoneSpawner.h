#ifndef _GZ_SKEL_SPAWN_H_
#define _GZ_SKEL_SPAWN_H_
#include "../../../../Engine/Object Manager/IObject.h"

class CSkeleton;
class CObjectManager;
class CAnimationManager;
class CDayNight;

class CGreenZoneSpawner : public IObject
{
public:
	CGreenZoneSpawner(CObjectManager* objManager, CAnimationManager* animManager, CDayNight* dayNight, XMFLOAT3 position);
	~CGreenZoneSpawner();
	virtual void Update();
	virtual void CollisionResponse(IObject * _cCollidedObject);


private:

	//vector<CSkeleton*> m_vSkeletons;
	//int m_nNumSpawning;
	//int m_nNumSpawned;
	//float m_fIncreaseTimer;
	//bool m_bMaxSpawnReached;
	//bool m_bActive;
};




#endif