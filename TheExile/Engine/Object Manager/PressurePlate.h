/***********************************************
* Filename:  		PressurePlate.h
* Date:      		9/1/2015
* Mod. Date:
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			Pressure plate object, for each trap that needs it.
************************************************/
#ifndef _PRESSURE_PLATE_H
#define _PRESSURE_PLATE_H
	
#include "IObject.h"
class CMesh;
class CPressurePlate : public IObject
{

	bool m_bFireTrapSoundPlayed = false;
public:
	/*****************************************************************
	* Update()				Empty Update for overload
	*
	* Ins:
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/02/2015
	* Mod. Initials:		NS
	*****************************************************************/
	void Update();

	/*****************************************************************
	* CPressurePlate()		Initializes the class 
	*
	* Ins:
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/02/2015
	* Mod. Initials:		NS
	*****************************************************************/
	CPressurePlate(XMFLOAT3 _location, bool _used);

	/*****************************************************************
	* ~CPressurePlate()		Cleans up memory / remove obj from objManager
	*
	* Ins:
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/02/2015
	* Mod. Initials:		NS
	*****************************************************************/
	~CPressurePlate();


	/*****************************************************************
	* CollisionResponse()	Handles response for each pressure plate
	*
	* Ins:
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/02/2015
	* Mod. Initials:		NS
	*****************************************************************/
	void CollisionResponse(IObject * _cCollidedObject);


	/*********************************
	MUTATORS
	************************************/
	void SetFireSoundPlayed(bool _played) { m_bFireTrapSoundPlayed = _played; }
};

#endif