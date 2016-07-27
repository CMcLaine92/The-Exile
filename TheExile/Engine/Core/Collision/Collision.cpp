
/***********************************************
* Filename:  		Collision.cpp
* Date:      		08/12/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Provides all functions necessary
*					for detecting collision
************************************************/

#include "Collision.h"
#include "AABB.h"
#include "Cylinder.h"
#include "Plane.h"
#include "Sphere.h"
#include "Capsule.h"
#include <math.h>
#include <iostream>
#include "../../Renderer/RenderMesh.h"
#define XMF3DotProduct(a,b) (a.x*b.x + a.y*b.y + a.z*b.z)

#define SameSign(a,b) ( ((*(unsigned int*)&(a)) & 0x80000000) == ((*(unsigned int*)&(b)) & 0x80000000) )
namespace
{
	enum EFrustumCorners{ FTL, FBL, FBR, FTR, NTL, NTR, NBR, NBL };
	enum EFrustumPlanes{ NEAR_PLANE, FAR_PLANE, LEFT_PLANE, RIGHT_PLANE, TOP_PLANE, BOTTOM_PLANE };

	inline void XMF3Cross(XMFLOAT3& f3Vec1, XMFLOAT3& f3Vec2,XMFLOAT3& f3VecOut)
	{
		f3VecOut.x = f3Vec1.y * f3Vec2.z - f3Vec1.z * f3Vec2.y;
		f3VecOut.y = f3Vec1.z * f3Vec2.x - f3Vec1.x * f3Vec2.z;
		f3VecOut.z = f3Vec1.x * f3Vec2.y - f3Vec1.y * f3Vec2.x;
	}
	inline XMFLOAT3 XMF3Subtraction(XMFLOAT3& f3Vec1, XMFLOAT3& f3Vec2)
	{
		XMFLOAT3 f3Output;
		f3Output.x = f3Vec1.x - f3Vec2.x;
		f3Output.y = f3Vec1.y - f3Vec2.y;
		f3Output.z = f3Vec1.z - f3Vec2.z;
		return f3Output;
	}

	float XMF3Distance(XMFLOAT3& f3Start, XMFLOAT3& f3End)
	{
		float fDist = pow(f3Start.x - f3End.x, 2) + pow(f3Start.y - f3End.y, 2) + pow(f3Start.z - f3End.z, 2);
		return sqrt(fDist);
	}
	float XMF3DistanceXZ(XMFLOAT3& f3Start, XMFLOAT3& f3End)
	{
		float fDist = pow(f3Start.x - f3End.x, 2)+ pow(f3Start.z - f3End.z, 2);
		return sqrt(fDist);
	}

	void ComputePlane(CPlane*& pcPlane, XMFLOAT3 f3PointA, XMFLOAT3 f3PointB, XMFLOAT3 f3PointC)
	{
		XMVECTOR vecNormal;
		XMVECTOR vecAtoB;
		XMVECTOR vecAtoC;

		vecAtoB.m128_f32[0] = f3PointB.x - f3PointA.x;
		vecAtoB.m128_f32[1] = f3PointB.y - f3PointA.y;
		vecAtoB.m128_f32[2] = f3PointB.z - f3PointA.z;

		vecAtoC.m128_f32[0] = f3PointC.x - f3PointA.x;
		vecAtoC.m128_f32[1] = f3PointC.y - f3PointA.y;
		vecAtoC.m128_f32[2] = f3PointC.z - f3PointA.z;

		vecNormal = XMVector3Cross(vecAtoB, vecAtoC);
		vecNormal = XMVector3Normalize(vecNormal);
		XMFLOAT3 f3PlaneNormal;
		XMStoreFloat3(&f3PlaneNormal, vecNormal);

		float fDistance;
		XMVECTOR vecPointA = XMLoadFloat3(&f3PointA);
		fDistance = XMVector3Dot(vecPointA, vecNormal).m128_f32[0];
		
		pcPlane->SetNormal(f3PlaneNormal);
		pcPlane->SetDistance(fDistance);
	}


	/*****************************************************************************
	* bool HalfSpaceTest(XMFLOAT3*,XMFLOAT3*)	Performs Half Space Test
	*
	* Ins:								XMFLOAT3*  f3Vector		 - The vector
	*									XMFLOAT3*  f3TestPoint   - The test point
	*
	* Outs:								None
	*
	* Returns:							bool - Returns result of Half Space Test
	*
	* Mod. Date:						08/12/2015
	* Mod. Initials:					MZ
	***************************************************************************/
	bool HalfSpaceTest(XMFLOAT3 f3Vector, XMFLOAT3 f3TestPoint)
	{
		if (0.0f > XMVector3Dot(XMLoadFloat3(&f3Vector), XMLoadFloat3(&f3TestPoint)).m128_f32[0])
		{
			return true;
		}
		return false;
	}

	/*****************************************************************************
	* bool PointToAABB(CAABB*,XMFLOAT3*,int)	Checks to see if Point is inside AABB
	*
	* Ins:								CAABB*  cAABB		 - The aabb
	*									XMFLOAT3*  f3Point   - The test point
	*									int nIgnoreAxis      - Optional axis to ignore 0x,1y,2z
	*
	* Outs:								None
	*
	* Returns:							bool - Returns result of Half Space Test
	*
	* Mod. Date:						08/12/2015
	* Mod. Initials:					MZ
	***************************************************************************/
	bool PointToAABB(CAABB* cAABB, XMFLOAT3 f3Point, int nIgnoreAxis = -1)
	{
		XMFLOAT3 f3Max, f3Min;
		f3Max = cAABB->Max();
		f3Min = cAABB->Min();
		if (nIgnoreAxis == 0)
		{
			if (f3Max.x <= f3Point.x || f3Min.x >= f3Point.x)
			{
				return false;
			}
		}
		if (nIgnoreAxis == 1)
		{
			if (f3Max.y <= f3Point.y || f3Min.y >= f3Point.y)
			{
				return false;
			}
		}
		if (nIgnoreAxis == 2)
		{
			if (f3Max.z <= f3Point.z || f3Min.z >= f3Point.z)
			{
				return false;
			}
		}
		return true;
	}


	/*****************************************************************************
	* XMFLOAT3 ClosestPointonLine(XMFLOAT3*,XMFLOAT3*,XMFLOAT3,bool)	Finds the closest point on a line
	*
	* Ins:								XMFLOAT3*  f3TestPoint		 - The test point
	*									XMFLOAT3*  f3Start			 - The Start point of the line
	*									XMFLOAT3*  f3End			 - The End point of the line
	*									int nIgnoreAxis				 - Optional axis to ignore 0x,1y,2z
	*
	* Outs:								bool - Is The closest point at the end of the line
	*
	* Returns:							bool - Returns result of Half Space Test
	*
	* Mod. Date:						08/12/2015
	* Mod. Initials:					MZ
	***************************************************************************/
	XMFLOAT3 ClosestPointOnLine(XMFLOAT3& f3TestPoint,XMFLOAT3& f3Start, XMFLOAT3& f3End)
	{
		XMVECTOR start = XMLoadFloat3(&f3Start);
		XMVECTOR end = XMLoadFloat3(&f3End);
		XMVECTOR test = XMLoadFloat3(&f3TestPoint);

		XMVECTOR l = end - start;
		XMVECTOR nL = XMVector3Normalize(l);
		XMVECTOR v = test - start;
		
		float d = XMVector3Dot(nL, v).m128_f32[0];
		if (d < 0.0f)
			return f3Start;
		else if (d > XMVector3Length(l).m128_f32[0])
			return f3End;
		else
		{
			XMFLOAT3 nPrime;
			nL = nL * d;
			nL = nL + start;
			XMStoreFloat3(&nPrime, nL);
			return nPrime;
		}
	}

#pragma region Cylinders are cut
	bool CylinderToAABB(CCylinder* cCylinder, CAABB* cAABB)
	{

		if (PointToAABB(cAABB, cCylinder->GetStart()) || PointToAABB(cAABB, cCylinder->GetEnd()))
		{
			return true;
		}

		XMFLOAT3 f3ClosestPoint = ClosestPointOnLine(cAABB->GetCenter(), cCylinder->GetStart(), cCylinder->GetEnd());

		if (PointToAABB(cAABB, f3ClosestPoint))
		{
			return true;
		}


		//I'm taking a break from this. Not A Real Thing?

		return false;
	}

	bool CylinderToCylinder(CCylinder*cCylinder1, CCylinder* cCylinder2)
	{
		XMFLOAT3 f3ClosestPoints[4];
		f3ClosestPoints[0] = ClosestPointOnLine(cCylinder2->GetStart(), cCylinder1->GetStart(), cCylinder1->GetEnd());
		f3ClosestPoints[1] = ClosestPointOnLine(cCylinder2->GetEnd(), cCylinder1->GetStart(), cCylinder1->GetEnd());
		f3ClosestPoints[2] = ClosestPointOnLine(cCylinder1->GetStart(), cCylinder2->GetStart(), cCylinder2->GetEnd());
		f3ClosestPoints[3] = ClosestPointOnLine(cCylinder1->GetEnd(), cCylinder2->GetStart(), cCylinder2->GetEnd());

		//Not Implemented  Cylinders are being cut
		return false;
	}


