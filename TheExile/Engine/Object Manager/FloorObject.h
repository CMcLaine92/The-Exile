/***********************************************
* Filename:  		FloorObject.h
* Date:      		8/14/2015
* Mod. Date: 		8/14/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents the floor object
************************************************/

#ifndef CFLOOROBJECT_H
#define CFLOOROBJECT_H

#include "IObject.h"
#include "../Core/Debug Console/DebugConsole.h"
#include "../../Engine/Renderer/Renderer.h"
#include <iostream>

class CFloorObject : public IObject
{
private:



public:

	CFloorObject(string szTag);
	~CFloorObject();

	void Update();

	

};

#endif