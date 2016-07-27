/***********************************************
* Filename:  		MainMenuState.h
* Date:      		8/18/2015
* Mod. Date: 		9/18/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:   		This is the game state that takes 
*					care of the main menu
************************************************/

#ifndef CMAINMENUSTATE_H
#define CMAINMENUSTATE_H

#include "State.h"
#include "../../../Renderer/RenderMesh.h"
#include "../../../Animation/Mesh.h"

#define NUMBER_OF_FACES 5
#define ROTATION_DIRECTIONS 4
#define ROTATION_DEGREE 90
#define SPLASH_TIME 3.0f
#define CLOUDS_OFFSET_Y 2.0f

struct TDirectLight;

class CInputManager;
class CObjectManager;
class CAssetManager;
class CRenderMesh;
class CHUDMesh;
class CCamera;
class CMesh;
class CParticleManager;

class CMainMenuState : public IState
{
private:

	enum eMenuFaces
	{
		ePlay_Face = 0,
		eOptions_Face = 1,
		eControls_Face = 2,
		eCredits_Face = 3,
		eExit_Face = 4,
	};

	static eMenuFaces m_cPreviousFace;

	enum eSelectedMesh
	{
		ePlay = 0,
		eOptions = 1,
		eControls = 2,
		eCredits = 3,
		eExit = 4,
	};

	enum eRotationDirection
	{
		eRotate_Right = 0,
		eRotate_Left = 1,
		eRotate_Up = 2,
		eRotate_Down = 3,
	};

	struct TMenuFace
	{
		eMenuFaces eFace;
		CMesh cMesh;
	};

	struct TMenuCube
	{
		TMenuFace m_tFaces[NUMBER_OF_FACES];
		eMenuFaces eCurrentFace;
		unsigned int unCursorMesh;
	};

	// Splash Screen Variables //
	int m_nSplashCount = 0;
	bool m_bSplashIntro = true;
	float m_fSplashTimer = SPLASH_TIME;
	CHUDMesh* m_cSplashScreen1 = nullptr;
	CHUDMesh* m_cSplashScreen2 = nullptr;
	CHUDMesh* m_cSplashScreen3 = nullptr;

	bool bExitPrompt;
	CHUDMesh* cpExitPrompt;

	CInputManager* m_cpInput;
	CAssetManager* m_cpAssetManager;
	CObjectManager* m_cpObjectManager;
	CCamera* m_cpCamera;
	TDirectLight* m_tDirectLight;
	AudioSystemWwise* m_cpAudio;


	float m_fInputTimer;//used to check if the cube is is a rotation or not
	bool m_bRotateFace[4];
	bool m_bToGamePlay = false;
	TMenuCube m_tTheCube;

	IObject* m_cpPlayPlane;
	IObject* m_cpOptionsPlane;
	IObject* m_cpControlsPlane;
	IObject* m_cpCreditsPlane;
	IObject* m_cpExitPlane;
	IObject* m_cpAttachObj;//used for attaching the camera too

	CRenderMesh* m_cpSkybox;

	bool m_bControllerPopUp;
	bool m_bDCControllerCursor;
	bool m_bControllerWasUsed;
	CHUDMesh* m_cpControllerPopUpMesh;

	CHUDMesh* m_cpMenuControlsMesh;
	bool m_bToggleControls;

	CParticleManager* m_cpParticleManager;
	enum ECloudParticles { eCloud1, eCloud2, eCloud3, eCloud4, eCloud5, eCloudCount };
	unsigned int m_unCloudParticles[eCloudCount];


	/// <summary>
	/// Checks the cube to see if its currently rotating or not
	/// </summary>
	/// <param> none</param>
	bool CheckRotatingFace();

	/// <summary>
	/// Creates the skybox for this scene
	/// </summary>
	/// <param> none</param>
	void CreateSkybox();

	void Load();
	std::string GetFilePath();

	void PlayCloudParticles();

public:

	/// <summary>
	/// Constructor for MainMenuState
	/// </summary>
	/// <param> none</param>
	CMainMenuState();

	/// <summary>
	/// Destructor for MainMenuState
	/// </summary>
	/// <param> none</param>
	~CMainMenuState() = default;
	eStateType GetStateType() { return eMAINMENU; }

	/// <summary>
	/// Updates the MainMenuState.
	/// </summary>
	/// <param> none</param>
	void Update();

	bool ToGameplay() { return m_bToGamePlay; }
	/// <summary>
	/// Terminates the MainMenuState.
	/// </summary>
	/// <param> none</param>
	EInputResult Input();
	void Enter();
	void Exit();
	/// <summary>
	/// Resets all objects to the renderer to be drawn
	/// </summary>
	/// <param> none</param>
	void ReInitialize();

};

#endif
