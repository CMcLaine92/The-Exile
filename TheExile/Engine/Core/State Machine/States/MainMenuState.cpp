/***********************************************
* Filename:  		MainMenuState.cpp
* Date:      		8/18/2015
* Mod. Date: 		9/18/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:   		This is the game state that takes
*					care of the main menu
************************************************/

#include "MainMenuState.h"
#include "../../MathWrapper.inl"
#include "../StateMachine.h"
#include "../../../Renderer/Renderer.h"
#include "../../../Renderer/HUDMesh.h"
#include "../../Input/InputManager.h"
#include "../../../Asset Manager/AssetManager.h"
#include "../../../Object Manager/ObjectManager.h"
#include "../../../../Game/Source/Camera/Camera.h"
#include "../../../Object Manager/FloorObject.h"
#include "../../../../Engine/Audio/Source/Wwise_IDs.h"
#include "../../../Particles/ParticleManager.h"
#include "../../../Particles/Emitter.h"
#include "../../../Renderer/Lights.h"
#include "../../../../Game/Source/Base/Maze.h"
#include "../../../../Engine/Object Manager/PrefabObject.h"

#include <fstream>
#include <sstream>
#include <ShlObj.h>

CMainMenuState::eMenuFaces CMainMenuState::m_cPreviousFace = CMainMenuState::eMenuFaces::ePlay_Face;



/*	PRIVATE FUNCTIONS	*/

/*****************************************************************
* CheckRotatingFace()	Checks the cube to see if its currently rotating or not
*
* Ins:					None
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    08/19/2015
* Mod. Initials:	    NH
*****************************************************************/
bool CMainMenuState::CheckRotatingFace()
{
	//Check if its currently rotating
	bool LockInput = false;
	for (unsigned int i = 0; i < ROTATION_DIRECTIONS; i++)
	{
		if (m_bRotateFace[i] == true)
		{
			LockInput = true;
		}
	}

	//check allow input for directions
	bool allowInput = true;
	if (LockInput == true)
	{
		m_fInputTimer += DELTA_TIME();

		if (m_fInputTimer > 0.0f && m_fInputTimer <= 1.0f)
		{
			allowInput = false;
		}
		else//the cube is not rotating
		{
			m_fInputTimer = 0.0f;

			for (unsigned int i = 0; i < ROTATION_DIRECTIONS; i++)
			{
				m_bRotateFace[i] = false;
			}
		}
	}

	return allowInput;
}

/*****************************************************************
* CreateSkybox()		Creates the main menu skybox
*
* Ins:					None
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    09/14/2015
* Mod. Initials:	    NH
*****************************************************************/
void CMainMenuState::CreateSkybox()
{
	// SkyBox Indices Initialization //
	CMesh cpSkyBox;
	std::vector<unsigned int> nSkyIndex;

	// Front Face //
	cpSkyBox.GetIndices().push_back(1);
	cpSkyBox.GetIndices().push_back(0);
	cpSkyBox.GetIndices().push_back(3);

	cpSkyBox.GetIndices().push_back(1);
	cpSkyBox.GetIndices().push_back(3);
	cpSkyBox.GetIndices().push_back(2);

	// Left Face //
	cpSkyBox.GetIndices().push_back(0);
	cpSkyBox.GetIndices().push_back(4);
	cpSkyBox.GetIndices().push_back(7);

	cpSkyBox.GetIndices().push_back(0);
	cpSkyBox.GetIndices().push_back(7);
	cpSkyBox.GetIndices().push_back(3);

	// Right Face //
	cpSkyBox.GetIndices().push_back(5);
	cpSkyBox.GetIndices().push_back(1);
	cpSkyBox.GetIndices().push_back(2);

	cpSkyBox.GetIndices().push_back(5);
	cpSkyBox.GetIndices().push_back(2);
	cpSkyBox.GetIndices().push_back(6);

	// Back Face //
	cpSkyBox.GetIndices().push_back(4);
	cpSkyBox.GetIndices().push_back(5);
	cpSkyBox.GetIndices().push_back(6);

	cpSkyBox.GetIndices().push_back(4);
	cpSkyBox.GetIndices().push_back(6);
	cpSkyBox.GetIndices().push_back(7);

	// Top Face //
	cpSkyBox.GetIndices().push_back(0);
	cpSkyBox.GetIndices().push_back(1);
	cpSkyBox.GetIndices().push_back(5);

	cpSkyBox.GetIndices().push_back(0);
	cpSkyBox.GetIndices().push_back(5);
	cpSkyBox.GetIndices().push_back(4);

	// Bottom Face //
	cpSkyBox.GetIndices().push_back(7);
	cpSkyBox.GetIndices().push_back(6);
	cpSkyBox.GetIndices().push_back(2);

	cpSkyBox.GetIndices().push_back(7);
	cpSkyBox.GetIndices().push_back(2);
	cpSkyBox.GetIndices().push_back(3);

	// SkyBox Vertices Initialization //
	std::vector<TVertex3D> tSkyVertex;

	cpSkyBox.GetVertices().push_back({ { -1, 1, -1, 1 } }); // 0 // Front Left  Top
	cpSkyBox.GetVertices().push_back({ { 1, 1, -1, 1 } }); // 1 // Front Right Top
	cpSkyBox.GetVertices().push_back({ { 1, -1, -1, 1 } }); // 2 // Front Right Bottom
	cpSkyBox.GetVertices().push_back({ { -1, -1, -1, 1 } }); // 3 // Front Left  Bottom
	cpSkyBox.GetVertices().push_back({ { -1, 1, 1, 1 } }); // 4 // Back  Left  Top
	cpSkyBox.GetVertices().push_back({ { 1, 1, 1, 1 } }); // 5 // Back  Right Top
	cpSkyBox.GetVertices().push_back({ { 1, -1, 1, 1 } }); // 6 // Back  Right Bottom
	cpSkyBox.GetVertices().push_back({ { -1, -1, 1, 1 } }); // 7 // Back  Left  Bottom

	// Allocate RenderMesh //
	m_cpSkybox = new CRenderMesh(&cpSkyBox, GRAPHICS->GetSkyboxVS(), GRAPHICS->GetSkyboxPS());
	m_cpSkybox->SetTexture(L"../Game/Assets/Art/2D/Textures/SkyBoxNight.dds"); // Set to skybox texture
	m_cpSkybox->SetNormals(L"../Game/Assets/Art/2D/Textures/SkyBox.dds"); //Set 2nd Texture
	m_cpSkybox->GetTransformMatrix() = m_cpCamera->GetWorldMatrix(); // Snap to camera
	m_cpSkybox->GetTransparencyOverride() = 0.0f;
	// Add To Renderer //
	GRAPHICS->AddRenderMesh(m_cpSkybox);
}

void CMainMenuState::Load()
{
	ifstream fin;
	fin.open(GetFilePath() + "options.txt");

	int nMusicVolume = 10;
	int nSFXVolume = 10;
	bool bFullScreen = true;
	int nResolution = false;
	bool bControlInverse = false;

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
		bControlInverse = (line == "1");

		fin.close();
	}


	bInverse = bControlInverse;


	AudioSystemWwise::Get()->SetRTCPValue(AK::GAME_PARAMETERS::MX_VOLUME, (AkRtpcValue)(nMusicVolume * 10));
	AudioSystemWwise::Get()->SetRTCPValue(AK::GAME_PARAMETERS::FX_VOLUME, (AkRtpcValue)(nSFXVolume * 10));

	if (nResolution == 1)
		GRAPHICS->ToggleFullScreen(1280, 720);
	else if (nResolution == 2)
		GRAPHICS->ToggleFullScreen(1280, 720);
	else if (nResolution == 3)
		GRAPHICS->ToggleFullScreen(1024, 768);

	if (bFullScreen || nResolution == 1)
		GRAPHICS->ToggleFullScreen();
}

