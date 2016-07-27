/***********************************************
* Filename:  		PitObject.h
* Date:      		8/14/2015
* Mod. Date: 		8/14/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a pit object
************************************************/

#ifndef CPITOBJECT_H
#define CPITOBJECT_H

#include "IObject.h"
#include "../Core/Debug Console/DebugConsole.h"
#include "../../Engine/Renderer/Renderer.h"
#include <iostream>

class CPitObject : public IObject
{
private:



public:

	CPitObject(string szTag);
	~CPitObject();

	void Update();
};

#endif