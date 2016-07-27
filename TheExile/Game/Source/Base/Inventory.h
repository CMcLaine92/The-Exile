/***********************************************
* Filename:  		Inventory.h
* Date:      		09/13/2015
* Mod. Date:		09/14/2015
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			Displayes and handles the stock of the players inventory.
************************************************/
#ifndef CINVENTORY_H
#define CINVENTORY_H

#define NUM_INPUT_IMAGES 4

class CHUDMesh;
struct TVertex2D;

class CInventory
{

public:

	enum eItems { Bones, Wood, Herb, Gunpowder, Bomb, Bridge, HealthPotion, ItemCount };


	CInventory();
	~CInventory();

	void Update();

	/*****************************************************************
	* Open()				Opens up and renders all the nessasery objects in the inventory.
	*
	* Ins:					None
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/15/2015
	* Mod. Initials:		NS
	*****************************************************************/
	void Open();

	/*****************************************************************
	* Close()				Closes and hides all the nessasery objects in the inventory.
	*
	* Ins:					None
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/15/2015
	* Mod. Initials:		NS
	*****************************************************************/
	void Close();

	/*****************************************************************
	* AddToInventory()		Adds The passed in item type to the current stock.
	*
	* Ins:					eItems
	*						int
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/15/2015
	* Mod. Initials:		NS
	*****************************************************************/
	void AddToInventory( eItems _type, int _ammount );

	/*****************************************************************
	* RemoveFromInventory() Removes The passed in item type from the current stock.
	*
	* Ins:					eItems
	*						int
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/15/2015
	* Mod. Initials:		NS
	*****************************************************************/
	void RemoveFromInventory( eItems _type, int _ammount );

	/*****************************************************************
	* GetStock()			Returns how many items of a spesific type there currently are.
	*
	* Ins:					eItems
	*
	* Outs:					None
	*
	* Returns:				int
	*
	* Mod. Date:		    09/15/2015
	* Mod. Initials:		NS
	*****************************************************************/
	int GetStock( eItems _type ) { return m_CurrentStock[_type]; }

	/*****************************************************************
	* Crafting()			Handles the crafting menu.
	*
	* Ins:					eItems
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/15/2015
	* Mod. Initials:		NS
	*****************************************************************/
	void Crafting();

	void AddGraveStoneStock( map<int, int> _stock );
	void ResetStock();

	void AddToRenderer();

	/*********************************
	ACCESSORS
	************************************/
	bool IsOpen() const { return m_bOpen; }
	vector<const wchar_t*> GetIconPaths() { return m_vIconPaths; }
	map<int, int> GetTotalStock() { return m_CurrentStock; }

	/*********************************
	MUTATORS
	************************************/
	void SetOpen( bool _open ) { m_bOpen = _open; }

private:

	float m_fPickUpTimer;

	bool m_bPickUp;

	int m_nCursor;

	map<int, int> m_CurrentStock;

	vector<vector<TVertex2D>> m_vvBoarderVerts;
	CHUDMesh* m_cpInventoryScreen;
	CHUDMesh* m_cpCurrentItem;
	CHUDMesh* m_cpSelectedIcon;

	vector<CHUDMesh*> m_vcBoarder;
	vector<CHUDMesh*> m_vcIcons;

	vector<const wchar_t*> m_vIconPaths;
	bool m_bOpen = false;

	//input images
	CHUDMesh* m_cpInputImgs[NUM_INPUT_IMAGES];
	vector<const wchar_t*> m_vInputImgPaths;

	//crafting
	CHUDMesh* m_cpCraftingBar;
	bool m_bCrafting;
	bool m_bCraftAll;
	bool m_bAbleToCraft;
	float m_fCraftTime;
	float m_fCraftBarSize;
	float m_fCraftBarStartX;

};

#endif