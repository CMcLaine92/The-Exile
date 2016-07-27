/***********************************************
* Filename:  		BedObject.cpp
* Date:      		09/13/2015
* Mod. Date:
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			Handles both transperant bridge and collidable bridge.
************************************************/
#include "BedObject.h"
#include "DayNight.h"
#include "Bridge.h"
#include "CrackedWall.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Core/Messages/AddObjectMessage.h"
#include "../../../Engine/Core/Messages/RemoveObjectMessage.h"
#include "../../../Engine/Core/State Machine/StateMachine.h"
#include "../../../Engine/Core/State Machine/States/GameplayState.h"
#include "../../../Engine/Renderer/Renderer.h"
#include "../../../Engine/Renderer/HUDMesh.h"
#include "../../../Engine/Cutscene/CutsceneManager.h"
#include "../../Source/Player/Player.h"
#include "../../Source/Player/Weapon.h"
#include "../../Source/HUD/HUD.h"
#include "../../Source/HUD/WorldMap.h"
#include "../../Source/Base/Inventory.h"
#include "../../Source/Base/StorageCrate.h"
#include "../../Source/Environment/Torch.h"
#include "../../../Engine/Core/Input/InputManager.h"
#include "../../../Engine/Object Manager/ObjectManager.h"
#include <fstream>
#include <sstream>
#include <ShlObj.h>

#define CD_TIME 5.0f

