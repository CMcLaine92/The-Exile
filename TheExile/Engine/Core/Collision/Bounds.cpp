/***********************************************
* Filename:  		Bounds.cpp
* Date:      		08/12/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Defines the base class for all
*                   bounding volumes
************************************************/

#include "Bounds.h"
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
Bounds::eBoundsType CBounds::GetType()
{
	return Bounds::Base;
}
/*****************************************************************************
* UpdatePosition(XMFLOAT3)
*
* Ins:						XMFLOAT3 - Unused
*
* Outs:						None
*
* Returns:					None
*
* Mod. Date:				08/12/2015
* Mod. Initials:			MZ
*
*NOTE: Left empty for inheritance reasons
***************************************************************************/
void CBounds::UpdatePosition(XMFLOAT3)
{

}