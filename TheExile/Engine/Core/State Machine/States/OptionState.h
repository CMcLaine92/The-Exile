/***********************************************
* Filename:  		OptionsState.h
* Date:      		8/20/2015
* Mod. Date: 		8/20/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:   		
************************************************/

#ifndef COPTIONSSTATE_H
#define COPTIONSSTATE_H

#include "State.h"


#define CURSOR_MAX 5
class CRenderMesh;
class CHUDMesh;
class CObjectManager;
class CCamera;
class CInputManager;
class CAssetManager;
class CMesh;
class COptionState : public IState
{
private:

	CMesh* cMesh1;
	CMesh* plane;


	CInputManager* m_cpInput;
	CAssetManager* m_cpAssetManager;
	CObjectManager* m_cpObjectManager;
	AudioSystemWwise* m_cpAudio;
	CCamera* m_cpCamera;

	// HUD Mesh Variables //
	CHUDMesh* cBackground;

	bool m_bControllerImage;
	CHUDMesh* m_cpMenuControlsMesh;

	
	IObject* m_cpPlaneObj;

	// Audio Variables //
	int nCursor;

	int nFontIndex;
	int nMusicVolume;
	int nSFXVolume;
	bool bFullScreen;
	int nResolution; // 1 - 1920x1080 // 2 - 1280x720 // 3 - 1024x768

	//controller
	bool m_bUseController;

	// Helper Functions //
	void Load();
	void Save();
	std::string GetFilePath();

public:

	COptionState();
	~COptionState() = default;
	virtual void Enter();
	virtual void Exit();
	void Update();
	EInputResult Input();
	eStateType GetStateType() { return eOPTIONS; }

	

};

#endif
