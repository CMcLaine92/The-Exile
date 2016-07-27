/***********************************************
* Filename:  		StorageCrate.cpp
* Date:      		11/11/2015
* Mod. Date:		11/11/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Displayes and handles the stock of the storage crate
************************************************/

#include "StorageCrate.h"
#include "../../../Engine/Renderer/HUDMesh.h"
#include "../../../Engine/Renderer/Renderer.h"
#include "../../../Engine/Core/Input/InputManager.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Core/Messages/AddObjectMessage.h"
#include "../../../Engine/Core/Messages/RemoveObjectMessage.h"
#include "../../../Game/Source/Base/Inventory.h"


CStorageCrate::CStorageCrate() : IObject("StorageCrate")
{
	m_bIsOpen = false;
	m_bAtCrate = false;
	m_bDrawIndicator = false;
	m_nCursor = eBones;

	//set all icon paths for player items/resources
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Bone.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Wood.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Herb.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Gunpowder.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Bomb.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Bridge.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_HealthPotion.dds");

	m_vInputImgPaths.push_back(L"../Game/Assets/Art/2D/Inventory/ZKey_Keyboard.dds");
	m_vInputImgPaths.push_back(L"../Game/Assets/Art/2D/Inventory/XKey_Keyboard.dds");
	m_vInputImgPaths.push_back(L"../Game/Assets/Art/2D/Inventory/NKey_Keyboard.dds");
	m_vInputImgPaths.push_back(L"../Game/Assets/Art/2D/Inventory/MKey_Keyboard.dds");
	m_vInputImgPaths.push_back(L"../Game/Assets/Art/2D/Inventory/UpArrow_Keyboard.dds");
	m_vInputImgPaths.push_back(L"../Game/Assets/Art/2D/Inventory/DownArrow_Keyboard.dds");

	//start with nothing in storage
	m_umCurrentStock[eBones] = 0;
	m_umCurrentStock[eWood] = 0;
	m_umCurrentStock[eHerb] = 0;
	m_umCurrentStock[eGunpowder] = 0;
	m_umCurrentStock[eBomb] = 0;
	m_umCurrentStock[eWoodPlank] = 0;
	m_umCurrentStock[eHealthPotion] = 0;

	m_cpInput = CInputManager::GetInstance();
	m_cpInventory = nullptr;

#pragma region Storage Crate Model/Rendermesh

	//set position
	XMFLOAT3 newLocation = { -1500.0f, 0.0f, -1000.0f };
	SetPosition(newLocation);

	CMesh* theMesh = CAssetManager::GetInstance()->GetPrefabMesh("WorkBench");
	m_cpRenderMesh = new CRenderMesh(theMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Storage Crate.dds");
	GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	//set collider
	newLocation.y += 75.0f;
	AddCollider(new CCollider(false, new CAABB(newLocation, { 100, 150, 100 }), true));

	CAddObjectMessage* addObj = new CAddObjectMessage(this, CObjectManager::Static);
	addObj->Send();

#pragma endregion

#pragma region Background Image Set Up

	vector<TVertex2D> vVerts;
	vector<unsigned int> vIndicies;

	vIndicies.push_back(0);
	vIndicies.push_back(1);
	vIndicies.push_back(2);
	vIndicies.push_back(0);
	vIndicies.push_back(2);
	vIndicies.push_back(3);

	vVerts.push_back // Top Left
		({
			{ -0.8f, 0.7f, -1.0f, 1 },
			{ 0, 0 }
	});

	vVerts.push_back // Top Right
		({
			{ 0.6f, 0.7f, -1.0f, 1 },
			{ 1, 0 }
	});

	vVerts.push_back // Bot Right
		({
			{ 0.6f, -0.7f, -1.0f, 1 },
			{ 1, 1 }
	});

	vVerts.push_back // Bot Left
		({
			{ -0.8f, -0.7f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_cpBackgroundImg = new CHUDMesh(vIndicies, vVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Inventory/Failbot_Storage_Background.dds");
	m_cpBackgroundImg->GetRender() = false;
	m_cpBackgroundImg->GetTransparent() = true;
	GRAPHICS->AddRenderMesh(m_cpBackgroundImg);

#pragma endregion

#pragma region Icons and Boarders Set Up

	float yOffset = 0.135f;

	//icon positions
	float fIconLeft = -0.61f;
	float fIconRight = -0.535f;
	float fIconTop = 0.35f;
	float fIconBottom = 0.275f;

	//boarder positions
	float fBoarderLeft = -0.635f;
	float fBoarderRight = -0.51f;
	float fBoarderTop = 0.375f;
	float fBoarderBottom = 0.25f;

	for (unsigned int i = 0; i < eItemCount; i++)
	{
		vector<unsigned int> Indicies;
		Indicies.push_back(0);
		Indicies.push_back(1);
		Indicies.push_back(2);
		Indicies.push_back(0);
		Indicies.push_back(2);
		Indicies.push_back(3);

		//icon image
		vector<TVertex2D> Verts;
		Verts.push_back // Top Left
			({
				{ fIconLeft, fIconTop, -1.0f, 1 },
				{ 0, 0 }
		});

		Verts.push_back // Top Right
			({
				{ fIconRight, fIconTop, -1.0f, 1 },
				{ 1, 0 }
		});

		Verts.push_back // Bot Right
			({
				{ fIconRight, fIconBottom, -1.0f, 1 },
				{ 1, 1 }
		});

		Verts.push_back // Bot Left
			({
				{ fIconLeft, fIconBottom, -1.0f, 1 },
				{ 0, 1 }

		});

		m_vpIcons.push_back(new CHUDMesh(Indicies, Verts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, m_vIconPaths[i]));
		m_vpIcons[i]->GetRender() = false;
		m_vpIcons[i]->GetTransparent() = true;


		//boarder image for this icon
		vector<TVertex2D> BoarderVerts;
		BoarderVerts.push_back // Top Left
			({
				{ fBoarderLeft, fBoarderTop, -1.0f, 1 },
				{ 0, 0 }
		});

		BoarderVerts.push_back // Top Right
			({
				{ fBoarderRight, fBoarderTop, -1.0f, 1 },
				{ 1, 0 }
		});

		BoarderVerts.push_back // Bot Right
			({
				{ fBoarderRight, fBoarderBottom, -1.0f, 1 },
				{ 1, 1 }
		});

		BoarderVerts.push_back // Bot Left
			({
				{ fBoarderLeft, fBoarderBottom, -1.0f, 1 },
				{ 0, 1 }
		});

		m_vpBoarders.push_back(new CHUDMesh(Indicies, BoarderVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Normal.dds"));
		m_vpBoarders[i]->GetRender() = false;
		m_vpBoarders[i]->GetTransparent() = true;

		GRAPHICS->AddRenderMesh(m_vpBoarders[i]);
		GRAPHICS->AddRenderMesh(m_vpIcons[i]);

		fIconTop -= yOffset;
		fIconBottom -= yOffset;
		fBoarderTop -= yOffset;
		fBoarderBottom -= yOffset;
	}

	m_vpBoarders[m_nCursor]->SetTexture(L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Selected.dds");

#pragma endregion

#pragma region TextFrame

	vector<TVertex2D> TextVerts;
	vector<unsigned int> TextInd;

	TextInd.push_back(0);
	TextInd.push_back(1);
	TextInd.push_back(2);

	TextInd.push_back(0);
	TextInd.push_back(2);
	TextInd.push_back(3);

	float fStartX = -0.30f;
	float fStartY = 0.55f;
	float fWidth = 0.6f;
	float fHeight = 0.2f;

	TextVerts.push_back // Top Left
		({
			{ fStartX, fStartY, -1.0f, 1 },
			{ 0, 0 }
	});

	TextVerts.push_back // Top Right
		({
			{ fStartX + fWidth, fStartY, -1.0f, 1 },
			{ 1, 0 }
	});

	TextVerts.push_back // Bot Right
		({
			{ fStartX + fWidth, fStartY - fHeight, -1.0f, 1 },
			{ 1, 1 }
	});

	TextVerts.push_back // Bot Left
		({
			{ fStartX, fStartY - fHeight, -1.0f, 1 },
			{ 0, 1 }
	});

	m_cpTextFrame = new CHUDMesh(TextInd, TextVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/Failbot_TextBackground.dds");
	m_cpTextFrame->GetRender() = false;
	m_cpTextFrame->GetTransparent() = true;
	GRAPHICS->AddRenderMesh(m_cpTextFrame);

#pragma endregion

#pragma region Input Images Set Up

	vVerts.clear();
	vVerts.push_back // Top Left
		({
			{ 0.0f, 0.0f, -1.0f, 1 },
			{ 0, 0 }
	});

	vVerts.push_back // Top Right
		({
			{ 0.2f, 0.0f, -1.0f, 1 },
			{ 1, 0 }
	});

	vVerts.push_back // Bot Right
		({
			{ 0.2f, -0.2f, -1.0f, 1 },
			{ 1, 1 }
	});

	vVerts.push_back // Bot Left
		({
			{ 0.0f, -0.2f, -1.0f, 1 },
			{ 0, 1 }
	});

	for (unsigned int i = 0; i < INPUT_IMAGES; i++)
	{
		m_cpInputImgs[i] = new CHUDMesh(vIndicies, vVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, m_vInputImgPaths[i]);
		m_cpInputImgs[i]->GetRender() = false;
		m_cpInputImgs[i]->GetTransparent() = true;
		GRAPHICS->AddRenderMesh(m_cpInputImgs[i]);
	}

	TranslateLocalX(m_cpInputImgs[0]->GetTransformMatrix(), -0.175f);//store one
	TranslateLocalY(m_cpInputImgs[0]->GetTransformMatrix(), 0.05f);
	TranslateLocalX(m_cpInputImgs[1]->GetTransformMatrix(), -0.175f);//store all
	TranslateLocalY(m_cpInputImgs[1]->GetTransformMatrix(), -0.1f);
								  
	TranslateLocalX(m_cpInputImgs[2]->GetTransformMatrix(), -0.175f);//take one
	TranslateLocalY(m_cpInputImgs[2]->GetTransformMatrix(), -0.25f);
	TranslateLocalX(m_cpInputImgs[3]->GetTransformMatrix(), -0.175f);//take all
	TranslateLocalY(m_cpInputImgs[3]->GetTransformMatrix(), -0.4f);

	TranslateLocalX(m_cpInputImgs[4]->GetTransformMatrix(), -0.8f);//arrow up
	TranslateLocalY(m_cpInputImgs[4]->GetTransformMatrix(), 0.1f);
	TranslateLocalX(m_cpInputImgs[5]->GetTransformMatrix(), -0.8f);//arrow down
	TranslateLocalY(m_cpInputImgs[5]->GetTransformMatrix(), -0.1f);

#pragma endregion

}

CStorageCrate::~CStorageCrate()
{
	SAFE_DELETE(m_cpBackgroundImg);
	SAFE_DELETE(m_cpTextFrame);

	for (unsigned int i = 0; i < m_vpIcons.size(); i++)
	{
		SAFE_DELETE(m_vpIcons[i]);
	}

	for (unsigned int i = 0; i < m_vpBoarders.size(); i++)
	{
		SAFE_DELETE(m_vpBoarders[i]);
	}

	for (unsigned int i = 0; i < INPUT_IMAGES; i++)
	{
		SAFE_DELETE(m_cpInputImgs[i]);
	}

}

void CStorageCrate::Update()
{
	if (m_bIsOpen == true)
	{
#pragma region Input (main menu registers)

		if (m_cpInput->IsKeyPressed("Arrow Down") || m_cpInput->IsButtonPressed("Cursor Down"))
		{
			m_vpBoarders[m_nCursor]->SetTexture(L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Normal.dds");
			m_nCursor++;
			if (m_nCursor > eHealthPotion)
			{
				m_nCursor = eBones;
			}
			m_vpBoarders[m_nCursor]->SetTexture(L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Selected.dds");
		}
		else if (m_cpInput->IsKeyPressed("Arrow Up") || m_cpInput->IsButtonPressed("Cursor Up"))
		{
			m_vpBoarders[m_nCursor]->SetTexture(L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Normal.dds");
			m_nCursor--;
			if (m_nCursor < eBones)
			{
				m_nCursor = eHealthPotion;
			}
			m_vpBoarders[m_nCursor]->SetTexture(L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Selected.dds");
		}

		if (m_cpInput->IsKeyPressed("Return") || m_cpInput->IsButtonPressed("Back"))
		{
			Close();
		}

#pragma endregion

#pragma region Cursor Check and Text Display

		GRAPHICS->RenderText(L"Storage Crate", { -100.0f, 650.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.85f, 0.8f }, (eTextMod)1);
		GRAPHICS->RenderText(L"Inventory Items", { -590.0f, 450.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.35f, 0.3f }, (eTextMod)1);

		switch (m_nCursor)
		{
		case EItems::eBones:
		{
			GRAPHICS->RenderText(L"Bones", { 100.0f, 400.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.75f, 0.7f }, (eTextMod)1);

			string szStoredAmount = "Stored: " + to_string(m_umCurrentStock[eBones]);
			wstring wszStored(szStoredAmount.begin(), szStoredAmount.end());
			GRAPHICS->RenderText(wszStored.c_str(), { 100.0f, 300.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.55f, 0.5f }, (eTextMod)1);

			CheckActiveCursor(eBones);
		}
		break;
		case EItems::eWood:
		{
			GRAPHICS->RenderText(L"Wood", { 100.0f, 400.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.75f, 0.7f }, (eTextMod)1);

			string szStoredAmount = "Stored: " + to_string(m_umCurrentStock[eWood]);
			wstring wszStored(szStoredAmount.begin(), szStoredAmount.end());
			GRAPHICS->RenderText(wszStored.c_str(), { 100.0f, 300.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.55f, 0.5f }, (eTextMod)1);

			CheckActiveCursor(eWood);
		}
		break;
		case EItems::eHerb:
		{
			GRAPHICS->RenderText(L"Herb", { 100.0f, 400.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.75f, 0.7f }, (eTextMod)1);

			string szStoredAmount = "Stored: " + to_string(m_umCurrentStock[eHerb]);
			wstring wszStored(szStoredAmount.begin(), szStoredAmount.end());
			GRAPHICS->RenderText(wszStored.c_str(), { 100.0f, 300.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.55f, 0.5f }, (eTextMod)1);

			CheckActiveCursor(eHerb);
		}
		break;
		case EItems::eGunpowder:
		{
			GRAPHICS->RenderText(L"Gunpowder", { 100.0f, 400.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.75f, 0.7f }, (eTextMod)1);

			string szStoredAmount = "Stored: " + to_string(m_umCurrentStock[eGunpowder]);
			wstring wszStored(szStoredAmount.begin(), szStoredAmount.end());
			GRAPHICS->RenderText(wszStored.c_str(), { 100.0f, 300.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.55f, 0.5f }, (eTextMod)1);

			CheckActiveCursor(eGunpowder);
		}
		break;
		case EItems::eBomb:
		{
			GRAPHICS->RenderText(L"Bomb", { 100.0f, 400.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.75f, 0.7f }, (eTextMod)1);

			string szStoredAmount = "Stored: " + to_string(m_umCurrentStock[eBomb]);
			wstring wszStored(szStoredAmount.begin(), szStoredAmount.end());
			GRAPHICS->RenderText(wszStored.c_str(), { 100.0f, 300.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.55f, 0.5f }, (eTextMod)1);

			CheckActiveCursor(eBomb);
		}
		break;
		case EItems::eWoodPlank:
		{
			GRAPHICS->RenderText(L"Wood Plank", { 100.0f, 400.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.75f, 0.7f }, (eTextMod)1);

			string szStoredAmount = "Stored: " + to_string(m_umCurrentStock[eWoodPlank]);
			wstring wszStored(szStoredAmount.begin(), szStoredAmount.end());
			GRAPHICS->RenderText(wszStored.c_str(), { 100.0f, 300.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.55f, 0.5f }, (eTextMod)1);

			CheckActiveCursor(eWoodPlank);
		}
		break;
		case EItems::eHealthPotion:
		{
			GRAPHICS->RenderText(L"Health Potion", { 100.0f, 400.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.75f, 0.7f }, (eTextMod)1);

			string szStoredAmount = "Stored: " + to_string(m_umCurrentStock[eHealthPotion]);
			wstring wszStored(szStoredAmount.begin(), szStoredAmount.end());
			GRAPHICS->RenderText(wszStored.c_str(), { 100.0f, 300.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.55f, 0.5f }, (eTextMod)1);

			CheckActiveCursor(eHealthPotion);
		}
		break;
		default:
			break;
		}

#pragma endregion

#pragma region Inventory Item Count Text Display

		float fOffsetY = 350.0f;
		for (unsigned int i = 0; i < eItemCount; i++)
		{
			string szItemAmount = "x" + to_string(m_cpInventory->GetStock((CInventory::eItems)i));
			wstring wszItems(szItemAmount.begin(), szItemAmount.end());
			GRAPHICS->RenderText(wszItems.c_str(), { -500.0f, fOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.45f, 0.4f });

			fOffsetY -= 135.0f;
		}

#pragma endregion

	}

	if (m_bAtCrate == false && m_bIsOpen == true)
	{
		Close();
	}
	m_bAtCrate = false;

	if (m_bAtCrate == false)
	{
		SetUnavailable();
	}

}

void CStorageCrate::Open(CInventory* inventory)
{
	m_bIsOpen = true;

	m_cpInventory = inventory;

	m_cpBackgroundImg->GetRender() = true;
	m_cpTextFrame->GetRender() = false;

	for (unsigned int i = 0; i < eItemCount; i++)
	{
		m_vpIcons[i]->GetRender() = true;
		m_vpBoarders[i]->GetRender() = true;
	}

	for (unsigned int i = 0; i < INPUT_IMAGES; i++)
	{
		m_cpInputImgs[i]->GetRender() = true;
	}

	if (m_cpInput->GetControllerBeingUsed() == true)
	{
		m_cpInputImgs[0]->SetTexture(L"../Game/Assets/Art/2D/Inventory/RightDPad_Controller.dds");
		m_cpInputImgs[1]->SetTexture(L"../Game/Assets/Art/2D/Inventory/AButton_Controller.dds");
		m_cpInputImgs[2]->SetTexture(L"../Game/Assets/Art/2D/Inventory/LeftDPad_Controller.dds");
		m_cpInputImgs[3]->SetTexture(L"../Game/Assets/Art/2D/Inventory/XButton_Controller.dds");
		m_cpInputImgs[4]->SetTexture(L"../Game/Assets/Art/2D/Inventory/UpDPad_Controller.dds");
		m_cpInputImgs[5]->SetTexture(L"../Game/Assets/Art/2D/Inventory/DownDPad_Controller.dds");
	}
	else
	{
		m_cpInputImgs[0]->SetTexture(L"../Game/Assets/Art/2D/Inventory/ZKey_Keyboard.dds");
		m_cpInputImgs[1]->SetTexture(L"../Game/Assets/Art/2D/Inventory/XKey_Keyboard.dds");
		m_cpInputImgs[2]->SetTexture(L"../Game/Assets/Art/2D/Inventory/NKey_Keyboard.dds");
		m_cpInputImgs[3]->SetTexture(L"../Game/Assets/Art/2D/Inventory/MKey_Keyboard.dds");
		m_cpInputImgs[4]->SetTexture(L"../Game/Assets/Art/2D/Inventory/UpArrow_Keyboard.dds");
		m_cpInputImgs[5]->SetTexture(L"../Game/Assets/Art/2D/Inventory/DownArrow_Keyboard.dds");
	}
}

void CStorageCrate::Close()
{
	m_bIsOpen = false;

	m_cpBackgroundImg->GetRender() = false;

	for (unsigned int i = 0; i < eItemCount; i++)
	{
		m_vpIcons[i]->GetRender() = false;
		m_vpBoarders[i]->GetRender() = false;
	}

	for (unsigned int i = 0; i < INPUT_IMAGES; i++)
	{
		m_cpInputImgs[i]->GetRender() = false;
	}
}

void CStorageCrate::Deposit(EItems _type, unsigned int _amount)
{
	m_umCurrentStock[_type] += _amount;
}

void CStorageCrate::Withdrawl(EItems _type, unsigned int _amount)
{
	m_umCurrentStock[_type] -= _amount;
}

void CStorageCrate::CheckActiveCursor(EItems type)
{
	string szWithdrawlALL = "Take All (" + to_string(m_umCurrentStock[type]) + ")";
	wstring wszWithdrawl(szWithdrawlALL.begin(), szWithdrawlALL.end());
	string szDepositlALL = "Store All (" + to_string(m_cpInventory->GetStock((CInventory::eItems)type)) + ")";
	wstring wszDeposit(szDepositlALL.begin(), szDepositlALL.end());

	GRAPHICS->RenderText(L"Store 1", { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.55f, 0.5f });
	GRAPHICS->RenderText(wszDeposit.c_str(), { 0.0f, -150.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.55f, 0.5f });
	GRAPHICS->RenderText(L"Take 1", { 0.0f, -300.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.55f, 0.5f });
	GRAPHICS->RenderText(wszWithdrawl.c_str(), { 0.0f, -450.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.55f, 0.5f });

	if (m_cpInput->IsKeyPressed("No") || m_cpInput->IsButtonPressed("Inventory"))//N : right d-pad
	{
		//take 1
		if (m_umCurrentStock[type] < 1)
		{
			return;
		}

		Withdrawl(type, 1);
		m_cpInventory->AddToInventory((CInventory::eItems)type, 1);
	}
	else if (m_cpInput->IsKeyPressed("Take All") || m_cpInput->IsButtonPressed("Jump Action"))//M : A
	{
		//take all
		if (m_umCurrentStock[type] < 1)
		{
			return;
		}

		int amount = m_umCurrentStock[type];
		Withdrawl(type, amount);
		m_cpInventory->AddToInventory((CInventory::eItems)type, amount);
	}
	else if (m_cpInput->IsKeyPressed("Store One") || m_cpInput->IsButtonPressed("Map"))//Z : left d-pad
	{
		//store 1
		if (m_cpInventory->GetStock((CInventory::eItems)type) < 1)
		{
			return;
		}

		Deposit(type, 1);
		m_cpInventory->RemoveFromInventory((CInventory::eItems)type, 1);
	}
	else if (m_cpInput->IsKeyPressed("Store All") || m_cpInput->IsButtonPressed("Interact"))//X : X
	{
		//store all
		if (m_cpInventory->GetStock((CInventory::eItems)type) < 1)
		{
			return;
		}

		int amount = m_cpInventory->GetStock((CInventory::eItems)type);
		Deposit(type, amount);
		m_cpInventory->RemoveFromInventory((CInventory::eItems)type, amount);
	}

}

void CStorageCrate::DrawIndicator()
{
	if (m_bIsOpen == true)
	{
		return;
	}

	if (m_cpInput->GetControllerBeingUsed() == true)
	{
		GRAPHICS->RenderText(L"X to Open", { -150, 500 }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ 0.5f, 0.5f });
	}
	else
	{
		GRAPHICS->RenderText(L"'F' to Open", { -150, 500 }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ 0.5f, 0.5f });
	}

	m_cpTextFrame->GetRender() = true;
	m_bDrawIndicator = true;
}

void CStorageCrate::SetUnavailable()
{
	m_cpTextFrame->GetRender() = false;
	m_bDrawIndicator = false;
}

bool CStorageCrate::IsOpen()
{
	return m_bIsOpen;
}

void CStorageCrate::SetAtTable()
{
	m_bAtCrate = true;
}

void CStorageCrate::AddToRenderer()
{
	if (m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	if (m_cpBackgroundImg != nullptr)
		GRAPHICS->AddRenderMesh(m_cpBackgroundImg);

	if (m_cpTextFrame != nullptr)
		GRAPHICS->AddRenderMesh(m_cpTextFrame);

	for (unsigned int i = 0; i < m_vpBoarders.size(); i++)
	{
		if (m_vpBoarders[i] != nullptr)
			GRAPHICS->AddRenderMesh(m_vpBoarders[i]);
	}

	for (unsigned int i = 0; i < m_vpIcons.size(); i++)
	{
		if (m_vpIcons[i] != nullptr)
			GRAPHICS->AddRenderMesh(m_vpIcons[i]);
	}

	for (unsigned int i = 0; i < INPUT_IMAGES; i++)
	{
		if (m_cpInputImgs[i] != nullptr)
			GRAPHICS->AddRenderMesh(m_cpInputImgs[i]);
	}

}