	bool CylinderToPlane(CCylinder* cCylinder, CPlane* cPlane)
	{
		//Not Really implemented. Cylinders are being cut
		if (cCylinder->GetStart().y < cPlane->GetCenter().y && cCylinder->GetEnd().y > cPlane->GetCenter().y)
		{
			return true;
		}
		return false;
	}
#pragma endregion
	/*****************************************************************************
	* bool SphereToAABB(CSphere*,CAABB*)	Tests collision between a Sphere and an AABB
	*
	* Ins:								CSphere*  cSphere  - The Sphere
	*									CAABB*    cAABB    - The AABB
	*
	* Outs:								None
	*
	* Returns:							bool - Return true if collision
	*
	* Mod. Date:						08/12/2015
	* Mod. Initials:					MZ
	***************************************************************************/
	bool SphereToAABB(CSphere* cSphere, CAABB* cAABB)
	{
		float f3ClosestPoint[3];
		float f3SphereCenter[3];
		float f3AABBMin[3];
		float f3AABBMax[3];

		XMFLOAT3 f3Center, f3Min, f3Max;
		f3Center = cSphere->GetCenter();
		f3Min = cAABB->Min();
		f3Max = cAABB->Max();

		f3SphereCenter[0] = f3Center.x;
		f3SphereCenter[1] = f3Center.y;
		f3SphereCenter[2] = f3Center.z;

		f3AABBMin[0] = f3Min.x;
		f3AABBMin[1] = f3Min.y;
		f3AABBMin[2] = f3Min.z;

		f3AABBMax[0] = f3Max.x;
		f3AABBMax[1] = f3Max.y;
		f3AABBMax[2] = f3Max.z;

		for (unsigned int i = 0; i < 3; i++)
		{
			if (f3SphereCenter[i] < f3AABBMin[i])
			{
				f3ClosestPoint[i] = f3AABBMin[i];
			}
			else if (f3SphereCenter[i] > f3AABBMax[i])
			{
				f3ClosestPoint[i] = f3AABBMax[i];
			}
			else
			{
				f3ClosestPoint[i] = f3SphereCenter[i];
			}
		}
		XMFLOAT3 f3Dist;
		f3Dist.x = f3SphereCenter[0] - f3ClosestPoint[0];
		f3Dist.y = f3SphereCenter[1] - f3ClosestPoint[1];
		f3Dist.z = f3SphereCenter[2] - f3ClosestPoint[2];
		float distance = sqrt(pow(f3Dist.x, 2) + pow(f3Dist.y, 2) + pow(f3Dist.z,2));
		if (distance > cSphere->GetRadius())
		{
			return false;
		}
		return true;
	}
	
	/*****************************************************************************
	* bool SphereToSphere(CSphere*,CSphere*)	Tests collision between two Spheres
	*
	* Ins:								CSphere*  cSphere1  - The first Sphere
	*									CSphere*  cSphere2  - The second Sphere
	*
	* Outs:								None
	*
	* Returns:							bool - Return true if collision
	*
	* Mod. Date:						08/12/2015
	* Mod. Initials:					MZ
	***************************************************************************/
	bool SphereToSphere(CSphere* cSphere1, CSphere* cSphere2)
	{
		XMFLOAT3 f3Dist;
		XMFLOAT3 f3Center1, f3Center2;
		f3Center1 = cSphere1->GetCenter();
		f3Center2 = cSphere2->GetCenter();
		f3Dist.x = f3Center1.x - f3Center2.x;
		f3Dist.y = f3Center1.y - f3Center2.y;
		f3Dist.z = f3Center1.z - f3Center2.z;
		float fDistance = XMVector3Length(XMLoadFloat3(&f3Dist)).m128_f32[0];
		if (fDistance <= cSphere1->GetRadius() + cSphere2->GetRadius())
		{
			return true;
		}
		return false;
	}

	/*****************************************************************************
	* bool CapsuleToAABB(CCapsule*,CPlane*)	Tests collision between a Capsule and a plane
	*
	* Ins:								CCapsule*  cCapsule  - The Capsule
	*									CAABB*     cAABB     - The cAABB
	*
	* Outs:								None
	*
	* Returns:							bool - Return true if collision
	*
	* Mod. Date:						08/12/2015
	* Mod. Initials:					MZ
	***************************************************************************/
	bool CapsuleToAABB(CCapsule* cCapsule, CAABB* cAABB)
	{
		XMFLOAT3 f3ClosestPoint = ClosestPointOnLine(cAABB->GetCenter(), cCapsule->GetStart(), cCapsule->GetEnd());
		CSphere cTestSphere(f3ClosestPoint, cCapsule->GetRadius());
		if (SphereToAABB(&cTestSphere, cAABB))
		{
			return true;
		}
		return false;
	}

	/*****************************************************************************
	* bool CapsulePlaneHalfSpaceTesting(CCapsule*,CPlane*)	Performs Half Space Testing between a Capsule and a plane
	*
	* Ins:								CCapsule*  cCapsule  - The Capsule
	*									CPlane*    cPlane    - The plane
	*
	* Outs:								None
	*
	* Returns:							bool - Returns result of Half Space Test
	*
	* Mod. Date:						08/12/2015
	* Mod. Initials:					MZ
	***************************************************************************/
	bool CapsulePlaneHalfSpaceTesting(CCapsule* cCapsule, CPlane* cPlane)
	{
		XMFLOAT2 f2TestPoints[8];
		XMFLOAT3 f3CapsuleStart = cCapsule->GetStart();
		float fCapsuleRadius = cCapsule->GetRadius();
		XMFLOAT3 f3PlaneMax, f3PlaneMin;
		f3PlaneMax = cPlane->Max();
		f3PlaneMin = cPlane->Min();
		f2TestPoints[0].x = f3CapsuleStart.x + fCapsuleRadius;
		f2TestPoints[0].y = f3CapsuleStart.z;

		f2TestPoints[1].x = f3CapsuleStart.x;
		f2TestPoints[1].y = f3CapsuleStart.z + fCapsuleRadius;

		f2TestPoints[2].x = f3CapsuleStart.x - fCapsuleRadius;
		f2TestPoints[2].y = f3CapsuleStart.z;
		
		f2TestPoints[3].x = f3CapsuleStart.x;
		f2TestPoints[3].y = f3CapsuleStart.z - fCapsuleRadius;

		f2TestPoints[4].x = f3CapsuleStart.x + fCapsuleRadius * .707f;
		f2TestPoints[4].y = f3CapsuleStart.z + fCapsuleRadius * .707f;

		f2TestPoints[5].x = f3CapsuleStart.x - fCapsuleRadius * .707f;
		f2TestPoints[5].y = f3CapsuleStart.z + fCapsuleRadius * .707f;

		f2TestPoints[6].x = f3CapsuleStart.x - fCapsuleRadius * .707f;
		f2TestPoints[6].y = f3CapsuleStart.z - fCapsuleRadius * .707f;

		f2TestPoints[7].x = f3CapsuleStart.x + fCapsuleRadius * .707f;
		f2TestPoints[7].y = f3CapsuleStart.z - fCapsuleRadius * .707f;

		if (f3CapsuleStart.x < f3PlaneMax.x && f3CapsuleStart.x > f3PlaneMin.x &&
			f3CapsuleStart.z < f3PlaneMax.z && f3CapsuleStart.z > f3PlaneMin.z)
		{
			return true;
		}

		for (unsigned int i = 0; i < 8; i++)
		{
			if (f2TestPoints[i].x < cPlane->Max().x && f2TestPoints[i].x > cPlane->Min().x &&
				f2TestPoints[i].y < cPlane->Max().z && f2TestPoints[i].y > cPlane->Min().z)
			{
				return true;
			}
		}

		return false;
	}

	/*****************************************************************************
	* bool CapsuleToPlane(CCapsule*,CPlane*)	Tests collision between a Capsule and a plane
	*
	* Ins:								CCapsule*  cCapsule  - The Capsule
	*									CPlane*    cPlane    - The plane
	*
	* Outs:								None
	*
	* Returns:							bool - Return true if collision
	*
	* Mod. Date:						08/12/2015
	* Mod. Initials:					MZ
	***************************************************************************/
	bool CapsuleToPlane(CCapsule* cCapsule, CPlane* cPlane)
	{
		int topInfront = 1;
		int botInfront = 1;

		float offsetDifference;
		XMVECTOR vecStart,vecNormal,vecEnd;
		XMFLOAT3 f3Start, f3End, f3Normal;
		f3Start = cCapsule->GetStart();
		f3Normal = cPlane->GetNormal();
		f3End = cCapsule->GetEnd();
		vecStart = XMLoadFloat3(&f3Start);
		vecNormal = XMLoadFloat3(&f3Normal);
		vecEnd = XMLoadFloat3(&f3End);
		offsetDifference = XMVector3Dot(vecStart,vecNormal ).m128_f32[0] - cPlane->GetDistance();
		if (offsetDifference > cCapsule->GetRadius())
		{
			topInfront = 1;
		}
		else if (-offsetDifference > cCapsule->GetRadius())
		{
			topInfront = 2;
		}
		else
		{
			return CapsulePlaneHalfSpaceTesting(cCapsule,cPlane);
		}
		offsetDifference = XMVector3Dot(vecEnd,vecNormal).m128_f32[0] - cPlane->GetDistance();
		if (offsetDifference > cCapsule->GetRadius())
		{
			botInfront = 1;
		}
		else if (-offsetDifference > cCapsule->GetRadius())
		{
			botInfront = 2;
		}
		else
		{
			return CapsulePlaneHalfSpaceTesting(cCapsule, cPlane);
		}

		if (topInfront == botInfront)
		{
			return false;
		}
		else
		{
			return CapsulePlaneHalfSpaceTesting(cCapsule, cPlane);
		}
	}
	
