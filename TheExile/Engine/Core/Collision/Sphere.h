#ifndef _SPHERE_H_
#define _SPHERE_H_


/***********************************************
* Filename:  		Sphere.h
* Date:      		08/12/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Defines a Sphere
*					for use in Collision
************************************************/

#include "Bounds.h"
class CSphere : public CBounds
{

private:
	XMFLOAT3 m_f3Center;
	float m_fRadius;
public:

	//Accessors
	XMFLOAT3 GetCenter()
	{
		return m_f3Center;
	}
	float GetRadius()
	{
		return m_fRadius;
	}

	//Mutators
	void SetCenter(XMFLOAT3 f3Center)
	{
		m_f3Center = f3Center;
	}
	void SetRadius(float fRadius)
	{
		m_fRadius = fRadius;
	}


	/// <summary>
	/// Returns type of Bounds the object is.
	/// </summary>
	/// <param> none</param>
	virtual Bounds::eBoundsType GetType();


	/// <summary>
	/// Constructor to initialize all data.
	/// </summary>
	/// <param name = "f3Start"> The center of the sphere.</param>
	/// <param name = "fRadius"> The radius of the sphere.</param>
	CSphere(XMFLOAT3 f3Center, float fRadius);


	/// <summary>
	/// Updates the position of the bounds based on velocity
	/// </summary>
	/// <param name = "f3Velocity"> The bounds's velocity.</param>
	void UpdatePosition(XMFLOAT3 f3Velocity) override;
};

#endif