/***********************************************
* Filename:  		WaypointObject.h
* Date:      		9/2/2015
* Mod. Date: 		9/2/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a waypoint 
************************************************/

#ifndef CWAYPOINTOBJECT_H
#define CWAYPOINTOBJECT_H

#include "IObject.h"
#include <iostream>


class CWaypointObject : public IObject
{

public:

	CWaypointObject(string szTag);
	~CWaypointObject();

	void Update();
};

#endif
