/***********************************************
* Filename:  		GameplayState.h
* Date:      		8/12/2015
* Mod. Date: 		8/24/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:   		This is the game state where all 
*					gameplay takes place
************************************************/

#ifndef CGAMEPLAYSTATE_H
#define CGAMEPLAYSTATE_H

#include "State.h"
#include "../../../../Engine/Animation/Animation.h"
#include <atomic>
#include "../../../../Game/Source/Environment/LightWeaponChest.h"
#include "../../../../Game/Source/Environment/HeavyWeaponChest.h"
#include "../../../../Game/Source/Environment/HealthUpgradeChest.h"

#define GAME_OVER_TIMER 7.5f
#define PAUSED_INPUT_TIME 0.15f
#define DAY_LIMIT 5

struct TDirectLight;

class CMessage;
class CLevelLoader;
class CInputManager;
class CObjectManager;
class CAssetManager;
class CRenderMesh;
class CHUDMesh;
class CNavGraph;
class CMesh;
class CAnimationManager;
class CSkeletonGroup;
class CSpawnControl;
class CParticleManager;
class CSpikeTrap;
class CDayNight;
class CBed;
class CPlayer;
class CCamera;
class CTree;
class CCrafting;
class CCutsceneManager;
class CMaze;
class CStorageCrate;
class CMinotaurSpawn;
class CExitKey;

//Used for Multithreaded loading
//struct TThreadData
//{
//	atomic<bool> bIsFinishedLoading = false;
//	CHUDMesh* cpControlsMesh = nullptr;
//	CHUDMesh* cpGameOverMesh = nullptr;
//	CHUDMesh* cpTextFrame = nullptr;
//	TDirectLight* cpTDirectLight = nullptr;
//	CAnimationManager* cpAnimationManager = nullptr;
//	CObjectManager* cpObjectManager = nullptr;
//	CLevelLoader* cpLevelLoader = nullptr;
//	CAssetManager* cpAssetManager = nullptr;
//	CPlayer* cpPlayer = nullptr;
//	CNavGraph* cpNavGraph = nullptr;
//	CCamera* cpCamera = nullptr;
//	CParticleManager* cpParticleManager = nullptr;
//	CDayNight* cpDayNight = nullptr;
//	CSpawnControl* cpSpawnControl = nullptr;
//};
class CGameplayState : public IState
{
private:

	enum EPausedOptions { eResume, eOptions, eControls, eQuit };

	CMinotaurSpawn* minSpawn = nullptr;
	/// <summary>
	/// This is the message callback function.
	/// </summary>
	/// <param name = "pMsg"> The message being processed.</param>
	static void MessageProc(const CMessage* pMsg);

	static CGameplayState* m_cpGameState;

	// Day Limit Variables //
	bool bLose = false;
	//unsigned int m_nDayCount = 0;
	float m_fDayPromptTimer = 5.0f;
	float m_fGameOverScreenTimer = 4.0f;
	CHUDMesh* m_cGameOverScreen = nullptr;

	CMesh* m_cpSkyBoxMesh;
	TDirectLight* m_tDirectLight = nullptr;

	CParticleManager* m_cpParticleManager;
	CLevelLoader* m_cpLevel;
	AudioSystemWwise m_cAudioManager;
	CInputManager* m_cpInputManager;
	CObjectManager* m_cpObjectManager;
	CAssetManager* m_cpAssetManager;
	CAnimationManager * m_cpAnimationManager;
	CCutsceneManager* m_cpCutsceneManager;
	//the player
	CRenderMesh* m_cPlayerMesh;
	IObject* m_cpThePlayer;
	CLightWeaponChest * m_cWeaponChest = nullptr;
	CHeavyWeaponChest * m_cShieldChest1 = nullptr;
	CHeavyWeaponChest * m_cShieldChest2 = nullptr;
	CHeavyWeaponChest * m_cShieldChest3 = nullptr;
	CHealthUpgradeChest * m_cHealthChest1 = nullptr;
	CHealthUpgradeChest * m_cHealthChest2 = nullptr;
	CHealthUpgradeChest * m_cHealthChest3 = nullptr;

	//the camera
	CCamera* m_cpTheCamera;
	
	// Skeleton for Monday build 8/17
	CSpawnControl* m_cpSpawnControl;
	CNavGraph* m_cpNavigation;

	//bridges
	vector<IObject*> m_vpBridges;
	CBed* m_cpBed = nullptr;
	// Bool for first build.
	bool m_bIsPlayingMusic = true;

	//pause menu
	bool m_bPaused = false;
	float m_fPausedInputTimer;
	int m_nPauseCursor;
	vector<CHUDMesh*> m_vpPausedControls;

	//in game controls(f1)
	bool m_bControlsON = false;
	CHUDMesh* m_cpControlsMesh;
	int m_nF1CurrentMenu = 0;
	vector<wstring> m_vszScreenNames;
	vector<wstring> m_vszF1Textures;
	CRenderMesh * m_cpRightArrowMesh;
	CRenderMesh * m_cpLeftArrowMesh;

	//game over 
	bool m_bGameOver = false;
	bool m_bExitDoorMove = false;
	float m_fExitDoorMoveTimer = 0.0f;
	float m_fGameOverTimer = 0.0f;
	unsigned int m_unGameOverAudioTics = 0;

	//controller
	bool m_bControllerPopUp;
	bool m_bDCControllerCursor;
	bool m_bControllerWasUsed;
	CHUDMesh* m_cpControllerPopUpMesh;

	//Day and Night
	CDayNight * m_pcDayNight = nullptr;

	//bridge teleporter
	unsigned int m_unCurrentBridgeID;

	//Thread Data For Loading
	

	// Objective Display timer.
	float m_fObjectiveTimer = 5.0f;

	bool m_bMusicPlaying = false;
	CHUDMesh* m_cpTextFrame;
	//Skybox Object
	CRenderMesh* m_cpSkybox = nullptr;

	vector<CTree*> theTree;

	CCrafting* m_cpCraftingTable;
	CStorageCrate* m_cpStorageCrate;

	// Temp moving trap
	IObject* tempTrap;

	CMaze* m_cpMaze;

	int m_nExitIndex = 0;
	int m_nBushIndex = 0;
	int m_nTreeIndex = 0;
	int m_nTrapIndex = 0;



	vector<IObject*> m_vpCrackedWalls;
	vector<IObject*> Traps;

	vector<IObject*> m_vpBushes;
	vector<IObject*> m_vpTrees;

	vector<IObject*> m_vpExits;

	vector<CExitKey*> m_vpExitKeys;

	//helper functions
	EInputResult HandleGameOver();
	void LoadingGraphics();
	EInputResult PauseMenu();
	void CreateSkybox();
	void DCControllerInit();

public:

	CGameplayState();
	~CGameplayState() = default;

	virtual void Enter();
	virtual void Exit();

	void ReInitialize();

	bool IsGameOver() { return m_bGameOver; }
	bool DidPlayerWin() { return bLose; }



	/// <summary>
	/// Updates the GameplayState.
	/// </summary>
	/// <param> none</param>
	void Update();

	/// <summary>
	/// Shuts down the only instane of the GameplayState.
	/// </summary>
	/// <param> none</param>
	eStateType GetStateType() { return eGAMEPLAY; }

	EInputResult Input();
	static CGameplayState * GetInstance() {return m_cpGameState;}
	CObjectManager* GetObjectManager() { return m_cpObjectManager; }


	vector<IObject*>& GetBridges() {return m_vpBridges;}
	IObject* GetPlayer() const {return m_cpThePlayer;}
};

#endif
