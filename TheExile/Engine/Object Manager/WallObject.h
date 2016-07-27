/***********************************************
* Filename:  		WallObject.h
* Date:      		8/14/2015
* Mod. Date: 		8/14/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents the wall object
************************************************/

#ifndef CWALLOBJECT_H
#define CWALLOBJECT_H

#include "IObject.h"
#include "../Core/Debug Console/DebugConsole.h"
#include "../../Engine/Renderer/Renderer.h"
#include "../Core/Collision/Collider.h"

#include <iostream>

class CWallObject :	public IObject
{
private:

public:

	CWallObject(string szTag);
	~CWallObject();

	void Update();
	
};

#endif
