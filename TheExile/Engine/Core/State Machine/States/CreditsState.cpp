/***********************************************
* Filename:  		HowToPlayState.cpp
* Date:      		8/20/2015
* Mod. Date: 		9/08/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:   		This is the game state that shows
*					the game credits
************************************************/

#include "CreditsState.h"
#include "../../../Object Manager/FloorObject.h"
#include "../StateMachine.h"
#include "../../Input/InputManager.h"
#include "../../../Asset Manager/AssetManager.h"
#include "../../../Renderer/RenderMesh.h"
#include "../../../Object Manager/ObjectManager.h"
#include "../../../../Game/Source/Camera/Camera.h"
#include "../../../Renderer/HUDMesh.h"

EInputResult CCreditsState::Input()
{
	if (m_cpInput->IsKeyPressed("Back") || m_cpInput->IsButtonPressed("Back") || m_fScrollOffsetY >= CREDITS_MOVEMENT_DISTANCE)
	{
		GRAPHICS->ClearScene();
		//		CStateMachine::GetInstance()->AddState(CStateMachine::eMainMenuState);
		m_cpAudio->PostEvent(AK::EVENTS::MENUENTER, XMFLOAT3(0, 0, 0));
		return eRemove;
	}
	return eContinue;
}

void CCreditsState::Enter()
{
	vector<TVertex> tvVertecies;
	vector<unsigned int> nvIndicies;
	cMesh = new CMesh;
	cMesh->GetIndices().push_back(0);
	cMesh->GetIndices().push_back(1);
	cMesh->GetIndices().push_back(2);

	cMesh->GetIndices().push_back(0);
	cMesh->GetIndices().push_back(2);
	cMesh->GetIndices().push_back(3);

	//top left
	cMesh->GetVertices().push_back
		({
			{ -1.75, 1, -1, 1 },
			{ 0, 0, -1, 0 },
			{ 0, 0 }
	});

	//top right
	cMesh->GetVertices().push_back
		({
			{ 1.75, 1, -1, 1 },
			{ 0, 0, -1 },
			{ 1, 0 }
	});

	//bottom right
	cMesh->GetVertices().push_back
		({
			{ 1.75, -1, -1, 1 },
			{ 0, 0, -1 },
			{ 1, 1 }
	});

	//bottom left
	cMesh->GetVertices().push_back
		({
			{ -1.75, -1, -1, 1 },
			{ 0, 0, -1 },
			{ 0, 1 }
	});

	m_cpPlaneObj = new CFloorObject("CreditsPlane");
	m_cpPlaneObj->SetRenderMesh(new CRenderMesh(cMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader()));
	m_cpPlaneObj->GetRenderMesh()->GetPositionMatrix() =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
	m_cpPlaneObj->AddCollider(new CCollider(false, Bounds::Plane, cMesh->GetVertices(), false));

#pragma endregion

#pragma region Credits Background Image

	vector<TVertex2D> tvBackgroundVerts;
	vector<unsigned int> tvBackgroundIndicies;

	tvBackgroundIndicies.push_back(0);
	tvBackgroundIndicies.push_back(1);
	tvBackgroundIndicies.push_back(2);

	tvBackgroundIndicies.push_back(0);
	tvBackgroundIndicies.push_back(2);
	tvBackgroundIndicies.push_back(3);

	//top left
	tvBackgroundVerts.push_back
		({
			{ -0.95f, 0.95f, -1.0f, 1 },
			{ 0, 0 }
	});

	//top right
	tvBackgroundVerts.push_back
		({
			{ 0.95f, 0.95f, -1.0f, 1 },
			{ 1, 0 }
	});

	//bottom right
	tvBackgroundVerts.push_back
		({
			{ 0.95f, -0.95f, -1.0f, 1 },
			{ 1, 1 }
	});

	//bottom left
	tvBackgroundVerts.push_back
		({
			{ -0.95f, -0.95f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_cpBackGroundMesh = new CHUDMesh(tvBackgroundIndicies, tvBackgroundVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/GameLogo.dds");
	m_cpBackGroundMesh->GetPositionMatrix() =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};

#pragma endregion

	//input
	m_cpInput = CInputManager::GetInstance();

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

	//render objects
	GRAPHICS->AddRenderMesh(m_cpBackGroundMesh);

	//audio
	m_cpAudio = m_cpAudio->Get();

	m_fScrollOffsetY = -700.0f;
	m_fPrevScrollOffsetY = m_fScrollOffsetY;

	m_bControllerUsed = m_cpInput->GetControllerBeingUsed();
	m_bControllerDC = false;
	m_fControllerDCTimer = 0.0f;

	tvBackgroundVerts.clear();
	tvBackgroundIndicies.clear();
}
void CCreditsState::Exit()
{
	SAFE_DELETE(cMesh);
	SAFE_DELETE(m_cpObjectManager);
	SAFE_DELETE(m_cpCamera);
	SAFE_DELETE(m_cpPlaneObj);
	SAFE_DELETE(m_cpBackGroundMesh);
}

CCreditsState::CCreditsState()
{
	cMesh = nullptr;
	m_cpInput = nullptr;
	m_cpAssetManager = nullptr;
	m_cpObjectManager = nullptr;
	m_cpAudio = nullptr;
	m_cpCamera = nullptr;

	//the attach object
	m_cpPlaneObj = nullptr;

	//the background texture
	m_cpBackGroundMesh = nullptr;

	m_bControllerUsed = false;
	m_bControllerDC = false;
	m_fControllerDCTimer = 0.0f;
	m_fScrollOffsetY = 0.0f;
	m_fPrevScrollOffsetY = 0.0f;
}

void CCreditsState::Update()
{
	m_cpObjectManager->UpdateAll();

#pragma region The Credits

	m_fScrollOffsetY = m_fPrevScrollOffsetY + DELTA_TIME() * (CREDITS_TOTAL_MOVE_DISTANCE / CREDITS_TOTAL_MOVE_TIME);
	m_fPrevScrollOffsetY = m_fScrollOffsetY;

	GRAPHICS->RenderText(L"Credits", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 0.0f }, { 2, 2 }, Center);
	GRAPHICS->RenderText(L"Credits", { 0.0f, m_fScrollOffsetY }, { 1.0f, 0.8f, 0.0f, 1.0f }, { 2, 2 }, Center);
	m_fScrollOffsetY -= 500.0f;

	//Lead Programmers
	GRAPHICS->RenderText(L"Lead Programmers", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 0.0f }, { 1.25f, 1.25f }, Center);
	GRAPHICS->RenderText(L"Lead Programmers", { 0.0f, m_fScrollOffsetY }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.25f, 1.25f }, Center);
	m_fScrollOffsetY -= 250.0f;

	GRAPHICS->RenderText(L"Nick Dalton", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Nick Hryshko", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Conner McLaine", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	m_fScrollOffsetY -= 300.0f;


	//Programmers
	GRAPHICS->RenderText(L"Programmers", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 0.0f }, { 1.25f, 1.25f }, Center);
	GRAPHICS->RenderText(L"Programmers", { 0.0f, m_fScrollOffsetY }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.25f, 1.25f }, Center);
	m_fScrollOffsetY -= 250.0f;

	GRAPHICS->RenderText(L"Spencer Slaton", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Numi Stefansson", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Matthew Zanini", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	m_fScrollOffsetY -= 300.0f;


	//Producers
	GRAPHICS->RenderText(L"Producers", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 0.0f }, { 1.25f, 1.25f }, Center);
	GRAPHICS->RenderText(L"Producers", { 0.0f, m_fScrollOffsetY }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.25f, 1.25f }, Center);
	m_fScrollOffsetY -= 250.0f;

	GRAPHICS->RenderText(L"Yuri Cantrell", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Mike Davis", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Patrick Kelly", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Samuel Kuykendall", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Jim Merrell", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Alan Roman", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Derrick Kayden Snodgrass", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	m_fScrollOffsetY -= 300.0f;


	//Product Owner
	GRAPHICS->RenderText(L"Product Owner", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 0.0f }, { 1.25f, 1.25f }, Center);
	GRAPHICS->RenderText(L"Product Owner", { 0.0f, m_fScrollOffsetY }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.25f, 1.25f }, Center);
	m_fScrollOffsetY -= 250.0f;

	GRAPHICS->RenderText(L"Carlos Lugo", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	m_fScrollOffsetY -= 300.0f;


	//Studio Manager
	GRAPHICS->RenderText(L"Studio Manager", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 0.0f }, { 1.25f, 1.25f }, Center);
	GRAPHICS->RenderText(L"Studio Manager", { 0.0f, m_fScrollOffsetY }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.25f, 1.25f }, Center);
	m_fScrollOffsetY -= 300.0f;

	GRAPHICS->RenderText(L"Mike Lebo", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	m_fScrollOffsetY -= 300.0f;


	//2D Artists
	GRAPHICS->RenderText(L"2D Artists", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 0.0f }, { 1.25f, 1.25f }, Center);
	GRAPHICS->RenderText(L"2D Artists", { 0.0f, m_fScrollOffsetY }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.25f, 1.25f }, Center);
	m_fScrollOffsetY -= 250.0f;

	GRAPHICS->RenderText(L"Sabrina Bee", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Gustavo Reyes", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Austen Seeberg", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	m_fScrollOffsetY -= 300.0f;


	//3D Artists
	GRAPHICS->RenderText(L"2D Artists", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 0.0f }, { 1.25f, 1.25f }, Center);
	GRAPHICS->RenderText(L"2D Artists", { 0.0f, m_fScrollOffsetY }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.25f, 1.25f }, Center);
	m_fScrollOffsetY -= 250.0f;

	GRAPHICS->RenderText(L"Michael Barita", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Preston Dennis", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Kris Ducote", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Kenneth Gonzales", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Edgar Ocampo", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Aidan Stelzried", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	m_fScrollOffsetY -= 300.0f;


	//Special Thanks To
	GRAPHICS->RenderText(L"Special Thanks To", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 0.0f }, { 1.25f, 1.25f }, Center);
	GRAPHICS->RenderText(L"Special Thanks To", { 0.0f, m_fScrollOffsetY }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.25f, 1.25f }, Center);
	m_fScrollOffsetY -= 250.0f;

	GRAPHICS->RenderText(L"Dan Fernandez", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Jason Hinders", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Chris Marks", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Rod Moye", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Justin Murphy", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Donald Smith", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"Kyle Turco", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

	GRAPHICS->RenderText(L"John Wilchek", { 0.0f, m_fScrollOffsetY }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	m_fScrollOffsetY -= 200.0f;

#pragma endregion

	if (m_cpInput->GetControllerBeingUsed() == false)
		GRAPHICS->RenderText(L"Escape to Return to Menu", { -950.0f, -900.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.35f, 0.3f }, Left);
	else
		GRAPHICS->RenderText(L"B to Return to Menu", { -950.0f, -900.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.35f, 0.3f }, Left);

#pragma region Controller Disconnect

	if (m_cpInput->DidControllerDisconnect() == true && m_bControllerUsed == true)
	{
		m_bControllerDC = true;
	}

	if (m_bControllerDC == true)
	{
		GRAPHICS->RenderText(L"Controller Disconnected", { 350.0f, -900.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.4f, 0.4f }, Left);

		m_fControllerDCTimer += DELTA_TIME();
		if (m_fControllerDCTimer > CONTROLLER_DC_TIMER)
		{
			m_fControllerDCTimer = 0.0f;
			m_bControllerDC = false;
			m_bControllerUsed = false;
		}

	}

#pragma endregion

}


