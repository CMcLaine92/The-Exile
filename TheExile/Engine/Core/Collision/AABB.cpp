/***********************************************
* Filename:  		AABB.cpp
* Date:      		08/12/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Defines an Axis Aligned Bounding Box
*					for use in Collision
************************************************/
#include "AABB.h"

/*****************************************************************
* Min()					Helper function to get center - extents
*
* Ins:					None
*
* Outs:					None
*
* Returns:				XMFLOAT3
*
* Mod. Date:		    08/12/2015
* Mod. Initials:	    MZ
*****************************************************************/
XMFLOAT3 CAABB::Min(void)
{
	XMFLOAT3 f3Output;
	f3Output.x = m_f3Center.x - m_f3Extents.x;
	f3Output.y = m_f3Center.y - m_f3Extents.y;
	f3Output.z = m_f3Center.z - m_f3Extents.z;
	return f3Output;
}

/*****************************************************************
* Max()					Helper function to get center + extents
*
* Ins:					None
*
* Outs:					None
*
* Returns:				XMFLOAT3
*
* Mod. Date:		    08/12/2015
* Mod. Initials:	    MZ
*****************************************************************/
XMFLOAT3 CAABB::Max(void)
{
	XMFLOAT3 f3Output;
	f3Output.x = m_f3Center.x + m_f3Extents.x;
	f3Output.y = m_f3Center.y + m_f3Extents.y;
	f3Output.z = m_f3Center.z + m_f3Extents.z;
	return f3Output;
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
Bounds::eBoundsType CAABB::GetType()
{
	return Bounds::AABB;
}


/*****************************************************************************
* CAABB(XMFLOAT3,XMFLOAT3)	Constructor that initializes the AABB
*							with passed in values
*
* Ins:						XMFLOAT3 f3Center - sets m_f3Center to this value 
*							XMFLOAT3 f3Extents - sets m_f3Extents to this value
*
* Outs:						None
*
* Returns:					None
*
* Mod. Date:				08/12/2015
* Mod. Initials:			MZ
***************************************************************************/
CAABB::CAABB(XMFLOAT3 f3Center, XMFLOAT3 f3Extents)
{
	m_f3Center = f3Center;
	m_f3Extents = f3Extents;
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
void CAABB::UpdatePosition(XMFLOAT3 f3Velocity)
{
	m_f3Center.x += f3Velocity.x;
	m_f3Center.y += f3Velocity.y;
	m_f3Center.z += f3Velocity.z;

}