std::string CMainMenuState::GetFilePath()
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

void CMainMenuState::PlayCloudParticles()
{
	//load and offset particles
	m_unCloudParticles[eCloud1] = m_cpParticleManager->LoadEmitter("CloudParticle.xml");//far right
	m_cpParticleManager->GetEmitter(eCloud1)->GetPosition(0) = m_cpAttachObj->GetPosition()->x + 5.0f;
	m_cpParticleManager->GetEmitter(eCloud1)->GetPosition(1) = m_cpAttachObj->GetPosition()->y - CLOUDS_OFFSET_Y;
	m_cpParticleManager->GetEmitter(eCloud1)->GetPosition(2) = m_cpAttachObj->GetPosition()->z;

	m_unCloudParticles[eCloud2] = m_cpParticleManager->LoadEmitter("CloudParticle.xml");//Mid right
	m_cpParticleManager->GetEmitter(eCloud2)->GetPosition(0) = m_cpAttachObj->GetPosition()->x + 2.0f;
	m_cpParticleManager->GetEmitter(eCloud2)->GetPosition(1) = m_cpAttachObj->GetPosition()->y - CLOUDS_OFFSET_Y;
	m_cpParticleManager->GetEmitter(eCloud2)->GetPosition(2) = m_cpAttachObj->GetPosition()->z;

	m_unCloudParticles[eCloud3] = m_cpParticleManager->LoadEmitter("CloudParticle.xml");//Mid left
	m_cpParticleManager->GetEmitter(eCloud3)->GetPosition(0) = m_cpAttachObj->GetPosition()->x - 1.0f;
	m_cpParticleManager->GetEmitter(eCloud3)->GetPosition(1) = m_cpAttachObj->GetPosition()->y - CLOUDS_OFFSET_Y;
	m_cpParticleManager->GetEmitter(eCloud3)->GetPosition(2) = m_cpAttachObj->GetPosition()->z;

	m_unCloudParticles[eCloud4] = m_cpParticleManager->LoadEmitter("CloudParticle.xml");//far left
	m_cpParticleManager->GetEmitter(eCloud4)->GetPosition(0) = m_cpAttachObj->GetPosition()->x - 3.0f;
	m_cpParticleManager->GetEmitter(eCloud4)->GetPosition(1) = m_cpAttachObj->GetPosition()->y - CLOUDS_OFFSET_Y;
	m_cpParticleManager->GetEmitter(eCloud4)->GetPosition(2) = m_cpAttachObj->GetPosition()->z;

	m_unCloudParticles[eCloud5] = m_cpParticleManager->LoadEmitter("CloudParticle2.xml");//white circles(cloud 2)
	m_cpParticleManager->GetEmitter(eCloud5)->GetPosition(0) = m_cpAttachObj->GetPosition()->x + 5.0f;
	m_cpParticleManager->GetEmitter(eCloud5)->GetPosition(1) = m_cpAttachObj->GetPosition()->y - 1.5f;
	m_cpParticleManager->GetEmitter(eCloud5)->GetPosition(2) = m_cpAttachObj->GetPosition()->z;

	//play all particles
	for (unsigned int i = 0; i < eCloudCount; i++)
	{
		m_cpParticleManager->GetEmitter(i)->Stop();
		m_cpParticleManager->GetEmitter(i)->ePlay();
	}

}



/*	PUBLIC FUNCTIONS	*/

/*****************************************************************
* CMainMenuState()		The constructor for MainMenuState
*
* Ins:					None
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NH
*****************************************************************/
CMainMenuState::CMainMenuState()
{
	cpExitPrompt = nullptr;

	m_cpInput = nullptr;
	m_cpAssetManager = nullptr;
	m_cpObjectManager = nullptr;
	m_cpCamera = nullptr;
	m_tDirectLight = nullptr;
	m_cpAudio = nullptr;
	m_cpPlayPlane = nullptr;
	m_cpOptionsPlane = nullptr;
	m_cpControlsPlane = nullptr;
	m_cpCreditsPlane = nullptr;
	m_cpExitPlane = nullptr;
	m_cpAttachObj = nullptr;//used for attaching the camera too
	m_cpControllerPopUpMesh = nullptr;
	m_cpSkybox = nullptr;
	m_cpMenuControlsMesh = nullptr;

	m_fInputTimer = 0.0f;//used to check if the cube is is a rotation or not

	bExitPrompt = 0;
	m_bControllerPopUp = 0;
	m_bDCControllerCursor = 0;
	m_bControllerWasUsed = 0;
	m_bToggleControls = 0;

	Load();

	/*ZeroMemory(m_bRotateFace, sizeof(bool) * 4);
	ZeroMemory(&m_tTheCube, sizeof(m_tTheCube));*/
}

