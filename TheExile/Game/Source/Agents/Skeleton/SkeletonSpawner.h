#ifndef SKELETON_SPAWN_H
#define SKELETON_SPAWN_H
#include "../../../../Engine/Object Manager/IObject.h"

#define SPAWN_COOLDOWN 60.0f

class CSkeletonController;
class CSkeletonSpawner : public IObject
{
public:
	CSkeletonSpawner(CSkeletonController* controller, XMFLOAT3 position);
	virtual ~CSkeletonSpawner();
	virtual void Update();
	virtual void CollisionResponse(IObject * _cCollidedObject);



private:
	XMFLOAT3 m_vPosition;
	CSkeletonController* m_cpSpawnController;
	float m_fSpawnCoolDown;

	float3 color;

};



#endif