/***********************************************
* Filename:  		DoorObject.h
* Date:      		8/14/2015
* Mod. Date: 		9/10/2015
* Mod. Initials:	MZ
* Author:    		Nick Hryshko
* Purpose:			Represents a door object
************************************************/

#ifndef CDOOROBJECT_H
#define CDOOROBJECT_H

#include "IObject.h"
#include "../Core/Debug Console/DebugConsole.h"
#include "../../Engine/Renderer/Renderer.h"
#include <iostream>
#define DOOR_MOVE_TIME 5.0f

class CDayNight;

class CDoorObject : public IObject
{
private:
	CDayNight* m_cpDayNight;

	//Only calculate move direction once
	bool m_bHasMoveDirection = false;
	unsigned int m_unDirection = 0;
	float m_fMoveTimer = -1.0f;

	bool bOpening = false;
	bool bClosing = false;

	//Helper functions
	//Find the direction to move based on collider size
	void CalculateDirection();
	void Movement();
public:
	void SetMoveTimer(float fMoveTimer);

	void CollisionResponse(IObject * _cCollidedObject) override;
	unsigned int GetDirection(){ return m_unDirection; }
	CDoorObject(XMFLOAT3 position, string szTag, bool vertical = false);
	~CDoorObject();

	void Update();

	void SetDayNight(CDayNight* _ptr) {m_cpDayNight = _ptr;}
	void TempOpen();
};

#endif