/***********************************************
* Filename:  		OptionsState.cpp
* Date:      		8/20/2015
* Mod. Date: 		8/20/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:
************************************************/

#include "OptionState.h"
#include "../../../Object Manager/FloorObject.h"
#include "../StateMachine.h"
#include "../../Input/InputManager.h"
#include "../../../Asset Manager/AssetManager.h"
#include "../../../Renderer/RenderMesh.h"
#include "../../../Renderer/HUDMesh.h"
#include "../../../Object Manager/ObjectManager.h"
#include "../../../../Game/Source/Camera/Camera.h"
#include "../../../Asset Manager/AssetManager.h"
#include "../../../Animation/Mesh.h"
#include <fstream>
#include <sstream>
#include <ShlObj.h>

EInputResult COptionState::Input()
{
	if (m_cpInput->IsKeyPressed("Escape") || m_cpInput->IsButtonPressed("Back"))
	{
		Save();
		//CStateMachine::GetInstance()->AddState(CStateMachine::eMainMenuState);
		
		m_cpAudio->PostEvent(AK::EVENTS::MENUENTER, XMFLOAT3(0, 0, 0));
		return eRemove;
	}

	// Menu Navigation //
	if (m_cpInput->IsKeyPressed("Left") || m_cpInput->IsAxisPressed("Left JS Neg X") || m_cpInput->IsButtonPressed("Map"))
	{
		if (nCursor == 0 && nMusicVolume > 0) // Music Volume
		{
			nMusicVolume--;
			AudioSystemWwise::Get()->SetRTCPValue(AK::GAME_PARAMETERS::MX_VOLUME, (float)nMusicVolume * 10.0f);
			// Change music volume here
		}
		else if (nCursor == 1 && nSFXVolume > 0) // SFX Volume
		{
			nSFXVolume--;
			// Change sfx volume here
			AudioSystemWwise::Get()->SetRTCPValue(AK::GAME_PARAMETERS::FX_VOLUME, (float)nSFXVolume * 10.0f);
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::LOOT, { 0, 0, 0 });
		}
		else if (nCursor == 2)
		{
			bFullScreen = !bFullScreen;

			if (bFullScreen)
			{
				nResolution = 1;
				GRAPHICS->ToggleFullScreen();
			}
			else if (CUSTOM_RES_WIDTH == 1280)
			{
				nResolution = 2;
				GRAPHICS->ToggleFullScreen(1280, 720);
			}
			else if (CUSTOM_RES_HEIGHT == 1024)
			{
				nResolution = 3;
				GRAPHICS->ToggleFullScreen(1024, 768);
			}
		}
		else if (nCursor == 3 && nResolution > 1)
		{
			nResolution--;

			if (nResolution == 1)
				GRAPHICS->ToggleFullScreen();
			if (nResolution == 2)
				GRAPHICS->ToggleFullScreen(1280, 720);
		}
		else if (nCursor == 4)
		{
			bInverse = !bInverse;
			// Change controls here
		}
		else if (nCursor == 5)
		{
			if (m_cpInput->IsControllerConnected() == true)
			{
				m_bUseController = !m_bUseController;
				(m_bUseController) ? m_cpInput->ActivateControllerUse() : m_cpInput->DeactivateControllerUse();
			}
		}
	}
	else if (m_cpInput->IsKeyPressed("Right") || m_cpInput->IsAxisPressed("Left JS Pos X") || m_cpInput->IsButtonPressed("Inventory"))
	{
		if (nCursor == 0 && nMusicVolume < 10) // Music Volume
		{
			nMusicVolume++;
			// Change music volume here
			AudioSystemWwise::Get()->SetRTCPValue(AK::GAME_PARAMETERS::MX_VOLUME, (float)nMusicVolume * 10.0f);
		}
		else if (nCursor == 1 && nSFXVolume < 10) // SFX Volume
		{
			nSFXVolume++;
			// Change sfx volume here
			AudioSystemWwise::Get()->SetRTCPValue(AK::GAME_PARAMETERS::FX_VOLUME, (float)nSFXVolume * 10.0f);
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::LOOT, { 0, 0, 0 });
		}
		else if (nCursor == 2)
		{
			bFullScreen = !bFullScreen;

			if (bFullScreen)
			{
				nResolution = 1;
				GRAPHICS->ToggleFullScreen();
			}
			else if (CUSTOM_RES_WIDTH == 1280)
			{
				nResolution = 2;
				GRAPHICS->ToggleFullScreen(1280, 720);
			}
			else if (CUSTOM_RES_HEIGHT == 1024)
			{
				nResolution = 3;
				GRAPHICS->ToggleFullScreen(1024, 768);
			}
		}
		else if (nCursor == 3 && nResolution < 3)
		{
			nResolution++;

			if (nResolution == 2)
				GRAPHICS->ToggleFullScreen(1280, 720);
			else if (nResolution == 3)
				GRAPHICS->ToggleFullScreen(1024, 768);
		}
		else if (nCursor == 4)
		{
			bInverse = !bInverse;
			// Change controls here
		}
		else if (nCursor == 5)
		{
			if (m_cpInput->IsControllerConnected() == true)
			{
				m_bUseController = !m_bUseController;
				(m_bUseController) ? m_cpInput->ActivateControllerUse() : m_cpInput->DeactivateControllerUse();
			}
		}
	}
	else if (m_cpInput->IsKeyPressed("Enter") || m_cpInput->IsButtonPressed("Jump Action"))
	{
		if (nCursor == 2)
		{
			bFullScreen = !bFullScreen;

			if (bFullScreen)
			{
				nResolution = 1;
				GRAPHICS->ToggleFullScreen();
			}
			else if (CUSTOM_RES_WIDTH == 1280)
			{
				nResolution = 2;
				GRAPHICS->ToggleFullScreen(1280, 720);
			}
			else if (CUSTOM_RES_WIDTH == 1024)
			{
				nResolution = 3;
				GRAPHICS->ToggleFullScreen(1024, 768);
			}
		}
		else if (nCursor == 4)
		{
			bInverse = !bInverse;
			// Change controls here
		}
		else if (nCursor == 5)
		{
			if (m_cpInput->IsControllerConnected() == true)
			{
				m_bUseController = !m_bUseController;
				(m_bUseController) ? m_cpInput->ActivateControllerUse() : m_cpInput->DeactivateControllerUse();
			}
		}
	}
	else if (m_cpInput->IsKeyPressed("Up") || m_cpInput->IsAxisPressed("Left JS Pos Y") || m_cpInput->IsButtonPressed("Cursor Up"))
	{
		if (nCursor == 0)
			nCursor = CURSOR_MAX;
		else
			nCursor--;
	}
	else if (m_cpInput->IsKeyPressed("Down") || m_cpInput->IsAxisPressed("Left JS Neg Y") || m_cpInput->IsButtonPressed("Cursor Down"))
	{
		if (nCursor == CURSOR_MAX)
			nCursor = 0;
		else
			nCursor++;
	}
	return eContinue;
}

