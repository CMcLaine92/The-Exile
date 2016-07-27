/***********************************************
* Filename:  		HowToPlayState.cpp
* Date:      		8/19/2015
* Mod. Date: 		8/20/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:   		This is the game state that informs
*					the player/user the controls
************************************************/

#include "HowToPlayState.h"
#include "../../../Object Manager/FloorObject.h"
#include "../StateMachine.h"
#include "../../Input/InputManager.h"
#include "../../../Asset Manager/AssetManager.h"
#include "../../../Renderer/HUDMesh.h"
#include "../../../Object Manager/ObjectManager.h"
#include "../../../../Game/Source/Camera/Camera.h"
#include "../../../Animation/Mesh.h"

/*****************************************************************
* CHowToPlayState()		Constructor that initialized the HotToPlayState
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
CHowToPlayState::CHowToPlayState() 
{
	m_cpMesh = nullptr;
	m_cpObjectManager = nullptr;
	m_cpCamera = nullptr;
	m_cpPlaneObj = nullptr;
	m_cpMiniControls = nullptr;
	m_cpControls = nullptr;
}

/*****************************************************************
* Update()				Updates the HotToPlayState
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
void CHowToPlayState::Enter()
{
	m_unCursor = 0;

#pragma region Plane with controls texture on it

	vector<TVertex2D> cv2DVerts;
	vector<unsigned int> nvIndicies;
	cv2DVerts.resize(4);
	nvIndicies.resize(6);

	nvIndicies[0] = 0;
	nvIndicies[1] = 1;
	nvIndicies[2] = 2;
	nvIndicies[3] = 0;
	nvIndicies[4] = 2;
	nvIndicies[5] = 3;

	cv2DVerts[0] = { { -0.9f, 0.7f, -1, 1 }, { 0, 0 } };
	cv2DVerts[1] = { { 0.9f, 0.7f, -1, 1 }, { 1, 0 } };
	cv2DVerts[2] = { { 0.9f, -0.7f, -1, 1 }, { 1, 1 } };
	cv2DVerts[3] = { { -0.9f, -0.7f, -1, 1 }, { 0, 1 } };

	//top left
	

	m_cpControls = new CHUDMesh(nvIndicies, cv2DVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/Controls.dds");
	m_cpControls->GetTransparent() = true;
	TranslateLocalY(m_cpControls->GetTransformMatrix(), -0.05f);
	GRAPHICS->AddRenderMesh(m_cpControls);


	m_cpMesh = new CMesh;
	m_cpMesh->GetIndices().push_back(0);
	m_cpMesh->GetIndices().push_back(1);
	m_cpMesh->GetIndices().push_back(2);

	m_cpMesh->GetIndices().push_back(0);
	m_cpMesh->GetIndices().push_back(2);
	m_cpMesh->GetIndices().push_back(3);

	//top left
	m_cpMesh->GetVertices().push_back
		({
			{ -1.75, 1, -1, 1 },
			{ 0, 0, -1, 0 },
			{ 0, 0 }
	});

	//top right
	m_cpMesh->GetVertices().push_back
		({
			{ 1.75, 1, -1, 1 },
			{ 0, 0, -1 },
			{ 1, 0 }
	});

	//bottom right
	m_cpMesh->GetVertices().push_back
		({
			{ 1.75, -1, -1, 1 },
			{ 0, 0, -1 },
			{ 1, 1 }
	});

	//bottom left
	m_cpMesh->GetVertices().push_back
		({
			{ -1.75, -1, -1, 1 },
			{ 0, 0, -1 },
			{ 0, 1 }
	});

	m_cpPlaneObj = new CFloorObject("Controls Camera Object");
	m_cpPlaneObj->SetRenderMesh(new CRenderMesh(m_cpMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader()));
	m_cpPlaneObj->GetWorldMatrix() =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	m_cpPlaneObj->AddCollider(new CCollider(false, Bounds::Plane, m_cpMesh->GetVertices(), false));

#pragma endregion

#pragma region Mini Controls Image

	cv2DVerts[0] = { { -0.2f, 0.1f, -1, 1 }, { 0, 0 } };
	cv2DVerts[1] = { { 0.2f, 0.1f, -1, 1 }, { 1, 0 } };
	cv2DVerts[2] = { { 0.2f, -0.1f, -1, 1 }, { 1, 1 } };
	cv2DVerts[3] = { { -0.2f, -0.1f, -1, 1 }, { 0, 1 } };
	
	//this states controls image
	m_cpMiniControls = new CHUDMesh(nvIndicies, cv2DVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/KeyboardNext.dds");
	m_cpMiniControls->GetTransparent() = true;
	TranslateLocalY(m_cpMiniControls->GetTransformMatrix(), -0.9f);
	TranslateLocalX(m_cpMiniControls->GetTransformMatrix(), 0.7f);

	GRAPHICS->AddRenderMesh(m_cpMiniControls);

#pragma endregion

#pragma region Controls Type Image

	cv2DVerts[0] = { { -0.3f, 0.15f, -1, 1 }, { 0, 0 } };
	cv2DVerts[1] = { { 0.3f, 0.15f, -1, 1 }, { 1, 0 } };
	cv2DVerts[2] = { { 0.3f, -0.15f, -1, 1 }, { 1, 1 } };
	cv2DVerts[3] = { { -0.3f, -0.15f, -1, 1 }, { 0, 1 } };

	m_cpControlsTypeImg = new CHUDMesh(nvIndicies, cv2DVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/KeyboardType.dds");
	m_cpControlsTypeImg->GetTransparent() = true;
	TranslateLocalY(m_cpControlsTypeImg->GetTransformMatrix(), 0.8f);
	GRAPHICS->AddRenderMesh(m_cpControlsTypeImg);

#pragma endregion

	//input
	m_cpInput = CInputManager::GetInstance();
	m_cpInput->RegisterKey("Escape", eKey::escape, eKey::Backspace);

	//asset manger
	m_cpAssetManager = CAssetManager::GetInstance();

	//object manger
	m_cpObjectManager = new CObjectManager();
	m_cpObjectManager->AddObject(m_cpPlaneObj, CObjectManager::Static);

	//set camera up
	m_cpCamera = new CCamera(0.0f, 0.0f, -2.5f);
	m_cpCamera->SetParent(m_cpPlaneObj);
	m_cpObjectManager->AddObject(m_cpCamera, CObjectManager::Dynamic);
	m_cpPlaneObj->AddChild(m_cpCamera);

	//audio
	m_cpAudio = m_cpAudio->Get();

	//controller 
	m_bControllerDC = false;
	m_bControllerUsed = m_cpInput->GetControllerBeingUsed();
	m_fControllerTimer = 0.0f;

	cv2DVerts.clear();
	nvIndicies.clear();

}

void CHowToPlayState::Exit()
{
	SAFE_DELETE(m_cpMesh);
	SAFE_DELETE(m_cpObjectManager);
	SAFE_DELETE(m_cpCamera);
	SAFE_DELETE(m_cpPlaneObj);
	SAFE_DELETE(m_cpMiniControls);
	SAFE_DELETE(m_cpControls);
	SAFE_DELETE(m_cpControlsTypeImg);
}

EInputResult CHowToPlayState::Input()
{
	if (m_cpInput->IsKeyPressed("Escape") || m_cpInput->IsButtonPressed("Back"))
	{
		GRAPHICS->ClearScene();
//		CStateMachine::GetInstance()->AddState(CStateMachine::eMainMenuState);
		m_cpAudio->PostEvent(AK::EVENTS::MENUENTER, XMFLOAT3(0, 0, 0));
		return eRemove;
	}
	return eContinue;
}

void CHowToPlayState::Update()
{
	m_cpObjectManager->UpdateAll();

#pragma region Mini Controls 

	if (m_cpInput->GetControllerBeingUsed() == false)
	{
		if (m_unCursor == eCursorStates::KEYBOARD_IMAGE)
		{
			m_cpMiniControls->SetTexture(L"../Game/Assets/Art/2D/Menu/KeyboardNext.dds");
		}
		else if (m_unCursor == eCursorStates::CONTROLLER_IMAGE)
		{
			m_cpMiniControls->SetTexture(L"../Game/Assets/Art/2D/Menu/KeyboardBack.dds");
		}

		GRAPHICS->RenderText(L"Escape to Return to Menu", { -950.0f, -900.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.4f, 0.4f }, Left);
	}
	else
	{
		if (m_unCursor == eCursorStates::KEYBOARD_IMAGE)
		{
			m_cpMiniControls->SetTexture(L"../Game/Assets/Art/2D/Menu/ControllerNext.dds");
		}
		else if (m_unCursor == eCursorStates::CONTROLLER_IMAGE)
		{
			m_cpMiniControls->SetTexture(L"../Game/Assets/Art/2D/Menu/ControllerBack.dds");
		}

		GRAPHICS->RenderText(L"B to Return to Menu", { -950.0f, -900.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.4f, 0.4f }, Left);
	}

#pragma endregion

#pragma region Input

	if (m_cpInput->IsKeyPressed("Menu Right") || m_cpInput->IsButtonPressed("Inventory") || m_cpInput->IsAxisPressed("Left JS Pos X"))//cursor move right
	{
		if (m_unCursor == eCursorStates::KEYBOARD_IMAGE)
		{
			m_cpControls->SetTexture(L"../Game/Assets/Art/2D/Menu/Controller_Controls.dds");
			m_cpControlsTypeImg->SetTexture(L"../Game/Assets/Art/2D/Menu/ControllerType.dds");
			m_unCursor = eCursorStates::CONTROLLER_IMAGE;
		}
	}
	else if (m_cpInput->IsKeyPressed("Menu Left") || m_cpInput->IsButtonPressed("Map") || m_cpInput->IsAxisPressed("Left JS Neg X"))//cursor move left
	{
		if (m_unCursor == eCursorStates::CONTROLLER_IMAGE)
		{
			m_cpControls->SetTexture(L"../Game/Assets/Art/2D/Menu/Controls.dds");
			m_cpControlsTypeImg->SetTexture(L"../Game/Assets/Art/2D/Menu/KeyboardType.dds");
			m_unCursor = eCursorStates::KEYBOARD_IMAGE;
		}
	}

	

#pragma endregion

#pragma region Controller Disconnect

	if (m_cpInput->DidControllerDisconnect() == true && m_bControllerUsed == true)
	{
		m_bControllerDC = true;
	}

	if (m_bControllerDC == true)
	{
		GRAPHICS->RenderText(L"Controller Disconnected", { 350.0f, -900.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.4f, 0.4f }, Left);

		m_fControllerTimer += DELTA_TIME();
		if (m_fControllerTimer > CONTROLLER_DC_TIMER)
		{
			m_fControllerTimer = 0.0f;
			m_bControllerDC = false;
			m_bControllerUsed = false;
		}
	}

#pragma endregion

}
