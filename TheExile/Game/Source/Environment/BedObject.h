/***********************************************
* Filename:  		BedObject.h
* Date:      		09/15/2015
* Mod. Date:
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			Allowes the player to pass the time back to morning when within the safe zone.
************************************************/
#ifndef CBED_H
#define CBED_H
#include "../../../Engine/Object Manager/IObject.h"

class CDayNight;
class CHUDMesh;
class CCutsceneManager;
class CGameplayState;
class CObjectManager;
class CHUD;

class CBed : public IObject
{
	// The current Day time pointer.
	CDayNight* m_cpTheDayNight = nullptr;

	CCutsceneManager* m_cpCutsceneManager;
	unsigned int m_unCutsceneID;

	CHUDMesh* m_cpTextFrame;
	CHUDMesh* m_cpOverlay;
	CHUDMesh* m_cpControllerPopUp;

	CHUD* m_cpHud = nullptr;

	bool m_bDrawInd = false;
	bool m_bActive = false;

	float m_fCooldown;

	// For saving
	CObjectManager* m_pObjectManager;

public:
	CBed(CDayNight* _ptr, CObjectManager* objManager);
	~CBed();

	void Update();

	void DrawUnAvailableIndicator();
	void DrawAvailableIndicator();

	void SetToMorning();
	bool SaveGame();
	string GetFilePath();
	/*********************************
	ACCESSORS
	************************************/
	bool IsAvailable();

	/*********************************
	MUTATORS
	************************************/
	void SetCutsceneManager(CCutsceneManager* _manager) { m_cpCutsceneManager = _manager; }
	void SetHUD(CHUD* hud) { m_cpHud = hud; }
	void AddToRenderer();

};

struct TSaveData
{
	vector<bool> m_vbBridges;
	vector<bool> m_vbCrackedWalls;
	vector<bool> m_vbMapFog;
	vector<int> m_vnInventory;
	vector<int> m_nStorageCrate;
	vector<bool> m_bTorches;
	bool m_bExitKey;
	float m_fTotalTime;

	// 0 - Weapon | 1 - Shield | 2 - Health |
	vector<int> m_Uprades;

	int m_nDayNumber;
};

#endif