void COptionState::Enter()
{
#pragma region Plane Set up
	
	cMesh1 = new CMesh;
	cMesh1->GetIndices().push_back(0);
	cMesh1->GetIndices().push_back(1);
	cMesh1->GetIndices().push_back(2);
	cMesh1->GetIndices().push_back(0);
	cMesh1->GetIndices().push_back(2);
	cMesh1->GetIndices().push_back(3);

	//top left
	cMesh1->GetVertices().push_back
		({
			{ -1.75, 1, -1, 1 },
			{ 0, 0, -1, 0 },
			{ 0, 0 }
	});

	//top right
	cMesh1->GetVertices().push_back
		({
			{ 1.75, 1, -1, 1 },
			{ 0, 0, -1 },
			{ 1, 0 }
	});

	//bottom right
	cMesh1->GetVertices().push_back
		({
			{ 1.75, -1, -1, 1 },
			{ 0, 0, -1 },
			{ 1, 1 }
	});

	//bottom left
	cMesh1->GetVertices().push_back
		({
			{ -1.75, -1, -1, 1 },
			{ 0, 0, -1 },
			{ 0, 1 }
	});

	m_cpPlaneObj = new CFloorObject("CreditsPlane");
	m_cpPlaneObj->SetRenderMesh(new CRenderMesh(cMesh1, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr));
	m_cpPlaneObj->GetRenderMesh()->GetPositionMatrix() =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
	m_cpPlaneObj->AddCollider(new CCollider(false, Bounds::Plane, cMesh1->GetVertices()));

	// HUD Mesh //
	std::vector<unsigned int> nIndexArr;
	std::vector<TVertex2D> tVertexArr;
	nIndexArr.resize(6);
	tVertexArr.resize(4);

	nIndexArr[0] = 0;
	nIndexArr[1] = 1;
	nIndexArr[2] = 2;
	nIndexArr[3] = 0;
	nIndexArr[4] = 2;
	nIndexArr[5] = 3;

	
	tVertexArr[0] = { { -1, 1 }, { 0, 0 } };
	tVertexArr[1] = { { 1, 1 }, { 1, 0 } };
	tVertexArr[2] = { { 1, -1 }, { 1, 1 } };
	tVertexArr[3] = { { -1, -1 }, { 0, 1} };


	

	cBackground = new CHUDMesh(nIndexArr, tVertexArr, GRAPHICS->GetStandard2DVS(), GRAPHICS->GetStandard2DPS());
	cBackground->SetTexture(L"../Game/Assets/Art/2D/FX/MazeScreen.dds");
	GRAPHICS->AddRenderMesh(cBackground);
	

#pragma endregion

#pragma region Menu Controls Mesh

	tVertexArr[0] = { { -0.2f, 0.35f, -1.0f, 1 }, { 0, 0 } };
	tVertexArr[1] =  { { 0.2f, 0.35f, -1.0f, 1 }, { 1, 0 } };
	tVertexArr[2] =  { { 0.2f, -0.35f, -1.0f, 1 }, { 1, 1 } };
	tVertexArr[3] =  { { -0.2f, -0.35f, -1.0f, 1 }, { 0, 1 } };


	m_cpMenuControlsMesh = new CHUDMesh(nIndexArr, tVertexArr, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/MenuControls.dds");
	m_cpMenuControlsMesh->GetPositionMatrix() =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};

	TranslateLocalX(m_cpMenuControlsMesh->GetTransformMatrix(), 0.8f);
	TranslateLocalY(m_cpMenuControlsMesh->GetTransformMatrix(), -0.5f);

	GRAPHICS->AddRenderMesh(m_cpMenuControlsMesh);
	
#pragma endregion

	//input
	m_cpInput = CInputManager::GetInstance();
	m_cpInput->RegisterKey("Escape", eKey::escape, eKey::Backspace);
	m_cpInput->RegisterKey("Enter", eKey::Enter);
	m_cpInput->RegisterKey("Left", eKey::LeftArrow, eKey::A);
	m_cpInput->RegisterKey("Right", eKey::RightArrow, eKey::D);
	m_cpInput->RegisterKey("Up", eKey::UpArrow, eKey::W);
	m_cpInput->RegisterKey("Down", eKey::DownArrow, eKey::S);
	

	//asset manger
	m_cpAssetManager = CAssetManager::GetInstance();

	//object manger
	m_cpObjectManager = new CObjectManager();
	m_cpObjectManager->AddObject(m_cpPlaneObj, CObjectManager::Static);

	//set camera up
	m_cpCamera = new CCamera(0.0f, 0.0f, -2.5f);
	m_cpCamera->SetParent(m_cpPlaneObj);
	m_cpCamera->GetLocked() = true;
	m_cpObjectManager->AddObject(m_cpCamera, CObjectManager::Dynamic);
	m_cpPlaneObj->AddChild(m_cpCamera);

	//render objects
	//GRAPHICS->AddRenderMesh(m_cpPlaneObj->GetRenderMesh());

	//audio
	m_cpAudio = m_cpAudio->Get();

	// Load Harlow //
	nFontIndex = GRAPHICS->LoadFont(L"Harlow.font");

	Load(); // Load app data settings

	//controller
	m_bUseController = m_cpInput->GetControllerBeingUsed();
	m_bControllerImage = m_bUseController;

	nIndexArr.clear();
	nIndexArr.clear();

}

