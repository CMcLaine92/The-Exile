/***********************************************
* Filename:  		Sphere.cpp
* Date:      		08/12/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Defines a Sphere
*					for use in Collision
************************************************/
#include "Sphere.h"
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
Bounds::eBoundsType CSphere::GetType()
{
	return Bounds::Sphere;
}

/*****************************************************************************
* CSphere(XMFLOAT3,float)	Constructor that initializes the Sphere
*							with passed in values
*
* Ins:						XMFLOAT3 f3Center - sets m_f3Center to this value
*							float	 fRadius - sets m_fRadius to this value
*
* Outs:						None
*
* Returns:					None
*
* Mod. Date:				08/12/2015
* Mod. Initials:			MZ
***************************************************************************/
CSphere::CSphere(XMFLOAT3 f3Center, float fRadius)
{
	m_f3Center = f3Center;
	m_fRadius = fRadius;
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
void CSphere::UpdatePosition(XMFLOAT3 f3Velocity)
{
	m_f3Center.x += f3Velocity.x;
	m_f3Center.y += f3Velocity.y;
	m_f3Center.z += f3Velocity.z;

}