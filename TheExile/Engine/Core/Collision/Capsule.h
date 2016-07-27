#ifndef _CAPSULE_H_
#define _CAPSULE_H_


/***********************************************
* Filename:  		Capsule.h
* Date:      		08/12/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Defines a Capsule
*					for use in Collision
************************************************/

#include "Bounds.h"
class CCapsule : public CBounds
{

private:
	XMFLOAT3 m_f3Start;
	XMFLOAT3 m_f3End;
	float m_fRadius;
public:

	//Accessors
	XMFLOAT3 GetStart()
	{
		return m_f3Start;
	}
	XMFLOAT3 GetEnd()
	{
		return m_f3End;
	}
	float GetRadius()
	{
		return m_fRadius;
	}

	//Mutators
	void SetStart(XMFLOAT3 f3Start)
	{
		m_f3Start = f3Start;
	}
	
	void SetEnd(XMFLOAT3 f3End)
	{
		m_f3End = f3End;
	}

	void SetRadius(float fRadius)
	{
		m_fRadius = fRadius;
	}

	virtual void SetCenter(XMFLOAT3 center);



	/// <summary>
	/// Returns type of Bounds the object is.
	/// </summary>
	/// <param> none</param>
	virtual Bounds::eBoundsType GetType();


	/// <summary>
	/// Constructor to initialize all data.
	/// </summary>
	/// <param name = "f3Start"> The start of the segment.</param>
	/// <param name = "f3End"> The end of the segment.</param>
	/// <param name = "fRadius"> The radius of the capsule.</param>
	CCapsule(XMFLOAT3 f3Start, XMFLOAT3 f3End, float fRadius);


	/// <summary>
	/// Updates the position of the bounds based on velocity
	/// </summary>
	/// <param name = "f3Velocity"> The bounds's velocity.</param>
	void UpdatePosition(XMFLOAT3 f3Velocity) override;
};

#endif