void COptionState::Exit()
{
	GRAPHICS->ClearScene();
	SAFE_DELETE(m_cpObjectManager);
	SAFE_DELETE(m_cpMenuControlsMesh);
	SAFE_DELETE(m_cpCamera);
	SAFE_DELETE(m_cpPlaneObj);
	SAFE_DELETE(cBackground);
	SAFE_DELETE(cMesh1);
}

COptionState::COptionState()
{
	m_cpObjectManager = nullptr;
	m_cpCamera = nullptr;
	m_cpPlaneObj = nullptr;
	cBackground = nullptr;
	m_cpMenuControlsMesh = nullptr;
	cMesh1 = nullptr;
	nCursor = 0;
	nFontIndex = 0;
	nMusicVolume = 0;
	nSFXVolume = 0;
	 bFullScreen = true;
	 m_bUseController = 0;
}

void COptionState::Update()
{
	//update all objects in this scene
	m_cpObjectManager->UpdateAll();

	if (WINDOW_WIDTH == 1920) // Changed via fullscreen button
		nResolution = 1;
	else if (WINDOW_WIDTH == 1280)
		nResolution = 2;
	else if (WINDOW_WIDTH == 1024)
		nResolution = 3;

	if (WINDOW_WIDTH == SCREEN_WIDTH)
		bFullScreen = true;
	else
		bFullScreen = false;

	// Draw Options //
	GRAPHICS->RenderText(L"Options Menu!", { 0, 800 }, { 1, 1, 1, 1 }, { 1, 1 }, Center, nFontIndex);

	float4 fSelected = { 0, 1, 1, 1 }; // Teal
	float4 fUnselected = { 1, 1, 1, 1 }; // White

	if (nCursor == 0) GRAPHICS->RenderText(std::wstring(L"Music Volume: " + std::to_wstring(nMusicVolume)).c_str(), { -950, 600 }, { 1, 1, 1, 0 }, { 1, 1 }, Left, nFontIndex);
	GRAPHICS->RenderText(std::wstring(L"Music Volume: " + std::to_wstring(nMusicVolume)).c_str(), { -950, 600 }, (nCursor == 0) ? fSelected : fUnselected, { 1, 1 }, Left, nFontIndex);

	if (nCursor == 1) GRAPHICS->RenderText(std::wstring(L"SFX Volume: " + std::to_wstring(nSFXVolume)).c_str(), { -950, 400 }, { 1, 1, 1, 0 }, { 1, 1 }, Left, nFontIndex);
	GRAPHICS->RenderText(std::wstring(L"SFX Volume: " + std::to_wstring(nSFXVolume)).c_str(), { -950, 400 }, (nCursor == 1) ? fSelected : fUnselected, { 1, 1 }, Left, nFontIndex);

	if (nCursor == 2) GRAPHICS->RenderText(std::wstring(L"Full Screen: " + std::wstring((bFullScreen) ? L"True" : L"False")).c_str(), { -950, 200 }, { 1, 1, 1, 0 }, { 1, 1 }, Left, nFontIndex);
	GRAPHICS->RenderText(std::wstring(L"Full Screen: " + std::wstring((bFullScreen) ? L"True" : L"False")).c_str(), { -950, 200 }, (nCursor == 2) ? fSelected : fUnselected, { 1, 1 }, Left, nFontIndex);

	if (nResolution == 1)
	{
		if (nCursor == 3)
			GRAPHICS->RenderText(L"Resolution: 1920x1080", { -950, 0 }, { 1, 1, 1, 0 }, { 1, 1 }, Left, nFontIndex);
		GRAPHICS->RenderText(L"Resolution: 1920x1080", { -950, 0 }, (nCursor == 3) ? fSelected : fUnselected, { 1, 1 }, Left, nFontIndex);
	}
	else if (nResolution == 2)
	{
		if (nCursor == 3)
			GRAPHICS->RenderText(L"Resolution: 1280x720", { -950, 0 }, { 1, 1, 1, 0 }, { 1, 1 }, Left, nFontIndex);
		GRAPHICS->RenderText(L"Resolution: 1280x720", { -950, 0 }, (nCursor == 3) ? fSelected : fUnselected, { 1, 1 }, Left, nFontIndex);
	}
	else if (nResolution == 3)
	{
		if (nCursor == 3)
			GRAPHICS->RenderText(L"Resolution: 1024x768", { -950, 0 }, { 1, 1, 1, 0 }, { 1, 1 }, Left, nFontIndex);
		GRAPHICS->RenderText(L"Resolution: 1024x768", { -950, 0 }, (nCursor == 3) ? fSelected : fUnselected, { 1, 1 }, Left, nFontIndex);
	}

	if (nCursor == 4) GRAPHICS->RenderText(std::wstring(L"Inverse Controls: " + std::wstring((bInverse) ? L"True" : L"False")).c_str(), { -950, -200 }, { 1, 1, 1, 0 }, { 1, 1 }, Left, nFontIndex);
	GRAPHICS->RenderText(std::wstring(L"Inverse Controls: " + std::wstring((bInverse) ? L"True" : L"False")).c_str(), { -950, -200 }, (nCursor == 4) ? fSelected : fUnselected, { 1, 1 }, Left, nFontIndex);
	
	//controller option
	if (m_cpInput->IsControllerConnected() == true)
	{
		GRAPHICS->RenderText(std::wstring(L"Input: " + std::wstring((m_bUseController) ? L"Controller" : L"Keyboard")).c_str(), { -950, -400 }, (nCursor == 4) ? fSelected : fUnselected, { 1, 1 }, Left, nFontIndex);
	}
	else
	{
		GRAPHICS->RenderText(L"Input: Keyboard", { -950, -400 }, (nCursor == 5) ? fSelected : fUnselected, { 1, 1 }, Left, nFontIndex);
		GRAPHICS->RenderText(L"No Controller Connected", { -300, -450 }, (nCursor == 5) ? fSelected : fUnselected, { 0.5f, 0.5f }, Left, nFontIndex);
	}
	
	if (m_bUseController == true && m_cpInput->IsControllerConnected() == true)
		GRAPHICS->RenderText(L"B to Go Back", { -950.0f, -900.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.35f, 0.3f }, Left);
	else
		GRAPHICS->RenderText(L"Escape to Go Back", { -950.0f, -900.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.35f, 0.3f }, Left);

	if (m_cpInput->GetControllerBeingUsed() == true && m_bControllerImage == false && m_bUseController == true)
	{
		m_bControllerImage = true;
		m_cpMenuControlsMesh->SetTexture(L"../Game/Assets/Art/2D/Menu/MenuControls_Controller.dds");
	}
	else if (m_cpInput->GetControllerBeingUsed() == false && m_bControllerImage == true && m_bUseController == false)
	{
		m_bControllerImage = false;
		m_cpMenuControlsMesh->SetTexture(L"../Game/Assets/Art/2D/Menu/MenuControls.dds");
	}

}


