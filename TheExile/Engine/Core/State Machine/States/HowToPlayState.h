/***********************************************
* Filename:  		HowToPlayState.h
* Date:      		8/19/2015
* Mod. Date: 		8/20/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:   		This is the game state that informs
*					the player/user the controls
************************************************/

#ifndef CHOWTOPLAYSTATE_H
#define CHOWTOPLAYSTATE_H

#include "State.h"

#define CONTROLLER_DC_TIMER 3.0f

class IObject;
class CHUDMesh;
class CInputManager;
class CObjectManager;
class CAssetManager;
class CHUDMesh;
class CCamera;
class CMesh;

class CHowToPlayState : public IState
{
private:

	enum eCursorStates { KEYBOARD_IMAGE, CONTROLLER_IMAGE };//may add keybindings options 

	CMesh* m_cpMesh;
	CInputManager* m_cpInput;
	CAssetManager* m_cpAssetManager;
	CObjectManager* m_cpObjectManager;
	AudioSystemWwise* m_cpAudio;
	CCamera* m_cpCamera;

	//the controls image
	CHUDMesh* m_cpControls;
	IObject* m_cpPlaneObj;//camera attach object
	unsigned int m_unCursor;

	//controller
	bool m_bControllerUsed;
	bool m_bControllerDC;
	float m_fControllerTimer;

	//this states controls image (next/back)
	CHUDMesh* m_cpMiniControls;

	//the controls type image (top center image)
	CHUDMesh* m_cpControlsTypeImg;

public:

	/// <summary>
	/// Constructor for HowToPlayState
	/// </summary>
	/// <param> none</param>
	CHowToPlayState();

	/// <summary>
	/// Default destructor for HowToPlayState
	/// </summary>
	/// <param> none</param>
	~CHowToPlayState() = default;

	/// <summary>
	/// Updates the HowToPlayState
	/// </summary>
	/// <param> none</param>
	void Update();

	eStateType GetStateType() { return eCONTROLS; }


	EInputResult Input();
	virtual void Enter();
	virtual void Exit();

};

#endif
