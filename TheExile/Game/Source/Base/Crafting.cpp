/***********************************************
* Filename:  		Crafting.h
* Date:      		10/1/2015
* Mod. Date:
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			Acts as a workstation in the world that enables crafting to be done.
************************************************/

#include "Crafting.h"
#include "Inventory.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Core/Messages/AddObjectMessage.h"
#include "../../../Engine/Core/Messages/RemoveObjectMessage.h"
#include "../../../Engine/Core/Input/InputManager.h"
#include "../../../Engine/Renderer/Renderer.h"
#include "../../../Engine/Renderer/HUDMesh.h"

#define NUM_RESOURCES 4
#define NUM_ITEMS 7
#define NUM_CRAFTED 3
#define CRAFT_TIME 2.0f

CCrafting::CCrafting() : IObject("CraftingTable")
{
	//input registers
	CInputManager::GetInstance()->RegisterKey("Down", eKey::DownArrow);
	CInputManager::GetInstance()->RegisterKey("Up", eKey::UpArrow);
	CInputManager::GetInstance()->RegisterKey("Craft", eKey::Enter);
	CInputManager::GetInstance()->RegisterKey("CraftAll", eKey::Spacebar);
	CInputManager::GetInstance()->RegisterKey("Exit", eKey::escape);

#pragma region Crafting Table 

	//set position
	XMFLOAT3 newLocation = { -1500.0f, 0.0f, -1000.0f };
	SetPosition(newLocation);

	//initailze variables
	m_fCraftTime = 0.0f;
	m_bIsCrafting = false;
	m_bAtTable = false;
	m_bCraftAll = false;
	m_nCursor = 0;
	m_eCraftedItem = CInventory::eItems::Bomb;
	m_unCraftAllAmount = 0;

	//set crafting table items
	m_mCraftingTable[0] = CInventory::eItems::Bomb;
	m_mCraftingTable[1] = CInventory::eItems::Bridge;
	m_mCraftingTable[2] = CInventory::eItems::HealthPotion;

	m_f4CraftableColor = { 0.0f, 0.0f, 0.0f, 1.0f };//rgba

	//set the crafting table mesh
	CMesh* theMesh = CAssetManager::GetInstance()->GetPrefabMesh("WorkBench");
	m_cpRenderMesh = new CRenderMesh(theMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Failbot_WorkBench.dds");
	//GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	//set collider
	newLocation.y += 75.0f;
	AddCollider(new CCollider(false, new CAABB(newLocation, { 100, 150, 100 }), true));

	//add the crafting table object to object manager
	CAddObjectMessage* addObj = new CAddObjectMessage(this, CObjectManager::Static);
	addObj->Send();

#pragma endregion

	//set all icon paths for player items/resources
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Bone.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Wood.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Herb.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Gunpowder.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Bomb.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_Bridge.dds");
	m_vIconPaths.push_back(L"../Game/Assets/Art/2D/Inventory/Failbot_Inv_HealthPotion.dds");

#pragma region Crafting Background Image

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

	m_cpCraftScreen = new CHUDMesh(ScreenIndicies, ScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Inventory/Failbot_Crafting_Background.dds");
	m_cpCraftScreen->GetRender() = false;
	m_cpCraftScreen->GetTransparent() = true;
	GRAPHICS->AddRenderMesh(m_cpCraftScreen);

#pragma endregion

#pragma region Icons

	float yOffset = 0.14f;

	//icon positions
	float fIconLeft = 0.275f;
	float fIconRight = 0.35f;
	float fIconTop = 0.4f;
	float fIconBottom = 0.325f;

	//boarder positions
	float fBoarderLeft = 0.25f;
	float fBoarderRight = 0.375f;
	float fBoarderTop = 0.425f;
	float fBoarderBottom = 0.3f;


#pragma region Player Resources/Items (Right Side)

	for (unsigned int i = 0; i < NUM_ITEMS; i++)
	{
		vector<TVertex2D> Verts;
		vector<TVertex2D> BoarderVerts;
		vector<unsigned int> Indicies;

		Indicies.push_back(0);
		Indicies.push_back(1);
		Indicies.push_back(2);

		Indicies.push_back(0);
		Indicies.push_back(2);
		Indicies.push_back(3);

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
	}

#pragma endregion

#pragma region Craftable Items (Left Side)

	yOffset = 0.305f;

	fIconLeft = -0.65f;
	fIconRight = -0.55f;
	fIconTop = 0.4f;
	fIconBottom = 0.3f;

	fBoarderLeft = -0.73f;
	fBoarderRight = -0.48f;
	fBoarderTop = 0.455f;
	fBoarderBottom = 0.24f;

	for (size_t i = 0; i < NUM_CRAFTED; i++)
	{
		vector<TVertex2D> Verts;
		vector<TVertex2D> BoarderVerts;
		vector<unsigned int> Indicies;

		Indicies.push_back(0);
		Indicies.push_back(1);
		Indicies.push_back(2);

		Indicies.push_back(0);
		Indicies.push_back(2);
		Indicies.push_back(3);

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

		m_vpCraftingIcons.push_back(new CHUDMesh(Indicies, Verts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, m_vIconPaths[i + NUM_RESOURCES]));
		m_vpCraftingIcons[i]->GetRender() = false;
		m_vpCraftingIcons[i]->GetTransparent() = true;


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

		m_vpCraftingBoarders.push_back(new CHUDMesh(Indicies, BoarderVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Normal.dds"));
		m_vpCraftingBoarders[i]->GetRender() = false;
		m_vpCraftingBoarders[i]->GetTransparent() = true;


		GRAPHICS->AddRenderMesh(m_vpCraftingBoarders[i]);
		GRAPHICS->AddRenderMesh(m_vpCraftingIcons[i]);


		fIconTop -= yOffset;
		fIconBottom -= yOffset;
		fBoarderTop -= yOffset;
		fBoarderBottom -= yOffset;
	}

	m_vpCraftingBoarders[m_nCursor]->SetTexture(L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Selected.dds");

#pragma endregion

#pragma endregion

#pragma region CraftBar

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
	GRAPHICS->AddRenderMesh(m_cpCraftingBar);

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
	float fWidth = .7f;
	float fHeight = .2f;

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

#pragma region Controller Pop-Up

	vector<TVertex2D> tempVerts;
	vector<unsigned int> tempInd;

	tempInd.push_back(0);
	tempInd.push_back(1);
	tempInd.push_back(2);

	tempInd.push_back(0);
	tempInd.push_back(2);
	tempInd.push_back(3);

	tempVerts.push_back // Top Left
		({
			{ -0.4f, 0.2f, -1.0f, 1 },
			{ 0, 0 }
	});

	tempVerts.push_back // Top Right
		({
			{ 0.4f, 0.2f, -1.0f, 1 },
			{ 1, 0 }
	});

	tempVerts.push_back // Bot Right
		({
			{ 0.4f, -0.2f, -1.0f, 1 },
			{ 1, 1 }
	});

	tempVerts.push_back // Bot Left
		({
			{ -0.4f, -0.2f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_cpControllerPopUp = new CHUDMesh(tempInd, tempVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Crafting_Controller_PopUp.dds");
	m_cpControllerPopUp->GetRender() = false;
	m_cpControllerPopUp->GetTransparent() = true;
	TranslateLocalY(m_cpControllerPopUp->GetTransformMatrix(), 0.4f);
	GRAPHICS->AddRenderMesh(m_cpControllerPopUp);

#pragma endregion

}

CCrafting::~CCrafting()
{
	CRemoveObjectMessage* removeObj = new CRemoveObjectMessage(this, CObjectManager::Static);
	removeObj->Send();

	SAFE_DELETE(m_cpCraftingBar);
	SAFE_DELETE(m_cpTextFrame);
	SAFE_DELETE(m_cpCraftScreen);
	SAFE_DELETE(m_cpControllerPopUp);

	for (unsigned int i = 0; i < m_vcBoarder.size(); i++)
	{
		SAFE_DELETE(m_vcBoarder[i]);
	}
	m_vcBoarder.clear();

	for (unsigned int i = 0; i < m_vcIcons.size(); i++)
	{
		SAFE_DELETE(m_vcIcons[i]);
	}
	m_vcIcons.clear();

	for (unsigned int i = 0; i < m_vpCraftingIcons.size(); i++)
	{
		SAFE_DELETE(m_vpCraftingIcons[i]);
	}
	m_vpCraftingIcons.clear();

	for (unsigned int i = 0; i < m_vpCraftingBoarders.size(); i++)
	{
		SAFE_DELETE(m_vpCraftingBoarders[i]);
	}
	m_vpCraftingBoarders.clear();

}

void CCrafting::Update()
{
	if (CInputManager::GetInstance()->IsKeyPressed("Exit") || CInputManager::GetInstance()->IsButtonPressed("Back"))
	{
		Close();
	}

	if (!m_bDrawIndicator)
	{
		m_cpTextFrame->GetRender() = false;
		m_cpControllerPopUp->GetRender() = false;
	}
	m_bDrawIndicator = false;


	if (m_bOpen)
	{
		GRAPHICS->RenderText(L"Crafting Table", { -300.0f, 665.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.85f, 0.8f }, (eTextMod)1);
		GRAPHICS->RenderText(L"Inventory Items", { 375.0f, 625.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.35f, 0.4f }, (eTextMod)1);

#pragma region Right Side Display (Player Inventory)

		float fStockY = 425.0f;
		for (size_t i = 0; i < NUM_ITEMS; i++)
		{
			string temp = "x";
			wstring temp1(temp.begin(), temp.end());

			string strAmmount = to_string(m_cpTheInventory->GetStock((CInventory::eItems)i));
			wstring printStr(strAmmount.begin(), strAmmount.end());

			GRAPHICS->RenderText(temp1.c_str(), { 375.0f, fStockY - 50.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.35f, 0.35f });
			GRAPHICS->RenderText(printStr.c_str(), { 400.0f, fStockY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.7f, 0.7f });

			fStockY -= 140.0f;
		}

#pragma endregion

#pragma region Input: Craftable Item Selection (Left Side)

		if ((CInputManager::GetInstance()->IsKeyPressed("Down") || CInputManager::GetInstance()->IsButtonPressed("Cursor Down")) && !m_bIsCrafting)
		{
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::LOOT);

			m_vpCraftingBoarders[m_nCursor]->GetRender() = true;
			m_vpCraftingBoarders[m_nCursor]->SetTexture(L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Normal.dds");
			
			m_nCursor++;
			if (m_nCursor >= NUM_CRAFTED)
			{
				m_nCursor = 0;
			}
			
			m_vpCraftingBoarders[m_nCursor]->GetRender() = true;
			m_vpCraftingBoarders[m_nCursor]->SetTexture(L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Selected.dds");
		}
		if ((CInputManager::GetInstance()->IsKeyPressed("Up") || CInputManager::GetInstance()->IsButtonPressed("Cursor Up")) && !m_bIsCrafting)
		{
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::LOOT);

			m_vpCraftingBoarders[m_nCursor]->GetRender() = true;
			m_vpCraftingBoarders[m_nCursor]->SetTexture(L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Normal.dds");
			
			m_nCursor--;
			if (m_nCursor < 0)
			{
				m_nCursor = NUM_CRAFTED - 1;
			}

			m_vpCraftingBoarders[m_nCursor]->GetRender() = true;
			m_vpCraftingBoarders[m_nCursor]->SetTexture(L"../Game/Assets/Art/2D/Inventory/Failbot_Icon_Selected.dds");
		}

#pragma endregion

#pragma region Text Rendering

		float fLeftOffset = -125.0f;

		//crafting info text
		if (m_nCursor == 0)
		{
			if ((m_cpTheInventory->GetStock(CInventory::eItems::Bones) / 2) < m_cpTheInventory->GetStock(CInventory::eItems::Gunpowder))
				m_unCraftAllAmount = m_cpTheInventory->GetStock(CInventory::eItems::Bones) / 2;
			else
				m_unCraftAllAmount = m_cpTheInventory->GetStock(CInventory::eItems::Gunpowder);

			GRAPHICS->RenderText(L"Bomb \n", { fLeftOffset, 400.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.75f, 0.75f }, eTextMod::Center);
			GRAPHICS->RenderText(L"Crafted with: \n", { fLeftOffset, 200.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.5f, 0.5f }, eTextMod::Center);
			GRAPHICS->RenderText(L" 2x Bone \n ", { fLeftOffset, 100.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.5f, 0.5f }, eTextMod::Center);
			GRAPHICS->RenderText(L" 1x Gundpowder", { fLeftOffset, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.5f, 0.5f }, eTextMod::Center);
		}
		else if (m_nCursor == 1)
		{
			m_unCraftAllAmount = m_cpTheInventory->GetStock(CInventory::eItems::Wood) * 2;

			GRAPHICS->RenderText(L"2 Wood Planks \n", { fLeftOffset, 400.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.75f, 0.75f }, eTextMod::Center);
			GRAPHICS->RenderText(L"Crafted with: \n", { fLeftOffset, 200.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.5f, 0.5f }, eTextMod::Center);
			GRAPHICS->RenderText(L" 1x Wood Log", { fLeftOffset, 100.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.5f, 0.5f }, eTextMod::Center);
		}
		else if (m_nCursor == 2)
		{
			if ((m_cpTheInventory->GetStock(CInventory::eItems::Bones) / 2) < m_cpTheInventory->GetStock(CInventory::eItems::Herb))
				m_unCraftAllAmount = m_cpTheInventory->GetStock(CInventory::eItems::Bones) / 2;
			else
				m_unCraftAllAmount = m_cpTheInventory->GetStock(CInventory::eItems::Herb);

			GRAPHICS->RenderText(L"Health Potion \n", { fLeftOffset, 400.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.7f, 0.7f }, eTextMod::Center);
			GRAPHICS->RenderText(L"Crafted with: \n", { fLeftOffset, 200.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.5f, 0.5f }, eTextMod::Center);
			GRAPHICS->RenderText(L" 2x Bone \n 1x Herb", { fLeftOffset, 100.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.5f, 0.5f }, eTextMod::Center);
		}

		//controller related text
		if (CInputManager::GetInstance()->GetControllerBeingUsed() == true)
		{
			GRAPHICS->RenderText(L"'B' Close", { -600.0f, -450.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.7f, 0.7f }, (eTextMod)1);

			GRAPHICS->RenderText(L"'Y' Craft All", { fLeftOffset, -450.0f }, m_f4CraftableColor, { 0.35f, 0.35f }, (eTextMod)1);

			string szCraftAll = "'A' Craft All (" + to_string(m_unCraftAllAmount) + ")";
			wstring wszCraftAll(szCraftAll.begin(), szCraftAll.end());
			GRAPHICS->RenderText(wszCraftAll.c_str(), { fLeftOffset, -500.0f }, m_f4CraftableColor, { 0.35f, 0.35f }, (eTextMod)1);
		}
		else
		{
			GRAPHICS->RenderText(L"'C' Close", { -600.0f, -450.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.7f, 0.7f }, (eTextMod)1);

			GRAPHICS->RenderText(L"'Enter' Craft One", { fLeftOffset, -450.0f }, m_f4CraftableColor, { 0.35f, 0.35f }, (eTextMod)1);

			string szCraftAll = "'Space' Craft All (" + to_string(m_unCraftAllAmount) + ")";
			wstring wszCraftAll(szCraftAll.begin(), szCraftAll.end());
			GRAPHICS->RenderText(wszCraftAll.c_str(), { fLeftOffset, -500.0f }, m_f4CraftableColor, { 0.35f, 0.35f }, (eTextMod)1);
		}

#pragma endregion

		Crafting();
	}

#pragma region Crafting Bar

	if (m_bIsCrafting)
	{
		GRAPHICS->RenderText(L"Crafting...", { -125.0f, -150.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.85f, 0.8f }, (eTextMod)1);

		m_fCraftTime += DELTA_TIME();
		if (m_fCraftTime >= CRAFT_TIME)
		{
			m_fCraftTime = 0.0f;
			m_bIsCrafting = false;
			m_cpCraftingBar->GetRender() = false;
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::CRAFT);

#pragma region Removing and Adding Items from Inventory

			if (m_bCraftAll == true)
			{
				switch (m_eCraftedItem)
				{
				case CInventory::Bomb:
				{
					unsigned int numBones = m_cpTheInventory->GetStock(CInventory::Bones);
					unsigned int numGunpowder = m_cpTheInventory->GetStock(CInventory::Gunpowder);

					if (numBones % 2 != 0)
					{
						numBones -= 1;
					}
					if (numGunpowder % 2 != 0)
					{
						numGunpowder -= 1;
					}

					unsigned int totalAdded = 0;
					if (numBones / 2 < numGunpowder)
					{
						totalAdded = numBones / 2;
					}
					else
					{
						totalAdded = numGunpowder;
					}

					for (unsigned int i = 0; i < totalAdded; i++)
					{
						m_cpTheInventory->RemoveFromInventory(CInventory::Bones, 2);
						m_cpTheInventory->RemoveFromInventory(CInventory::Gunpowder, 1);

						m_cpTheInventory->AddToInventory(CInventory::Bomb, 1);
					}
				}
				break;
				case CInventory::Bridge://wood plank
				{
					unsigned int numWood = m_cpTheInventory->GetStock(CInventory::Wood);
					for (unsigned int i = 0; i < numWood; i++)
					{
						m_cpTheInventory->RemoveFromInventory(CInventory::Wood, 1);

						m_cpTheInventory->AddToInventory(CInventory::Bridge, 2);
					}
				}
				break;
				case CInventory::HealthPotion:
				{
					unsigned int numBones = m_cpTheInventory->GetStock(CInventory::Bones);
					unsigned int numHerbs = m_cpTheInventory->GetStock(CInventory::Herb);

					if (numBones % 2 != 0)
					{
						numBones -= 1;
					}

					unsigned int totalAdded = 0;
					if (numBones / 2 < numHerbs)
					{
						totalAdded = numBones / 2;
					}
					else
					{
						totalAdded = numHerbs;
					}

					for (unsigned int i = 0; i < totalAdded; i++)
					{
						m_cpTheInventory->RemoveFromInventory(CInventory::Bones, 2);
						m_cpTheInventory->RemoveFromInventory(CInventory::Herb, 1);

						m_cpTheInventory->AddToInventory(CInventory::HealthPotion, 1);
					}
				}
				break;
				default:
					break;
				}
			}
			else
			{
				switch (m_eCraftedItem)
				{
				case CInventory::Bomb:
				{
					m_cpTheInventory->RemoveFromInventory(CInventory::Bones, 2);
					m_cpTheInventory->RemoveFromInventory(CInventory::Gunpowder, 1);

					m_cpTheInventory->AddToInventory(CInventory::Bomb, 1);
				}
				break;
				case CInventory::Bridge://wood plank
				{
					m_cpTheInventory->RemoveFromInventory(CInventory::Wood, 1);

					m_cpTheInventory->AddToInventory(CInventory::Bridge, 2);
				}
				break;
				case CInventory::HealthPotion:
				{
					m_cpTheInventory->RemoveFromInventory(CInventory::Bones, 2);
					m_cpTheInventory->RemoveFromInventory(CInventory::Herb, 1);

					m_cpTheInventory->AddToInventory(CInventory::HealthPotion, 1);
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

	// Set to always false unless players collision set's to true.
	if (!m_bAtTable && m_bOpen)
	{
		Close();
	}
	m_bAtTable = false;
}

void CCrafting::DrawIndicator()
{
	if (CInputManager::GetInstance()->GetControllerBeingUsed() == true)
	{
		//GRAPHICS->RenderText(L"X to Craft!", { -250, 500 }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ .5f, .5f });
		m_cpControllerPopUp->GetRender() = true;
	}
	else
	{
		GRAPHICS->RenderText(L"'C' to Craft!", { -250, 500 }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ .5f, .5f });
	}


	m_cpTextFrame->GetVertex2DArr()[1].m_fPosition[0] = .15f;
	m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[0] = .15f;
	m_cpTextFrame->GetRender() = true;

	m_bDrawIndicator = true;
}

void CCrafting::Crafting()
{
	string szCraftable = "'Enter' to Craft";
	string szUnCraftable = "Missing Resource";
	wstring wszCraft(szCraftable.begin(), szCraftable.end());
	wstring wszUnCraft(szUnCraftable.begin(), szUnCraftable.end());
	XMFLOAT2 position = { -200.0f, 300.0f };

	//bomb = 2 bone and 2 gunpowder
	//wood plank = 2 wood
	//health potion = 1 bone and 2 herb

	if (m_mCraftingTable[m_nCursor] == CInventory::Bomb)
	{
		//check if the player has enough resources to craft a bomb
		if (m_cpTheInventory->GetStock(CInventory::Bones) >= 2 && m_cpTheInventory->GetStock(CInventory::Gunpowder) >= 1)
		{
			GRAPHICS->RenderText(wszCraft.c_str(), position, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.4f, .03f }, eTextMod::Center);

			m_f4CraftableColor = { 0.0f, 1.0f, 0.0f, 1.0f };

			if ((CInputManager::GetInstance()->IsKeyPressed("Craft") || CInputManager::GetInstance()->IsButtonPressed("Jump Action")) && !m_bIsCrafting)
			{
				m_bIsCrafting = true;
				m_cpCraftingBar->GetRender() = true;
				m_bCraftAll = false;

				m_eCraftedItem = CInventory::Bomb;
			}
			else if ((CInputManager::GetInstance()->IsKeyPressed("CraftAll") || CInputManager::GetInstance()->IsButtonPressed("Health Potion")) && !m_bIsCrafting)
			{
				m_bIsCrafting = true;
				m_cpCraftingBar->GetRender() = true;
				m_bCraftAll = true;

				m_eCraftedItem = CInventory::Bomb;
			}
		}
		else
		{
			m_f4CraftableColor = { 0.0f, 0.0f, 0.0f, 1.0f };

			GRAPHICS->RenderText(wszUnCraft.c_str(), position, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.4f, .03f });
		}
	}
	else if (m_mCraftingTable[m_nCursor] == CInventory::Bridge)//actually wood plank
	{
		//check if the player has enough resources to craft a wood plank
		if (m_cpTheInventory->GetStock(CInventory::Wood) >= 1)
		{
			GRAPHICS->RenderText(wszCraft.c_str(), position, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.4f, .03f }, eTextMod::Center);

			m_f4CraftableColor = { 0.0f, 1.0f, 0.0f, 1.0f };

			if ((CInputManager::GetInstance()->IsKeyPressed("Craft") || CInputManager::GetInstance()->IsButtonPressed("Jump Action")) && !m_bIsCrafting)
			{
				m_bIsCrafting = true;
				m_cpCraftingBar->GetRender() = true;
				m_bCraftAll = false;

				m_eCraftedItem = CInventory::Bridge;
			}
			else if ((CInputManager::GetInstance()->IsKeyPressed("CraftAll") || CInputManager::GetInstance()->IsButtonPressed("Health Potion")) && !m_bIsCrafting)
			{
				m_bIsCrafting = true;
				m_cpCraftingBar->GetRender() = true;
				m_bCraftAll = true;

				m_eCraftedItem = CInventory::Bridge;
			}
		}
		else
		{
			m_f4CraftableColor = { 0.0f, 0.0f, 0.0f, 1.0f };

			GRAPHICS->RenderText(wszUnCraft.c_str(), position, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.4f, .03f });
		}
	}

	else if (m_mCraftingTable[m_nCursor] == CInventory::HealthPotion)
	{
		//check if the player has enough resources to craft a wood plank
		if (m_cpTheInventory->GetStock(CInventory::Herb) >= 1 && m_cpTheInventory->GetStock(CInventory::Bones) >= 2)
		{
			GRAPHICS->RenderText(wszCraft.c_str(), position, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.4f, .03f }, eTextMod::Center);

			m_f4CraftableColor = { 0.0f, 1.0f, 0.0f, 1.0f };

			if ((CInputManager::GetInstance()->IsKeyPressed("Craft") || CInputManager::GetInstance()->IsButtonPressed("Jump Action")) && !m_bIsCrafting)
			{
				m_bIsCrafting = true;
				m_cpCraftingBar->GetRender() = true;
				m_bCraftAll = false;

				m_eCraftedItem = CInventory::HealthPotion;
			}
			else if ((CInputManager::GetInstance()->IsKeyPressed("CraftAll") || CInputManager::GetInstance()->IsButtonPressed("Health Potion")) && !m_bIsCrafting)
			{
				m_bIsCrafting = true;
				m_cpCraftingBar->GetRender() = true;
				m_bCraftAll = true;

				m_eCraftedItem = CInventory::HealthPotion;
			}
		}
		else
		{
			m_f4CraftableColor = { 0.0f, 0.0f, 0.0f, 1.0f };

			GRAPHICS->RenderText(wszUnCraft.c_str(), position, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.4f, .03f });
		}
	}

}

void CCrafting::Open()
{
	m_bOpen = true;

	m_cpCraftScreen->GetRender() = true;

	for (size_t i = 0; i < m_vcIcons.size(); i++)
	{
		m_vcIcons[i]->GetRender() = true;
	}

	for (size_t i = 0; i < m_vcBoarder.size(); i++)
	{
		m_vcBoarder[i]->GetRender() = true;
	}

	for (unsigned int i = 0; i < m_vpCraftingIcons.size(); i++)
	{
		m_vpCraftingIcons[i]->GetRender() = true;
	}

	for (unsigned int i = 0; i < m_vpCraftingBoarders.size(); i++)
	{
		m_vpCraftingBoarders[i]->GetRender() = true;
	}
}

void CCrafting::Close()
{
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::OPENINV);

	m_bOpen = false;

	m_cpCraftScreen->GetRender() = false;

	for (size_t i = 0; i < m_vcIcons.size(); i++)
	{
		m_vcIcons[i]->GetRender() = false;
	}

	for (size_t i = 0; i < m_vcBoarder.size(); i++)
	{
		m_vcBoarder[i]->GetRender() = false;
	}

	for (unsigned int i = 0; i < m_vpCraftingIcons.size(); i++)
	{
		m_vpCraftingIcons[i]->GetRender() = false;
	}

	for (unsigned int i = 0; i < m_vpCraftingBoarders.size(); i++)
	{
		m_vpCraftingBoarders[i]->GetRender() = false;
	}

	m_fCraftTime = 0.0f;
	m_bIsCrafting = false;
	m_cpCraftingBar->GetRender() = false;
}