CBed::CBed(CDayNight* _ptr, CObjectManager* objManager) : IObject("Bed")
{
	m_cpTheDayNight = _ptr;
	m_fCooldown = CD_TIME;
	m_unCutsceneID = -1;
	m_pObjectManager = objManager;
	m_cpHud = nullptr;

	XMFLOAT3 newLocation = { -1500.0f, 0.0f, -1500.0f };
	SetPosition(newLocation);

	CMesh * theMesh = CAssetManager::GetInstance()->GetPrefabMesh("Bed");
	m_cpRenderMesh = new CRenderMesh(theMesh,
		GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(),
		nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Failbot_Bed.dds");

	GRAPHICS->AddRenderMesh(m_cpRenderMesh);


	AddCollider(new CCollider(false, Bounds::AABB, theMesh->GetVertices(), true));
	XMFLOAT3 centerLocation = reinterpret_cast<CAABB*> (m_pvColliders[0]->GetBounds())->GetCenter();
	reinterpret_cast<CAABB*> (m_pvColliders[0]->GetBounds())->SetCenter(XMFLOAT3(newLocation.x, centerLocation.y, newLocation.z));
	XMFLOAT3 colExtents = reinterpret_cast<CAABB*> (m_pvColliders[0]->GetBounds())->GetExtents();
	colExtents.x += 50;
	colExtents.y += 100;
	colExtents.z += 50;
	reinterpret_cast<CAABB*> (m_pvColliders[0]->GetBounds())->SetExtents(colExtents);

	CAddObjectMessage* addObj = new CAddObjectMessage(this, CObjectManager::Static);
	addObj->Send();


#pragma region TextFrame

	vector<TVertex2D> ScreenVerts;
	vector<unsigned int> ScreenIndicies;

	ScreenIndicies.push_back(0);
	ScreenIndicies.push_back(1);
	ScreenIndicies.push_back(2);

	ScreenIndicies.push_back(0);
	ScreenIndicies.push_back(2);
	ScreenIndicies.push_back(3);

	float fStartX = -0.30f;
	float fStartY = 0.55f;
	float fWidth = .7f;
	float fHeight = .2f;

	ScreenVerts.push_back // Top Left
		({
			{ fStartX, fStartY, -1.0f, 1 },
			{ 0, 0 }
	});

	ScreenVerts.push_back // Top Right
		({
			{ fStartX + fWidth, fStartY, -1.0f, 1 },
			{ 1, 0 }
	});

	ScreenVerts.push_back // Bot Right
		({
			{ fStartX + fWidth, fStartY - fHeight, -1.0f, 1 },
			{ 1, 1 }
	});

	ScreenVerts.push_back // Bot Left
		({
			{ fStartX, fStartY - fHeight, -1.0f, 1 },
			{ 0, 1 }
	});

	m_cpTextFrame = new CHUDMesh(ScreenIndicies, ScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/Failbot_TextBackground.dds");
	m_cpTextFrame->GetRender() = false;
	m_cpTextFrame->GetTransparent() = true;
	GRAPHICS->AddRenderMesh(m_cpTextFrame);

#pragma endregion
#pragma region FadeIn

	vector<TVertex2D> OverlayVerts;
	vector<unsigned int> OverlayIndicies;

	OverlayIndicies.push_back(0);
	OverlayIndicies.push_back(1);
	OverlayIndicies.push_back(2);

	OverlayIndicies.push_back(0);
	OverlayIndicies.push_back(2);
	OverlayIndicies.push_back(3);

	float fOverStartX = -1.0f;
	float fOverStartY = 1.0f;
	float fOverWidth = 2.0f;
	float fOverHeight = 2.0f;

	OverlayVerts.push_back // Top Left
		({
			{ fOverStartX, fOverStartY, -1.0f, 1 },
			{ 0, 0 }
	});

	OverlayVerts.push_back // Top Right
		({
			{ fOverStartX + fOverWidth, fOverStartY, -1.0f, 1 },
			{ 1, 0 }
	});

	OverlayVerts.push_back // Bot Right
		({
			{ fOverStartX + fOverWidth, fOverStartY - fOverHeight, -1.0f, 1 },
			{ 1, 1 }
	});

	OverlayVerts.push_back // Bot Left
		({
			{ fOverStartX, fOverStartY - fOverHeight, -1.0f, 1 },
			{ 0, 1 }
	});

	m_cpOverlay = new CHUDMesh(OverlayIndicies, OverlayVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Failbot_SleepFade.dds");
	m_cpOverlay->GetRender() = false;
	m_cpOverlay->GetTransparent() = true;
	m_cpOverlay->GetTransparencyOverride() = 0.0f;
	GRAPHICS->AddRenderMesh(m_cpOverlay);

#pragma endregion

#pragma region Controller Door Pop-Up

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

	m_cpControllerPopUp = new CHUDMesh(tempInd, tempVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Sleep_Controller_PopUp.dds");
	m_cpControllerPopUp->GetRender() = false;
	m_cpControllerPopUp->GetTransparent() = true;
	TranslateLocalY(m_cpControllerPopUp->GetTransformMatrix(), 0.4f);
	GRAPHICS->AddRenderMesh(m_cpControllerPopUp);

#pragma endregion

}

CBed::~CBed()
{
	CRemoveObjectMessage* removeObj = new CRemoveObjectMessage(this, CObjectManager::Static, false);
	removeObj->Send();

	//if (m_cpRenderMesh)
	//{
	//	delete m_cpRenderMesh;
	//	m_cpRenderMesh = nullptr;
	//}

	delete m_cpTextFrame;
	m_cpTextFrame = false;
	delete m_cpOverlay;
	m_cpOverlay = false;

	SAFE_DELETE(m_cpControllerPopUp);
}

void CBed::Update()
{
	if (m_unCutsceneID == -1)
	{
		m_unCutsceneID = m_cpCutsceneManager->LoadCutscene("Failbot_BedCutscene.xml");
	}

	if (m_fCooldown > 0.0f)
	{
		m_fCooldown -= DELTA_TIME();

		if (m_cpOverlay->GetTransparencyOverride() < 1.0f && m_fCooldown > 2.5f)
		{
			m_cpOverlay->GetTransparencyOverride() += DELTA_TIME() / 2.5f;
		}
		else
		{
			m_cpOverlay->GetTransparencyOverride() -= DELTA_TIME() / 2.5f;
		}

		if (m_cpOverlay->GetTransparencyOverride() < 0.0f)
		{
			m_cpOverlay->GetTransparencyOverride() = 0.0f;
		}

	}
	else if (m_bActive)
	{
		m_cpCutsceneManager->GetPlayer()->GetRenderMesh()->GetRender() = true;
		m_cpCutsceneManager->GetPlayer()->GetCurrentWep()->GetRenderMesh()->GetRender() = true;
		m_cpOverlay->GetRender() = false;
		m_cpOverlay->GetTransparencyOverride() = 0.0f;
		m_bActive = false;

		// If it's night time, set to day
		if (m_cpTheDayNight->GetTimeOfDay() > 190.0f)
		{
			m_cpTheDayNight->SendDayNightMessage();
			m_cpTheDayNight->SetDay(true);
			m_cpTheDayNight->SetTimeOfDay(0);
		}
		else // Set to night
		{
			m_cpTheDayNight->SetDay(false);
			m_cpTheDayNight->SendDayNightMessage();
			m_cpTheDayNight->SetTimeOfDay(190.0f);
		}

	}

	if (!m_bDrawInd)
	{
		m_cpTextFrame->GetRender() = false;
		m_cpControllerPopUp->GetRender() = false;
	}
	m_bDrawInd = false;
}

void CBed::SetToMorning()
{
	if (IsAvailable() && m_cpTheDayNight->GetDoorTimer() < 0.0f)
	{
		SaveGame();

		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::SLEEPING);

		m_cpCutsceneManager->PlayCutscene(m_unCutsceneID);
		m_cpCutsceneManager->GetPlayer()->GetRenderMesh()->GetRender() = false;
		m_cpCutsceneManager->GetPlayer()->GetCurrentWep()->GetRenderMesh()->GetRender() = false;

		m_bActive = true;

		m_fCooldown = CD_TIME;
		m_cpOverlay->GetRender() = true;
	}
}

void CBed::DrawAvailableIndicator()
{
	if (CInputManager::GetInstance()->GetControllerBeingUsed() == true)
	{
		//GRAPHICS->RenderText(L"X to Sleep!", { -250, 500 }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ .5f, .5f });
		m_cpControllerPopUp->GetRender() = true;
	}
	else
	{
		GRAPHICS->RenderText(L"'F' to Sleep!", { -250, 500 }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ .5f, .5f });
	}

	m_cpTextFrame->GetVertex2DArr()[1].m_fPosition[0] = .15f;
	m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[0] = .15f;
	m_cpTextFrame->GetRender() = true;

	m_bDrawInd = true;
}

void CBed::DrawUnAvailableIndicator()
{
	if (m_cpHud->GetCurrentDay() == 4 && m_cpTheDayNight->IsNight() == true)//last night
	{
		GRAPHICS->RenderText(L"Last Night. Can't Sleep!", { -250, 500 }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ 0.5f, 0.5f });

		m_cpTextFrame->GetVertex2DArr()[1].m_fPosition[0] = 0.45f;
		m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[0] = 0.45f;
	}
	else if (m_cpTheDayNight->GetDoorTimer() < 0.0f && m_cpHud->GetCurrentDay() <= 4)
	{
		string szCountDown = "Wait: " + to_string((int)m_fCooldown);
		wstring wszCount(szCountDown.begin(), szCountDown.end());
		GRAPHICS->RenderText(wszCount.c_str(), { -250, 500 }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ .5f, .5f });

		m_cpTextFrame->GetVertex2DArr()[1].m_fPosition[0] = 0.0f;
		m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[0] = 0.0f;
	}
	else
	{
		GRAPHICS->RenderText(L"Wait to Sleep", { -275, 500 }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ 0.5f, 0.5f });

		m_cpTextFrame->GetVertex2DArr()[1].m_fPosition[0] = 0.15f;
		m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[0] = 0.15f;
	}

	m_cpTextFrame->GetRender() = true;
	m_bDrawInd = true;
}

