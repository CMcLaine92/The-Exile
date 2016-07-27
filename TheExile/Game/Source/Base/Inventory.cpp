/***********************************************
* Filename:  		Inventory.cpp
* Date:      		09/13/2015
* Mod. Date:		09/14/2015
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			Displayes and handles the stock of the players inventory.
************************************************/

#include "Inventory.h"
#include "../../../Engine/Renderer/HUDMesh.h"
#include "../../../Engine/Renderer/Renderer.h"
#include "../../../Engine/Core/Input/InputManager.h"
#include <iostream>

#define NUM_ITEMS 7
#define FLASH_TIME 2.0f
#define CRAFT_TIME 2.0f
#define NUM_RESOURCES 4
#define NUM_CRAFTED 3

CInventory::CInventory()
{
	CInputManager::GetInstance()->RegisterKey("Down", eKey::DownArrow);
	CInputManager::GetInstance()->RegisterKey("Up", eKey::UpArrow);
	CInputManager::GetInstance()->RegisterKey("Craft", eKey::Enter);
	CInputManager::GetInstance()->RegisterKey("CraftAll", eKey::Shift);
	CInputManager::GetInstance()->RegisterKey("Escape", eKey::escape);

	m_nCursor = Bones;
	m_fPickUpTimer = FLASH_TIME;
	m_bPickUp = false;

	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Bone.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Wood.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Herb.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Gunpowder.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Bomb.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Bridge.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_HealthPotion.dds");

#pragma region Background Image

	vector<TVertex2D> ScreenVerts;
	vector<unsigned int> ScreenIndicies;

	ScreenIndicies.push_back(0);
	ScreenIndicies.push_back(1);
	ScreenIndicies.push_back(2);

	ScreenIndicies.push_back(0);
	ScreenIndicies.push_back(2);
	ScreenIndicies.push_back(3);

	ScreenVerts.push_back // Top Left
		({
			{ -0.8f, 0.7f, -1.0f, 1 },
			{ 0, 0 }
	});

	ScreenVerts.push_back // Top Right
		({
			{ 0.6f, 0.7f, -1.0f, 1 },
			{ 1, 0 }
	});

	ScreenVerts.push_back // Bot Right
		({
			{ 0.6f, -0.7f, -1.0f, 1 },
			{ 1, 1 }
	});

	ScreenVerts.push_back // Bot Left
		({
			{ -0.8f, -0.7f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_cpInventoryScreen = new CHUDMesh(ScreenIndicies, ScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Background.dds");
	m_cpInventoryScreen->GetRender() = false;
	m_cpInventoryScreen->GetTransparent() = true;
	GRAPHICS->AddRenderMesh(m_cpInventoryScreen);

#pragma endregion

#pragma region Icons and Boarders Set Up

	float yOffset = 0.1275f;

	//icon positions
	float fIconLeft = -0.61f;
	float fIconRight = -0.535f;
	float fIconTop = 0.375f;
	float fIconBottom = 0.3f;

	//boarder positions
	float fBoarderLeft = -0.635f;
	float fBoarderRight = -0.51f;
	float fBoarderTop = 0.4f;
	float fBoarderBottom = 0.275f;

	for (unsigned int i = 0; i < ItemCount; i++)
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

		m_vcIcons.push_back(new CHUDMesh(Indicies, Verts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, m_vIconPaths[i]));
		m_vcIcons[i]->GetRender() = false;
		m_vcIcons[i]->GetTransparent() = true;


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

		m_vcBoarder.push_back(new CHUDMesh(Indicies, BoarderVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Normal.dds"));
		m_vcBoarder[i]->GetRender() = false;
		m_vcBoarder[i]->GetTransparent() = true;

		GRAPHICS->AddRenderMesh(m_vcBoarder[i]);
		GRAPHICS->AddRenderMesh(m_vcIcons[i]);

		fIconTop -= yOffset;
		fIconBottom -= yOffset;
		fBoarderTop -= yOffset;
		fBoarderBottom -= yOffset;

		if (i == Gunpowder)
		{
			fIconTop -= 0.1f;
			fIconBottom -= 0.1f;
			fBoarderTop -= 0.1f;
			fBoarderBottom -= 0.1f;
		}
		if (i == Bomb)
		{
			fIconTop -= 0.0075f;
			fIconBottom -= 0.0075f;
			fBoarderTop -= 0.0075f;
			fBoarderBottom -= 0.0075f;
		}
	}

	m_vcBoarder[m_nCursor]->SetTexture(L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Selected.dds");

#pragma endregion

#pragma region CurItem

	vector<unsigned int> vIndicies;
	vIndicies.push_back(0);
	vIndicies.push_back(1);
	vIndicies.push_back(2);
	vIndicies.push_back(0);
	vIndicies.push_back(2);
	vIndicies.push_back(3);

	vector<TVertex2D> vVerts;
	vVerts.push_back // Top Left
		({
			{ 0.0f, 0.0f, -1.0f, 1 },
			{ 0, 0 }
	});

	vVerts.push_back // Top Right
		({
			{ 0.175f, 0.0f, -1.0f, 1 },
			{ 1, 0 }
	});

	vVerts.push_back // Bot Right
		({
			{ 0.175f, -0.175f, -1.0f, 1 },
			{ 1, 1 }
	});

	vVerts.push_back // Bot Left
		({
			{ 0.0f, -0.175f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_cpSelectedIcon = new CHUDMesh(vIndicies, vVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Normal.dds");
	m_cpSelectedIcon->GetRender() = false;
	m_cpSelectedIcon->GetTransparent() = true;
	TranslateLocalX(m_cpSelectedIcon->GetTransformMatrix(), -0.3f);
	TranslateLocalY(m_cpSelectedIcon->GetTransformMatrix(), 0.325f);
	GRAPHICS->AddRenderMesh(m_cpSelectedIcon);


	vVerts.clear();
	vVerts.push_back // Top Left
		({
			{ 0.0f, 0.0f, -1.0f, 1 },
			{ 0, 0 }
	});

	vVerts.push_back // Top Right
		({
			{ 0.125f, 0.0f, -1.0f, 1 },
			{ 1, 0 }
	});

	vVerts.push_back // Bot Right
		({
			{ 0.125f, -0.125f, -1.0f, 1 },
			{ 1, 1 }
	});

	vVerts.push_back // Bot Left
		({
			{ 0.0f, -0.125f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_cpCurrentItem = new CHUDMesh(vIndicies, vVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Bone.dds");
	m_cpCurrentItem->GetRender() = false;
	m_cpCurrentItem->GetTransparent() = true;
	TranslateLocalX(m_cpCurrentItem->GetTransformMatrix(), -0.275f);
	TranslateLocalY(m_cpCurrentItem->GetTransformMatrix(), 0.3f);
	GRAPHICS->AddRenderMesh(m_cpCurrentItem);

#pragma endregion

#pragma region Input Images Set Up

	m_vInputImgPaths.push_back(L"../Game/Assets/Art/2D/Inventory/UpArrow_Keyboard.dds");
	m_vInputImgPaths.push_back(L"../Game/Assets/Art/2D/Inventory/DownArrow_Keyboard.dds");
	m_vInputImgPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Enter_Keyboard.dds");
	m_vInputImgPaths.push_back(L"../Game/Assets/Art/2D/Inventory/SpaceBar_Keyboard.dds");

	vIndicies.clear();
	vIndicies.push_back(0);
	vIndicies.push_back(1);
	vIndicies.push_back(2);
	vIndicies.push_back(0);
	vIndicies.push_back(2);
	vIndicies.push_back(3);

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

	for (unsigned int i = 0; i < NUM_INPUT_IMAGES; i++)
	{
		m_cpInputImgs[i] = new CHUDMesh(vIndicies, vVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, m_vInputImgPaths[i]);
		m_cpInputImgs[i]->GetRender() = false;
		m_cpInputImgs[i]->GetTransparent() = true;
		GRAPHICS->AddRenderMesh(m_cpInputImgs[i]);
	}

	TranslateLocalX(m_cpInputImgs[0]->GetTransformMatrix(), -0.8f);//arrow up
	TranslateLocalY(m_cpInputImgs[0]->GetTransformMatrix(), 0.1f);
	TranslateLocalX(m_cpInputImgs[1]->GetTransformMatrix(), -0.8f);//arrow down
	TranslateLocalY(m_cpInputImgs[1]->GetTransformMatrix(), -0.1f);

	m_cpInputImgs[2]->GetVertex2DArr()[0].m_fPosition[1] = 0.1f;//enter key
	m_cpInputImgs[2]->GetVertex2DArr()[1].m_fPosition[1] = 0.1f;
	TranslateLocalX(m_cpInputImgs[2]->GetTransformMatrix(), -0.35f);
	TranslateLocalY(m_cpInputImgs[2]->GetTransformMatrix(), -0.5f);

	m_cpInputImgs[3]->GetVertex2DArr()[1].m_fPosition[0] = 0.2f;//space bar
	m_cpInputImgs[3]->GetVertex2DArr()[2].m_fPosition[0] = 0.2f;
	m_cpInputImgs[3]->GetVertex2DArr()[0].m_fPosition[1] = 0.1f;
	m_cpInputImgs[3]->GetVertex2DArr()[1].m_fPosition[1] = 0.1f;
	TranslateLocalX(m_cpInputImgs[3]->GetTransformMatrix(), 0.05f);
	TranslateLocalY(m_cpInputImgs[3]->GetTransformMatrix(), -0.5f);

#pragma endregion

#pragma region Crafting Bar Set Up

	vector<TVertex2D> barVerts;
	vector<unsigned int> barIndicies;

	barIndicies.push_back(0);
	barIndicies.push_back(1);
	barIndicies.push_back(2);
	barIndicies.push_back(0);
	barIndicies.push_back(2);
	barIndicies.push_back(3);

	float fxBar = -0.4f;
	float fyBar = -0.3f;

	float fwBar = 0.55f;
	float fhBar = 0.1f;

	m_fCraftBarSize = fwBar;
	m_fCraftBarStartX = fxBar;
	m_fCraftTime = 0.0f;
	m_bCrafting = false;
	m_bCraftAll = false;
	m_bAbleToCraft = false;

	barVerts.push_back // Top Left
		({
			{ fxBar, fyBar, -1.0f, 1 },
			{ 0, 0 }
	});

	barVerts.push_back // Top Right
		({
			{ fxBar + fwBar, fyBar, -1.0f, 1 },
			{ 1, 0 }
	});

	barVerts.push_back // Bot Right
		({
			{ fxBar + fwBar, fyBar - fhBar, -1.0f, 1 },
			{ 1, 1 }
	});

	barVerts.push_back // Bot Left
		({
			{ fxBar, fyBar - fhBar, -1.0f, 1 },
			{ 0, 1 }
	});

	m_cpCraftingBar = new CHUDMesh(barIndicies, barVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Inventory/Failbot_Craft_Bar.dds");
	m_cpCraftingBar->GetRender() = false;
	m_cpCraftingBar->GetTransparent() = true;
	TranslateLocalX(m_cpCraftingBar->GetTransformMatrix(), 0.35f);
	TranslateLocalY(m_cpCraftingBar->GetTransformMatrix(), 0.0875f);
	GRAPHICS->AddRenderMesh(m_cpCraftingBar);

#pragma endregion

	m_CurrentStock[Bones] = 0;
	m_CurrentStock[Wood] = 0;
	m_CurrentStock[Herb] = 0;
	m_CurrentStock[Gunpowder] = 0;
	m_CurrentStock[Bomb] = 0;
	m_CurrentStock[Bridge] = 0;
	m_CurrentStock[HealthPotion] = 0;

	m_vcBoarder[m_nCursor]->SetTexture(L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Selected.dds");
}

CInventory::~CInventory()
{
	SAFE_DELETE(m_cpCraftingBar);
	SAFE_DELETE(m_cpCurrentItem);
	SAFE_DELETE(m_cpInventoryScreen);
	SAFE_DELETE(m_cpSelectedIcon);

	for (unsigned int i = 0; i < m_vcBoarder.size(); i++)
		SAFE_DELETE(m_vcBoarder[i]);

	for (unsigned int i = 0; i < m_vcIcons.size(); i++)
		SAFE_DELETE(m_vcIcons[i]);

	for (unsigned int i = 0; i < NUM_INPUT_IMAGES; i++)
		SAFE_DELETE(m_cpInputImgs[i]);
}

void CInventory::Update()
{
	if (m_bOpen)
	{
		GRAPHICS->RenderText(L"Inventory", { -300.0f, 650.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.85f, 0.8f });
		GRAPHICS->RenderText(L"Resources", { -700.0f, 475.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.45f, 0.4f });
		GRAPHICS->RenderText(L"Items", { -640.0f, -135.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.45f, 0.4f });

		string enumSzItem[NUM_ITEMS] = { "Bones", "Wood Log", "Herb", "Gunpowder", "Bomb", "Wood Plank", "Health Potion" };
		string numBones = enumSzItem[m_nCursor];
		wstring wBones(numBones.begin(), numBones.end());
		GRAPHICS->RenderText(wBones.c_str(), { -100.0f, 300.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.7f, 0.65f });

		float fStockY = 365.0f;
		for (size_t i = 0; i < NUM_ITEMS; i++)
		{
			string strAmmount = "x" + to_string(m_CurrentStock[i]);
			wstring printStr(strAmmount.begin(), strAmmount.end());
			GRAPHICS->RenderText(printStr.c_str(), { -500.0f, fStockY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.4f, 0.3f });

			fStockY -= 127.5f;
			if (i == 3)
			{
				fStockY -= 115.0f;
			}
		}

#pragma region Check Cursor for Craftability

		//bomb = 2 bone and 2 gunpowder
		//wood plank = 2 wood
		//health potion = 1 bone and 2 herb
		if (CInputManager::GetInstance()->GetControllerBeingUsed() == true)
		{
			m_cpInputImgs[0]->SetTexture(L"../Game/Assets/Art/2D/Inventory/UpDPad_Controller.dds");
			m_cpInputImgs[1]->SetTexture(L"../Game/Assets/Art/2D/Inventory/DownDPad_Controller.dds");
			m_cpInputImgs[2]->SetTexture(L"../Game/Assets/Art/2D/Inventory/AButton_Controller.dds");
			m_cpInputImgs[3]->SetTexture(L"../Game/Assets/Art/2D/Inventory/XButton_Controller.dds");

			//scale down the button images for crafting
			m_cpInputImgs[2]->GetVertex2DArr()[1].m_fPosition[0] = 0.15f;//top-right X
			m_cpInputImgs[2]->GetVertex2DArr()[2].m_fPosition[0] = 0.15f;//bottom-right X
			m_cpInputImgs[2]->GetVertex2DArr()[0].m_fPosition[1] = 0.0f;//top left Y
			m_cpInputImgs[2]->GetVertex2DArr()[1].m_fPosition[1] = 0.0f;//top-right Y

			m_cpInputImgs[3]->GetVertex2DArr()[1].m_fPosition[0] = 0.15f;//top-right X
			m_cpInputImgs[3]->GetVertex2DArr()[2].m_fPosition[0] = 0.15f;//bottom-right X
			m_cpInputImgs[3]->GetVertex2DArr()[0].m_fPosition[1] = 0.0f;//top left Y
			m_cpInputImgs[3]->GetVertex2DArr()[1].m_fPosition[1] = 0.0f;//top-right Y

			m_cpInputImgs[2]->SetPosition(XMFLOAT3{ -0.3f, -0.45f, m_cpInputImgs[2]->GetPosition().z});
			m_cpInputImgs[3]->SetPosition(XMFLOAT3{ 0.125f, -0.45f, m_cpInputImgs[3]->GetPosition().z });
		}
		else
		{
			m_cpInputImgs[0]->SetTexture(L"../Game/Assets/Art/2D/Inventory/UpArrow_Keyboard.dds");
			m_cpInputImgs[1]->SetTexture(L"../Game/Assets/Art/2D/Inventory/DownArrow_Keyboard.dds");
			m_cpInputImgs[2]->SetTexture(L"../Game/Assets/Art/2D/Inventory/Enter_Keyboard.dds");
			m_cpInputImgs[3]->SetTexture(L"../Game/Assets/Art/2D/Inventory/SpaceBar_Keyboard.dds");

			//scale up the keyboard images for crafting
			m_cpInputImgs[2]->GetVertex2DArr()[1].m_fPosition[0] = 0.2f;//top-right X
			m_cpInputImgs[2]->GetVertex2DArr()[2].m_fPosition[0] = 0.2f;//bottom-right X
			m_cpInputImgs[2]->GetVertex2DArr()[0].m_fPosition[1] = 0.1f;//top left Y
			m_cpInputImgs[2]->GetVertex2DArr()[1].m_fPosition[1] = 0.1f;//top-right Y

			m_cpInputImgs[3]->GetVertex2DArr()[1].m_fPosition[0] = 0.2f;//top-right X
			m_cpInputImgs[3]->GetVertex2DArr()[2].m_fPosition[0] = 0.2f;//bottom-right X
			m_cpInputImgs[3]->GetVertex2DArr()[0].m_fPosition[1] = 0.1f;//top left Y
			m_cpInputImgs[3]->GetVertex2DArr()[1].m_fPosition[1] = 0.1f;//top-right Y

			m_cpInputImgs[2]->SetPosition(XMFLOAT3{ -0.35f, -0.5f, m_cpInputImgs[2]->GetPosition().z });
			m_cpInputImgs[3]->SetPosition(XMFLOAT3{ 0.05f, -0.5f, m_cpInputImgs[3]->GetPosition().z });
		}

		switch ((eItems)m_nCursor)
		{
		case CInventory::Bones:
		case CInventory::Wood:
		case CInventory::Herb:
		case CInventory::Gunpowder:
		{
			m_bAbleToCraft = false;

			for (unsigned int i = 0; i < 2; i++)
				m_cpInputImgs[i]->GetRender() = true;

			m_cpInputImgs[2]->GetRender() = false;
			m_cpInputImgs[3]->GetRender() = false;
		}
		break;
		case CInventory::Bomb:
		{
			if (GetStock(Gunpowder) >= 2 && GetStock(Bones) >= 2)
			{
				m_bAbleToCraft = true;
				GRAPHICS->RenderText(L"Craft:", { -200.0f, -200.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.75f, 0.7f }, (eTextMod)1);
			}
			else
			{
				m_bAbleToCraft = false;
				GRAPHICS->RenderText(L"Craft:", { -200.0f, -200.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.75f, 0.7f }, (eTextMod)1);
				GRAPHICS->RenderText(L"Not Enough Resources", { 250.0f, -225.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.45f, 0.4f }, (eTextMod)1);
			}

			for (unsigned int i = 0; i < 4; i++)
				m_cpInputImgs[i]->GetRender() = true;

			GRAPHICS->RenderText(L"Craft One", { -175.0f, -525.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.35f, 0.3f });

			unsigned int unCraftAllAmount = 0;
			if (GetStock(Bones) / 2 < GetStock(Gunpowder) / 2)
				unCraftAllAmount = GetStock(Bones) / 2;
			else
				unCraftAllAmount = GetStock(Gunpowder) / 2;

			string szCraftAll = "Craft All (" + to_string(unCraftAllAmount) + ")";
			wstring wszCraftAll(szCraftAll.begin(), szCraftAll.end());
			GRAPHICS->RenderText(wszCraftAll.c_str(), { 375.0f, -525.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.35f, 0.3f }, (eTextMod)1);
		}
		break;
		case CInventory::Bridge:
		{
			if (GetStock(Wood) >= 1)
			{
				m_bAbleToCraft = true;
				GRAPHICS->RenderText(L"Craft:", { -200.0f, -200.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.75f, 0.7f }, (eTextMod)1);
			}
			else
			{
				m_bAbleToCraft = false;
				GRAPHICS->RenderText(L"Craft:", { -200.0f, -200.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.75f, 0.7f }, (eTextMod)1);
				GRAPHICS->RenderText(L"Not Enough Resources", { 250.0f, -225.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.45f, 0.4f }, (eTextMod)1);
			}

			for (unsigned int i = 0; i < 4; i++)
				m_cpInputImgs[i]->GetRender() = true;

			GRAPHICS->RenderText(L"Craft One", { -175.0f, -525.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.35f, 0.3f });

			unsigned int unCraftAllAmount = GetStock(Wood) * 2;
			string szCraftAll = "Craft All (" + to_string(unCraftAllAmount) + ")";
			wstring wszCraftAll(szCraftAll.begin(), szCraftAll.end());
			GRAPHICS->RenderText(wszCraftAll.c_str(), { 375.0f, -525.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.35f, 0.3f }, (eTextMod)1);

		}
		break;
		case CInventory::HealthPotion:
		{
			if (GetStock(Herb) >= 2 && GetStock(Bones) >= 1)
			{
				m_bAbleToCraft = true;
				GRAPHICS->RenderText(L"Craft:", { -200.0f, -200.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.75f, 0.7f }, (eTextMod)1);
			}
			else
			{
				m_bAbleToCraft = false;
				GRAPHICS->RenderText(L"Craft:", { -200.0f, -200.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.75f, 0.7f }, (eTextMod)1);
				GRAPHICS->RenderText(L"Not Enough Resources", { 250.0f, -225.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.45f, 0.4f }, (eTextMod)1);
			}

			for (unsigned int i = 0; i < 4; i++)
				m_cpInputImgs[i]->GetRender() = true;

			GRAPHICS->RenderText(L"Craft One", { -175.0f, -525.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.35f, 0.3f });

			unsigned int unCraftAllAmount = 0;
			if (GetStock(Bones) / 2 < GetStock(Herb))
				unCraftAllAmount = GetStock(Bones) / 2;
			else
				unCraftAllAmount = GetStock(Herb);

			string szCraftAll = "Craft All (" + to_string(unCraftAllAmount) + ")";
			wstring wszCraftAll(szCraftAll.begin(), szCraftAll.end());
			GRAPHICS->RenderText(wszCraftAll.c_str(), { 375.0f, -525.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.35f, 0.3f }, (eTextMod)1);
		}
		break;
		default:
			break;
		}

#pragma endregion

#pragma region Input

		if (CInputManager::GetInstance()->IsKeyPressed("Down") || CInputManager::GetInstance()->IsButtonPressed("Cursor Down"))
		{
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::LOOT);
			m_vcBoarder[m_nCursor]->GetRender() = true;
			m_vcBoarder[m_nCursor]->SetTexture(L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Normal.dds");

			if (m_nCursor++ >= (NUM_ITEMS - 1))
			{
				m_nCursor = 0;
			}

			m_vcBoarder[m_nCursor]->SetTexture(L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Selected.dds");
			m_cpCurrentItem->SetTexture(m_vIconPaths[m_nCursor]);
		}
		if (CInputManager::GetInstance()->IsKeyPressed("Up") || CInputManager::GetInstance()->IsButtonPressed("Cursor Up"))
		{
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::LOOT);
			m_vcBoarder[m_nCursor]->GetRender() = true;
			m_vcBoarder[m_nCursor]->SetTexture(L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Normal.dds");

			if (m_nCursor-- <= 0)
			{
				m_nCursor = NUM_ITEMS - 1;
			}

			m_vcBoarder[m_nCursor]->SetTexture(L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Selected.dds");
			m_cpCurrentItem->SetTexture(m_vIconPaths[m_nCursor]);
		}
		if ((CInputManager::GetInstance()->IsKeyPressed("Craft") || CInputManager::GetInstance()->IsButtonPressed("Jump Action")) && m_bAbleToCraft == true)
		{
			m_bCrafting = true;
			m_bCraftAll = false;
		}
		if ((CInputManager::GetInstance()->IsKeyPressed("CraftAll") || CInputManager::GetInstance()->IsButtonPressed("Health Potion")) && m_bAbleToCraft == true)
		{
			m_bCrafting = true;
			m_bCraftAll = true;
		}

#pragma endregion

#pragma region Input Option text Display

		string szTooltip[NUM_ITEMS] =
		{
			"Dropped by Skeletons",
			"Dropped from Trees",
			"Dropped from Bushes",
			"Dropped by Orcs",
			"Craftable Item\nRequires: ",
			"Craftable Item\nRequires: ",
			"Craftable Item\nRequires: ",
		};
		wstring wToolTip(szTooltip[m_nCursor].begin(), szTooltip[m_nCursor].end());
		GRAPHICS->RenderText(wToolTip.c_str(), { -300.0f, 100.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.55f, 0.5f });

		if (m_nCursor > eItems::Gunpowder)
		{
			if (m_nCursor == eItems::Bomb)
			{
				XMFLOAT4 f2Color1 = { 1.0f, 0.0f, 0.0f, 1.0f };
				XMFLOAT4 f2Color2 = { 1.0f, 0.0f, 0.0f, 1.0f };

				string szRequirement1 = "2 Bones";
				string szRequirement2 = "2 Gunpowder";
				if (GetStock(eItems::Bones) >= 2)
				{
					f2Color1 = { 0.0f, 1.0f, 0.0f, 1.0f };
				}
				if (GetStock(eItems::Gunpowder) >= 2)
				{
					f2Color2 = { 0.0f, 1.0f, 0.0f, 1.0f };
				}

				wstring wInfo1(szRequirement1.begin(), szRequirement1.end());
				wstring wInfo2(szRequirement2.begin(), szRequirement2.end());

				GRAPHICS->RenderText(wInfo1.c_str(), { 0.0f, 0.0f }, f2Color1, { 0.55f, 0.5f });
				GRAPHICS->RenderText(wInfo2.c_str(), { 0.0f, -60.0f }, f2Color2, { 0.55f, 0.5f });
			}
			else if (m_nCursor == eItems::HealthPotion)
			{
				XMFLOAT4 f2Color1 = { 1.0f, 0.0f, 0.0f, 1.0f };
				XMFLOAT4 f2Color2 = { 1.0f, 0.0f, 0.0f, 1.0f };

				string szRequirement1 = "2 Herbs";
				string szRequirement2 = "1 Bone";
				if (GetStock(eItems::Herb) >= 2)
				{
					f2Color1 = { 0.0f, 1.0f, 0.0f, 1.0f };
				}
				if (GetStock(eItems::Bones) >= 1)
				{
					f2Color2 = { 0.0f, 1.0f, 0.0f, 1.0f };
				}

				wstring wInfo1(szRequirement1.begin(), szRequirement1.end());
				wstring wInfo2(szRequirement2.begin(), szRequirement2.end());

				GRAPHICS->RenderText(wInfo1.c_str(), { 0.0f, 0.0f }, f2Color1, { 0.55f, 0.5f });
				GRAPHICS->RenderText(wInfo2.c_str(), { 0.0f, -60.0f }, f2Color2, { 0.55f, 0.5f });
			}
			else if (m_nCursor == eItems::Bridge)
			{
				XMFLOAT4 f2Color = { 1.0f, 0.0f, 0.0f, 1.0f };
				string szRequirement1 = "1 Wood Log";
				if (GetStock(eItems::Wood) >= 1)
				{
					f2Color = { 0.0f, 1.0f, 0.0f, 1.0f };
				}

				wstring wInfo1(szRequirement1.begin(), szRequirement1.end());
				GRAPHICS->RenderText(wInfo1.c_str(), { 0.0f, 0.0f }, f2Color, { 0.55f, 0.5f });
			}
		}

#pragma endregion

#pragma region Crafting

		//user is in the process of crafting
		if (m_bCrafting == true)
		{
			m_cpCraftingBar->GetRender() = true;
			m_fCraftTime += DELTA_TIME();
			if (m_fCraftTime >= CRAFT_TIME)
			{
				m_bCrafting = false;
				m_fCraftTime = 0.0f;
				m_cpCraftingBar->GetRender() = false;
				AudioSystemWwise::Get()->PostEvent(AK::EVENTS::CRAFT);

#pragma region Removing and Adding Items from Inventory

				if (m_bCraftAll == true)
				{
					switch ((eItems)m_nCursor)
					{
					case CInventory::Bomb:
					{
						unsigned int numBones = GetStock(Bones);
						unsigned int numGunpowder = GetStock(Gunpowder);

						if (numBones % 2 != 0)
							numBones -= 1;
						if (numGunpowder % 2 != 0)
							numGunpowder -= 1;

						unsigned int totalAdded = 0;
						if (numBones / 2 < numGunpowder / 2)
							totalAdded = numBones / 2;
						else
							totalAdded = numGunpowder / 2;

						for (unsigned int i = 0; i < totalAdded; i++)
						{
							RemoveFromInventory(Bones, 2);
							RemoveFromInventory(Gunpowder, 2);

							AddToInventory(Bomb, 1);
						}
					}
					break;
					case CInventory::Bridge://wood plank
					{
						unsigned int numWood = GetStock(Wood);
						for (unsigned int i = 0; i < numWood; i++)
						{
							RemoveFromInventory(Wood, 1);

							AddToInventory(Bridge, 2);
						}
					}
					break;
					case CInventory::HealthPotion:
					{
						unsigned int numBones = GetStock(Bones);
						unsigned int numHerbs = GetStock(Herb);

						if (numHerbs % 2 != 0)
							numHerbs -= 1;

						unsigned int totalAdded = 0;
						if (numHerbs / 2 < numBones)
							totalAdded = numHerbs / 2;
						else
							totalAdded = numBones;

						for (unsigned int i = 0; i < totalAdded; i++)
						{
							RemoveFromInventory(Bones, 1);
							RemoveFromInventory(Herb, 2);

							AddToInventory(HealthPotion, 1);
						}
					}
					break;
					default:
						break;
					}
				}
				else
				{
					switch ((eItems)m_nCursor)
					{
					case CInventory::Bomb:
					{
						RemoveFromInventory(Bones, 2);
						RemoveFromInventory(Gunpowder, 2);

						AddToInventory(Bomb, 1);
					}
					break;
					case CInventory::Bridge://wood plank
					{
						RemoveFromInventory(Wood, 1);

						AddToInventory(Bridge, 2);
					}
					break;
					case CInventory::HealthPotion:
					{
						RemoveFromInventory(Bones, 1);
						RemoveFromInventory(Herb, 2);

						AddToInventory(HealthPotion, 1);
					}
					break;
					default:
						break;
					}
				}

#pragma endregion

				m_cpCraftingBar->GetVertex2DArr()[1].m_fPosition[0] = m_fCraftBarStartX;
				m_cpCraftingBar->GetVertex2DArr()[2].m_fPosition[0] = m_fCraftBarStartX;
			}

			// update index 1 and 2 of 2dVert
			float fRatio = m_fCraftTime / CRAFT_TIME;
			m_cpCraftingBar->GetVertex2DArr()[1].m_fPosition[0] = m_fCraftBarStartX + (m_fCraftBarSize * fRatio);
			m_cpCraftingBar->GetVertex2DArr()[2].m_fPosition[0] = m_fCraftBarStartX + (m_fCraftBarSize * fRatio);
		}

#pragma endregion

		if (CInputManager::GetInstance()->IsKeyPressed("Escape"))
			Close();
	}

}

void CInventory::Open()
{
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::OPENINV);

	m_bOpen = true;
	m_cpInventoryScreen->GetRender() = true;
	m_cpCurrentItem->GetRender() = true;
	m_cpSelectedIcon->GetRender() = true;


	m_cpCraftingBar->GetRender() = true;
	m_cpCraftingBar->GetVertex2DArr()[1].m_fPosition[0] = m_fCraftBarStartX;
	m_cpCraftingBar->GetVertex2DArr()[2].m_fPosition[0] = m_fCraftBarStartX;

	for (size_t i = 0; i < m_vcIcons.size(); i++)
	{
		m_vcIcons[i]->GetRender() = true;
	}

	for (size_t i = 0; i < m_vcBoarder.size(); i++)
	{
		m_vcBoarder[i]->GetRender() = true;
	}

	for (unsigned int i = 0; i < 2; i++)
	{
		m_cpInputImgs[i]->GetRender() = true;
	}

}

void CInventory::Close()
{
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::OPENINV);

	m_bOpen = false;
	m_cpInventoryScreen->GetRender() = false;
	m_cpSelectedIcon->GetRender() = false;
	m_cpCurrentItem->GetRender() = false;
	m_cpCraftingBar->GetRender() = false;

	for (size_t i = 0; i < m_vcIcons.size(); i++)
	{
		m_vcIcons[i]->GetRender() = false;
	}

	for (size_t i = 0; i < m_vcBoarder.size(); i++)
	{
		m_vcBoarder[i]->GetRender() = false;
	}

	for (unsigned int i = 0; i < NUM_INPUT_IMAGES; i++)
	{
		m_cpInputImgs[i]->GetRender() = false;
	}

}

void CInventory::AddToInventory(eItems _type, int _ammount)
{
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::LOOT, { 0, 0, 0 });

	//m_cpPickUpIcon->SetTexture( m_vIconPaths[_type] );
	//m_cpPickUpBorder->GetRender() = true;
	//m_cpPickUpIcon->GetRender() = true;

	//m_bPickUp = true;

	m_CurrentStock[_type] += _ammount;
}

void CInventory::RemoveFromInventory(eItems _type, int _ammount)
{
	m_CurrentStock[_type] -= _ammount;
}

void CInventory::AddGraveStoneStock(map<int, int> _stock)
{
	for (size_t i = 0; i < m_CurrentStock.size(); i++)
	{
		m_CurrentStock[i] += _stock[i];
	}
}

void CInventory::ResetStock()
{
	for (size_t i = 0; i < m_CurrentStock.size(); i++)
	{
		m_CurrentStock[i] = 0;
	}
}

void CInventory::AddToRenderer()
{
	if (m_cpInventoryScreen != nullptr)
		GRAPHICS->AddRenderMesh(m_cpInventoryScreen);

	for (unsigned int i = 0; i < m_vcBoarder.size(); i++)
	{
		if (m_vcBoarder[i] != nullptr)
			GRAPHICS->AddRenderMesh(m_vcBoarder[i]);
	}

	for (unsigned int i = 0; i < m_vcIcons.size(); i++)
	{
		if (m_vcIcons[i] != nullptr)
			GRAPHICS->AddRenderMesh(m_vcIcons[i]);
	}

	if (m_cpSelectedIcon != nullptr)
		GRAPHICS->AddRenderMesh(m_cpSelectedIcon);

	if (m_cpCurrentItem != nullptr)
		GRAPHICS->AddRenderMesh(m_cpCurrentItem);

	for (unsigned int i = 0; i < NUM_INPUT_IMAGES; i++)
	{
		if (m_cpInputImgs[i] != nullptr)
			GRAPHICS->AddRenderMesh(m_cpInputImgs[i]);
	}

	if (m_cpCraftingBar != nullptr)
		GRAPHICS->AddRenderMesh(m_cpCraftingBar);
}
