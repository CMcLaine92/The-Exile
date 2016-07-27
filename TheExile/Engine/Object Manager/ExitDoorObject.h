/***********************************************
* Filename:  		ExitDoorObject.h
* Date:      		8/20/2015
* Mod. Date: 		8/20/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents the exit door
************************************************/

#ifndef CEXITDOOROBJECT_H
#define CEXITDOOROBJECT_H

#include "IObject.h"
#include "../../Engine/Renderer/Renderer.h"

#define MOVE_DISTANCE 500.0f

class CExitDoorObject : public IObject
{
private:

	//verticle movement
	float m_fMoveTimer;
	float m_fTotalMoveTime;
	bool m_bMoving;
	bool m_bPlayAudio;
	int m_nMoveCounter;//to ensure it only moves once

	bool m_bIsActive;
	AudioSystemWwise* m_cpExitDoorAudio;

public:

	CExitDoorObject(XMFLOAT3 position, string szTag, bool verical = false);
	~CExitDoorObject();

	const bool GetActive() const { return m_bIsActive; }
	void MakeActive()  { m_bIsActive = true; }

	void Update();

	void ActivateVerticleMovement(float moveTime);


	//keep this for old levelloader to compile
	void SetMovementTime(float moveTime);


	virtual void SetWorldMatrix(const XMFLOAT4X4 world);

};

#endif
