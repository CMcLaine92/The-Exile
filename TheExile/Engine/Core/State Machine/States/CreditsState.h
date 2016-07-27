/***********************************************
* Filename:  		HowToPlayState.h
* Date:      		8/20/2015
* Mod. Date: 		9/08/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:   		This is the game state that shows
*					the game credits
************************************************/

#ifndef CCREDITSSTATE_H
#define CCREDITSSTATE_H

#include "State.h"

#define CREDITS_MOVEMENT_SPEED 0.25f
#define CREDITS_MOVEMENT_DISTANCE 1200.0f
#define CREDITS_TOTAL_MOVE_TIME 20.0f //10 seconds to scroll through all credits
#define CREDITS_TOTAL_MOVE_DISTANCE 4600.0f 

class CInputManager;
class CObjectManager;
class CAssetManager;
class CHUDMesh;
class CCamera;
class CMesh;
class CCreditsState : public IState
{
private:
	CMesh* cMesh;
	CInputManager* m_cpInput;
	CAssetManager* m_cpAssetManager;
	CObjectManager* m_cpObjectManager;
	AudioSystemWwise* m_cpAudio;
	CCamera* m_cpCamera;

	//the attach object
	IObject* m_cpPlaneObj;

	//the background texture
	CHUDMesh* m_cpBackGroundMesh;

	bool m_bControllerUsed;
	bool m_bControllerDC;
	float m_fControllerDCTimer;

	float m_fScrollOffsetY;
	float m_fPrevScrollOffsetY;

public:

	CCreditsState();
	~CCreditsState() = default;
	virtual void Enter();
	virtual void Exit();
	eStateType GetStateType() { return eCREDITS; }

	void Update();
	EInputResult Input();
};

#endif
