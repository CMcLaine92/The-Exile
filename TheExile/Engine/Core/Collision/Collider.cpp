/***********************************************
* Filename:  		Collider.cpp
* Date:      		08/12/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Defines the collidable objects
*					that will be added to objects and
*                   used in collision detection and
*                   collision responses
************************************************/

#include "Collider.h"
#include "AABB.h"
#include "Plane.h"
#include "Capsule.h"
#include "Cylinder.h"
/*****************************************************************************
* CCollider(bool,CBounds*)	Constructor that initializes the Collider
*							with passed in values
*
* Ins:						bool	 bDynamic - sets m_bDynamic to this valud
*							CBounds* cBounds  - sets m_cBounds to this value
*
* Outs:						None
*
* Returns:					None
*
* Mod. Date:				08/12/2015
* Mod. Initials:			MZ
***************************************************************************/
CCollider::CCollider(bool bDynamic, CBounds* cBounds, bool bActive,bool bCatchRays)
{
	m_bDynamic = bDynamic;
	m_cBounds = cBounds;
	m_bCollided = false;
	m_bActive = bActive;
	m_bCatchRays = bCatchRays;
}

/*****************************************************************************
* ~CCollider()				Destructor that deletes bounds if not nullptr
*
* Ins:						None
*
* Outs:						None
*
* Returns:					None
*
* Mod. Date:				08/12/2015
* Mod. Initials:			MZ
***************************************************************************/
CCollider::~CCollider()
{
	SAFE_DELETE(m_cBounds);
	
}