	/*****************************************************************************
	* bool CapsuleToCapsule(CCapsule*,CCapsule*)	Tests collision between two Capsules
	*
	* Ins:								CCapsule*  cCapsule1  - The first Capsule
	*									CCapsule*  cCapsule2  - The second Capsule
	*
	* Outs:								None
	*
	* Returns:							bool - Return true if collision
	*
	* Mod. Date:						08/12/2015
	* Mod. Initials:					MZ
	***************************************************************************/
	bool CapsuleToCapsule(CCapsule* cCapsule1, CCapsule* cCapsule2)
	{
		XMFLOAT3 f3ClosestPoints[4];

		f3ClosestPoints[0] = ClosestPointOnLine(cCapsule2->GetStart(), cCapsule1->GetStart(), cCapsule1->GetEnd());
		f3ClosestPoints[1] = ClosestPointOnLine(cCapsule2->GetEnd(), cCapsule1->GetStart(), cCapsule1->GetEnd());
		f3ClosestPoints[2] = ClosestPointOnLine(cCapsule1->GetStart(), cCapsule2->GetStart(), cCapsule2->GetEnd());
		f3ClosestPoints[3] = ClosestPointOnLine(cCapsule1->GetEnd(), cCapsule2->GetStart(), cCapsule2->GetEnd());

		CSphere cSphere1(f3ClosestPoints[0], cCapsule2->GetRadius());
		CSphere cSphere2(f3ClosestPoints[1], cCapsule2->GetRadius());
		CSphere cSphere3(f3ClosestPoints[2], cCapsule1->GetRadius());
		CSphere cSphere4(f3ClosestPoints[3], cCapsule1->GetRadius());

		if (SphereToSphere(&cSphere1, &cSphere3) || SphereToSphere(&cSphere1, &cSphere4) || SphereToSphere(&cSphere2, &cSphere3) || SphereToSphere(&cSphere1, &cSphere4))
		{
			return true;
		}
		return false;
	}

	/*****************************************************************************
	* bool AABBToAABB(CAABB*,CAABB*)	Tests collision between two AABBs
	*
	* Ins:								CAABB* cAABB1 - Object to test collision
	*									CAABB* cAABB2 - Object to test collision against
	*
	* Outs:								None
	*
	* Returns:							bool - Returns true if collision
	*
	* Mod. Date:						08/12/2015
	* Mod. Initials:					MZ
	***************************************************************************/
	bool AABBToAABB(CAABB* cAABB1, CAABB* cAABB2)
	{
		XMFLOAT3 f3Max1,f3Max2, f3Min1,f3Min2;
		f3Max1 = cAABB1->Max();
		f3Min1 = cAABB1->Min();
		f3Max2 = cAABB2->Max();
		f3Min2 = cAABB2->Min();
		if (f3Max1.x <= f3Min2.x || f3Min1.x >= f3Max2.x)
		{
			return false;
		}
		if (f3Max1.y <= f3Min2.y || f3Min1.y >= f3Max2.y)
		{
			return false;
		}
		if (f3Max1.z <= f3Min2.z || f3Min1.z >= f3Max2.z)
		{
			return false;
		}

		return true;
	}

	/*****************************************************************************
	* bool AABBToPlane(CAABB*,CPlane*)	Tests collision between an AABB and a Plane
	*
	* Ins:								CAABB*  cAABB  - The AABB to check collision with
	*									CPlane* cPlane - The Plane to check collision against
	*
	* Outs:								None
	*
	* Returns:							bool - Return true if collision
	*
	* Mod. Date:						08/12/2015
	* Mod. Initials:					MZ
	***************************************************************************/
	bool AABBToPlane(CAABB* cAABB, CPlane* cPlane)
	{
		
		XMFLOAT3 f3CenterPoint = cAABB->GetCenter();
		XMFLOAT3 f3PlaneNormal;
		f3PlaneNormal.x = abs(cPlane->GetNormal().x);
		f3PlaneNormal.y = abs(cPlane->GetNormal().y);
		f3PlaneNormal.z = abs(cPlane->GetNormal().z);
		XMFLOAT3 f3Extents = cAABB->GetExtents();
		
		XMFLOAT3 f3PN = cPlane->GetNormal();
		
		float fOffsetDifference = XMVector3Dot(XMLoadFloat3(&f3CenterPoint), XMLoadFloat3(&f3PN)).m128_f32[0] - cPlane->GetDistance();
		float fRadius = XMVector3Dot(XMLoadFloat3(&f3Extents), XMLoadFloat3(&f3PlaneNormal)).m128_f32[0];
		if (fOffsetDifference > fRadius || fOffsetDifference < -fRadius)
		{
			return false;
		}
		else
		{
			//True on Infinite Plane. 
			//Removed in favor of capsules
			return true;
		}
	}