void COptionState::Load()
{
	ifstream fin;
	fin.open(GetFilePath() + "options.txt");

	if (fin.is_open())
	{
		string line;

		// Music Volume //
		getline(fin, line);
		nMusicVolume = atoi(line.c_str());

		// SFX Volume //
		getline(fin, line);
		nSFXVolume = atoi(line.c_str());

		// Full Screen //
		getline(fin, line);
		bFullScreen = (line == "1");

		// Resolution //
		getline(fin, line);
		nResolution = atoi(line.c_str());

		// Control Inverse //
		getline(fin, line);
		bInverse = (line == "1");

		fin.close();
	}

	AudioSystemWwise::Get()->SetRTCPValue(AK::GAME_PARAMETERS::MX_VOLUME, (float)nMusicVolume * 10.0f);
	AudioSystemWwise::Get()->SetRTCPValue(AK::GAME_PARAMETERS::FX_VOLUME, (float)nSFXVolume * 10.0f);
}

void COptionState::Save()
{
	ofstream fout;
	fout.open(GetFilePath() + "options.txt");

	if (fout.is_open())
	{
		fout << nMusicVolume << endl;
		fout << nSFXVolume << endl;
		fout << ((bFullScreen) ? "1" : "0") << endl;
		fout << nResolution << endl;
		fout << ((bInverse) ? "1" : "0") << endl;
		fout.close();
	}
}

std::string COptionState::GetFilePath()
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
	filePath += "\\" + std::string("Failbot") + "\\" + std::string("The Exile") + "\\";

	// Create our directory
	SHCreateDirectoryExA(NULL, filePath.c_str(), 0);

	return filePath;
}