/*****************************************************************************
* CCollider(bool,eBoundsType, vector<TVertex>)	Constructor that initializes the Collider
*												and dynamically allocates the bounds object
*
* Ins:											bool	 bDynamic - sets m_bDynamic to this valud
*												eBoundsType eType - Type of bounds to create
*												vector<TVertex>	  - Vertex values to be used in creation of bounds.
*
* Outs:											None
*
* Returns:										None
*
* Mod. Date:									08/16/2015
* Mod. Initials:								MZ
*
* NOTE: Capsule and Cylinders will be upright no matter what.  Don't use this if you want sideways ones.
* NOTE: Don't Use this function anymore for anything other than AABBs please
***************************************************************************/
CCollider::CCollider(bool bDynamic, Bounds::eBoundsType eType, std::vector<TVertex> vVertices, bool bActive, bool bCatchRays)
{
	float f3Min[3] = 
	{
		FLT_MAX,
		FLT_MAX,
		FLT_MAX
	};
	float f3Max[3] =
	{
		-FLT_MAX,
		-FLT_MAX,
		-FLT_MAX
	};
	m_bDynamic = bDynamic;
	m_bCollided = false;
	m_bActive = bActive;
	m_bCatchRays = bCatchRays;
	if (eType == Bounds::AABB)
	{	
		for (unsigned int i = 0; i < vVertices.size(); i++)
		{
			for (unsigned int xyz = 0; xyz < 3; xyz++)
			{
				if (vVertices[i].m_fPosition[xyz] < f3Min[xyz])
				{
					f3Min[xyz] = vVertices[i].m_fPosition[xyz];
				}
				if (vVertices[i].m_fPosition[xyz] > f3Max[xyz])
				{
					f3Max[xyz] = vVertices[i].m_fPosition[xyz];
				}
			}
		}
		XMFLOAT3 f3Center;
		XMFLOAT3 f3Extents;

		f3Center.x = (f3Min[0] + f3Max[0]) / 2.0f;
		f3Center.y = (f3Min[1] + f3Max[1]) / 2.0f;
		f3Center.z = (f3Min[2] + f3Max[2]) / 2.0f;

		f3Extents.x = f3Max[0] - f3Center.x;
		f3Extents.y = f3Max[1] - f3Center.y;
		f3Extents.z = f3Max[2] - f3Center.z;

		m_cBounds = new CAABB(f3Center, f3Extents);
	}
	else if (eType == Bounds::Plane)
	{
		
		float fDistance;
		XMFLOAT3 f3Normal;

		f3Normal.x = vVertices[0].m_fNormal[0];
		f3Normal.y = vVertices[0].m_fNormal[1];
		f3Normal.z = vVertices[0].m_fNormal[2];

		XMFLOAT3 f3PointOnPlane = XMFLOAT3(vVertices[0].m_fPosition[0], vVertices[0].m_fPosition[1], vVertices[0].m_fPosition[2]);

		fDistance = XMVector3Dot(XMLoadFloat3(&f3Normal), XMLoadFloat3(&f3PointOnPlane)).m128_f32[0];

		
		for (unsigned int VertIndex = 0; VertIndex < vVertices.size(); VertIndex++)
		{
			for (unsigned int xyz = 0; xyz < 3; xyz++)
			{
				if (vVertices[VertIndex].m_fPosition[xyz] < f3Min[xyz])
				{
					f3Min[xyz] = vVertices[VertIndex].m_fPosition[xyz];
				}
				if (vVertices[VertIndex].m_fPosition[xyz] > f3Max[xyz])
				{
					f3Max[xyz] = vVertices[VertIndex].m_fPosition[xyz];
				}
			}
		}

		XMFLOAT3 f3Center;
		XMFLOAT3 f3Extents;

		f3Center.x = (f3Min[0] + f3Max[0]) / 2.0f;
		f3Center.y = (f3Min[1] + f3Max[1]) / 2.0f;
		f3Center.z = (f3Min[2] + f3Max[2]) / 2.0f;

		f3Extents.x = f3Max[0] - f3Center.x;
		f3Extents.y = f3Max[1] - f3Center.y;
		f3Extents.z = f3Max[2] - f3Center.z;


		m_cBounds = new CPlane(fDistance, f3Normal,f3Center,f3Extents);	
	}
	else if (eType == Bounds::Cylinder)
	{
		for (unsigned int i = 0; i < vVertices.size(); i++)
		{
			for (unsigned int xyz = 0; xyz < 3; xyz++)
			{
				if (vVertices[i].m_fPosition[xyz] < f3Min[xyz])
				{
					f3Min[xyz] = vVertices[i].m_fPosition[xyz];
				}
				if (vVertices[i].m_fPosition[xyz] > f3Max[xyz])
				{
					f3Max[xyz] = vVertices[i].m_fPosition[xyz];
				}
			}
		}
		XMFLOAT3 f3Center;
		XMFLOAT3 f3Extents;

		f3Center.x = (f3Min[0] + f3Max[0]) / 2.0f;
		f3Center.y = (f3Min[1] + f3Max[1]) / 2.0f;
		f3Center.z = (f3Min[2] + f3Max[2]) / 2.0f;

		f3Extents.x = f3Max[0] - f3Center.x;
		f3Extents.y = f3Max[1] - f3Center.y;
		f3Extents.z = f3Max[2] - f3Center.z;

		float fRadius = f3Extents.x;
		if (f3Extents.z > fRadius)
		{
			fRadius = f3Extents.z;
		}


		XMFLOAT3 f3Start, f3End;
		f3Start = f3End = f3Center;
		f3Start.y = f3Min[1];
		f3End.y = f3Max[1];

		m_cBounds = new CCylinder(f3Start,f3End,fRadius);
	}
	else if (eType == Bounds::Capsule)
	{
		for (unsigned int i = 0; i < vVertices.size(); i++)
		{
			for (unsigned int xyz = 0; xyz < 3; xyz++)
			{
				if (vVertices[i].m_fPosition[xyz] < f3Min[xyz])
				{
					f3Min[xyz] = vVertices[i].m_fPosition[xyz];
				}
				if (vVertices[i].m_fPosition[xyz] > f3Max[xyz])
				{
					f3Max[xyz] = vVertices[i].m_fPosition[xyz];
				}
			}
		}
		XMFLOAT3 f3Center;
		XMFLOAT3 f3Extents;

		f3Center.x = (f3Min[0] + f3Max[0]) / 2.0f;
		f3Center.y = (f3Min[1] + f3Max[1]) / 2.0f;
		f3Center.z = (f3Min[2] + f3Max[2]) / 2.0f;

		f3Extents.x = f3Max[0] - f3Center.x;
		f3Extents.y = f3Max[1] - f3Center.y;
		f3Extents.z = f3Max[2] - f3Center.z;

		float fRadius = fabs(f3Extents.x);
		if (fabs(f3Extents.z) > fRadius)
		{
			fRadius = fabs(f3Extents.z);
		}
		//TESTING CODE 
		fRadius -= 34.0f;


		XMFLOAT3 f3Start, f3End;
		f3Start = f3End = f3Center;
		f3Start.y = f3Min[1] + fRadius;
		f3End.y = f3Max[1] - fRadius;
		m_cBounds = new CCapsule(f3Start, f3End, fRadius);
	}
	else
	{
		assert(0 && "Unsupported Type ATM.  Let Matt know that the bounds you want doesnt work.");
	}
}


/*****************************************************************************
* UpdatePosition(XMFLOAT3)
*
* Ins:						XMFLOAT3 f3Velocity - Current Velocity of owning object
*												  multiplied by Delta Time
*
* Outs:						None
*
* Returns:					None
*
* Mod. Date:				08/12/2015
* Mod. Initials:			MZ
***************************************************************************/
void CCollider::UpdatePosition(XMFLOAT3 f3Velocity)
{
	m_cBounds->UpdatePosition(f3Velocity);
}

/*****************************************************************************
* SetResetVelocity(XMFLOAT3)
*
* Ins:						XMFLOAT3 f3Velocity - Current Velocity of owning object before Collision
*
* Outs:						None
*
* Returns:					None
*
* Mod. Date:				08/12/2015
* Mod. Initials:			MZ
***************************************************************************/
void CCollider::SetResetVelocity(XMFLOAT3 f3Velocity)
{
	m_f3ResetVelocity.x = -f3Velocity.x;
	m_f3ResetVelocity.y = -f3Velocity.y;
	m_f3ResetVelocity.z = -f3Velocity.z;
}

/*****************************************************************
* GetType()				Returns the type of Bounds the object is
*
* Ins:					None
*
* Outs:					None
*
* Returns:				eBoundsType
*
* Mod. Date:		    08/12/2015
* Mod. Initials:	    MZ
*****************************************************************/
Bounds::eBoundsType CCollider::GetType()
{
	return m_cBounds->GetType();
}