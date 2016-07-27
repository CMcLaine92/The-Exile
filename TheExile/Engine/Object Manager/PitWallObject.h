/***********************************************
* Filename:  		PitWallObject.h
* Date:      		8/14/2015
* Mod. Date: 		8/14/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a pit wall object
************************************************/

#ifndef CPITWALLOBJECT_H
#define CPITWALLOBJECT_H

#include "IObject.h"
#include "../Core/Debug Console/DebugConsole.h"
#include "../../Engine/Renderer/Renderer.h"
#include <iostream>

class CPitWallObject : public IObject
{
private:



public:

	CPitWallObject(string szTag);
	~CPitWallObject();

	void Update();
};

#endif