EInputResult CMainMenuState::Input()
{
	if (m_bSplashIntro)
	{
		if (m_cpInput->IsKeyPressed("Action") || m_cpInput->IsKeyPressed("Back") || m_cpInput->IsButtonPressed("Jump Action"))
		{
			m_bSplashIntro = false;
			GRAPHICS->RemoveRenderMesh(m_cSplashScreen1);
			GRAPHICS->RemoveRenderMesh(m_cSplashScreen2);
			GRAPHICS->RemoveRenderMesh(m_cSplashScreen3);
		}

		return eContinue;
	}

	bool bAllowInput = CheckRotatingFace();
	if (m_bControllerPopUp == true)
		bAllowInput = false;

	if ((m_cpInput->IsKeyPressed("Back") || m_cpInput->IsButtonPressed("Pause") || m_cpInput->IsButtonPressed("Back")) && (!bExitPrompt && m_bControllerPopUp == false && bAllowInput == true))
	{
		bExitPrompt = true;
		GRAPHICS->AddRenderMesh(cpExitPrompt);
	}
	else if ((m_cpInput->IsKeyPressed("Back") || m_cpInput->IsKeyPressed("No") || m_cpInput->IsButtonPressed("Back")) && (bExitPrompt && bAllowInput == true))
	{
		bExitPrompt = false;
		GRAPHICS->RemoveRenderMesh(cpExitPrompt);
	}

	if ((m_cpInput->IsKeyPressed("Action") || m_cpInput->IsKeyPressed("Yes") || m_cpInput->IsButtonPressed("Jump Action")) && (bExitPrompt && bAllowInput == true))
	{
		GRAPHICS->ClearScene();
		m_cpAudio->PostEvent(AK::EVENTS::MENUENTER, XMFLOAT3(0, 0, 0));
		m_bToGamePlay = false;
		return eRemove;
	}

	if (bExitPrompt == true)
	{
		bAllowInput = false;
	}

#pragma region Face Input

	switch (m_tTheCube.eCurrentFace)
	{
	case eMenuFaces::ePlay_Face:
	{
		if ((m_cpInput->IsKeyPressed("Menu Right") || m_cpInput->IsButtonPressed("Inventory") || m_cpInput->IsAxisPressed("Left JS Pos X")) && bAllowInput == true)//move right (options)
		{
			//set play plane texture to match cursor as it moves
			m_cpPlayPlane->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Menu/Menu_PlayFace_OptionsSelected.dds");
			m_cpOptionsPlane->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Menu/Menu_OptionsFace_OptionsSelected.dds");

			//start the rotation
			m_bRotateFace[eRotationDirection::eRotate_Right] = true;
			m_bRotateFace[eRotationDirection::eRotate_Left] = false;
			m_bRotateFace[eRotationDirection::eRotate_Up] = false;
			m_bRotateFace[eRotationDirection::eRotate_Down] = false;

			//set the next face
			m_tTheCube.eCurrentFace = eMenuFaces::eOptions_Face;
			m_tTheCube.unCursorMesh = eSelectedMesh::eOptions;

			//play menu rotate sound
			m_cpAudio->PostEvent(AK::EVENTS::MENUROTATE, XMFLOAT3(0, 0, 0));
		}
		else if ((m_cpInput->IsKeyPressed("Menu Left") || m_cpInput->IsButtonPressed("Map") || m_cpInput->IsAxisPressed("Left JS Neg X")) && bAllowInput == true)//move left (controls)
		{
			m_cpPlayPlane->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Menu/Menu_PlayFace_ControlsSelected.dds");
			m_cpControlsPlane->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Menu/Menu_ControlsFace_ControlsSelected.dds");

			m_bRotateFace[eRotationDirection::eRotate_Right] = false;
			m_bRotateFace[eRotationDirection::eRotate_Left] = true;
			m_bRotateFace[eRotationDirection::eRotate_Up] = false;
			m_bRotateFace[eRotationDirection::eRotate_Down] = false;

			m_tTheCube.eCurrentFace = eMenuFaces::eControls_Face;
			m_tTheCube.unCursorMesh = eSelectedMesh::eControls;

			m_cpAudio->PostEvent(AK::EVENTS::MENUROTATE, XMFLOAT3(0, 0, 0));
		}
		else if ((m_cpInput->IsKeyPressed("Menu Up") || m_cpInput->IsButtonPressed("Cursor Up") || m_cpInput->IsAxisPressed("Left JS Pos Y")) && bAllowInput == true)//move up (credits)
		{
			m_cpPlayPlane->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Menu/Menu_PlayFace_CreditsSelected.dds");
			m_cpCreditsPlane->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Menu/Menu_CreditsFace_CreditsSelected.dds");

			m_bRotateFace[eRotationDirection::eRotate_Right] = false;
			m_bRotateFace[eRotationDirection::eRotate_Left] = false;
			m_bRotateFace[eRotationDirection::eRotate_Up] = true;
			m_bRotateFace[eRotationDirection::eRotate_Down] = false;

			m_tTheCube.eCurrentFace = eMenuFaces::eCredits_Face;
			m_tTheCube.unCursorMesh = eSelectedMesh::eCredits;

			m_cpAudio->PostEvent(AK::EVENTS::MENUROTATE, XMFLOAT3(0, 0, 0));
		}
		else if ((m_cpInput->IsKeyPressed("Menu Down") || m_cpInput->IsButtonPressed("Cursor Down") || m_cpInput->IsAxisPressed("Left JS Neg Y")) && bAllowInput == true)//move down (exit)
		{
			m_cpPlayPlane->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Menu/Menu_PlayFace_ExitSelected.dds");
			m_cpExitPlane->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Menu/Menu_ExitFace_ExitSelected.dds");

			m_bRotateFace[eRotationDirection::eRotate_Right] = false;
			m_bRotateFace[eRotationDirection::eRotate_Left] = false;
			m_bRotateFace[eRotationDirection::eRotate_Up] = false;
			m_bRotateFace[eRotationDirection::eRotate_Down] = true;

			m_tTheCube.eCurrentFace = eMenuFaces::eExit_Face;
			m_tTheCube.unCursorMesh = eSelectedMesh::eExit;

			m_cpAudio->PostEvent(AK::EVENTS::MENUROTATE, XMFLOAT3(0, 0, 0));
		}
		else if ((m_cpInput->IsKeyPressed("Action") || m_cpInput->IsButtonPressed("Jump Action")) && bAllowInput == true)//enter gameplay
		{
			for (unsigned int i = 0; i < eCloudCount; i++)
				m_cpParticleManager->GetEmitter(i)->Stop();

			m_bToGamePlay = true;
			return eRemove;
		}
	}
	break;
	case eMenuFaces::eOptions_Face:
	{
		if ((m_cpInput->IsKeyPressed("Menu Left") || m_cpInput->IsButtonPressed("Map") || m_cpInput->IsAxisPressed("Left JS Neg X")) && bAllowInput == true)//move left(play)
		{
			m_cpOptionsPlane->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Menu/Menu_OptionsFace_PlaySelected.dds");
			m_cpPlayPlane->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Menu/Menu_PlayFace_PlaySelected.dds");

			m_bRotateFace[eRotationDirection::eRotate_Right] = false;
			m_bRotateFace[eRotationDirection::eRotate_Left] = true;
			m_bRotateFace[eRotationDirection::eRotate_Up] = false;
			m_bRotateFace[eRotationDirection::eRotate_Down] = false;

			m_tTheCube.eCurrentFace = eMenuFaces::ePlay_Face;
			m_tTheCube.unCursorMesh = eSelectedMesh::ePlay;

			m_cpAudio->PostEvent(AK::EVENTS::MENUROTATE, XMFLOAT3(0, 0, 0));
		}
		else if ((m_cpInput->IsKeyPressed("Action") || m_cpInput->IsButtonPressed("Jump Action")) && bAllowInput == true)//enter
		{
			for (unsigned int i = 0; i < eCloudCount; i++)

				m_cpParticleManager->GetEmitter(i)->Stop();
			GRAPHICS->ClearScene();
			CStateMachine::GetInstance()->PushState(eOPTIONS);
		}
	}
	break;
	case eMenuFaces::eControls_Face:
	{
		if ((m_cpInput->IsKeyPressed("Menu Right") || m_cpInput->IsButtonPressed("Inventory") || m_cpInput->IsAxisPressed("Left JS Pos X")) && bAllowInput == true)//move right(play)
		{
			m_cpControlsPlane->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Menu/Menu_ControlsFace_PlaySelected.dds");
			m_cpPlayPlane->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Menu/Menu_PlayFace_PlaySelected.dds");

			m_bRotateFace[eRotationDirection::eRotate_Right] = true;
			m_bRotateFace[eRotationDirection::eRotate_Left] = false;
			m_bRotateFace[eRotationDirection::eRotate_Up] = false;
			m_bRotateFace[eRotationDirection::eRotate_Down] = false;

			m_tTheCube.eCurrentFace = eMenuFaces::ePlay_Face;
			m_tTheCube.unCursorMesh = eSelectedMesh::ePlay;

			m_cpAudio->PostEvent(AK::EVENTS::MENUROTATE, XMFLOAT3(0, 0, 0));
		}
		else if ((m_cpInput->IsKeyPressed("Action") || m_cpInput->IsButtonPressed("Jump Action")) && bAllowInput == true)//enter
		{
			for (unsigned int i = 0; i < eCloudCount; i++)
				m_cpParticleManager->GetEmitter(i)->Stop();

			GRAPHICS->ClearScene();
			CStateMachine::GetInstance()->PushState(eCONTROLS);
		}
	}
	break;
	case eMenuFaces::eCredits_Face:
	{
		if ((m_cpInput->IsKeyPressed("Menu Down") || m_cpInput->IsButtonPressed("Cursor Down") || m_cpInput->IsAxisPressed("Left JS Neg Y")) && bAllowInput == true)//move down (play)
		{
			m_cpCreditsPlane->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Menu/Menu_CreditsFace_PlaySelected.dds");
			m_cpPlayPlane->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Menu/Menu_PlayFace_PlaySelected.dds");

			m_bRotateFace[eRotationDirection::eRotate_Right] = false;
			m_bRotateFace[eRotationDirection::eRotate_Left] = false;
			m_bRotateFace[eRotationDirection::eRotate_Up] = false;
			m_bRotateFace[eRotationDirection::eRotate_Down] = true;

			m_tTheCube.eCurrentFace = eMenuFaces::ePlay_Face;
			m_tTheCube.unCursorMesh = eSelectedMesh::ePlay;

			m_cpAudio->PostEvent(AK::EVENTS::MENUROTATE, XMFLOAT3(0, 0, 0));
		}
		else if ((m_cpInput->IsKeyPressed("Action") || m_cpInput->IsButtonPressed("Jump Action")) && bAllowInput == true)//enter
		{
			for (unsigned int i = 0; i < eCloudCount; i++)
				m_cpParticleManager->GetEmitter(i)->Stop();

			GRAPHICS->ClearScene();
			CStateMachine::GetInstance()->PushState(eCREDITS);
			return eAdd;
		}
	}
	break;
	case eMenuFaces::eExit_Face:
	{
		if ((m_cpInput->IsKeyPressed("Menu Up") || m_cpInput->IsButtonPressed("Cursor Up") || m_cpInput->IsAxisPressed("Left JS Pos Y")) && bAllowInput == true)//move up (play)
		{
			m_cpExitPlane->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Menu/Menu_ExitFace_PlaySelected.dds");
			m_cpPlayPlane->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Menu/Menu_PlayFace_PlaySelected.dds");

			m_bRotateFace[eRotationDirection::eRotate_Right] = false;
			m_bRotateFace[eRotationDirection::eRotate_Left] = false;
			m_bRotateFace[eRotationDirection::eRotate_Up] = true;
			m_bRotateFace[eRotationDirection::eRotate_Down] = false;

			m_tTheCube.eCurrentFace = eMenuFaces::ePlay_Face;
			m_tTheCube.unCursorMesh = eSelectedMesh::ePlay;

			m_cpAudio->PostEvent(AK::EVENTS::MENUROTATE, XMFLOAT3(0, 0, 0));
		}
		else if ((m_cpInput->IsKeyPressed("Action") || m_cpInput->IsButtonPressed("Jump Action")) && bAllowInput == true)//enter
		{
			for (unsigned int i = 0; i < eCloudCount; i++)
				m_cpParticleManager->GetEmitter(i)->Stop();

			m_bToGamePlay = false;
			return eRemove;
		}
	}
	break;
	default:
		break;
	}

#pragma endregion

	return eContinue;
}