bool CBed::IsAvailable()
{
	return (m_fCooldown < 0.0f && m_cpTheDayNight->GetDoorTimer() < 0.0f);
}

bool CBed::SaveGame()
{
	ofstream binOut(GetFilePath() + "Save_01.bin", ios_base::binary);


	TSaveData toSave;

	// - Bridge/Walls
	vector<IObject*> vpBridges = m_pObjectManager->FindObjectsWithTag("Bridge", CObjectManager::eObjectType::Static);
	for (size_t i = 0; i < vpBridges.size(); i++)
	{
		CBridge* currBridge = reinterpret_cast<CBridge*>(vpBridges[i]);
		toSave.m_vbBridges.push_back(currBridge->IsActive());
	}

	vector<IObject*> vpWalls = m_pObjectManager->FindObjectsWithTag("CrackedWall", CObjectManager::eObjectType::Static);
	for (size_t i = 0; i < vpWalls.size(); i++)
	{
		CCrackedWall* currWall = reinterpret_cast<CCrackedWall*>(vpWalls[i]);
		toSave.m_vbCrackedWalls.push_back(currWall->IsActive());
	}

	// - Player Ugrades
	CPlayer* curPlayer = reinterpret_cast<CPlayer*>(m_pObjectManager->GetPlayer());

	// 0 - Weapon | 1 - Shield | 2 - Health |
	toSave.m_Uprades.push_back(curPlayer->GetCurrentWepUpgrade());
	toSave.m_Uprades.push_back(curPlayer->GetCurrentShi());
	toSave.m_Uprades.push_back(curPlayer->GetCurrentHealthLevel());

	// - Day Counter
	toSave.m_nDayNumber = curPlayer->GetHUD()->GetCurrentDay();

	// - Inventory
	for (size_t i = 0; i < curPlayer->GetInventory()->GetTotalStock().size(); i++)
	{
		int curStock = curPlayer->GetInventory()->GetTotalStock()[i];
		toSave.m_vnInventory.push_back(curStock);
	}

	// - Map
	for (size_t i = 0; i < curPlayer->GetWorldMap()->GetFog().size(); i++)
	{
		TFog curFog = curPlayer->GetWorldMap()->GetFog()[i];
		toSave.m_vbMapFog.push_back(curFog.bIsActive);
	}

	// Storage Crate //
	for (auto iter = curPlayer->GetStorageCrate()->GetStock().begin(); iter != curPlayer->GetStorageCrate()->GetStock().end(); iter++)
	{
		OUTPUT(to_string(iter->first).c_str());
		toSave.m_nStorageCrate.push_back(iter->second);
	}

	// Exit Key //
	toSave.m_bExitKey = curPlayer->GetKeyFound();

	// Torches //
	vector<IObject*> m_cTorchList = m_pObjectManager->FindObjectsWithTag("Torch", CObjectManager::eObjectType::Static);

	for (unsigned int i = 0; i < m_cTorchList.size(); i++)
	{
		if (((CTorch*)m_cTorchList[i])->GetFlameOn())
			OUTPUT("On\n");
		else
			OUTPUT("Off\n");

		toSave.m_bTorches.push_back(((CTorch*)m_cTorchList[i])->GetFlameOn());
	}

	// Total Time //
	toSave.m_fTotalTime = curPlayer->GetTotalPlayTime();

	// Write out
	if (binOut.is_open())
	{

		int nVecSize;
		bool bReadOut;

		// Bridges
		nVecSize = (int)toSave.m_vbBridges.size();
		binOut.write((char*)&nVecSize, sizeof(int));

		for (int i = 0; i < nVecSize; i++)
		{
			bReadOut = toSave.m_vbBridges[i];
			binOut.write((char*)&bReadOut, sizeof(bool));

			//binOut.write( (char*)&toSave.m_vbBridges[i], sizeof( bool ) );
		}

		// Walls
		nVecSize = (int)toSave.m_vbCrackedWalls.size();
		binOut.write((char*)&nVecSize, sizeof(int));

		for (int i = 0; i < nVecSize; i++)
		{
			bReadOut = toSave.m_vbCrackedWalls[i];
			binOut.write((char*)&bReadOut, sizeof(bool));

			//binOut.write( (char*)&toSave.m_vbCrackedWalls[i], sizeof( bool ) );
		}

		// m_vbMapFog
		nVecSize = (int)toSave.m_vbMapFog.size();
		binOut.write((char*)&nVecSize, sizeof(int));

		for (int i = 0; i < nVecSize; i++)
		{
			bReadOut = toSave.m_vbMapFog[i];
			binOut.write((char*)&bReadOut, sizeof(bool));
			//binOut.write( (char*)&toSave.m_vbMapFog[i], sizeof( bool ) );
		}

		// Inventory
		nVecSize = (int)toSave.m_vnInventory.size();
		binOut.write((char*)&nVecSize, sizeof(int));

		for (int i = 0; i < nVecSize; i++)
		{
			binOut.write((char*)&toSave.m_vnInventory[i], sizeof(int));
		}

		// Upgrades - messed up values
		nVecSize = (int)toSave.m_Uprades.size();

		for (int i = 0; i < nVecSize; i++)
		{
			binOut.write((char*)&toSave.m_Uprades[i], sizeof(int));
		}

		// Days
		binOut.write((char*)&toSave.m_nDayNumber, sizeof(int));

		// Storage Crate //
		for (unsigned int i = 0; i < toSave.m_nStorageCrate.size(); i++)
			binOut.write((char*)&toSave.m_nStorageCrate[i], sizeof(int));

		// Exit Key //
		char key;

		if (toSave.m_bExitKey)
			key = 1;
		else
			key = 0;

		binOut.write((char*)&key, sizeof(char));

		// Torches //
		for (unsigned int i = 0; i < toSave.m_bTorches.size(); i++)
		{
			char state;
			if (toSave.m_bTorches[i])
				state = 1;
			else
				state = 0;

			binOut.write((char*)&state, sizeof(char));
		}

		// Total Time //
		binOut.write((char*)&toSave.m_fTotalTime, sizeof(float));


		binOut.close();
	}
	return true;
}

std::string CBed::GetFilePath()
{
	HRESULT hr;
	std::ostringstream stringStream;
	char path[MAX_PATH];
	LPWSTR wszPath = NULL;
	size_t   size;

	// Get the path to the app data folder
	hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, 0, &wszPath);

	// Convert from LPWSTR to char[]
	wcstombs_s(&size, path, MAX_PATH, wszPath, MAX_PATH);

	// Convert char types
	if (hr == S_OK)
		stringStream << path;
	std::string filePath = stringStream.str();

	// Add the company and game information
	filePath += "\\" + std::string("Failbot") + "\\" + std::string("The Exile") + "\\" + std::string("Save") + "\\";

	// Create our directory
	SHCreateDirectoryExA(NULL, filePath.c_str(), 0);

	return filePath;
}

void CBed::AddToRenderer()
{
	if (m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	if (m_cpTextFrame != nullptr)
		GRAPHICS->AddRenderMesh(m_cpTextFrame);

	if (m_cpOverlay != nullptr)
		GRAPHICS->AddRenderMesh(m_cpOverlay);

	if (m_cpControllerPopUp != nullptr)
		GRAPHICS->AddRenderMesh(m_cpControllerPopUp);

}
