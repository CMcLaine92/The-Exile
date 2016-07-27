/***********************************************
* Filename:  		StorageCrate.h
* Date:      		11/11/2015
* Mod. Date:		11/11/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Displayes and handles the stock of the storage crate
************************************************/

#ifndef CSTORAGECRATE_H
#define CSTORAGECRATE_H

#include <unordered_map>
#include "../../../Engine/Object Manager/IObject.h"

#define STOP_INPUT_TIME 0.25f
#define INPUT_IMAGES 6

class CHUDMesh;
class CInputManager;
class CInventory;

class CStorageCrate : public IObject
{
private:

	enum EItems { eBones, eWood, eHerb, eGunpowder, eBomb, eWoodPlank, eHealthPotion, eItemCount };//used for cursor 

	bool m_bIsOpen;
	bool m_bAtCrate;
	bool m_bDrawIndicator;
	int m_nCursor;
	unordered_map<int, int> m_umCurrentStock;//type : amount of that type
	vector<const wchar_t*> m_vIconPaths;
	vector<const wchar_t*> m_vInputImgPaths;

	CHUDMesh* m_cpBackgroundImg;
	vector<CHUDMesh*> m_vpIcons;
	vector<CHUDMesh*> m_vpBoarders;
	CHUDMesh* m_cpTextFrame;
	CHUDMesh* m_cpInputImgs[INPUT_IMAGES];

	CInputManager* m_cpInput;
	CInventory* m_cpInventory;

	void CheckActiveCursor(EItems type);

public:

	CStorageCrate();
	~CStorageCrate();

	void Update();
	void Open(CInventory* inventory);
	void Close();

	void Deposit(EItems _type, unsigned int _amount);
	void Withdrawl(EItems _type, unsigned int _amount);

	void DrawIndicator();

	void AddToRenderer();

	//accessors/mutators
	void SetUnavailable();
	bool IsOpen();
	void SetAtTable();

	unordered_map<int, int>& GetStock() { return m_umCurrentStock; }

};

#endif