void CMainMenuState::Enter()
{
	m_cpAssetManager = CAssetManager::GetInstance();

	m_tDirectLight = new TDirectLight
		({
			{ 1, -0.75f, 0.5f, 1 },
			{ 1, 1, 1 }
	});

	GRAPHICS->SetDirectionalLight(m_tDirectLight);

#pragma region Cube Set up

	//set up cube indicies
	for (unsigned int i = 0; i < NUMBER_OF_FACES; i++)//num faces
	{
		m_tTheCube.m_tFaces[i].cMesh.GetIndices().push_back(0);
		m_tTheCube.m_tFaces[i].cMesh.GetIndices().push_back(1);
		m_tTheCube.m_tFaces[i].cMesh.GetIndices().push_back(2);

		m_tTheCube.m_tFaces[i].cMesh.GetIndices().push_back(0);
		m_tTheCube.m_tFaces[i].cMesh.GetIndices().push_back(2);
		m_tTheCube.m_tFaces[i].cMesh.GetIndices().push_back(3);
	}

	//set start up face to play face : play cursor
	m_tTheCube.eCurrentFace = eMenuFaces::ePlay_Face;
	m_tTheCube.unCursorMesh = eSelectedMesh::ePlay;


#pragma region ePlay Face 

	m_tTheCube.m_tFaces[eMenuFaces::ePlay_Face].eFace = eMenuFaces::ePlay_Face;

	m_tTheCube.m_tFaces[eMenuFaces::ePlay_Face].cMesh.GetVertices().push_back(
	{
		{ -1, 1, -1, 1 },
		{ 0, 0, -1, 0 },
		{ 0, 0 }
	});

	m_tTheCube.m_tFaces[eMenuFaces::ePlay_Face].cMesh.GetVertices().push_back(
	{
		{ 1, 1, -1, 1 },
		{ 0, 0, -1 },
		{ 1, 0 }
	});

	m_tTheCube.m_tFaces[eMenuFaces::ePlay_Face].cMesh.GetVertices().push_back(
	{
		{ 1, -1, -1, 1 },
		{ 0, 0, -1 },
		{ 1, 1 }
	});

	m_tTheCube.m_tFaces[eMenuFaces::ePlay_Face].cMesh.GetVertices().push_back(
	{
		{ -1, -1, -1, 1 },
		{ 0, 0, -1 },
		{ 0, 1 }
	});

	m_cpPlayPlane = new CFloorObject("PlayFace");
	m_cpPlayPlane->SetRenderMesh(new CRenderMesh(&m_tTheCube.m_tFaces[eMenuFaces::ePlay_Face].cMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr,
		L"../Game/Assets/Art/2D/Menu/Menu_PlayFace_PlaySelected.dds"));//starts on play selected

#pragma endregion

#pragma region Options Face  

	m_tTheCube.m_tFaces[eMenuFaces::eOptions_Face].eFace = eMenuFaces::eOptions_Face;

	m_tTheCube.m_tFaces[eMenuFaces::eOptions_Face].cMesh.GetVertices().push_back(
	{
		{ 1, 1, -1, 1 },
		{ 1, 0, 0 },
		{ 0, 0 }
	});

	m_tTheCube.m_tFaces[eMenuFaces::eOptions_Face].cMesh.GetVertices().push_back(
	{
		{ 1, 1, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 0 }
	});

	m_tTheCube.m_tFaces[eMenuFaces::eOptions_Face].cMesh.GetVertices().push_back(
	{
		{ 1, -1, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 1 }
	});

	m_tTheCube.m_tFaces[eMenuFaces::eOptions_Face].cMesh.GetVertices().push_back(
	{
		{ 1, -1, -1, 1 },
		{ 1, 0, 0 },
		{ 0, 1 }
	});

	m_cpOptionsPlane = new CFloorObject("OptionsFace");
	m_cpOptionsPlane->SetRenderMesh(new CRenderMesh(&m_tTheCube.m_tFaces[eMenuFaces::eOptions_Face].cMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr,
		L"../Game/Assets/Art/2D/Menu/Menu_OptionsFace_PlaySelected.dds"));//starts on play selected

#pragma endregion

#pragma region Controls Face  

	m_tTheCube.m_tFaces[eMenuFaces::eControls_Face].eFace = eMenuFaces::eControls_Face;

	m_tTheCube.m_tFaces[eMenuFaces::eControls_Face].cMesh.GetVertices().push_back(
	{
		{ -1, 1, 1, 1 },
		{ -1, 0, 0 },
		{ 0, 0 }
	});

	m_tTheCube.m_tFaces[eMenuFaces::eControls_Face].cMesh.GetVertices().push_back(
	{
		{ -1, 1, -1, 1 },
		{ -1, 0, 0 },
		{ 1, 0 }
	});

	m_tTheCube.m_tFaces[eMenuFaces::eControls_Face].cMesh.GetVertices().push_back(
	{
		{ -1, -1, -1, 1 },
		{ -1, 0, 0 },
		{ 1, 1 }
	});

	m_tTheCube.m_tFaces[eMenuFaces::eControls_Face].cMesh.GetVertices().push_back(
	{
		{ -1, -1, 1, 1 },
		{ -1, 0, 0 },
		{ 0, 1 }
	});

	m_cpControlsPlane = new CFloorObject("ControlsFace");
	m_cpControlsPlane->SetRenderMesh(new CRenderMesh(&m_tTheCube.m_tFaces[eMenuFaces::eControls_Face].cMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr,
		L"../Game/Assets/Art/2D/Menu/Menu_ControlsFace_PlaySelected.dds"));//start on play selected

#pragma endregion

#pragma region Credits Face  

	m_tTheCube.m_tFaces[eMenuFaces::eCredits_Face].eFace = eMenuFaces::eCredits_Face;

	m_tTheCube.m_tFaces[eMenuFaces::eCredits_Face].cMesh.GetVertices().push_back(
	{
		{ -1, 1, 1, 1 },
		{ 0, 1, 0 },
		{ 0, 0 }
	});

	m_tTheCube.m_tFaces[eMenuFaces::eCredits_Face].cMesh.GetVertices().push_back(
	{
		{ 1, 1, 1, 1 },
		{ 0, 1, 0 },
		{ 1, 0 }
	});

	m_tTheCube.m_tFaces[eMenuFaces::eCredits_Face].cMesh.GetVertices().push_back(
	{
		{ 1, 1, -1, 1 },
		{ 0, 1, 0 },
		{ 1, 1 }
	});

	m_tTheCube.m_tFaces[eMenuFaces::eCredits_Face].cMesh.GetVertices().push_back(
	{
		{ -1, 1, -1, 1 },
		{ 0, 1, 0 },
		{ 0, 1 }
	});

	m_cpCreditsPlane = new CFloorObject("CreditsFace");
	m_cpCreditsPlane->SetRenderMesh(new CRenderMesh(&m_tTheCube.m_tFaces[eMenuFaces::eCredits_Face].cMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr,
		L"../Game/Assets/Art/2D/Menu/Menu_CreditsFace_PlaySelected.dds"));//start on play selected

#pragma endregion

#pragma region Exit Face 

	m_tTheCube.m_tFaces[eMenuFaces::eExit_Face].eFace = eMenuFaces::eExit_Face;

	m_tTheCube.m_tFaces[eMenuFaces::eExit_Face].cMesh.GetVertices().push_back(
	{
		{ -1, -1, -1, 1 },
		{ 0, -1, 0 },
		{ 0, 0 }
	});

	m_tTheCube.m_tFaces[eMenuFaces::eExit_Face].cMesh.GetVertices().push_back(
	{
		{ 1, -1, -1, 1 },
		{ 0, -1, 0 },
		{ 1, 0 }
	});

	m_tTheCube.m_tFaces[eMenuFaces::eExit_Face].cMesh.GetVertices().push_back(
	{
		{ 1, -1, 1, 1 },
		{ 0, -1, 0 },
		{ 1, 1 }
	});

	m_tTheCube.m_tFaces[eMenuFaces::eExit_Face].cMesh.GetVertices().push_back(
	{
		{ -1, -1, 1, 1 },
		{ 0, -1, 0 },
		{ 0, 1 }
	});

	m_cpExitPlane = new CFloorObject("ExitFace");
	m_cpExitPlane->SetRenderMesh(new CRenderMesh(&m_tTheCube.m_tFaces[eMenuFaces::eExit_Face].cMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr,
		L"../Game/Assets/Art/2D/Menu/Menu_ExitFace_PlaySelected.dds"));//start on play selected

#pragma endregion

	GRAPHICS->AddRenderMesh(m_cpPlayPlane->GetRenderMesh());
	GRAPHICS->AddRenderMesh(m_cpOptionsPlane->GetRenderMesh());
	GRAPHICS->AddRenderMesh(m_cpControlsPlane->GetRenderMesh());
	GRAPHICS->AddRenderMesh(m_cpCreditsPlane->GetRenderMesh());
	GRAPHICS->AddRenderMesh(m_cpExitPlane->GetRenderMesh());

#pragma endregion 

#pragma region Menu Controls Mesh

	vector<TVertex2D> tvMenuControlsVerts;
	vector<unsigned int> tvMenuControlIndicies;

	tvMenuControlIndicies.push_back(0);
	tvMenuControlIndicies.push_back(1);
	tvMenuControlIndicies.push_back(2);

	tvMenuControlIndicies.push_back(0);
	tvMenuControlIndicies.push_back(2);
	tvMenuControlIndicies.push_back(3);

	//top left
	tvMenuControlsVerts.push_back
		({
			{ -0.2f, 0.35f, -1.0f, 1 },
			{ 0, 0 }
	});

	//top right
	tvMenuControlsVerts.push_back
		({
			{ 0.2f, 0.35f, -1.0f, 1 },
			{ 1, 0 }
	});

	//bottom right
	tvMenuControlsVerts.push_back
		({
			{ 0.2f, -0.35f, -1.0f, 1 },
			{ 1, 1 }
	});

	//bottom left
	tvMenuControlsVerts.push_back
		({
			{ -0.2f, -0.35f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_cpMenuControlsMesh = new CHUDMesh(tvMenuControlIndicies, tvMenuControlsVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/MenuControls.dds");
	m_cpMenuControlsMesh->GetPositionMatrix() =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};

	m_cpMenuControlsMesh->GetTransparent() = true;

	TranslateLocalX(m_cpMenuControlsMesh->GetTransformMatrix(), 0.8f);
	TranslateLocalY(m_cpMenuControlsMesh->GetTransformMatrix(), -0.5f);

	GRAPHICS->AddRenderMesh(m_cpMenuControlsMesh);

	cpExitPrompt = new CHUDMesh(tvMenuControlIndicies, tvMenuControlsVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/ExitPrompt.dds");
	cpExitPrompt->GetTransparent() = true;

	m_bToggleControls = false;

#pragma endregion

#pragma region Controller Pop Up

	vector<TVertex2D> tvControllerVerts;
	vector<unsigned int> tvControllerIndicies;

	tvControllerIndicies.push_back(0);
	tvControllerIndicies.push_back(1);
	tvControllerIndicies.push_back(2);

	tvControllerIndicies.push_back(0);
	tvControllerIndicies.push_back(2);
	tvControllerIndicies.push_back(3);

	//top left
	tvControllerVerts.push_back
		({
			{ -0.4f, 0.3f, -1.0f, 1 },
			{ 0, 0 }
	});

	//top right
	tvControllerVerts.push_back
		({
			{ 0.4f, 0.3f, -1.0f, 1 },
			{ 1, 0 }
	});

	//bottom right
	tvControllerVerts.push_back
		({
			{ 0.4f, -0.3f, -1.0f, 1 },
			{ 1, 1 }
	});

	//bottom left
	tvControllerVerts.push_back
		({
			{ -0.4f, -0.3f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_bControllerPopUp = false;
	m_bDCControllerCursor = false;
	m_cpControllerPopUpMesh = new CHUDMesh(tvControllerIndicies, tvControllerVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/ExitPrompt.dds");
	m_cpControllerPopUpMesh->GetTransparent() = true;
	m_cpControllerPopUpMesh->GetRender() = false;
	GRAPHICS->AddRenderMesh(m_cpControllerPopUpMesh);

#pragma endregion

	m_cpInput = CInputManager::GetInstance();
	m_cpObjectManager = new CObjectManager;

	//add objects to object manager
	m_cpObjectManager->AddObject(m_cpPlayPlane, CObjectManager::eObjectType::Dynamic);
	m_cpObjectManager->AddObject(m_cpOptionsPlane, CObjectManager::eObjectType::Dynamic);
	m_cpObjectManager->AddObject(m_cpControlsPlane, CObjectManager::eObjectType::Dynamic);
	m_cpObjectManager->AddObject(m_cpCreditsPlane, CObjectManager::eObjectType::Dynamic);
	m_cpObjectManager->AddObject(m_cpExitPlane, CObjectManager::eObjectType::Dynamic);

	m_fInputTimer = 0.0f;

	for (unsigned int i = 0; i < ROTATION_DIRECTIONS; i++)
	{
		m_bRotateFace[i] = false;
	}

#pragma region Controller Registers

	m_cpInput->RegisterButton("Jump Action", eButton::AButton);
	m_cpInput->RegisterButton("Back", eButton::BButton);
	m_cpInput->RegisterButton("Interact", eButton::XButton);
	m_cpInput->RegisterButton("Health Potion", eButton::YButton);
	m_cpInput->RegisterButton("Pause", eButton::Start);
	m_cpInput->RegisterButton("Controls", eButton::Back);
	//m_cpInput->RegisterButton("Block", eButton::RightBumper);
	m_cpInput->RegisterButton("Weapon Switch", eButton::RightBumper);
	m_cpInput->RegisterButton("Inventory", eButton::DpadRight);
	m_cpInput->RegisterButton("Map", eButton::DpadLeft);
	m_cpInput->RegisterButton("Cursor Up", eButton::DpadUp);
	m_cpInput->RegisterButton("Cursor Down", eButton::DpadDown);
	m_cpInput->RegisterAxis("Left JS Neg X", "Left JS Pos X", eAxis::LeftX);
	m_cpInput->RegisterAxis("Left JS Neg Y", "Left JS Pos Y", eAxis::LeftY);
	m_cpInput->RegisterAxis("Right JS Neg X", "Right JS Pos X", eAxis::RightX);
	m_cpInput->RegisterAxis("Right JS Neg Y", "Right JS Pos Y", eAxis::RightY);
	m_cpInput->RegisterTrigger("Right Attack", eTriggers::RightTrigger);
	m_cpInput->RegisterTrigger("Left Attack", eTriggers::LeftTrigger);

	m_bControllerWasUsed = m_cpInput->GetControllerBeingUsed();

#pragma endregion

	m_cpInput->RegisterKey("Arrow Right", eKey::RightArrow);
	m_cpInput->RegisterKey("Arrow Left", eKey::LeftArrow);
	m_cpInput->RegisterKey("Arrow Up", eKey::UpArrow);
	m_cpInput->RegisterKey("Arrow Down", eKey::DownArrow);
	m_cpInput->RegisterKey("Return", eKey::escape);
	m_cpInput->RegisterKey("Store One", eKey::Z);
	m_cpInput->RegisterKey("Store All", eKey::X);
	m_cpInput->RegisterKey("Take All", eKey::M);

	m_cpInput->RegisterKey("Menu Right", eKey::RightArrow, eKey::D);
	m_cpInput->RegisterKey("Menu Left", eKey::LeftArrow, eKey::A);
	m_cpInput->RegisterKey("Menu Down", eKey::DownArrow, eKey::S);
	m_cpInput->RegisterKey("Menu Up", eKey::UpArrow, eKey::W);
	m_cpInput->RegisterKey("Action", eKey::Enter);
	m_cpInput->RegisterKey("Back", eKey::escape, eKey::Backspace);
	m_cpInput->RegisterKey("Toggle Controls", eKey::Tab);
	m_cpInput->RegisterKey("Yes", eKey::Y);
	m_cpInput->RegisterKey("No", eKey::N);
	m_cpInput->FlushKeys();

#pragma region Attach Object 

	CMesh cAttachObjMesh;

	//indicies
	cAttachObjMesh.GetIndices().push_back(0);
	cAttachObjMesh.GetIndices().push_back(1);
	cAttachObjMesh.GetIndices().push_back(2);
	cAttachObjMesh.GetIndices().push_back(0);
	cAttachObjMesh.GetIndices().push_back(2);
	cAttachObjMesh.GetIndices().push_back(3);

	//verticies
	cAttachObjMesh.GetVertices().push_back(
	{
		{ -0.5f, 0.5f, -0.5f, 1 },
		{ 0, 0, -1, 0 },
		{ 0, 0 }
	});
	cAttachObjMesh.GetVertices().push_back(
	{
		{ 0.5f, 0.5f, -0.5f, 1 },
		{ 0, 0, -1 },
		{ 1, 0 }
	});
	cAttachObjMesh.GetVertices().push_back(
	{
		{ 0.5f, -0.5f, -0.5f, 1 },
		{ 0, 0, -1 },
		{ 1, 1 }
	});
	cAttachObjMesh.GetVertices().push_back(
	{
		{ -0.5f, -0.5f, -0.5f, 1 },
		{ 0, 0, -1 },
		{ 0, 1 }
	});

	m_cpAttachObj = new CFloorObject("CameraAttachObject");
	m_cpAttachObj->SetRenderMesh(new CRenderMesh(&cAttachObjMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader()));
	m_cpAttachObj->GetRenderMesh()->GetPositionMatrix() =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, -2, 1,
	};
	m_cpAttachObj->AddCollider(new CCollider(false, Bounds::Plane, cAttachObjMesh.GetVertices(), false));

	m_cpObjectManager->AddObject(m_cpAttachObj, CObjectManager::Static);

#pragma endregion

	//camera
	m_cpCamera = new CCamera(0.0f, 0.575f, -3.5f);
	m_cpCamera->SetParent(m_cpAttachObj);
	m_cpObjectManager->AddObject(m_cpCamera, CObjectManager::Dynamic);
	m_cpAttachObj->AddChild(m_cpCamera);

	m_cpAudio = AudioSystemWwise::Get();
	m_cpAudio->PostEvent(AK::EVENTS::STOPBACKGROUND, XMFLOAT3(0, 0, 0));
	m_cpAudio->PostEvent(AK::EVENTS::MENU, XMFLOAT3(0, 0, 0));

	CreateSkybox();

	m_cpCamera->GetLocked() = true;

	// Splash Screen Setup //
	std::vector<TVertex2D> tSplashVertices;
	std::vector<unsigned int> nSplashIndices;

	nSplashIndices.push_back(0);
	nSplashIndices.push_back(1);
	nSplashIndices.push_back(2);

	nSplashIndices.push_back(0);
	nSplashIndices.push_back(2);
	nSplashIndices.push_back(3);

	tSplashVertices.push_back
	({
		{ -1.0f, 1.0f, -1.0f, 1.0f },
		{ 0, 0 }
	});

	tSplashVertices.push_back
	({
		{ 1.0f, 1.0f, -1.0f, 1.0f },
		{ 1, 0 }
	});

	tSplashVertices.push_back
	({
		{ 1.0f, -1.0f, -1.0f, 1.0f },
		{ 1, 1 }
	});

	tSplashVertices.push_back
	({
		{ -1.0f, -1.0f, -1.0f, 1.0f },
		{ 0, 1 }
	});

	m_cSplashScreen1 = new CHUDMesh(nSplashIndices, tSplashVertices, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D());
	m_cSplashScreen2 = new CHUDMesh(nSplashIndices, tSplashVertices, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D());
	m_cSplashScreen3 = new CHUDMesh(nSplashIndices, tSplashVertices, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D());

	m_cSplashScreen1->SetTexture(L"../Game/Assets/Art/2D/Textures/SplashScreen1.dds");
	m_cSplashScreen2->SetTexture(L"../Game/Assets/Art/2D/Textures/SplashScreen2.dds");
	m_cSplashScreen3->SetTexture(L"../Game/Assets/Art/2D/Textures/Black.dds");

	m_cSplashScreen1->GetTransparent() = true;
	m_cSplashScreen2->GetTransparent() = true;
	m_cSplashScreen3->GetTransparent() = true;

	m_cSplashScreen1->GetTransparencyOverride() = 1.0f;
	m_cSplashScreen2->GetTransparencyOverride() = 1.0f;
	m_cSplashScreen3->GetTransparencyOverride() = 1.0f;

	if (m_bSplashIntro)
	{
		GRAPHICS->AddRenderMesh(m_cSplashScreen3);
		GRAPHICS->AddRenderMesh(m_cSplashScreen2);
		GRAPHICS->AddRenderMesh(m_cSplashScreen1);
	}

	//particles
	m_cpParticleManager = new CParticleManager(m_cpObjectManager);
	PlayCloudParticles();
}

void CMainMenuState::Exit()
{
	GRAPHICS->ClearScene();

	SAFE_DELETE(m_cSplashScreen1);
	SAFE_DELETE(m_cSplashScreen2);
	SAFE_DELETE(m_cSplashScreen3);

	SAFE_DELETE(m_tDirectLight);
	SAFE_DELETE(m_cpCamera);
	SAFE_DELETE(cpExitPrompt);
	SAFE_DELETE(m_cpControllerPopUpMesh);
	
	SAFE_DELETE(m_cpPlayPlane);
	SAFE_DELETE(m_cpOptionsPlane);
	SAFE_DELETE(m_cpControlsPlane);
	SAFE_DELETE(m_cpCreditsPlane);
	SAFE_DELETE(m_cpExitPlane);

	SAFE_DELETE(m_cpAttachObj);
	SAFE_DELETE(m_cpSkybox);
	SAFE_DELETE(m_cpObjectManager);
	SAFE_DELETE(m_cpMenuControlsMesh);
}

/*****************************************************************
* Update()				Updates the MainMenuState
*
* Ins:					None
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NH
*****************************************************************/
void CMainMenuState::Update()
{
	// Splash Screens //
	if (m_bSplashIntro)
	{
		if (m_nSplashCount == 0)
		{
			m_fSplashTimer -= DELTA_TIME();

			if (m_fSplashTimer < SPLASH_TIME / 2.0f)
				m_cSplashScreen1->GetTransparencyOverride() -= DELTA_TIME();
			if (m_cSplashScreen1->GetTransparencyOverride() < 0.0f)
				m_cSplashScreen1->GetTransparencyOverride() = 0.0f;
			if (m_fSplashTimer < 0.0f)
			{
				m_nSplashCount++;
				m_fSplashTimer = SPLASH_TIME;
				GRAPHICS->RemoveRenderMesh(m_cSplashScreen1);
			}
		}
		else if (m_nSplashCount == 1)
		{
			m_fSplashTimer -= DELTA_TIME();

			if (m_fSplashTimer < SPLASH_TIME / 2.0f)
				m_cSplashScreen2->GetTransparencyOverride() -= DELTA_TIME();
			if (m_cSplashScreen2->GetTransparencyOverride() < 0.0f)
				m_cSplashScreen2->GetTransparencyOverride() = 0.0f;
			if (m_fSplashTimer < 0.0f)
			{
				m_bSplashIntro = false;
				GRAPHICS->RemoveRenderMesh(m_cSplashScreen2);
				GRAPHICS->RemoveRenderMesh(m_cSplashScreen3);
			}
		}
	}
	else
	{
		GRAPHICS->RenderText(L"The", { -25.0f, 850.0f }, { 1.0f, 0.8f, 0.0f, 1.0f }, { 1.0f, 1.0f }, Center);
		GRAPHICS->RenderText(L"Exile", { -25.0f, 700.0f }, { 1.0f, 0.8f, 0.0f, 1.0f }, { 2.0f, 2.0f }, Center);
	}

#pragma region Exit Prompt

	

	if (bExitPrompt)
	{
		GRAPHICS->RenderText(L"EXIT?", { -15, 250 }, { 0, 0, 0, 0 }, { 1, 1 }, Center);
		if (m_cpInput->GetControllerBeingUsed() == true)
		{
			GRAPHICS->RenderText(L"A/B", { -15, 25 }, { 0, 0, 0, 0 }, { 1, 1 }, Center);
			GRAPHICS->RenderText(L"Yes    No", { -7.5f, -125.0f }, { 0, 0, 0, 0 }, { 0.3f, 0.3f }, Center);
		}
		else
		{
			GRAPHICS->RenderText(L"Y/N", { -15, 25 }, { 0, 0, 0, 0 }, { 1, 1 }, Center);
		}
		return;
	}

#pragma endregion



#pragma region Check Cubes Rotation Status

	//check if the cube needs to be rotated or not
	unsigned int rotateDirections = 0;
	for (unsigned int i = 0; i < ROTATION_DIRECTIONS; i++)
	{
		if (m_bRotateFace[i] == true)
		{
			rotateDirections++;
		}
	}

	//the cube needs to be rotated. 
	if (rotateDirections > 0)
	{
		//GRAPHICS->ClearScene();

		//now find what way to rotate
		if (m_bRotateFace[eRotationDirection::eRotate_Right] == true)
		{
			//play face
			XMFLOAT4X4 temp = m_cpPlayPlane->GetWorldMatrix();
			RotateLocalY(temp, ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpPlayPlane)->SetWorldMatrix(temp);

			//options face
			temp = m_cpOptionsPlane->GetWorldMatrix();
			RotateLocalY(temp, ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpOptionsPlane)->SetWorldMatrix(temp);

			//controls face
			temp = m_cpControlsPlane->GetWorldMatrix();
			RotateLocalY(temp, ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpControlsPlane)->SetWorldMatrix(temp);

			//credits face
			temp = m_cpCreditsPlane->GetWorldMatrix();
			RotateLocalY(temp, ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpCreditsPlane)->SetWorldMatrix(temp);

			//exit face
			temp = m_cpExitPlane->GetWorldMatrix();
			RotateLocalY(temp, ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpExitPlane)->SetWorldMatrix(temp);
		}
		if (m_bRotateFace[eRotationDirection::eRotate_Left] == true)
		{
			//play face
			XMFLOAT4X4 temp = m_cpPlayPlane->GetWorldMatrix();
			RotateLocalY(temp, -ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpPlayPlane)->SetWorldMatrix(temp);

			//options face
			temp = m_cpOptionsPlane->GetWorldMatrix();
			RotateLocalY(temp, -ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpOptionsPlane)->SetWorldMatrix(temp);

			//controls face
			temp = m_cpControlsPlane->GetWorldMatrix();
			RotateLocalY(temp, -ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpControlsPlane)->SetWorldMatrix(temp);

			//credits face
			temp = m_cpCreditsPlane->GetWorldMatrix();
			RotateLocalY(temp, -ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpCreditsPlane)->SetWorldMatrix(temp);

			//exit face
			temp = m_cpExitPlane->GetWorldMatrix();
			RotateLocalY(temp, -ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpExitPlane)->SetWorldMatrix(temp);
		}
		if (m_bRotateFace[eRotationDirection::eRotate_Up] == true)
		{
			//play face
			XMFLOAT4X4 temp = m_cpPlayPlane->GetWorldMatrix();
			RotateLocalX(temp, -ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpPlayPlane)->SetWorldMatrix(temp);

			//options face
			temp = m_cpOptionsPlane->GetWorldMatrix();
			RotateLocalX(temp, -ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpOptionsPlane)->SetWorldMatrix(temp);

			//controls face
			temp = m_cpControlsPlane->GetWorldMatrix();
			RotateLocalX(temp, -ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpControlsPlane)->SetWorldMatrix(temp);

			//credits face
			temp = m_cpCreditsPlane->GetWorldMatrix();
			RotateLocalX(temp, -ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpCreditsPlane)->SetWorldMatrix(temp);

			//exit face
			temp = m_cpExitPlane->GetWorldMatrix();
			RotateLocalX(temp, -ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpExitPlane)->SetWorldMatrix(temp);
		}
		if (m_bRotateFace[eRotationDirection::eRotate_Down] == true)
		{
			//play face
			XMFLOAT4X4 temp = m_cpPlayPlane->GetWorldMatrix();
			RotateLocalX(temp, ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpPlayPlane)->SetWorldMatrix(temp);

			//options face
			temp = m_cpOptionsPlane->GetWorldMatrix();
			RotateLocalX(temp, ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpOptionsPlane)->SetWorldMatrix(temp);

			//controls face
			temp = m_cpControlsPlane->GetWorldMatrix();
			RotateLocalX(temp, ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpControlsPlane)->SetWorldMatrix(temp);

			//credits face
			temp = m_cpCreditsPlane->GetWorldMatrix();
			RotateLocalX(temp, ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpCreditsPlane)->SetWorldMatrix(temp);

			//exit face
			temp = m_cpExitPlane->GetWorldMatrix();
			RotateLocalX(temp, ROTATION_DEGREE * DELTA_TIME());
			((CFloorObject*)m_cpExitPlane)->SetWorldMatrix(temp);
		}
	}

#pragma endregion

	//update all objects in the scene
	m_cpObjectManager->UpdateAll();
	m_cpParticleManager->Update();

#pragma region Toggle Menu Controls

	if (!m_bSplashIntro)
	{
		if (m_cpInput->GetControllerBeingUsed() == true)
		{
			if (m_bControllerPopUp == false)
			{
				GRAPHICS->RenderText(L"   Press Y to\nToggle Controls", { 800.0f, -875.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.3f, 0.3f }, Center);
				m_cpMenuControlsMesh->SetTexture(L"../Game/Assets/Art/2D/Menu/MenuControls_Controller.dds");
			}
			else
			{
				GRAPHICS->RenderText(L"   Press Tab to\nToggle Controls", { 800.0f, -875.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.3f, 0.3f }, Center);
				m_cpMenuControlsMesh->SetTexture(L"../Game/Assets/Art/2D/Menu/MenuControls.dds");
			}

		}
		else
		{
			GRAPHICS->RenderText(L"   Press Tab to\nToggle Controls", { 800.0f, -875.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.3f, 0.3f }, Center);
			m_cpMenuControlsMesh->SetTexture(L"../Game/Assets/Art/2D/Menu/MenuControls.dds");
		}

		if (m_bControllerPopUp == false)
		{
			//toggle controls
			if (m_cpInput->IsKeyPressed("Toggle Controls") || m_cpInput->IsButtonPressed("Health Potion"))
			{
				m_bToggleControls = !m_bToggleControls;
				if (m_bToggleControls == true)
				{
					m_cpMenuControlsMesh->GetRender() = false;
				}
				else
				{
					m_cpMenuControlsMesh->GetRender() = true;
				}
			}
		}
	}

#pragma endregion

#pragma region Controller Disconnect

	if (m_cpInput->DidControllerDisconnect() == true && m_bControllerWasUsed == true)
	{
		//controlelr pop up
		m_cpControllerPopUpMesh->GetRender() = true;
		m_bControllerPopUp = true;
	}

	if (m_bControllerPopUp == true)
	{
		GRAPHICS->RenderText(L"Controller Disconnected", { 0, 250 }, { 0, 0, 0, 0 }, { 0.5f, 0.5f }, Center);

		if (m_cpInput->IsControllerConnected() == true)//controller has connected since it has been disconnected (user sat on this screen and reconnected the controller)
		{
			m_cpInput->ActivateControllerUse();

			//give option to turn on controller
			if (m_cpInput->IsKeyPressed("Menu Right") || m_cpInput->IsButtonPressed("Inventory") || m_cpInput->IsAxisPressed("Left JS Pos X"))
			{
				m_bDCControllerCursor = !m_bDCControllerCursor;
			}
			else if (m_cpInput->IsKeyPressed("Menu Left") || m_cpInput->IsButtonPressed("Map") || m_cpInput->IsAxisPressed("Left JS Neg X"))
			{
				m_bDCControllerCursor = !m_bDCControllerCursor;
			}

			GRAPHICS->RenderText(std::wstring(L"Input: " + std::wstring((m_bDCControllerCursor) ? L"Controller" : L" Keyboard")).c_str(), { -375, 50 }, { 0, 0, 0, 0 }, { 0.75f, 0.75f }, Left);

		}
		else
		{
			//only allow keyboard input. give prompt saying no controller connected
			GRAPHICS->RenderText(L"Input:  Keyboard", { -375, 50 }, { 0, 0, 0, 0 }, { 0.75f, 0.75f }, Left);
			GRAPHICS->RenderText(L"No Controller Connected", { 0, -50 }, { 0, 0, 0, 0 }, { 0.4f, 0.4f }, Center);

			m_bDCControllerCursor = false;
		}


		if (m_bDCControllerCursor == true)//controller is on
		{
			GRAPHICS->RenderText(L"Press A to Continue", { -375, -225 }, { 0, 0, 0, 0 }, { 0.3f, 0.3f }, Left);
			m_cpInput->ActivateControllerUse();
		}
		else
		{
			GRAPHICS->RenderText(L"Press Enter to Continue", { -375, -225 }, { 0, 0, 0, 0 }, { 0.3f, 0.3f }, Left);
			m_cpInput->DeactivateControllerUse();
		}

		if (m_cpInput->IsKeyPressed("Action") || m_cpInput->IsButtonPressed("Jump Action"))
		{
			m_bControllerPopUp = false;
			m_cpControllerPopUpMesh->GetRender() = false;

			if (m_bDCControllerCursor == false)
			{
				m_bControllerWasUsed = false;
			}
			else
			{
				m_bControllerWasUsed = true;
			}
		}

	}

#pragma endregion

}

/*****************************************************************
* ReInitialize()		Resets all objects to the renderer to be drawn
*
* Ins:					None
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NH
*****************************************************************/
void CMainMenuState::ReInitialize()
{
	m_bSplashIntro = false;

	m_cpAudio->PostEvent(AK::EVENTS::STOPBACKGROUND, XMFLOAT3(0, 0, 0));
	m_cpAudio->PostEvent(AK::EVENTS::MENU, XMFLOAT3(0, 0, 0));

	GRAPHICS->AddRenderMesh(m_cpPlayPlane->GetRenderMesh());
	GRAPHICS->AddRenderMesh(m_cpOptionsPlane->GetRenderMesh());
	GRAPHICS->AddRenderMesh(m_cpControlsPlane->GetRenderMesh());
	GRAPHICS->AddRenderMesh(m_cpCreditsPlane->GetRenderMesh());
	GRAPHICS->AddRenderMesh(m_cpExitPlane->GetRenderMesh());

	GRAPHICS->AddRenderMesh(m_cpSkybox);
	GRAPHICS->AddRenderMesh(m_cpMenuControlsMesh);
	m_bToggleControls = false;

	GRAPHICS->SetDirectionalLight(m_tDirectLight);

#pragma region Controller Disconnect Pop-Up

	m_cpControllerPopUpMesh->GetRender() = false;
	m_bControllerWasUsed = m_cpInput->GetControllerBeingUsed();
	m_bControllerPopUp = false;
	m_bDCControllerCursor = false;

	GRAPHICS->AddRenderMesh(m_cpControllerPopUpMesh);

#pragma endregion

	m_cpParticleManager = new CParticleManager(m_cpObjectManager);
	PlayCloudParticles();
}
