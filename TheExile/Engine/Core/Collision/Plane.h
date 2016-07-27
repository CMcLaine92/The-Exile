#ifndef _PLANE_H_
#define _PLANE_H_

/***********************************************
* Filename:  		Plane.h
* Date:      		08/12/2015
* Mod. Date: 		08/16/2015
* Mod. Initials:	MZ
* Author:    		Matthew Zanini
* Purpose:   		Defines an Plane
*					for use in Collision
************************************************/

#include "Bounds.h"
class CPlane : public CBounds
{

private:
	//Distance from Origin
	float m_fDistance;

	XMFLOAT3 m_f3Normal;

	XMFLOAT3 m_f3Center;

	XMFLOAT3 m_f3Extents;
public:


	/// <summary>
	/// Returns smallest value in AABB which is equal to Center-Extents.
	/// </summary>
	/// <param> none</param>
	XMFLOAT3 Min(void);

	/// <summary>
	/// Returns largest value in AABB which is equal to Center+Extents.
	/// </summary>
	/// <param> none</param>
	XMFLOAT3 Max(void);


	/**************************
		Accessors
	**************************/

	float GetDistance()
	{
		return m_fDistance;
	}
	XMFLOAT3 GetNormal()
	{
		return m_f3Normal;
	}
	XMFLOAT3 GetExtents()
	{
		return m_f3Extents;
	}
	XMFLOAT3 GetCenter()
	{
		return m_f3Center;
	}

	/*********************************
		Mutators
	************************************/

	void SetNormal(XMFLOAT3 f3Normal)
	{
		m_f3Normal = f3Normal;
	}

	void SetDistance(float fDistance)
	{
		m_fDistance = fDistance;
	}
	void SetExtents(XMFLOAT3 f3Extents)
	{
		m_f3Extents = f3Extents;
	}
	void SetCenter(XMFLOAT3 f3Center)
	{
		m_f3Center = f3Center;
	}

	/// <summary>
	/// Returns type of Bounds the object is.
	/// </summary>
	/// <param> none</param>
	virtual Bounds::eBoundsType GetType();

	/// <summary>
	/// Constructor to initialize all data.
	/// </summary>
	/// <param name = "fDistance"> Distance away from origin.</param>
	/// <param name = "f3Normal"> Normal of the plane.</param>
	/// <param name = "f3Center"> The Center of the plane.</param>
	/// <param name = "f3Extents"> The extents of the plane.</param>
	CPlane(float fDistance, XMFLOAT3 f3Normal, XMFLOAT3 f3Center, XMFLOAT3 f3Extents);

	/// <summary>
	/// Updates the position of the collider based on velocity
	/// </summary>
	/// <param name = "f3Velocity"> The collider's velocity.</param>
	void UpdatePosition(XMFLOAT3 f3Velocity) override;
};

#endif