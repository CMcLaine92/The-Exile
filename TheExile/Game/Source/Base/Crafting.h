/***********************************************
* Filename:  		Crafting.cpp
* Date:      		10/1/2015
* Mod. Date:
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			Acts as a workstation in the world that enables crafting to be done.
************************************************/

#ifndef CCRAFTING_H
#define CCRAFTING_H

#include "../../../Engine/Object Manager/IObject.h"
#include "../../../Game/Source/Base/Inventory.h"

class CMesh;
class CHUDMesh;

class CCrafting : public IObject
{

	CInventory* m_cpTheInventory;

	map<int, int> m_mCraftingTable;

	CHUDMesh* m_cpTextFrame;
	CHUDMesh* m_cpCraftingBar;
	CHUDMesh* m_cpCraftScreen;
	CHUDMesh* m_cpControllerPopUp;

	int m_nCursor;

	float m_fCraftTime;
	float m_fCraftBarSize;
	float m_fCraftBarStartX;

	vector<const wchar_t*> m_vIconPaths;
	//inventory items(right side)
	vector<CHUDMesh*> m_vcIcons;
	vector<CHUDMesh*> m_vcBoarder;
	//crafting items(left side)
	vector<CHUDMesh*> m_vpCraftingIcons;
	vector<CHUDMesh*> m_vpCraftingBoarders;

	bool m_bDrawIndicator;
	bool m_bOpen = false;

	bool m_bIsCrafting;
	bool m_bAtTable;
	bool m_bCraftAll;
	unsigned int m_unCraftAllAmount;
	CInventory::eItems m_eCraftedItem;

	XMFLOAT4 m_f4CraftableColor;

public:

	CCrafting();
	~CCrafting();

	void Update();

	void DrawIndicator();

	void Crafting();

	void Open();

	void Close();

	/*********************************
	ACCESSORS
	************************************/
	bool IsOpen() const { return m_bOpen; }
	bool IsAtTable() const { return m_bAtTable; }

	/*********************************
	MUTATORS
	************************************/
	void SetOpen( bool _open ) { m_bOpen = _open; }
	void SetAtTable( bool _table ) { m_bAtTable = _table; }
	void SetDrawIndicator( bool _bool ) { m_bDrawIndicator = _bool; }
	void SetCurrentInventory( CInventory* _inv ) { m_cpTheInventory = _inv; }
};

#endif