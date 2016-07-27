#ifndef _COLLISION_H_
#define _COLLISION_H_

/***********************************************
* Filename:  		Collision.h
* Date:      		08/12/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Provides all functions necessary
*					for detecting collision 
************************************************/

#include "Collider.h"
#include "../../Object Manager/IObject.h"
#include <vector>
#include "AABB.h"
#include "Plane.h"
#include "Sphere.h"
struct TFrustum
{
	CPlane* cPlane[6];
	XMFLOAT3 f3Corner[8];

	TFrustum()
	{
		for (unsigned int i = 0; i < 6; i++)
		{
			cPlane[i] = new CPlane(0,XMFLOAT3(0,0,0),XMFLOAT3(0,0,0),XMFLOAT3(0,0,0));
		}
	}

	~TFrustum()
	{
		for (unsigned int i = 0; i < 6; i++)
		{
			if (cPlane[i] != nullptr)
			{
				delete cPlane[i];
			}
		}
	}
};

struct TRay
{
	//Ray Start Position
	XMFLOAT3 f3Start;
	//Ray Direction Normalized
	XMFLOAT3 f3Direction;
	//Length of Ray
	float fDistance;
};

/// <summary>
/// Returns true if collision has been detected.
/// </summary>
/// <param name = "pObj"> Object being tested for collision.</param>
/// <param name = "vpTestObjects"> List of objects to test against.</param>
/// <param name = "vpCollidedObjects"> OUT PARAM: Filled out if collision was detected.</param>
bool CheckCollision(IObject* pObj, std::vector<IObject*> vpTestObjects, std::vector<IObject*>& vpCollidedObjects);

/// <summary>
/// Returns true if collision has been detected.
/// </summary>
/// <param name = "pCollider"> Object being tested for collision.</param>
/// <param name = "vpTestObjects"> List of objects to test against.</param>
/// <param name = "vpCollidedObjects"> OUT PARAM: Filled out if collision was detected.</param>
bool CheckCollision(CCollider* pCollider, std::vector<IObject*> vpTestObjects, std::vector<IObject*>& vpCollidedObjects);

/// <summary>
/// Changes pObj's velocity based on collision of it's pCapsule and the AABB of the pWall
/// </summary>
/// <param name = "pObj"> Object responding to the wall collision.</param>
/// <param name = "pCapsule"> Capsule collider of the pObj.</param>
/// <param name = "pWall"> The collider of the wall.</param>
void CapsuleToWall(IObject* pObj,CCollider* pCapsule, CCollider* pWall);

/// <summary>
/// Changes pObj's velocity based on collision of it's pCapsule and the Plane of the pFloor
/// </summary>
/// <param name = "pObj"> Object responding to the wall collision.</param>
/// <param name = "pCapsule"> Capsule collider of the pObj.</param>
/// <param name = "pFloor"> The collider of the floor.</param>
void CapsuleToFloor(IObject* pObj, CCollider* pCapsule, CCollider* pFloor);

void CapsuleToCapsule(IObject* pObj, CCollider* pCapsule, CCollider*pOtherCapsule);

void BuildFrustum(TFrustum*& tFrustum, float fFov, float fNearDist, float fFarDist, float fRatio, XMFLOAT4X4 f44CameraTransform);

void FrustumCull(TFrustum* ptFrustum, std::vector<IObject*> vpcAllObjects);

bool SphereToFrustum(TFrustum* tFrustum, CSphere* pSphere);
/// <summary>
/// Checks the ray.  Returns if intersected
/// </summary>
/// <param name = "tRay"> Ray to test with.</param>
/// <param name = "vpcObjects"> All objects to test against.</param>
/// <param name = "fHitTime"> Time of first hit.</param>
bool LineSegmentIntersection(TRay& tRay, std::vector<IObject*> vpcObjects, float& fHitTime);

bool LineSegmentObjectIntersection(TRay& tRay, std::vector<IObject*> vpcObjects, float& fHitTime,IObject* pObject);
/*****************************************
*Class used for Testing Purposes Only*
*Class used for Testing Purposes Only*
*****************************************/
class CCollisionTestObject : public IObject
{
private:

public:
	void CollisionResponse(std::vector<IObject*>vpCollidedObjects);
	CCollisionTestObject(std::string szTag);
	void Update();
};


#endif