	bool AABBInFrustum(CPlane* cPlane, CAABB* cAABB)
	{
		XMFLOAT3 f3CenterPoint = cAABB->GetCenter();
		XMFLOAT3 f3PlaneNormal;
		f3PlaneNormal.x = abs(cPlane->GetNormal().x);
		f3PlaneNormal.y = abs(cPlane->GetNormal().y);
		f3PlaneNormal.z = abs(cPlane->GetNormal().z);
		XMFLOAT3 f3Extents = cAABB->GetExtents();
		XMFLOAT3 f3PN = cPlane->GetNormal();
		float fOffsetDifference = XMVector3Dot(XMLoadFloat3(&f3CenterPoint), XMLoadFloat3(&f3PN)).m128_f32[0] - cPlane->GetDistance();
		float fRadius = XMVector3Dot(XMLoadFloat3(&f3Extents), XMLoadFloat3(&f3PlaneNormal)).m128_f32[0];
		if (fOffsetDifference > fRadius)
		{
			return true;
		}
		else if (fOffsetDifference < -fRadius)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	bool PlaneInFrustum(CPlane* pcFrustumPlane, CPlane* pcTestPlane)
	{
		XMFLOAT3 f3CenterPoint = pcTestPlane->GetCenter();
		XMFLOAT3 f3PlaneNormal;
		f3PlaneNormal.x = abs(pcFrustumPlane->GetNormal().x);
		f3PlaneNormal.y = abs(pcFrustumPlane->GetNormal().y);
		f3PlaneNormal.z = abs(pcFrustumPlane->GetNormal().z);
		XMFLOAT3 f3Extents = pcTestPlane->GetExtents();
		XMFLOAT3 f3FPN = pcFrustumPlane->GetNormal();
		float fOffsetDifference = XMVector3Dot(XMLoadFloat3(&f3CenterPoint), XMLoadFloat3(&f3FPN)).m128_f32[0] - pcFrustumPlane->GetDistance();
		float fRadius = XMVector3Dot(XMLoadFloat3(&f3Extents), XMLoadFloat3(&f3PlaneNormal)).m128_f32[0];
		if (fOffsetDifference > fRadius)
		{
			return true;
		}
		else if (fOffsetDifference < -fRadius)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	bool FrustumToAABB(TFrustum* ptFrustum, CAABB* cAABB)
	{
		for (unsigned int i = 0; i < 6; i++)
		{
			if (AABBInFrustum(ptFrustum->cPlane[i],cAABB) == false)
			{
				return false;
			}
		}
		return true;
	}
	
	bool FrustumToPlane(TFrustum* ptFrustum, CPlane* cPlane)
	{
		for (unsigned int i = 0; i < 6; i++)
		{
			if (PlaneInFrustum(ptFrustum->cPlane[i], cPlane) == false)
			{
				return false;
			}
		}
		return true;
	}

	bool RayToTriangle(TRay& tRay, XMFLOAT3& vert1, XMFLOAT3& vert2, XMFLOAT3& vert3, XMFLOAT3& norm, float& fHitTime)
	{
		//Early out
		float fAngleDifference = XMF3DotProduct(norm, tRay.f3Direction);
		XMFLOAT3 f3ToTri;
		f3ToTri.x = tRay.f3Start.x - vert1.x;
		f3ToTri.y = tRay.f3Start.y - vert1.y;
		f3ToTri.z = tRay.f3Start.z - vert1.z;
		float fBackface = XMF3DotProduct(norm, f3ToTri);
		if (fAngleDifference > 0.0f || fBackface < 0.0f)
		{
			return false;
		}

		XMFLOAT3 f3VertToStart[3];
		f3VertToStart[0] = XMF3Subtraction(vert1, tRay.f3Start);
		f3VertToStart[1] = XMF3Subtraction(vert2, tRay.f3Start);
		f3VertToStart[2] = XMF3Subtraction(vert3, tRay.f3Start);

		XMFLOAT3 f3PlaneNormals[3];
		XMF3Cross(f3VertToStart[2], f3VertToStart[1], f3PlaneNormals[0]);
		XMF3Cross(f3VertToStart[0], f3VertToStart[2], f3PlaneNormals[1]);
		XMF3Cross(f3VertToStart[1], f3VertToStart[0], f3PlaneNormals[2]);

		float fDotProducts[3];
		for (unsigned int i = 0; i < 3; i++)
		{
			fDotProducts[i] = XMF3DotProduct(f3PlaneNormals[i], tRay.f3Direction);
		}
		if (SameSign(fDotProducts[0], fDotProducts[1]) && SameSign(fDotProducts[1], fDotProducts[2]))
		{
			fHitTime = (XMF3DotProduct(vert1, norm) - XMF3DotProduct(tRay.f3Start, norm)) / XMF3DotProduct(norm, tRay.f3Direction);
			if (fHitTime <= tRay.fDistance)
				return true;
		}
		return false;
	}

	bool RayToAABB(CAABB* cAABB, TRay& tRay, float& fHitTime)
	{
		bool bOutput = false;
		XMFLOAT3 f3Verts[8];
		XMFLOAT3 f3Center,f3Extents;
		f3Center = cAABB->GetCenter();
		f3Extents = cAABB->GetExtents();
		f3Verts[FTL].x = f3Center.x - f3Extents.x;
		f3Verts[FTL].y = f3Center.y + f3Extents.y;
		f3Verts[FTL].z = f3Center.z + f3Extents.z;

		f3Verts[FBL] = f3Verts[FTL];
		f3Verts[FBL].y = f3Center.y - f3Extents.y;

		f3Verts[FBR] = f3Verts[FBL];
		f3Verts[FBR].x = f3Center.x + f3Extents.x;

		f3Verts[FTR] = f3Verts[FTL];
		f3Verts[FTR].x = f3Center.x + f3Extents.x;

		f3Verts[NTL] = f3Verts[FTL];
		f3Verts[NTL].z = f3Center.z - f3Extents.z;

		f3Verts[NBL] = f3Verts[NTL];
		f3Verts[NBL].y = f3Center.y - f3Extents.y;

		f3Verts[NBR] = f3Verts[NBL];
		f3Verts[NBR].x = f3Center.x + f3Extents.x;

		f3Verts[NTR] = f3Verts[NTL];
		f3Verts[NTR].x = f3Center.x + f3Extents.x;

		XMFLOAT3 f3Normal[6];
		f3Normal[0] = XMFLOAT3(0.0f, 0.0f, -1.0f);//Back
		f3Normal[1] = XMFLOAT3(0.0f, 0.0f, 1.0f);//Front
		f3Normal[2] = XMFLOAT3(1.0f, 0.0f, 0.0f);//Right
		f3Normal[3] = XMFLOAT3(-1.0f, 0.0f, 0.0f);//Left
		f3Normal[4] = XMFLOAT3(0.0f, 1.0f, 0.0f);//Top
		f3Normal[5] = XMFLOAT3(0.0f, -1.0f, 0.0f);//Bottom


		float fTempHitTime;
		//BACK
		if (RayToTriangle(tRay, f3Verts[NBL], f3Verts[NTL], f3Verts[NTR], f3Normal[0], fTempHitTime))
		{
			bOutput = true;
			fHitTime = fTempHitTime;
		}

		if (RayToTriangle(tRay, f3Verts[NTR], f3Verts[NBR], f3Verts[NBL], f3Normal[0], fTempHitTime))
		{
			if (bOutput == true)
			{
				if (fHitTime > fTempHitTime)
				{
					fHitTime = fTempHitTime;
				}
			}
			else
			{
				bOutput = true;
				fHitTime = fTempHitTime;
			}
		}
		//FRONT
		if (RayToTriangle(tRay, f3Verts[FBL], f3Verts[FBR], f3Verts[FTR], f3Normal[1], fTempHitTime))
		{
			if (bOutput == true)
			{
				if (fHitTime > fTempHitTime)
				{
					fHitTime = fTempHitTime;
				}
			}
			else
			{
				bOutput = true;
				fHitTime = fTempHitTime;
			}
		}
		if (RayToTriangle(tRay, f3Verts[FTR], f3Verts[FTL], f3Verts[FBL], f3Normal[1], fTempHitTime))
		{
			if (bOutput == true)
			{
				if (fHitTime > fTempHitTime)
				{
					fHitTime = fTempHitTime;
				}
			}
			else
			{
				bOutput = true;
				fHitTime = fTempHitTime;
			}
		}

		//LEFT
		if (RayToTriangle(tRay, f3Verts[FTL], f3Verts[NTL], f3Verts[FBL], f3Normal[3], fTempHitTime))
		{
			if (bOutput == true)
			{
				if (fHitTime > fTempHitTime)
				{
					fHitTime = fTempHitTime;
				}
			}
			else
			{
				bOutput = true;
				fHitTime = fTempHitTime;
			}
		}
		if (RayToTriangle(tRay, f3Verts[FBL], f3Verts[NTL], f3Verts[NBL], f3Normal[3], fTempHitTime))
		{
			if (bOutput == true)
			{
				if (fHitTime > fTempHitTime)
				{
					fHitTime = fTempHitTime;
				}
			}
			else
			{
				bOutput = true;
				fHitTime = fTempHitTime;
			}
		}
		//Right
		if (RayToTriangle(tRay, f3Verts[FTR], f3Verts[NTR], f3Verts[FBR], f3Normal[2], fTempHitTime))
		{
			if (bOutput == true)
			{
				if (fHitTime > fTempHitTime)
				{
					fHitTime = fTempHitTime;
				}
			}
			else
			{
				bOutput = true;
				fHitTime = fTempHitTime;
			}
		}
		if (RayToTriangle(tRay, f3Verts[FBR], f3Verts[NTR], f3Verts[NBR], f3Normal[2], fTempHitTime))
		{
			if (bOutput == true)
			{
				if (fHitTime > fTempHitTime)
				{
					fHitTime = fTempHitTime;
				}
			}
			else
			{
				bOutput = true;
				fHitTime = fTempHitTime;
			}
		}
		//Bottom
		if (RayToTriangle(tRay, f3Verts[FBR], f3Verts[NBR], f3Verts[FBL], f3Normal[5], fTempHitTime))
		{
			if (bOutput == true)
			{
				if (fHitTime > fTempHitTime)
				{
					fHitTime = fTempHitTime;
				}
			}
			else
			{
				bOutput = true;
				fHitTime = fTempHitTime;
			}
		}
		if (RayToTriangle(tRay, f3Verts[FBL], f3Verts[NBR], f3Verts[NBL], f3Normal[5], fTempHitTime))
		{
			if (bOutput == true)
			{
				if (fHitTime > fTempHitTime)
				{
					fHitTime = fTempHitTime;
				}
			}
			else
			{
				bOutput = true;
				fHitTime = fTempHitTime;
			}
		}
		//Top
		if (RayToTriangle(tRay, f3Verts[FTR], f3Verts[NTR], f3Verts[FTL], f3Normal[4], fTempHitTime))
		{
			if (bOutput == true)
			{
				if (fHitTime > fTempHitTime)
				{
					fHitTime = fTempHitTime;
				}
			}
			else
			{
				bOutput = true;
				fHitTime = fTempHitTime;
			}
		}
		if (RayToTriangle(tRay, f3Verts[FTL], f3Verts[NTR], f3Verts[NTL], f3Normal[4], fTempHitTime))
		{
			if (bOutput == true)
			{
				if (fHitTime > fTempHitTime)
				{
					fHitTime = fTempHitTime;
				}
			}
			else
			{
				bOutput = true;
				fHitTime = fTempHitTime;
			}
		}
		return bOutput;
	}

	bool RayToPlane(CPlane* cPlane, TRay& tRay, float& fHitTime)
	{
		bool bOutput = false;
		float fTempHitTime;
		XMFLOAT3 f3Verts[4];
		XMFLOAT3 f3Center, f3Extents;
		f3Center = cPlane->GetCenter();
		f3Extents = cPlane->GetExtents();
		f3Verts[0].x = f3Center.x - f3Extents.x;
		f3Verts[0].y = f3Center.y + f3Extents.y;
		f3Verts[0].z = f3Center.z - f3Extents.z;

		f3Verts[1].x = f3Center.x + f3Extents.x;
		f3Verts[1].y = f3Center.y + f3Extents.y;
		f3Verts[1].z = f3Center.z + f3Extents.z;

		f3Verts[2].x = f3Center.x + f3Extents.x;
		f3Verts[2].y = f3Center.y + f3Extents.y;
		f3Verts[2].z = f3Center.z - f3Extents.z;

		f3Verts[3].x = f3Center.x - f3Extents.x;
		f3Verts[3].y = f3Center.y + f3Extents.y;
		f3Verts[3].z = f3Center.z + f3Extents.z;

		XMFLOAT3 f3Normal = XMFLOAT3(0, 1, 0);
		if (RayToTriangle(tRay, f3Verts[0], f3Verts[1], f3Verts[2], f3Normal, fTempHitTime))
		{
			if (bOutput == true)
			{
				if (fHitTime > fTempHitTime)
				{
					fHitTime = fTempHitTime;
				}
			}
			else
			{
				bOutput = true;
				fHitTime = fTempHitTime;
			}
		}

		if (RayToTriangle(tRay, f3Verts[0], f3Verts[1], f3Verts[3], f3Normal, fTempHitTime))
		{
			if (bOutput == true)
			{
				if (fHitTime > fTempHitTime)
				{
					fHitTime = fTempHitTime;
				}
			}
			else
			{
				bOutput = true;
				fHitTime = fTempHitTime;
			}
		}

		return bOutput;
	}

	bool SphereInFrustum(CPlane* cPlane, CSphere* pSphere)
	{
		float fOffset = XMF3DotProduct(pSphere->GetCenter(),cPlane->GetNormal()) - cPlane->GetDistance();
		return (-fOffset > pSphere->GetRadius());
		
	}
}

/********************************************************************************************************
* bool CheckCollision(IObject*,vector<IObject*>,vector<IObject*>)	Checks the object for collision against all
*																	other passed in objects.
*
* Ins:																IObject*  pObj					    - The object that is checking collision
*																	vector<IObject*> vpTestObjects	    - All other objects to test against
*
* Outs:																vector<IObject*>& vpCollidedObjects - OUT PARAM: All objects that have collided with pObj
*
* Returns:															bool - Return true if collision
*
* Mod. Date:														08/12/2015
* Mod. Initials:													MZ
********************************************************************************************************/
bool CheckCollision(IObject* pObj, std::vector<IObject*> vpTestObjects, std::vector<IObject*>& vpCollidedObjects)
{
	try
	{
		bool bDidCollide = false;
		bool bMarkForAdd = false;
		std::vector<CCollider*>& vColliders = pObj->GetColliders();
		vpCollidedObjects.clear();
		unsigned int collidersSize = (unsigned int)vColliders.size();
		for (unsigned int pObjIndex = 0; pObjIndex < collidersSize; pObjIndex++)
		{
			vColliders[pObjIndex]->SetCollided(false);
			vColliders[pObjIndex]->GetCollidedObjects().clear();
			if (vColliders[pObjIndex]->GetDynamic() == false || vColliders[pObjIndex]->GetActive() == false)
			{
				continue;
			}
			Bounds::eBoundsType eCurrentType = vColliders[pObjIndex]->GetBounds()->GetType();
			if (eCurrentType == Bounds::AABB)
			{
				unsigned int testObjects = (unsigned int)vpTestObjects.size();

				for (unsigned int testObjectIndices = 0; testObjectIndices < testObjects; testObjectIndices++)
				{

					//Is same object?
					if (vpTestObjects[testObjectIndices] == pObj)
					{
						continue;
					}

					bMarkForAdd = false;
					std::vector<CCollider*>& vTestObjectColliders = vpTestObjects[testObjectIndices]->GetColliders();
					unsigned int totalClear = (unsigned int)vTestObjectColliders.size();
					for (unsigned int clearIndicies = 0; clearIndicies < totalClear; clearIndicies++)
					{
						vTestObjectColliders[clearIndicies]->SetCollided(false);
						vTestObjectColliders[clearIndicies]->GetCollidedObjects().clear();
					}
					unsigned int testColliders = vTestObjectColliders.size();
					for (unsigned int testColliderIndices = 0; testColliderIndices < testColliders; testColliderIndices++)
					{
						if (vTestObjectColliders[testColliderIndices]->GetActive() == false)
						{
							continue;
						}
						if (vTestObjectColliders[testColliderIndices]->GetBounds()->GetType() == Bounds::AABB)
						{
							CAABB* left = static_cast<CAABB*> (vColliders[pObjIndex]->GetBounds());
							CAABB* right = static_cast<CAABB*> (vTestObjectColliders[testColliderIndices]->GetBounds());

							if (AABBToAABB(left, right))
							{
								vColliders[pObjIndex]->SetCollided(true);
								vTestObjectColliders[testColliderIndices]->SetCollided(true);
								vTestObjectColliders[testColliderIndices]->GetCollidedObjects().push_back(pObj);
								bDidCollide = true;
								bMarkForAdd = true;
							}
						}
						else if (vTestObjectColliders[testColliderIndices]->GetBounds()->GetType() == Bounds::Plane)
						{
							CAABB* aabb = static_cast<CAABB*> (vColliders[pObjIndex]->GetBounds());
							CPlane* plane = static_cast<CPlane*> (vTestObjectColliders[testColliderIndices]->GetBounds());
							if (AABBToPlane(aabb, plane))
							{
								vColliders[pObjIndex]->SetCollided(true);
								vTestObjectColliders[testColliderIndices]->SetCollided(true);
								vTestObjectColliders[testColliderIndices]->GetCollidedObjects().push_back(pObj);
								bDidCollide = true;
								bMarkForAdd = true;
							}
						}
						else if (vTestObjectColliders[testColliderIndices]->GetBounds()->GetType() == Bounds::Capsule)
						{

							CAABB* aabb = static_cast<CAABB*> (vColliders[pObjIndex]->GetBounds());
							CCapsule* capsule = static_cast<CCapsule*> (vTestObjectColliders[testColliderIndices]->GetBounds());
							if (CapsuleToAABB(capsule, aabb))
							{
								vColliders[pObjIndex]->SetCollided(true);
								vTestObjectColliders[testColliderIndices]->SetCollided(true);
								vTestObjectColliders[testColliderIndices]->GetCollidedObjects().push_back(pObj);
								bDidCollide = true;
								bMarkForAdd = true;
							}
						}
						else
						{
							assert(0 && "Using type not yet added or Matt broke something. Let him know this occured unless you know where you made a mistake");
						}
					}
					if (bMarkForAdd == true)
					{
						vpCollidedObjects.push_back(vpTestObjects[testObjectIndices]);
						vColliders[pObjIndex]->GetCollidedObjects().push_back(vpTestObjects[testObjectIndices]);
					}
				}
			}
			else if (eCurrentType == Bounds::Capsule)
			{
				for (unsigned int testObjectIndices = 0; testObjectIndices < vpTestObjects.size(); testObjectIndices++)
				{

					//Is same object?
					if (vpTestObjects[testObjectIndices] == pObj)
					{
						continue;
					}

					bool bMarkForAdd = false;
					std::vector<CCollider*>& vTestObjectColliders = vpTestObjects[testObjectIndices]->GetColliders();
					for (unsigned int clearIndicies = 0; clearIndicies < vTestObjectColliders.size(); clearIndicies++)
					{
						vTestObjectColliders[clearIndicies]->SetCollided(false);
						vTestObjectColliders[clearIndicies]->GetCollidedObjects().clear();
					}
					for (unsigned int testColliderIndices = 0; testColliderIndices < vTestObjectColliders.size(); testColliderIndices++)
					{
						if (vpTestObjects[testObjectIndices]->GetColliders()[testColliderIndices]->GetActive() == false)
						{
							continue;
						}

						if (vTestObjectColliders[testColliderIndices]->GetBounds()->GetType() == Bounds::AABB)
						{
							CCapsule* capsule = static_cast<CCapsule*> (vColliders[pObjIndex]->GetBounds());
							CAABB* aabb = static_cast<CAABB*> (vTestObjectColliders[testColliderIndices]->GetBounds());
							if (CapsuleToAABB(capsule, aabb))
							{
								vColliders[pObjIndex]->SetCollided(true);
								vTestObjectColliders[testColliderIndices]->SetCollided(true);
								vTestObjectColliders[testColliderIndices]->GetCollidedObjects().push_back(pObj);

								bDidCollide = true;
								bMarkForAdd = true;
							}
						}
						else if (vTestObjectColliders[testColliderIndices]->GetBounds()->GetType() == Bounds::Plane)
						{
							CCapsule* capsule = static_cast<CCapsule*> (vColliders[pObjIndex]->GetBounds());
							CPlane* plane = static_cast<CPlane*> (vTestObjectColliders[testColliderIndices]->GetBounds());
							if (CapsuleToPlane(capsule, plane))
							{
								vColliders[pObjIndex]->SetCollided(true);
								vTestObjectColliders[testColliderIndices]->SetCollided(true);
								vTestObjectColliders[testColliderIndices]->GetCollidedObjects().push_back(pObj);
								bDidCollide = true;
								bMarkForAdd = true;
							}
						}
						else if (vTestObjectColliders[testColliderIndices]->GetBounds()->GetType() == Bounds::Capsule)
						{
							CCapsule* capsuleL = static_cast<CCapsule*> (vColliders[pObjIndex]->GetBounds());
							CCapsule* capsuleR = static_cast<CCapsule*> (vTestObjectColliders[testColliderIndices]->GetBounds());
							if (CapsuleToCapsule(capsuleL, capsuleR))
							{
								vColliders[pObjIndex]->SetCollided(true);
								vTestObjectColliders[testColliderIndices]->SetCollided(true);
								vTestObjectColliders[testColliderIndices]->GetCollidedObjects().push_back(pObj);
								bDidCollide = true;
								bMarkForAdd = true;
							}
						}
						else
						{
							assert(0 && "Using type not yet added or Matt broke something. Let him know this occured unless you know where you made a mistake");
						}

					}
					if (bMarkForAdd == true)
					{
						bool bAlreadyAdded = false;
						for (unsigned int i = 0; i < vpCollidedObjects.size(); i++)
						{
							if (vpTestObjects[testObjectIndices] == vpCollidedObjects[i])
							{
								bAlreadyAdded = true;
							}
						}
						if (bAlreadyAdded == false)
						{
							vpCollidedObjects.push_back(vpTestObjects[testObjectIndices]);
							vColliders[pObjIndex]->GetCollidedObjects().push_back(vpTestObjects[testObjectIndices]);
						}
					}
				}
			}
			else
			{
				assert(0 && "Type returned something that doesn't have collision functions.  Use a different type or tell Matt he broke it.");
			}
		}
		return bDidCollide;
	}
	catch (const std::bad_alloc& e)
	{
		DBG_UNREFERENCED_PARAMETER(e);
		//static bool bPrintOutError = true;
		//if (bPrintOutError)
		//{
			cout << pObj->GetTag().c_str() << "Could be corrupted\n";
		//	bPrintOutError = false;
		//}
		return true;
	}
}


/********************************************************************************************************
* bool CheckCollision(IObject*,vector<IObject*>,vector<IObject*>)	Checks the object for collision against all
*																	other passed in objects.
*
* Ins:																CCollider*  pCollider				- The Collider that is checking collision
*																	vector<IObject*> vpTestObjects	    - All other objects to test against
*																	
* Outs:																vector<IObject*>& vpCollidedObjects - OUT PARAM: All objects that have collided with pObj
*
* Returns:															bool - Return true if collision
*
* Mod. Date:														08/12/2015
* Mod. Initials:													MZ
********************************************************************************************************/
bool CheckCollision(CCollider* pCollider, std::vector<IObject*> vpTestObjects, std::vector<IObject*>& vpCollidedObjects)
{
	bool bDidCollide = false;
	vpCollidedObjects.clear();

		pCollider->SetCollided(false);
		pCollider->GetCollidedObjects().clear();
		if (pCollider->GetActive() == false)
		{
			return false;
		}
		Bounds::eBoundsType eCurrentType = pCollider->GetBounds()->GetType();
		if (eCurrentType == Bounds::AABB)
		{
			for (unsigned int testObjectIndices = 0; testObjectIndices < vpTestObjects.size(); testObjectIndices++)
			{
				bool bMarkForAdd = false;
				std::vector<CCollider*>& vTestObjectColliders = vpTestObjects[testObjectIndices]->GetColliders();
				for (unsigned int testColliderIndices = 0; testColliderIndices < vTestObjectColliders.size(); testColliderIndices++)
				{
					if (vpTestObjects[testObjectIndices]->GetColliders()[testColliderIndices]->GetActive() == false)
					{
						continue;
					}
					if (vTestObjectColliders[testColliderIndices]->GetBounds()->GetType() == Bounds::AABB)
					{
						if (AABBToAABB(reinterpret_cast<CAABB*> (pCollider->GetBounds()), reinterpret_cast<CAABB*> (vTestObjectColliders[testColliderIndices]->GetBounds())))
						{
							pCollider->SetCollided(true);
							bDidCollide = true;
							bMarkForAdd = true;
						}
					}
					else if (vTestObjectColliders[testColliderIndices]->GetBounds()->GetType() == Bounds::Plane)
					{
						if (AABBToPlane(reinterpret_cast<CAABB*> (pCollider->GetBounds()), reinterpret_cast<CPlane*> (vTestObjectColliders[testColliderIndices]->GetBounds())))
						{
							pCollider->SetCollided(true);
							bDidCollide = true;
							bMarkForAdd = true;
						}
					}
					else if (vTestObjectColliders[testColliderIndices]->GetBounds()->GetType() == Bounds::Capsule)
					{
						if (CapsuleToAABB(reinterpret_cast<CCapsule*> (vTestObjectColliders[testColliderIndices]->GetBounds()), reinterpret_cast<CAABB*> (pCollider->GetBounds())))
						{
							pCollider->SetCollided(true);
							bDidCollide = true;
							bMarkForAdd = true;
						}
					}
					else
					{
						assert(0 && "Using type not yet added or Matt broke something. Let him know this occured unless you know where you made a mistake");
					}
				}
				if (bMarkForAdd == true)
				{
					vpCollidedObjects.push_back(vpTestObjects[testObjectIndices]);
					pCollider->GetCollidedObjects().push_back(vpTestObjects[testObjectIndices]);
				}
			}
		}
		else if (eCurrentType == Bounds::Capsule)
		{
			for (unsigned int testObjectIndices = 0; testObjectIndices < vpTestObjects.size(); testObjectIndices++)
			{

				bool bMarkForAdd = false;
				std::vector<CCollider*>& vTestObjectColliders = vpTestObjects[testObjectIndices]->GetColliders();
				for (unsigned int testColliderIndices = 0; testColliderIndices < vTestObjectColliders.size(); testColliderIndices++)
				{
					if (vpTestObjects[testObjectIndices]->GetColliders()[testColliderIndices]->GetActive() == false)
					{
						continue;
					}

					if (vTestObjectColliders[testColliderIndices]->GetBounds()->GetType() == Bounds::AABB)
					{
						if (CapsuleToAABB(reinterpret_cast<CCapsule*> (pCollider->GetBounds()), reinterpret_cast<CAABB*> (vTestObjectColliders[testColliderIndices]->GetBounds())))
						{
							pCollider->SetCollided(true);
							bDidCollide = true;
							bMarkForAdd = true;
						}
					}
					else if (vTestObjectColliders[testColliderIndices]->GetBounds()->GetType() == Bounds::Plane)
					{
						if (CapsuleToPlane(reinterpret_cast<CCapsule*> (pCollider->GetBounds()), reinterpret_cast<CPlane*> (vTestObjectColliders[testColliderIndices]->GetBounds())))
						{
							pCollider->SetCollided(true);
							bDidCollide = true;
							bMarkForAdd = true;
						}
					}
					else if (vTestObjectColliders[testColliderIndices]->GetBounds()->GetType() == Bounds::Capsule)
					{
						if (CapsuleToCapsule(reinterpret_cast<CCapsule*> (pCollider->GetBounds()), reinterpret_cast<CCapsule*> (vTestObjectColliders[testColliderIndices]->GetBounds())))
						{
							pCollider->SetCollided(true);
							bDidCollide = true;
							bMarkForAdd = true;
						}
					}
					else
					{
						assert(0 && "Using type not yet added or Matt broke something. Let him know this occured unless you know where you made a mistake");
					}

				}
				if (bMarkForAdd == true)
				{
					bool bAlreadyAdded = false;
					for (unsigned int i = 0; i < vpCollidedObjects.size(); i++)
					{
						if (vpTestObjects[testObjectIndices] == vpCollidedObjects[i])
						{
							bAlreadyAdded = true;
						}
					}
					if (bAlreadyAdded == false)
					{
						vpCollidedObjects.push_back(vpTestObjects[testObjectIndices]);
						pCollider->GetCollidedObjects().push_back(vpTestObjects[testObjectIndices]);
					}
				}
			}
		}
		else
		{
			assert(0 && "Type returned something that doesn't have collision functions.  Use a different type or tell Matt he broke it.");
		}
	
	return bDidCollide;
}
/********************************************************************************************************
* void CapsuleToWall(IObject*,CCollider*,CCollider*)				Capsule to wall response
*
* Ins:																IObject*  pObj	    - The object with velocity to manipulate
*																	CCollider* pCapsule - The capsule collider of the object
*																	CCollider* pWall	- The collider of the wall
*
* Outs:																None
*
* Returns:															void
*
* Mod. Date:														08/20/2015
* Mod. Initials:													MZ
********************************************************************************************************/
void CapsuleToWall(IObject* pObj,CCollider* pCapsule, CCollider* pWall)
{
	CCapsule* pCapsuleBounds = (CCapsule*)pCapsule->GetBounds();
	CAABB* pWallBounds = (CAABB*)pWall->GetBounds();

	XMFLOAT2 f2WallTestPoints[4]; //Right, Top, Left, Bottom
	

	f2WallTestPoints[0].x = pWallBounds->Max().x;
	f2WallTestPoints[0].y = pWallBounds->GetCenter().z;

	f2WallTestPoints[1].x = pWallBounds->GetCenter().x;
	f2WallTestPoints[1].y = pWallBounds->Max().z;

	f2WallTestPoints[2].x = pWallBounds->Min().x;
	f2WallTestPoints[2].y = pWallBounds->GetCenter().z;

	f2WallTestPoints[3].x = pWallBounds->GetCenter().x;
	f2WallTestPoints[3].y = pWallBounds->Min().z;

	XMFLOAT3 f3WallTestPoints[4]; // used for Half Space Testing

	f3WallTestPoints[0].x = pWallBounds->Max().x;
	f3WallTestPoints[0].y = pWallBounds->GetCenter().y;
	f3WallTestPoints[0].z = pWallBounds->GetCenter().z;


	f3WallTestPoints[1].x = pWallBounds->GetCenter().x;
	f3WallTestPoints[1].y = pWallBounds->GetCenter().y;
	f3WallTestPoints[1].z = pWallBounds->Max().z;

	f3WallTestPoints[2].x = pWallBounds->Min().x;
	f3WallTestPoints[2].y = pWallBounds->GetCenter().y;
	f3WallTestPoints[2].z = pWallBounds->GetCenter().z;


	f3WallTestPoints[3].x = pWallBounds->GetCenter().x;
	f3WallTestPoints[3].y = pWallBounds->GetCenter().y;
	f3WallTestPoints[3].z = pWallBounds->Min().z;


	XMFLOAT2 f2CapsuleTestPoints[4];
	for (unsigned int i = 0; i < 4; i++)
	{
		f2CapsuleTestPoints[i].x = pCapsuleBounds->GetStart().x;
		f2CapsuleTestPoints[i].y = pCapsuleBounds->GetStart().z;
	}

	XMFLOAT2 f2CornerTestPoints[4];
	f2CornerTestPoints[0].x = pWallBounds->Max().x;
	f2CornerTestPoints[0].y = pWallBounds->Max().z;
	f2CornerTestPoints[1].x = pWallBounds->Min().x;
	f2CornerTestPoints[1].y = pWallBounds->Min().z;
	f2CornerTestPoints[2].x = pWallBounds->Min().x;
	f2CornerTestPoints[2].y = pWallBounds->Max().z;
	f2CornerTestPoints[3].x = pWallBounds->Max().x;
	f2CornerTestPoints[3].y = pWallBounds->Min().z;



	XMFLOAT3 f3PlayerToWall[4];
	XMFLOAT3 f3WallNormal[4];
	for (unsigned int i = 0; i < 4; i++)
	{
		f3PlayerToWall[i].x = f3WallTestPoints[i].x - pCapsuleBounds->GetStart().x;
		f3PlayerToWall[i].y = 0.0f;
		f3PlayerToWall[i].z = f3WallTestPoints[i].z - pCapsuleBounds->GetStart().z;

		f3WallNormal[i].x = f3WallTestPoints[i].x - pWallBounds->GetCenter().x;
		f3WallNormal[i].y = 0.0f;
		f3WallNormal[i].z = f3WallTestPoints[i].z - pWallBounds->GetCenter().z;
	}

	
	


	XMVECTOR vecPlayerToWall[4];
	XMVECTOR vecWallNormal[4];

	for (unsigned int i = 0; i < 4; i++)
	{
		vecWallNormal[i] = XMLoadFloat3(&f3WallNormal[i]);
		vecPlayerToWall[i] = XMLoadFloat3(&f3PlayerToWall[i]);
		XMStoreFloat3(&f3WallNormal[i], XMVector3Normalize(vecWallNormal[i]));
		XMStoreFloat3(&f3PlayerToWall[i], XMVector3Normalize(vecPlayerToWall[i]));
	}

	//if Colliding with corner,  Collide with greater side rather than lesser side

	bool bCornerCollision = false;

	for (unsigned int i = 0; i < 4; i++)
	{
		float fXDif, fYDif;
		fXDif = f2CornerTestPoints[i].x - pCapsuleBounds->GetStart().x;
		fYDif = f2CornerTestPoints[i].y - pCapsuleBounds->GetStart().z;
		fXDif *= fXDif;
		fYDif *= fYDif;
		float fDistanceToCorner = sqrtf(fXDif + fYDif);
		if (fDistanceToCorner <= pCapsuleBounds->GetRadius())
		{
			bCornerCollision = true;
			break;
		}
	}



	float fVelocityChange;
	XMFLOAT3 f3Velocity = pObj->GetWorldVelocity();

	int unTestIndex = -1;
	float fDistance = abs(pCapsuleBounds->GetStart().x - f2WallTestPoints[0].x);


	float fTestDistance = abs(pCapsuleBounds->GetStart().x - f2WallTestPoints[2].x);
	if (bCornerCollision == false)
	{

		if (HalfSpaceTest(f3WallNormal[0], f3PlayerToWall[0]) == false)
		{
			fDistance = FLT_MAX;
		}
		else
		{
			unTestIndex = 0;
		}

		if (fTestDistance < fDistance && HalfSpaceTest(f3WallNormal[2], f3PlayerToWall[2]))
		{
			unTestIndex = 2;
			fDistance = fTestDistance;
		}
		fTestDistance = abs(pCapsuleBounds->GetStart().z - f2WallTestPoints[1].y);
		if (fTestDistance < fDistance && HalfSpaceTest(f3WallNormal[1], f3PlayerToWall[1]))
		{
			unTestIndex = 1;
			fDistance = fTestDistance;
		}


		fTestDistance = abs(pCapsuleBounds->GetStart().z - f2WallTestPoints[3].y);
		if (fTestDistance < fDistance && HalfSpaceTest(f3WallNormal[3], f3PlayerToWall[3]))
		{
			unTestIndex = 3;
			fDistance = fTestDistance;
		}
	}
	else
	{

		if (HalfSpaceTest(f3WallNormal[0], f3PlayerToWall[0]) == false)
		{
			fDistance = -FLT_MAX;
		}
		else
		{
			unTestIndex = 0;
		}

		if (fTestDistance > fDistance && HalfSpaceTest(f3WallNormal[2], f3PlayerToWall[2]))
		{
			unTestIndex = 2;
			fDistance = fTestDistance;
		}
		fTestDistance = abs(pCapsuleBounds->GetStart().z - f2WallTestPoints[1].y);
		if (fTestDistance > fDistance && HalfSpaceTest(f3WallNormal[1], f3PlayerToWall[1]))
		{
			unTestIndex = 1;
			fDistance = fTestDistance;

		}

		fTestDistance = abs(pCapsuleBounds->GetStart().z - f2WallTestPoints[3].y);
		if (fTestDistance > fDistance && HalfSpaceTest(f3WallNormal[3], f3PlayerToWall[3]))
		{
			unTestIndex = 3;
			fDistance = fTestDistance;
		}
	}


	if (unTestIndex == 0 )
	{
		fVelocityChange = pCapsuleBounds->GetRadius() - abs( f2WallTestPoints[unTestIndex].x - pCapsuleBounds->GetStart().x);
		fVelocityChange /= DELTA_TIME();
		f3Velocity.x += fVelocityChange;
		//if (abs(f3Velocity.x) > 0.1f)
		//	cout << f3Velocity.x << "x\n";

	}
	else if (unTestIndex == 2)
	{
		fVelocityChange = pCapsuleBounds->GetRadius() - abs(f2WallTestPoints[unTestIndex].x - pCapsuleBounds->GetStart().x);
		fVelocityChange /= DELTA_TIME();
		f3Velocity.x -= fVelocityChange;
		//if (abs(f3Velocity.x) > 0.1f)
		//	cout << f3Velocity.x << "x\n";
	}
	else if (unTestIndex == 1)
	{
		fVelocityChange = pCapsuleBounds->GetRadius() - abs(f2WallTestPoints[unTestIndex].y - pCapsuleBounds->GetStart().z);
		fVelocityChange /= DELTA_TIME();
		f3Velocity.z += fVelocityChange;
		//if (abs(f3Velocity.z) > 0.1f)
		//	cout << f3Velocity.z << "z\n";
	}
	else if (unTestIndex == 3)
	{
		fVelocityChange = pCapsuleBounds->GetRadius() - abs(f2WallTestPoints[unTestIndex].y - pCapsuleBounds->GetStart().z);
		fVelocityChange /= DELTA_TIME();
		f3Velocity.z -= fVelocityChange;
		//if (abs(f3Velocity.z) > 0.1f)
		//	cout << f3Velocity.z << "z\n";
	}



	pObj->SetWorldVelocity(f3Velocity);
}

/********************************************************************************************************
* void CapsuleToFloor(IObject*,CCollider*,CCollider*)				Capsule to floor response
*
* Ins:																IObject*  pObj	    - The object with velocity to manipulate
*																	CCollider* pCapsule - The capsule collider of the object
*																	CCollider* 			- The collider of the floor
*
* Outs:																None
*
* Returns:															void
*
* Mod. Date:														08/20/2015
* Mod. Initials:													MZ
********************************************************************************************************/
void CapsuleToFloor(IObject* pObj, CCollider* pCapsule, CCollider* )
{
	CCapsule* pCapsuleBounds = (CCapsule*)pCapsule->GetBounds();
	
	XMFLOAT3 f3BottomOfCapsule = pCapsuleBounds->GetStart();
	f3BottomOfCapsule.y -= pCapsuleBounds->GetRadius();
	XMFLOAT3 f3Velocity = pObj->GetVelocity();
	f3Velocity.y = 0.0f;

	pObj->SetVelocity(f3Velocity);
}

void CapsuleToCapsule(IObject* pObj, CCollider* pCapsule, CCollider*pOtherCapsule)
{
	CCapsule* pCapsuleBounds = (CCapsule*)pCapsule->GetBounds();
	CCapsule* pOtherCapsuleBounds = (CCapsule*)pOtherCapsule->GetBounds();

	float fRadius1 = pCapsuleBounds->GetRadius();
	float fRadius2 = pOtherCapsuleBounds->GetRadius();

	XMFLOAT3 f3Velocity = pObj->GetWorldVelocity();

	float fDist = XMF3DistanceXZ(pCapsuleBounds->GetStart(),pOtherCapsuleBounds->GetStart());
	XMFLOAT3 f3AdjustVector = pOtherCapsuleBounds->GetStart();
	f3AdjustVector.x -= pCapsuleBounds->GetStart().x;
	f3AdjustVector.y = 0.0f;
	f3AdjustVector.z -= pCapsuleBounds->GetStart().z;
	float fDifference = fDist - (fRadius1 + fRadius2);

	float fAdjustDistance = sqrt(XMF3DotProduct(f3AdjustVector, f3AdjustVector));
	f3AdjustVector.x /= fAdjustDistance;
	f3AdjustVector.z /= fAdjustDistance;
	fDifference /= DELTA_TIME();
	f3AdjustVector.x *= fDifference;
	f3AdjustVector.z *= fDifference;

	f3Velocity.x += f3AdjustVector.x;
	f3Velocity.z += f3AdjustVector.z;

	pObj->SetWorldVelocity(f3Velocity);
}


void BuildFrustum(TFrustum*& tFrustum ,float fFov, float fNearDist, float fFarDist, float fRatio, XMFLOAT4X4 f44CameraTransform)
{
	XMFLOAT3 f3NearCenter; 
	f3NearCenter.x = f44CameraTransform.m[3][0] + f44CameraTransform.m[2][0] * fNearDist;
	f3NearCenter.y = f44CameraTransform.m[3][1] + f44CameraTransform.m[2][1] * fNearDist;
	f3NearCenter.z = f44CameraTransform.m[3][2] + f44CameraTransform.m[2][2] * fNearDist;
	XMFLOAT3 f3FarCenter;
	f3FarCenter.x = f44CameraTransform.m[3][0] + f44CameraTransform.m[2][0] * fFarDist;
	f3FarCenter.y = f44CameraTransform.m[3][1] + f44CameraTransform.m[2][1] * fFarDist;
	f3FarCenter.z = f44CameraTransform.m[3][2] + f44CameraTransform.m[2][2] * fFarDist;

	float fHNear = 2.0f * tan(fFov / 2.0f) * fNearDist;
	float fHFar = 2.0f * tan(fFov / 2.0f)  * fFarDist;
	float fWNear = fHNear* fRatio;
	float fWFar = fHFar * fRatio;

	tFrustum->f3Corner[FTL].x = f3FarCenter.x + f44CameraTransform.m[1][0] * (fHFar * .5f) - f44CameraTransform.m[0][0] * (fWFar * .5f);
	tFrustum->f3Corner[FTL].y = f3FarCenter.y + f44CameraTransform.m[1][1] * (fHFar * .5f) - f44CameraTransform.m[0][1] * (fWFar * .5f);
	tFrustum->f3Corner[FTL].z = f3FarCenter.z + f44CameraTransform.m[1][2] * (fHFar * .5f) - f44CameraTransform.m[0][2] * (fWFar * .5f);

	tFrustum->f3Corner[FTR].x = f3FarCenter.x + f44CameraTransform.m[1][0] * (fHFar * .5f) + f44CameraTransform.m[0][0] * (fWFar * .5f);
	tFrustum->f3Corner[FTR].y = f3FarCenter.y + f44CameraTransform.m[1][1] * (fHFar * .5f) + f44CameraTransform.m[0][1] * (fWFar * .5f);
	tFrustum->f3Corner[FTR].z = f3FarCenter.z + f44CameraTransform.m[1][2] * (fHFar * .5f) + f44CameraTransform.m[0][2] * (fWFar * .5f);
			
	tFrustum->f3Corner[FBL].x = f3FarCenter.x - f44CameraTransform.m[1][0] * (fHFar * .5f) - f44CameraTransform.m[0][0] * (fWFar * .5f);
	tFrustum->f3Corner[FBL].y = f3FarCenter.y - f44CameraTransform.m[1][1] * (fHFar * .5f) - f44CameraTransform.m[0][1] * (fWFar * .5f);
	tFrustum->f3Corner[FBL].z = f3FarCenter.z - f44CameraTransform.m[1][2] * (fHFar * .5f) - f44CameraTransform.m[0][2] * (fWFar * .5f);
			
	tFrustum->f3Corner[FBR].x = f3FarCenter.x - f44CameraTransform.m[1][0] * (fHFar * .5f) + f44CameraTransform.m[0][0] * (fWFar * .5f);
	tFrustum->f3Corner[FBR].y = f3FarCenter.y - f44CameraTransform.m[1][1] * (fHFar * .5f) + f44CameraTransform.m[0][1] * (fWFar * .5f);
	tFrustum->f3Corner[FBR].z = f3FarCenter.z - f44CameraTransform.m[1][2] * (fHFar * .5f) + f44CameraTransform.m[0][2] * (fWFar * .5f);
			
	tFrustum->f3Corner[NTL].x = f3NearCenter.x + f44CameraTransform.m[1][0] * (fHNear * .5f) - f44CameraTransform.m[0][0] * (fWNear * .5f);
	tFrustum->f3Corner[NTL].y = f3NearCenter.y + f44CameraTransform.m[1][1] * (fHNear * .5f) - f44CameraTransform.m[0][1] * (fWNear * .5f);
	tFrustum->f3Corner[NTL].z = f3NearCenter.z + f44CameraTransform.m[1][2] * (fHNear * .5f) - f44CameraTransform.m[0][2] * (fWNear * .5f);
			
	tFrustum->f3Corner[NTR].x = f3NearCenter.x + f44CameraTransform.m[1][0] * (fHNear * .5f) + f44CameraTransform.m[0][0] * (fWNear * .5f);
	tFrustum->f3Corner[NTR].y = f3NearCenter.y + f44CameraTransform.m[1][1] * (fHNear * .5f) + f44CameraTransform.m[0][1] * (fWNear * .5f);
	tFrustum->f3Corner[NTR].z = f3NearCenter.z + f44CameraTransform.m[1][2] * (fHNear * .5f) + f44CameraTransform.m[0][2] * (fWNear * .5f);
			
	tFrustum->f3Corner[NBL].x = f3NearCenter.x - f44CameraTransform.m[1][0] * (fHNear * .5f) - f44CameraTransform.m[0][0] * (fWNear * .5f);
	tFrustum->f3Corner[NBL].y = f3NearCenter.y - f44CameraTransform.m[1][1] * (fHNear * .5f) - f44CameraTransform.m[0][1] * (fWNear * .5f);
	tFrustum->f3Corner[NBL].z = f3NearCenter.z - f44CameraTransform.m[1][2] * (fHNear * .5f) - f44CameraTransform.m[0][2] * (fWNear * .5f);
			
	tFrustum->f3Corner[NBR].x = f3NearCenter.x - f44CameraTransform.m[1][0] * (fHNear * .5f) + f44CameraTransform.m[0][0] * (fWNear * .5f);
	tFrustum->f3Corner[NBR].y = f3NearCenter.y - f44CameraTransform.m[1][1] * (fHNear * .5f) + f44CameraTransform.m[0][1] * (fWNear * .5f);
	tFrustum->f3Corner[NBR].z = f3NearCenter.z - f44CameraTransform.m[1][2] * (fHNear * .5f) + f44CameraTransform.m[0][2] * (fWNear * .5f);

	ComputePlane(tFrustum->cPlane[NEAR_PLANE], tFrustum->f3Corner[NBL], tFrustum->f3Corner[NTL], tFrustum->f3Corner[NTR]);
	ComputePlane(tFrustum->cPlane[FAR_PLANE], tFrustum->f3Corner[FBL], tFrustum->f3Corner[FBR], tFrustum->f3Corner[FTR]);
	ComputePlane(tFrustum->cPlane[LEFT_PLANE], tFrustum->f3Corner[NBL], tFrustum->f3Corner[FBL], tFrustum->f3Corner[FTL]);
	ComputePlane(tFrustum->cPlane[RIGHT_PLANE], tFrustum->f3Corner[NBR], tFrustum->f3Corner[NTR], tFrustum->f3Corner[FBR]);
	ComputePlane(tFrustum->cPlane[TOP_PLANE], tFrustum->f3Corner[FTL], tFrustum->f3Corner[FTR], tFrustum->f3Corner[NTR]);  
	ComputePlane(tFrustum->cPlane[BOTTOM_PLANE], tFrustum->f3Corner[NBL], tFrustum->f3Corner[NBR], tFrustum->f3Corner[FBR]); 

	for (unsigned int i = 0; i < 6; i++)
	{
		XMFLOAT3 f3Normal = tFrustum->cPlane[i]->GetNormal();
		f3Normal.x *= -1.0f;
		f3Normal.y *= -1.0f;
		f3Normal.z *= -1.0f;
		tFrustum->cPlane[i]->SetNormal(f3Normal);
		float fDistance = tFrustum->cPlane[i]->GetDistance();
		fDistance *= -1.0f;
		tFrustum->cPlane[i]->SetDistance(fDistance);
	}
}

void FrustumCull(TFrustum* ptFrustum,std::vector<IObject*> vpcAllObjects)
{
	for (unsigned int i = 0; i < vpcAllObjects.size(); i++)
	{
		if (vpcAllObjects[i]->GetColliders().size() > 0 && vpcAllObjects[i]->GetFrustumCull())
		{
			if (vpcAllObjects[i]->GetColliders()[0]->GetType() == Bounds::AABB)
			{
				if (FrustumToAABB(ptFrustum, (CAABB*)vpcAllObjects[i]->GetColliders()[0]->GetBounds()))
				{
					if (vpcAllObjects[i]->GetRenderMesh() != nullptr)
					{
						vpcAllObjects[i]->GetRenderMesh()->GetRender() = true;
					}
				}
				else
				{
					if (vpcAllObjects[i]->GetRenderMesh() != nullptr)
					{
						vpcAllObjects[i]->GetRenderMesh()->GetRender() = false;
					}
				}
			}
			else if (vpcAllObjects[i]->GetColliders()[0]->GetType() == Bounds::Plane)
			{
				if (FrustumToPlane(ptFrustum, (CPlane*)vpcAllObjects[i]->GetColliders()[0]->GetBounds()))
				{
					if (vpcAllObjects[i]->GetRenderMesh() != nullptr)
					{
						vpcAllObjects[i]->GetRenderMesh()->GetRender() = true;
					}
				}
				else
				{
					if (vpcAllObjects[i]->GetRenderMesh() != nullptr)
					{
						vpcAllObjects[i]->GetRenderMesh()->GetRender() = false;
					}
				}
			}
		}
	}
}

bool SphereToFrustum(TFrustum* tFrustum, CSphere* pSphere)
{
	for (unsigned int i = 0; i < 6; i++)
	{
		if (SphereInFrustum(tFrustum->cPlane[i], pSphere))
		{
			return false;
		}
	}
	return true;
}

bool LineSegmentIntersection(TRay& tRay, std::vector<IObject*> vpcObjects, float& fHitTime)
{
	bool bOutput = false;
	fHitTime = FLT_MAX;
	for (unsigned int ObjectIndex = 0; ObjectIndex < vpcObjects.size(); ObjectIndex++)
	{
		IObject* pcCurObject = vpcObjects[ObjectIndex];
		for (unsigned int ColliderIndex = 0; ColliderIndex < pcCurObject->GetColliders().size(); ColliderIndex++)
		{
			CCollider* pcCurCollider = pcCurObject->GetColliders()[ColliderIndex];
			if (pcCurCollider->GetActive() == false || pcCurCollider->GetCatchRays() == false)
			{
				continue;
			}			
			if (pcCurCollider->GetType() == Bounds::AABB)
			{
				float fTempHit;
				if (RayToAABB(reinterpret_cast<CAABB*>(pcCurCollider->GetBounds()), tRay, fTempHit))
				{
					if (bOutput == true)
					{
						if (fTempHit < fHitTime)
						{
							fHitTime = fTempHit;
						}
					}
					else
					{
						fHitTime = fTempHit;
						bOutput = true;
					}
				}
			}
			else if (pcCurCollider->GetType() == Bounds::Plane)
			{
				float fTempHit;
				if (RayToPlane(reinterpret_cast<CPlane*>(pcCurCollider->GetBounds()), tRay, fTempHit))
				{
					if (bOutput == true)
					{
						if (fTempHit < fHitTime)
						{
							fHitTime = fTempHit;
						}
					}
					else
					{
						fHitTime = fTempHit;
						bOutput = true;
					}
				}
			}
		}
	}
	return bOutput;
}

bool LineSegmentObjectIntersection(TRay& tRay, std::vector<IObject*> vpcObjects, float& fHitTime, IObject* pObject)
{
	bool bOutput = false;
	fHitTime = FLT_MAX;
	for (unsigned int ObjectIndex = 0; ObjectIndex < vpcObjects.size(); ObjectIndex++)
	{
		IObject* pcCurObject = vpcObjects[ObjectIndex];
		for (unsigned int ColliderIndex = 0; ColliderIndex < pcCurObject->GetColliders().size(); ColliderIndex++)
		{
			CCollider* pcCurCollider = pcCurObject->GetColliders()[ColliderIndex];
			if (pcCurCollider->GetActive() == false || pcCurCollider->GetCatchRays() == false)
			{
				continue;
			}
			if (pcCurCollider->GetType() == Bounds::AABB)
			{
				float fTempHit;
				if (RayToAABB(reinterpret_cast<CAABB*>(pcCurCollider->GetBounds()), tRay, fTempHit))
				{
					if (bOutput == true)
					{
						if (fTempHit < fHitTime)
						{
							fHitTime = fTempHit;
							pObject = pcCurObject;
						}
					}
					else
					{
						fHitTime = fTempHit;
						bOutput = true;
						pObject = pcCurObject;
					}
				}
			}
			else if (pcCurCollider->GetType() == Bounds::Plane)
			{
				float fTempHit;
				if (RayToPlane(reinterpret_cast<CPlane*>(pcCurCollider->GetBounds()), tRay, fTempHit))
				{
					if (bOutput == true)
					{
						if (fTempHit < fHitTime)
						{
							fHitTime = fTempHit;
							pObject = pcCurObject;
						}
					}
					else
					{
						fHitTime = fTempHit;
						bOutput = true;
						pObject = pcCurObject;
					}
				}
			}
		}
	}
	return bOutput;
}
/*****************************************
*Class used for Testing Purposes Only*
*Class used for Testing Purposes Only*
*****************************************/
void CCollisionTestObject::CollisionResponse(std::vector<IObject*>vpCollidedObjects)
{

}

/*****************************************
*Class used for Testing Purposes Only*
*Class used for Testing Purposes Only*
*****************************************/
void CCollisionTestObject::Update()
{

}

/*****************************************
*Class used for Testing Purposes Only*
*Class used for Testing Purposes Only*
*****************************************/
CCollisionTestObject::CCollisionTestObject(std::string szTag) :IObject(szTag)
{
	SetPosition(XMFLOAT3(0, 0, 0));
	SetXAxis(XMFLOAT3(1, 0, 0));
	SetYAxis(XMFLOAT3(0, 1, 0));
	SetZAxis(XMFLOAT3(0, 0, 1));

}