/***********************************************
* Filename:  		Capsule.h
* Date:      		08/12/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Defines a Capsule
*					for use in Collision
************************************************/

#include "Capsule.h"
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
Bounds::eBoundsType CCapsule::GetType()
{
	return Bounds::Capsule;
}

/*****************************************************************************
* CCapsule(XMFLOAT3,float)	Constructor that initializes the Capsule
*							with passed in values
*
* Ins:						XMFLOAT3 f3Start - sets m_f3Start to this value
*							XMFLOAT3 f3End   - sets m_f3End to this value
*							float	 fRadius - sets m_fRadius to this value
*
* Outs:						None
*
* Returns:					None
*
* Mod. Date:				08/12/2015
* Mod. Initials:			MZ
***************************************************************************/
CCapsule::CCapsule(XMFLOAT3 f3Start, XMFLOAT3 f3End, float fRadius)
{
	m_f3Start = f3Start;
	m_f3End = f3End;
	m_fRadius = fRadius;
}
void CCapsule::SetCenter(XMFLOAT3 center)
{
	float storedYValue = m_f3Start.y;
	m_f3Start = center;
	m_f3Start.y = storedYValue;

	storedYValue = m_f3End.y;
	m_f3End = center;
	m_f3End.y = storedYValue;
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
void CCapsule::UpdatePosition(XMFLOAT3 f3Velocity)
{
	m_f3Start.x += f3Velocity.x;
	m_f3Start.y += f3Velocity.y;
	m_f3Start.z += f3Velocity.z;

	m_f3End.x += f3Velocity.x;
	m_f3End.y += f3Velocity.y;
	m_f3End.z += f3Velocity.z;
}
