#ifndef _AABB_H_
#define _AABB_H_


/***********************************************
* Filename:  		AABB.h
* Date:      		08/12/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Defines an Axis Aligned Bounding Box
*					for use in Collision
************************************************/

#include "Bounds.h"
class CAABB : public CBounds
{

private:
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

	XMFLOAT3 GetCenter()
	{ 
		return m_f3Center;
	}
	XMFLOAT3 GetExtents()
	{
		return m_f3Extents;
	}

	/*********************************
		Mutators
	************************************/

	virtual void SetCenter(XMFLOAT3 f3Center)
	{
		m_f3Center = f3Center;
	}
	void SetExtents(XMFLOAT3 f3Extents)
	{
		m_f3Extents = f3Extents;
	}


	/// <summary>
	/// Returns type of Bounds the object is.
	/// </summary>
	/// <param> none</param>
	virtual Bounds::eBoundsType GetType();


	/// <summary>
	/// Constructor to initialize all data.
	/// </summary>
	/// <param name = "f3Center"> The center of the AABB.</param>
	/// <param name = "f3Extents"> The distance from center to corner of the AABB.</param>
	CAABB(XMFLOAT3 f3Center, XMFLOAT3 f3Extents);


	/// <summary>
	/// Updates the position of the collider based on velocity
	/// </summary>
	/// <param name = "f3Velocity"> The collider's velocity multiplied by Delta time.</param>
	void UpdatePosition(XMFLOAT3 f3Velocity) override;
};

#endif