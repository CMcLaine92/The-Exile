/***********************************************
* Filename:  		GameplayState.cpp
* Date:      		8/12/2015
* Mod. Date: 		8/14/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:   		This is the game state where all
*					gameplay takes place
************************************************/

#include "GameplayState.h"
#include "../../../../Game/Source/Agents/SpawnControl.h"
#include "../../Messages/Message.h"
#include "../../Messages/MessageManager.h"
#include "../../../Asset Manager/LevelLoader.h"
#include "../../Input/InputManager.h"
#include "../../Debug Console/DebugConsole.h"
#include "../../../Object Manager/ObjectManager.h"
#include "../../../Object Manager/PrefabObject.h"
#include "../../../Object Manager/ExitDoorObject.h"

#include "../../../Asset Manager/AssetManager.h"
#include "../../../../Game/Source/Player/Player.h"
#include "../../../../Game/Source/Agents/Skeleton/Skeleton.h"
#include "../../../../Game/Source/Camera/Camera.h"
#include "../../../Animation/AnimationManager.h"
#include "../../../Animation/Animation.h"
#include "../StateMachine.h"
#include "../../Collision/Capsule.h"
#include "../../Collision/AABB.h"
#include "../../Messages/EnemySpawnMessage.h"
#include "../../Messages/WinConditionMessage.h"
#include "../../Messages/AddObjectMessage.h"
#include "../../Messages/RemoveObjectMessage.h"
#include "../../Messages/DestroyBushMessage.h"
#include "../../Messages/ReSpawnBushMessage.h"
#include "../../../../Game/Source/Agents/Skeleton/skeletongroup.h"

#include "../../../Object Manager/MinotaurSpawn.h"

#include <iostream>
#include "../../../Particles/ParticleManager.h"
#include "../../../Particles/Emitter.h"
#include "../../../../Game/Source/Traps/SpikeTrap.h"
#include "../../../../Game/Source/Traps/SpinningBlade.h"
#include "../../../../Game/Source/Traps/FireTrap.h"
#include "../../../Renderer/RenderMesh.h"
#include "../../../Renderer/HUDMesh.h"
#include "../../../Renderer/Lights.h"
#include "../../../AI/NavGraphManager.h"
#include "../../../AI/CNavGraph.h"
#include "../../../../Game/Source/HUD/HUD.h"
#include "../../../Object Manager/DoorObject.h"
#include "../../../../Game/Source/Environment/DayNight.h"
#include "../../../Object Manager/WaypointObject.h"
#include "../../../../Game/Source/Environment/Bridge.h"
#include "../../../../Game/Source/Environment/CrackedWall.h"
#include "../../../Object Manager/PitObject.h"
#include "../../../../Game/Source/Environment/BedObject.h"
#include "../../../../Game/Source/Base/Crafting.h"
#include "../../../Renderer/SpritesheetMesh.h"
#include <thread>
#include "../../../../Game/Source/Environment/Tree.h"
#include "../../../Cutscene/CutsceneManager.h"
#include "../../../../Game/Source/Environment/IChest.h"
#include "../../../../Game/Source/Base/Maze.h"
#include "../../../../Game/Source/Player/Player.h"
#include "../../../../Game/Source/HUD/WorldMap.h"
#include "../../../../Game/Source/Base/Inventory.h"
#include "../../../../Game/Source/Base/StorageCrate.h"
#include "../../../../Game/Source/Environment/ExitKey.h"

#define NUM_F1_MENUS 4
CGameplayState* CGameplayState::m_cpGameState = nullptr;

////Forward Declaration
//void LoadingThread(TThreadData* tThreadData);



/*	PRIVATE FUNCTIONS	*/

/*****************************************************************
* MessageProc()			The message callback function.
*
* Ins:					pMsg*
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    09/17/2015
* Mod. Initials:	    NH
*****************************************************************/
void CGameplayState::MessageProc(const CMessage* pMsg)
{
	switch (pMsg->GetMessageID())
	{
	case MessageID::MSG_NONE:
	{

	}
	break;
	case MessageID::MSG_START_DAY:
	{

		std::vector<IObject*> vDoors = m_cpGameState->m_cpObjectManager->FindObjectsWithTag("Door", CObjectManager::eObjectType::Static);
		for (unsigned int i = 0; i < vDoors.size(); i++)
		{
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::DOORSLIDE, vDoors[i]);
			m_cpGameState->m_pcDayNight->AddDoors(i, vDoors[i]);
			reinterpret_cast<CDoorObject*>(vDoors[i])->SetMoveTimer(DOOR_MOVE_TIME);
		}
		m_cpGameState->m_pcDayNight->SetTimer(DOOR_MOVE_TIME - .2f);
		//Update Skybox 
		//	m_cpGameState->m_cpSkybox->SetTexture( L"../Game/Assets/Art/2D/Textures/SkyBox.dds" );

		((CPlayer*)m_cpGameState->m_cpThePlayer)->GetHUD()->IncrementDayCounter();
	}
	break;
	case MessageID::MSG_START_NIGHT:
	{
		std::vector<IObject*> vDoors = m_cpGameState->m_cpObjectManager->FindObjectsWithTag("Door", CObjectManager::eObjectType::Static);
		for (unsigned int i = 0; i < vDoors.size(); i++)
		{
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::DOORSLIDE, vDoors[i]);
			m_cpGameState->m_pcDayNight->AddDoors(i, vDoors[i]);
			reinterpret_cast<CDoorObject*>(vDoors[i])->SetMoveTimer(DOOR_MOVE_TIME);
		}
		m_cpGameState->m_pcDayNight->SetTimer(DOOR_MOVE_TIME - .2f);
	}
	break;
	case MessageID::MSG_OPEN_DOOR:
	{

	}
	break;
	case MessageID::MSG_CLOSE_DOOR:
	{

	}
	break;
	case MessageID::MSG_WIN_GAME:
	{
		m_cpGameState->m_bGameOver = true;
		cout << "Game Over\n";
	}
	break;
	case MessageID::MSG_ADD_OBJECT:
	{
		const CAddObjectMessage* addObject = reinterpret_cast<const CAddObjectMessage*>(pMsg);
		m_cpGameState->m_cpObjectManager->AddObject(addObject->GetTheObject(), addObject->GetTheObjectType());

	}
	break;
	case MessageID::MSG_REMOVE_OBJECT:
	{
		const CRemoveObjectMessage* removeObject = reinterpret_cast<const CRemoveObjectMessage*>(pMsg);

		m_cpGameState->m_cpObjectManager->RemoveObject(removeObject->GetTheObject(), removeObject->GetTheObjectType());

		if (removeObject->GetObjectDelete() == true && removeObject->GetTheObject())
			delete removeObject->GetTheObject();
	}
	break;
	case MessageID::MSG_EXITKEY_FOUND:
	{
		for (unsigned int i = 0; i < m_cpGameState->m_vpExitKeys.size(); i++)
		{
			m_cpGameState->m_vpExitKeys[i]->DeAactive();

			m_cpGameState->m_cpObjectManager->RemoveObject(m_cpGameState->m_vpExitKeys[i], CObjectManager::eObjectType::Static);
			//SAFE_DELETE(m_cpGameState->m_vpExitKeys[i]);
		}
	}
	break;
	default:
		break;
	}

}

/*****************************************************************
* LoadingGraphics()		Multithreaded Loading Thread display
*
* Ins:					None
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    09/15/2015
* Mod. Initials:	    MZ
*****************************************************************/
void CGameplayState::LoadingGraphics()
{

}

EInputResult CGameplayState::Input()
{
	if (m_bGameOver == false && m_cpCutsceneManager->IsCutsceneActive() == false)
	{
#pragma region Controls Pop-Up

		//in game controls pop-up
		if (m_cpInputManager->IsKeyPressed("Controls") || m_cpInputManager->IsButtonPressed("Controls"))
		{
			if (m_bPaused == true)
			{
				m_fPausedInputTimer = 0.0f;
				m_bPaused = false;
				m_cpTextFrame->GetRender() = false;
			}

			CInventory* cInv = reinterpret_cast<CPlayer*>(m_cpThePlayer)->GetInventory();
			CWorldMap* cMap = reinterpret_cast<CPlayer*>(m_cpThePlayer)->GetWorldMap();
			cMap->Close();
			cInv->Close();
			//m_cpCraftingTable->Close();

			m_bControlsON = !m_bControlsON;
			if (m_bControlsON == true)
			{
				m_nF1CurrentMenu = 0;
				if (m_cpInputManager->GetControllerBeingUsed() == true)
					m_vszF1Textures[m_nF1CurrentMenu] = L"../Game/Assets/Art/2D/Menu/Controller_Controls.dds";
				else
					m_vszF1Textures[m_nF1CurrentMenu] = L"../Game/Assets/Art/2D/Menu/Controls.dds";

				m_cpControlsMesh->SetTexture(m_vszF1Textures[m_nF1CurrentMenu].c_str());
				m_cpControlsMesh->GetRender() = true;
				m_cpRightArrowMesh->GetRender() = true;
				m_cpLeftArrowMesh->GetRender() = false;

			}
			else
			{
				m_cpControlsMesh->GetRender() = false;
				m_cpRightArrowMesh->GetRender() = false;
				m_cpLeftArrowMesh->GetRender() = false;
			}
		}

		if (m_bControlsON)
		{
			GRAPHICS->RenderText(L"Paused", { 0.0f, 750.0f }, { 1.0f, 0.8f, 0.0f, 1.0f }, { 2, 2 }, Center);
			if (m_cpInputManager->GetControllerBeingUsed() == false)
				GRAPHICS->RenderText(L"Press F1 to Resume Game", { 0.0f, -650.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.6f, 0.7f }, Center);
			else
				GRAPHICS->RenderText(L"Press Back to Resume Game", { 0.0f, -650.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.6f, 0.7f }, Center);

			m_cpTextFrame->GetRender() = false;



			if (m_cpInputManager->IsKeyPressed("MenuRight") || m_cpInputManager->IsButtonPressed("Inventory") || m_cpInputManager->IsAxisPressed("Left JS Pos X"))
			{
				m_nF1CurrentMenu++;
				if (m_nF1CurrentMenu > (NUM_F1_MENUS - 1))
				{
					m_nF1CurrentMenu = NUM_F1_MENUS - 1;
				}

				m_cpControlsMesh->SetTexture(m_vszF1Textures[m_nF1CurrentMenu].c_str());

				m_cpRightArrowMesh->GetRender() = true;
				m_cpLeftArrowMesh->GetRender() = true;

				if (m_nF1CurrentMenu == NUM_F1_MENUS - 1)
					m_cpRightArrowMesh->GetRender() = false;
				if (m_nF1CurrentMenu == 0)
					m_cpLeftArrowMesh->GetRender() = false;

			}

			if (m_cpInputManager->IsKeyPressed("MenuLeft") || m_cpInputManager->IsButtonPressed("Map") || m_cpInputManager->IsAxisPressed("Left JS Neg X"))
			{
				m_nF1CurrentMenu--;
				if (m_nF1CurrentMenu < 0)
				{
					m_nF1CurrentMenu = 0;
				}

				m_cpControlsMesh->SetTexture(m_vszF1Textures[m_nF1CurrentMenu].c_str());

				m_cpRightArrowMesh->GetRender() = true;
				m_cpLeftArrowMesh->GetRender() = true;

				if (m_nF1CurrentMenu == 0)
				{
					m_cpLeftArrowMesh->GetRender() = false;

					if (m_cpInputManager->GetControllerBeingUsed() == true)
						m_vszF1Textures[m_nF1CurrentMenu] = L"../Game/Assets/Art/2D/Menu/Controller_Controls.dds";
					else
						m_vszF1Textures[m_nF1CurrentMenu] = L"../Game/Assets/Art/2D/Menu/Controls.dds";
				}
				if (m_nF1CurrentMenu == NUM_F1_MENUS - 1)
				{
					m_cpRightArrowMesh->GetRender() = false;
				}
			}

			string page;
			page += to_string(m_nF1CurrentMenu + 1);
			page += " / ";
			page += to_string(NUM_F1_MENUS);

			wstring pageNum(page.begin(), page.end());

			float2 curPos = { -50.0f, 400.0f };
			float4 curColor = { 0.0f, 0.0f, 0.0f, 1.0f };

			GRAPHICS->RenderText(pageNum.c_str(), curPos, curColor, { 0.75f, 0.75f });

			wstring pageTitle = m_vszScreenNames[m_nF1CurrentMenu];

			float2 tcurPos = { -200.0f, 290.0f };
			float4 tcurColor = { 0.0f, 0.0f, 0.0f, 1.0f };

			GRAPHICS->RenderText(pageTitle.c_str(), tcurPos, tcurColor, { 0.4f, 0.4f });

			if (m_cpInputManager->IsKeyPressed("Pause") || m_cpInputManager->IsButtonPressed("Pause"))
			{
				m_bControlsON = false;
				m_bPaused = false;
				m_cpControlsMesh->GetRender() = false;
				m_cpRightArrowMesh->GetRender() = false;
				m_cpLeftArrowMesh->GetRender() = false;
			}
		}

#pragma endregion
	}

	if (m_bPaused == false && m_bGameOver == false)
	{
#pragma region Debug Input

		//DEBUG: teleport to exit
		if (m_cpInputManager->IsKeyDown("Ctrl") && m_cpInputManager->IsKeyPressed("TeleportToExit"))
		{
			IObject* exit = m_vpExits[m_nExitIndex];
			XMFLOAT3 tempPos;
			if (m_nExitIndex == 0)//bottom left
			{
				tempPos.x = exit->GetWorldMatrix()._41;
				tempPos.y = exit->GetWorldMatrix()._42;
				tempPos.z = exit->GetWorldMatrix()._43 + 4750.0f;
			}
			else if (m_nExitIndex == 1)//bttom right
			{
				tempPos.x = exit->GetWorldMatrix()._41 - 4750.0f;
				tempPos.y = exit->GetWorldMatrix()._42;
				tempPos.z = exit->GetWorldMatrix()._43;
			}
			else if (m_nExitIndex == 2)//top left
			{
				tempPos.x = exit->GetWorldMatrix()._41 + 4750.0f;
				tempPos.y = exit->GetWorldMatrix()._42;
				tempPos.z = exit->GetWorldMatrix()._43;
			}
			else if (m_nExitIndex == 3)//top right
			{
				tempPos.x = exit->GetWorldMatrix()._41;
				tempPos.y = exit->GetWorldMatrix()._42;
				tempPos.z = exit->GetWorldMatrix()._43 - 4750.0f;
			}
			m_cpThePlayer->SetPosition(tempPos);

			for (unsigned int i = 0; i < m_cpThePlayer->GetColliders().size(); i++)
			{
				if (m_cpThePlayer->GetColliders()[i]->GetBounds()->GetType() == Bounds::Capsule)
				{
					((CCapsule*)m_cpThePlayer->GetColliders()[i]->GetBounds())->SetStart(XMFLOAT3(m_cpThePlayer->GetWorldMatrix()._41, m_cpThePlayer->GetWorldMatrix()._42 + 45.0f, m_cpThePlayer->GetWorldMatrix()._43));
					((CCapsule*)m_cpThePlayer->GetColliders()[i]->GetBounds())->SetEnd(XMFLOAT3(m_cpThePlayer->GetWorldMatrix()._41, m_cpThePlayer->GetWorldMatrix()._42 + 135.0f, m_cpThePlayer->GetWorldMatrix()._43));
					((CCapsule*)m_cpThePlayer->GetColliders()[i]->GetBounds())->SetRadius(45.0f);
				}
				else if (m_cpThePlayer->GetColliders()[i]->GetBounds()->GetType() == Bounds::AABB)
					((CAABB*)m_cpThePlayer->GetColliders()[i]->GetBounds())->SetCenter({ m_cpThePlayer->GetWorldMatrix()._41, m_cpThePlayer->GetWorldMatrix()._42 + 45, m_cpThePlayer->GetWorldMatrix()._43 });

				m_cpThePlayer->GetColliders()[i]->UpdatePosition(m_cpThePlayer->GetWorldVelocity());
			}
			m_nExitIndex++;
			if (m_nExitIndex >= (int)m_vpExits.size())
				m_nExitIndex = 0;
		}

		//chest cheats
		if (m_cpInputManager->IsKeyDown("Ctrl") && m_cpInputManager->IsKeyPressed("UpgradeWeapon"))
		{
			if (((CPlayer*)m_cpThePlayer)->UnlockWeapon() == true)
			{
				if (m_cpObjectManager->FindObjectsWithTag("WeaponChest", CObjectManager::eObjectType::Dynamic).size() >= 1)
				{
					m_cpObjectManager->FindObjectsWithTag("WeaponChest", CObjectManager::eObjectType::Dynamic)[0]->GetRenderMesh()->GetRender() = false;
					m_cpObjectManager->RemoveObject(m_cpObjectManager->FindObjectsWithTag("WeaponChest", CObjectManager::eObjectType::Dynamic)[0], CObjectManager::eObjectType::Dynamic);
				}
			}
		}
		//if (m_cpInputManager->IsKeyDown("Ctrl") && m_cpInputManager->IsKeyPressed("UpgradeShield"))
		//{
		//	if (((CPlayer*)m_cpThePlayer)->UpgradeShield() == true)
		//	{
		//		if (m_cpObjectManager->FindObjectsWithTag("ShieldChest", CObjectManager::eObjectType::Dynamic).size() >= 1)
		//		{
		//			m_cpObjectManager->FindObjectsWithTag("ShieldChest", CObjectManager::eObjectType::Dynamic)[0]->GetRenderMesh()->GetRender() = false;
		//			m_cpObjectManager->RemoveObject(m_cpObjectManager->FindObjectsWithTag("ShieldChest", CObjectManager::eObjectType::Dynamic)[0], CObjectManager::eObjectType::Dynamic);
		//		}
		//	}
		//}
		if (m_cpInputManager->IsKeyDown("Ctrl") && m_cpInputManager->IsKeyPressed("IncreaseMaxHealth"))
		{
			if (((CPlayer*)m_cpThePlayer)->UpgradeHealth() == true)
			{
				if (m_cpObjectManager->FindObjectsWithTag("HealthChest", CObjectManager::eObjectType::Dynamic).size() >= 1)
				{
					m_cpObjectManager->FindObjectsWithTag("HealthChest", CObjectManager::eObjectType::Dynamic)[0]->GetRenderMesh()->GetRender() = false;
					m_cpObjectManager->RemoveObject(m_cpObjectManager->FindObjectsWithTag("HealthChest", CObjectManager::eObjectType::Dynamic)[0], CObjectManager::eObjectType::Dynamic);
				}
			}
		}

#pragma endregion

		CWorldMap* cMap = reinterpret_cast<CPlayer*>(m_cpThePlayer)->GetWorldMap();
		CInventory* cInv = reinterpret_cast<CPlayer*>(m_cpThePlayer)->GetInventory();
		//pause menu - Opens pause regardless of anything.
		if ((m_cpInputManager->IsKeyPressed("Pause") || m_cpInputManager->IsButtonPressed("Pause"))
			&& m_bPaused == false && !cInv->IsOpen() /*&& !m_cpCraftingTable->IsOpen()*/ && !cMap->IsOpen() && m_bControllerPopUp == false && !m_cpCutsceneManager->IsCutsceneActive() && m_cpStorageCrate->IsOpen() == false)
		{
			m_bPaused = true;
		}

#pragma region Controller Disconnect

		if (m_cpInputManager->DidControllerDisconnect() == true && m_bControllerWasUsed == true && m_bGameOver == false)
		{
			m_bControllerPopUp = true;
			m_cpControllerPopUpMesh->GetRender() = true;

			if (cInv->IsOpen())
				cInv->Close();

			if (cMap->IsOpen())
				cMap->Close();

			if (m_bPaused == true)
			{
				m_bPaused = false;
				m_cpTextFrame->GetRender() = false;
			}

			if (m_bControlsON == true)
			{
				m_cpControlsMesh->GetRender() = false;
				m_cpRightArrowMesh->GetRender() = false;
				m_cpLeftArrowMesh->GetRender() = false;

				m_bControlsON = false;
			}

			//if (m_cpCraftingTable->IsOpen())
			//	m_cpCraftingTable->Close();
		}

		if (m_bControllerPopUp == true)
		{
			GRAPHICS->RenderText(L"Controller Disconnected", { 0, 250 }, { 0, 0, 0, 0 }, { 0.5f, 0.5f }, Center);

			//controller has connected since it has been disconnected (user sat on this screen and reconnected the controller)
			if (m_cpInputManager->IsControllerConnected() == true)
			{
				m_cpInputManager->ActivateControllerUse();

				//give option to turn on controller
				if (m_cpInputManager->IsKeyPressed("Menu Right") || m_cpInputManager->IsButtonPressed("Inventory") || m_cpInputManager->IsAxisPressed("Left JS Pos X"))
					m_bDCControllerCursor = !m_bDCControllerCursor;
				else if (m_cpInputManager->IsKeyPressed("Menu Left") || m_cpInputManager->IsButtonPressed("Map") || m_cpInputManager->IsAxisPressed("Left JS Neg X"))
					m_bDCControllerCursor = !m_bDCControllerCursor;

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
				m_cpInputManager->ActivateControllerUse();
			}
			else
			{
				GRAPHICS->RenderText(L"Press Enter to Continue", { -375, -225 }, { 0, 0, 0, 0 }, { 0.3f, 0.3f }, Left);
				m_cpInputManager->DeactivateControllerUse();
			}

			if (m_cpInputManager->IsKeyPressed("Action") || m_cpInputManager->IsButtonPressed("Jump Action"))
			{
				m_bControllerPopUp = false;
				m_cpControllerPopUpMesh->GetRender() = false;

				if (m_bDCControllerCursor == false)
					m_bControllerWasUsed = false;
				else
					m_bControllerWasUsed = true;
			}
		}

#pragma endregion

		return eContinue;
	}
	else if ((m_bPaused && m_bGameOver == false))
		return PauseMenu();
	else if (m_bGameOver == true)
		return HandleGameOver();

	return eContinue;

}

/*****************************************************************
* LoadingThread()		Multithreaded Loading Thread
*
* Ins:					TThreadData*
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    09/15/2015
* Mod. Initials:	    MZ
*****************************************************************/
//void LoadingThread(TThreadData* tThreadData)
//{
//
//#pragma region TextFrame
//
//	vector<TVertex2D> ScreenVerts;
//	vector<unsigned int> ScreenIndicies;
//
//	ScreenIndicies.push_back(0);
//	ScreenIndicies.push_back(1);
//	ScreenIndicies.push_back(2);
//
//	ScreenIndicies.push_back(0);
//	ScreenIndicies.push_back(2);
//	ScreenIndicies.push_back(3);
//
//	float fStartX = -0.85f;
//	float fStartY = 0.5f;
//	float fWidth = 1.7f;
//	float fHeight = 1.0f;
//
//	ScreenVerts.push_back // Top Left
//		({
//			{ fStartX, fStartY, -1.0f, 1 },
//			{ 0, 0 }
//	});
//
//	ScreenVerts.push_back // Top Right
//		({
//			{ fStartX + fWidth, fStartY, -1.0f, 1 },
//			{ 1, 0 }
//	});
//
//	ScreenVerts.push_back // Bot Right
//		({
//			{ fStartX + fWidth, fStartY - fHeight, -1.0f, 1 },
//			{ 1, 1 }
//	});
//
//	ScreenVerts.push_back // Bot Left
//		({
//			{ fStartX, fStartY - fHeight, -1.0f, 1 },
//			{ 0, 1 }
//	});
//
//	tThreadData->cpTextFrame = new CHUDMesh(ScreenIndicies, ScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/Failbot_Pause_Background.dds");
//
//	tThreadData->cpTextFrame->GetRender() = false;
//	tThreadData->cpTextFrame->GetTransparent() = true;
//
//
//
//#pragma endregion
//#pragma region Controls Plane
//
//	vector<TVertex2D> m_cvControlsVertecies;
//	vector<unsigned int> m_nvControlsIndicies;
//
//	m_nvControlsIndicies.push_back(0);
//	m_nvControlsIndicies.push_back(1);
//	m_nvControlsIndicies.push_back(2);
//
//	m_nvControlsIndicies.push_back(0);
//	m_nvControlsIndicies.push_back(2);
//	m_nvControlsIndicies.push_back(3);
//
//	m_cvControlsVertecies.push_back
//		({
//			{ -0.8f, 0.55f, -1.0f, 1 },
//			{ 0, 0 }
//	});
//
//	m_cvControlsVertecies.push_back
//		({
//			{ 0.8f, 0.55f, -1.0f, 1 },
//			{ 1, 0 }
//	});
//
//	m_cvControlsVertecies.push_back
//		({
//			{ 0.8f, -0.55f, -1.0f, 1 },
//			{ 1, 1 }
//	});
//
//	m_cvControlsVertecies.push_back
//		({
//			{ -0.8f, -0.55f, -1.0f, 1 },
//			{ 0, 1 }
//	});
//
//	tThreadData->cpControlsMesh = new CHUDMesh(m_nvControlsIndicies, m_cvControlsVertecies, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/Controls.dds");
//	tThreadData->cpControlsMesh->GetTransparent() = true;
//
//	TranslateLocalY(tThreadData->cpControlsMesh->GetTransformMatrix(), -0.1f);
//
//#pragma endregion
//
//
//
//	tThreadData->bIsFinishedLoading = true
//}


/*****************************************************************
* HandleGameOver()			Handles the game over sequence
*
* Ins:						None
*
* Outs:						None
*
* Returns:					None
*
* Mod. Date:				09/17/2015
* Mod. Initials:			NH
*****************************************************************/
EInputResult CGameplayState::HandleGameOver()
{
	GRAPHICS->RenderText(L"Game Over", { 0.0f, 600.0f }, { 1.0f, 0.8f, 0.0f, 1.0f }, { 3, 3 }, Center);

	if (bLose)
	{
		GRAPHICS->RenderText(L"You Lose", { 0.0f, 200.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 2, 2 }, Center);
		GRAPHICS->RenderText(L"You Ran Out Of Days", { 0.0f, -100.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1, 1 }, Center);
	}
	else
		GRAPHICS->RenderText(L"You Win!", { 0.0f, 200.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 2, 2 }, Center);


	//move exit door 
	for (unsigned int i = 0; i < m_vpExits.size(); i++)
	{
		IObject* exit = ((CPrefabObject*)m_vpExits[i])->GetExitDoorObject();

		if (((CExitDoorObject*)exit)->GetActive() == true)
		{
			((CExitDoorObject*)exit)->ActivateVerticleMovement(3.0f);
		}
	}

	m_fGameOverTimer += DELTA_TIME();

	m_unGameOverAudioTics++;
	if (m_unGameOverAudioTics <= 1 && bLose == false)//so it only happens once. even if the message is sent multiple times
	{
		m_cAudioManager.PostEvent(AK::EVENTS::WIN, XMFLOAT3(0, 0, 0));
	}

	if (m_fGameOverTimer >= GAME_OVER_TIMER)
	{
		m_cAudioManager.PostEvent(AK::EVENTS::STOPBACKGROUND, XMFLOAT3(0, 0, 0));
		m_bIsPlayingMusic = false;

		GRAPHICS->ClearScene();
		return eRemove;
		//			CStateMachine::GetInstance()->RemoveState();
		//			CStateMachine::GetInstance()->AddState(CStateMachine::eCreditsState);
	}
	return eContinue;

}

/*****************************************************************
* HandleGameOver()			Handles the game over sequence
*
* Ins:						None
*
* Outs:						None
*
* Returns:					None
*
* Mod. Date:				09/17/2015
* Mod. Initials:			NH
*****************************************************************/
EInputResult CGameplayState::PauseMenu()
{
	GRAPHICS->RenderText(L"Paused", { 0.0f, 525.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 2, 2 }, Center);
	m_cpTextFrame->GetRender() = true;

	//controls images
	for (unsigned int i = 0; i < m_vpPausedControls.size(); i++)
	{
		m_vpPausedControls[i]->GetRender() = true;
	}
	if (m_cpInputManager->GetControllerBeingUsed() == true)
	{
		m_vpPausedControls[0]->SetTexture(L"../Game/Assets/Art/2D/Inventory/UpDPad_Controller.dds");
		m_vpPausedControls[1]->SetTexture(L"../Game/Assets/Art/2D/Inventory/DownDPad_Controller.dds");
	}
	else
	{
		m_vpPausedControls[0]->SetTexture(L"../Game/Assets/Art/2D/Inventory/UpArrow_Keyboard.dds");
		m_vpPausedControls[1]->SetTexture(L"../Game/Assets/Art/2D/Inventory/DownArrow_Keyboard.dds");
	}

	m_fPausedInputTimer += DELTA_TIME();
	if (m_fPausedInputTimer >= PAUSED_INPUT_TIME)
	{
		if (m_cpInputManager->IsKeyPressed("Arrow Down") || m_cpInputManager->IsButtonPressed("Cursor Down"))
		{
			m_nPauseCursor++;
			if (m_nPauseCursor > EPausedOptions::eQuit)
			{
				m_nPauseCursor = EPausedOptions::eResume;
			}
		}
		else if (m_cpInputManager->IsKeyPressed("Arrow Up") || m_cpInputManager->IsButtonPressed("Cursor Up"))
		{
			m_nPauseCursor--;
			if (m_nPauseCursor < EPausedOptions::eResume)
			{
				m_nPauseCursor = EPausedOptions::eQuit;
			}
		}

		if (m_cpInputManager->IsKeyPressed("Pause") || m_cpInputManager->IsButtonPressed("Pause"))
		{
			m_bPaused = false;
			m_fPausedInputTimer = 0.0f;
			m_cpTextFrame->GetRender() = false;

			for (unsigned int i = 0; i < m_vpPausedControls.size(); i++)
				m_vpPausedControls[i]->GetRender() = false;

			if (m_bControlsON == true)
			{
				m_bControlsON = false;
				m_cpControlsMesh->GetRender() = false;
			}
		}

		if (m_cpInputManager->IsKeyPressed("Action") || m_cpInputManager->IsButtonPressed("Jump Action"))
		{
			switch ((EPausedOptions)m_nPauseCursor)
			{
			case EPausedOptions::eResume:
			{
				m_bPaused = false;
				m_fPausedInputTimer = 0.0f;
				m_cpTextFrame->GetRender() = false;

				for (unsigned int i = 0; i < m_vpPausedControls.size(); i++)
					m_vpPausedControls[i]->GetRender() = false;

				if (m_bControlsON == true)
				{
					m_bControlsON = false;
					m_cpControlsMesh->GetRender() = false;
				}

				m_nPauseCursor = eResume;
			}
			break;
			case EPausedOptions::eOptions:
			{
				if (++l < 3) CStateMachine::GetInstance()->PushState(eOPTIONS);
			}
			break;
			case EPausedOptions::eControls:
			{
				m_fPausedInputTimer = 0.0f;
				m_bPaused = false;
				m_cpTextFrame->GetRender() = false;
				m_bControlsON = true;

				for (unsigned int i = 0; i < m_vpPausedControls.size(); i++)
					m_vpPausedControls[i]->GetRender() = false;

				m_nF1CurrentMenu = 0;
				if (m_cpInputManager->GetControllerBeingUsed() == true)
					m_vszF1Textures[m_nF1CurrentMenu] = L"../Game/Assets/Art/2D/Menu/Controller_Controls.dds";
				else
					m_vszF1Textures[m_nF1CurrentMenu] = L"../Game/Assets/Art/2D/Menu/Controls.dds";

				m_cpControlsMesh->SetTexture(m_vszF1Textures[m_nF1CurrentMenu].c_str());
				m_cpControlsMesh->GetRender() = true;
				m_cpRightArrowMesh->GetRender() = true;
				m_cpLeftArrowMesh->GetRender() = false;

				m_nPauseCursor = eResume;
			}
			break;
			case EPausedOptions::eQuit:
			{
				m_cAudioManager.PostEvent(AK::EVENTS::STOPBACKGROUND, XMFLOAT3(0, 0, 0));
				m_bIsPlayingMusic = false;
				m_bPaused = false;
				m_fPausedInputTimer = 0.0f;
				m_bControlsON = false;
				

				for (unsigned int i = 0; i < m_vpPausedControls.size(); i++)
					m_vpPausedControls[i]->GetRender() = false;

				GRAPHICS->ClearScene();

				m_nPauseCursor = eResume;
				return eRemove;
			}
			break;
			default:
				break;
			}
		}
	}

	XMFLOAT4 f4Selected = { 0.0f, 0.0, 1.0f, 1.0f };
	XMFLOAT4 f4UnSelected = { 0.0f, 0.0, 0.0f, 1.0f };

	if ((EPausedOptions)m_nPauseCursor == eResume) GRAPHICS->RenderText(L"Resume Game", { 0.0f, 150.0f }, f4UnSelected, { 1, 1 }, Center);
	GRAPHICS->RenderText(L"Resume Game", { 0.0f, 150.0f }, ((EPausedOptions)m_nPauseCursor == eResume) ? f4Selected : f4UnSelected, { 1, 1 }, Center);
	if ((EPausedOptions)m_nPauseCursor == eOptions) GRAPHICS->RenderText(L"Options", { 0.0f, -50.0f }, f4UnSelected, { 1, 1 }, Center);
	GRAPHICS->RenderText(L"Options", { 0.0f, -50.0f }, ((EPausedOptions)m_nPauseCursor == eOptions) ? f4Selected : f4UnSelected, { 1, 1 }, Center);
	if ((EPausedOptions)m_nPauseCursor == eControls) GRAPHICS->RenderText(L"How To Play", { 0.0f, -250.0f }, f4UnSelected, { 1, 1 }, Center);
	GRAPHICS->RenderText(L"How To Play", { 0.0f, -250.0f }, ((EPausedOptions)m_nPauseCursor == eControls) ? f4Selected : f4UnSelected, { 1, 1 }, Center);
	if ((EPausedOptions)m_nPauseCursor == eQuit) GRAPHICS->RenderText(L"Exit Game", { 0.0f, -450.0f }, f4UnSelected, { 1, 1 }, Center);
	GRAPHICS->RenderText(L"Exit Game", { 0.0f, -450.0f }, ((EPausedOptions)m_nPauseCursor == eQuit) ? f4Selected : f4UnSelected, { 1, 1 }, Center);

	//play time
	float playSeconds = ((CPlayer*)m_cpThePlayer)->GetTotalPlayTime();
	int playMinutes = (int)playSeconds / 60;
	int playHours = playMinutes / 60;

	if (playSeconds >= 60)//more than a minute
	{
		//cap seconds
		playSeconds -= (float)(playMinutes * 60);

		//cap minutes
		playMinutes -= playHours * 60;
	}

	string szDisplayTime = "Play Time: " + to_string(playHours) + ":" + to_string(playMinutes) + ":" + to_string((int)playSeconds);
	wstring wszDisplay(szDisplayTime.begin(), szDisplayTime.end());
	GRAPHICS->RenderText(wszDisplay.c_str(), { 0.0f, -650.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.45f, 0.4f }, Center);

	return eContinue;
}

void CGameplayState::CreateSkybox()
{
	m_cpSkyBoxMesh = new CMesh;
	// SkyBox Indices Initialization //
	std::vector<unsigned int> nSkyIndex;

	// Front Face //
	m_cpSkyBoxMesh->GetIndices().push_back(1);
	m_cpSkyBoxMesh->GetIndices().push_back(0);
	m_cpSkyBoxMesh->GetIndices().push_back(3);

	m_cpSkyBoxMesh->GetIndices().push_back(1);
	m_cpSkyBoxMesh->GetIndices().push_back(3);
	m_cpSkyBoxMesh->GetIndices().push_back(2);

	// Left Face //
	m_cpSkyBoxMesh->GetIndices().push_back(0);
	m_cpSkyBoxMesh->GetIndices().push_back(4);
	m_cpSkyBoxMesh->GetIndices().push_back(7);

	m_cpSkyBoxMesh->GetIndices().push_back(0);
	m_cpSkyBoxMesh->GetIndices().push_back(7);
	m_cpSkyBoxMesh->GetIndices().push_back(3);

	// Right Face //
	m_cpSkyBoxMesh->GetIndices().push_back(5);
	m_cpSkyBoxMesh->GetIndices().push_back(1);
	m_cpSkyBoxMesh->GetIndices().push_back(2);

	m_cpSkyBoxMesh->GetIndices().push_back(5);
	m_cpSkyBoxMesh->GetIndices().push_back(2);
	m_cpSkyBoxMesh->GetIndices().push_back(6);

	// Back Face //
	m_cpSkyBoxMesh->GetIndices().push_back(4);
	m_cpSkyBoxMesh->GetIndices().push_back(5);
	m_cpSkyBoxMesh->GetIndices().push_back(6);

	m_cpSkyBoxMesh->GetIndices().push_back(4);
	m_cpSkyBoxMesh->GetIndices().push_back(6);
	m_cpSkyBoxMesh->GetIndices().push_back(7);

	// Top Face //
	m_cpSkyBoxMesh->GetIndices().push_back(0);
	m_cpSkyBoxMesh->GetIndices().push_back(1);
	m_cpSkyBoxMesh->GetIndices().push_back(5);

	m_cpSkyBoxMesh->GetIndices().push_back(0);
	m_cpSkyBoxMesh->GetIndices().push_back(5);
	m_cpSkyBoxMesh->GetIndices().push_back(4);

	// Bottom Face //
	m_cpSkyBoxMesh->GetIndices().push_back(7);
	m_cpSkyBoxMesh->GetIndices().push_back(6);
	m_cpSkyBoxMesh->GetIndices().push_back(2);

	m_cpSkyBoxMesh->GetIndices().push_back(7);
	m_cpSkyBoxMesh->GetIndices().push_back(2);
	m_cpSkyBoxMesh->GetIndices().push_back(3);

	// SkyBox Vertices Initialization //
	std::vector<TVertex3D> tSkyVertex;

	m_cpSkyBoxMesh->GetVertices().push_back({ { -1, 1, -1, 1 } }); // 0 // Front Left  Top
	m_cpSkyBoxMesh->GetVertices().push_back({ { 1, 1, -1, 1 } }); // 1 // Front Right Top
	m_cpSkyBoxMesh->GetVertices().push_back({ { 1, -1, -1, 1 } }); // 2 // Front Right Bottom
	m_cpSkyBoxMesh->GetVertices().push_back({ { -1, -1, -1, 1 } }); // 3 // Front Left  Bottom
	m_cpSkyBoxMesh->GetVertices().push_back({ { -1, 1, 1, 1 } }); // 4 // Back  Left  Top
	m_cpSkyBoxMesh->GetVertices().push_back({ { 1, 1, 1, 1 } }); // 5 // Back  Right Top
	m_cpSkyBoxMesh->GetVertices().push_back({ { 1, -1, 1, 1 } }); // 6 // Back  Right Bottom
	m_cpSkyBoxMesh->GetVertices().push_back({ { -1, -1, 1, 1 } }); // 7 // Back  Left  Bottom

	// Allocate RenderMesh //
	m_cpSkybox = new CRenderMesh(m_cpSkyBoxMesh, GRAPHICS->GetSkyboxVS(), GRAPHICS->GetSkyboxPS());
	m_cpSkybox->SetTexture(L"../Game/Assets/Art/2D/Textures/Skybox.dds"); // Set to skybox texture
	m_cpSkybox->SetNormals(L"../Game/Assets/Art/2D/Textures/SkyBoxNight.dds"); // Set 2nd Texture
	m_cpSkybox->GetTransparencyOverride() = 0.0f;
	m_cpSkybox->GetTransformMatrix() = GRAPHICS->GetViewMatrix(); // Snap to camera

	// Add To Renderer //
	GRAPHICS->AddRenderMesh(m_cpSkybox);
}

void CGameplayState::DCControllerInit()
{
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
	m_bControllerWasUsed = m_cpInputManager->GetControllerBeingUsed();
	GRAPHICS->AddRenderMesh(m_cpControllerPopUpMesh);
}



/*	PUBLIC FUNCTIONS	*/

/*****************************************************************
* CGameplayState()		The constructor for the GameplayState. Initializes the state.
*
* Ins:					None
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    09/17/2015
* Mod. Initials:	    NH
*****************************************************************/
void CGameplayState::Enter()
{
	bLose = false;
	m_fObjectiveTimer = 5.0f;
	m_bControlsON = false;
	m_bGameOver = false;
	m_bExitDoorMove = false;
	m_fExitDoorMoveTimer = 0.0f;
	m_fGameOverTimer = 0.0f;
	m_unGameOverAudioTics = 0;
	m_bPaused = false;
	m_fPausedInputTimer = 0.0f;

	LoadingGraphics();
	m_nExitIndex = 0;


	m_nPauseCursor = eResume;

#pragma region TextFrame

	vector<TVertex2D> ScreenVerts;
	vector<unsigned int> ScreenIndicies;

	ScreenIndicies.push_back(0);
	ScreenIndicies.push_back(1);
	ScreenIndicies.push_back(2);

	ScreenIndicies.push_back(0);
	ScreenIndicies.push_back(2);
	ScreenIndicies.push_back(3);

	float fLeft = 0.0f;
	float fTop = 0.0f;
	float fRight = 1.5f;
	float fBottom = -1.4f;

	ScreenVerts.push_back // Top Left
		({
			{ fLeft, fTop, -1.0f, 1 },
			{ 0, 0 }
	});

	ScreenVerts.push_back // Top Right
		({
			{ fRight, fTop, -1.0f, 1 },
			{ 1, 0 }
	});

	ScreenVerts.push_back // Bot Right
		({
			{ fRight, fBottom, -1.0f, 1 },
			{ 1, 1 }
	});

	ScreenVerts.push_back // Bot Left
		({
			{ fLeft, fBottom, -1.0f, 1 },
			{ 0, 1 }
	});

	m_cpTextFrame = new CHUDMesh(ScreenIndicies, ScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/Failbot_Pause_Background.dds");
	m_cpTextFrame->GetRender() = false;
	m_cpTextFrame->GetTransparent() = true;
	TranslateLocalX(m_cpTextFrame->GetPositionMatrix(), -0.75f);
	TranslateLocalY(m_cpTextFrame->GetPositionMatrix(), 0.6f);
	GRAPHICS->AddRenderMesh(m_cpTextFrame);

#pragma endregion

#pragma region Pause Menu Controls

	ScreenVerts.clear();
	ScreenIndicies.clear();

	ScreenIndicies.push_back(0);
	ScreenIndicies.push_back(1);
	ScreenIndicies.push_back(2);
	ScreenIndicies.push_back(0);
	ScreenIndicies.push_back(2);
	ScreenIndicies.push_back(3);

	ScreenVerts.push_back // Top Left
		({
			{ 0.0f, 0.0f, -1.0f, 1 },
			{ 0, 0 }
	});

	ScreenVerts.push_back // Top Right
		({
			{ 0.2f, 0.0f, -1.0f, 1 },
			{ 1, 0 }
	});

	ScreenVerts.push_back // Bot Right
		({
			{ 0.2f, -0.2f, -1.0f, 1 },
			{ 1, 1 }
	});

	ScreenVerts.push_back // Bot Left
		({
			{ 0.0f, -0.2f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_vpPausedControls.clear();

	//up arrow
	m_vpPausedControls.push_back(new CHUDMesh(ScreenIndicies, ScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Inventory/UpArrow_Keyboard.dds"));
	m_vpPausedControls[0]->GetRender() = false;
	m_vpPausedControls[0]->GetTransparent() = true;
	TranslateLocalX(m_vpPausedControls[0]->GetPositionMatrix(), -0.6f);
	TranslateLocalY(m_vpPausedControls[0]->GetPositionMatrix(), -0.05f);

	//down arrow
	m_vpPausedControls.push_back(new CHUDMesh(ScreenIndicies, ScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Inventory/DownArrow_Keyboard.dds"));
	m_vpPausedControls[1]->GetRender() = false;
	m_vpPausedControls[1]->GetTransparent() = true;
	TranslateLocalX(m_vpPausedControls[1]->GetPositionMatrix(), -0.6f);
	TranslateLocalY(m_vpPausedControls[1]->GetPositionMatrix(), -0.25f);

	//enter key
	//m_vpPausedControls.push_back(new CHUDMesh(ScreenIndicies, ScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Inventory/Enter_Keyboard.dds"));
	//m_vpPausedControls[2]->GetRender() = false;
	//m_vpPausedControls[2]->GetTransparent() = true;
	//m_vpPausedControls[2]->GetVertex2DArr()[1].m_fPosition[0] = 0.3f;
	//m_vpPausedControls[2]->GetVertex2DArr()[2].m_fPosition[0] = 0.3f;
	////TranslateLocalX(m_vpPausedControls[2]->GetPositionMatrix(), -0.3f);
	//TranslateLocalY(m_vpPausedControls[2]->GetPositionMatrix(), -0.8f);

	for (unsigned int i = 0; i < m_vpPausedControls.size(); i++)
		GRAPHICS->AddRenderMesh(m_vpPausedControls[i]);

#pragma endregion

#pragma region Controls Plane

	vector<TVertex2D> m_cvControlsVertecies;
	vector<unsigned int> m_nvControlsIndicies;

	m_nvControlsIndicies.push_back(0);
	m_nvControlsIndicies.push_back(1);
	m_nvControlsIndicies.push_back(2);

	m_nvControlsIndicies.push_back(0);
	m_nvControlsIndicies.push_back(2);
	m_nvControlsIndicies.push_back(3);

	m_cvControlsVertecies.push_back
		({
			{ -0.8f, 0.55f, -1.0f, 1 },
			{ 0, 0 }
	});

	m_cvControlsVertecies.push_back
		({
			{ 0.8f, 0.55f, -1.0f, 1 },
			{ 1, 0 }
	});

	m_cvControlsVertecies.push_back
		({
			{ 0.8f, -0.55f, -1.0f, 1 },
			{ 1, 1 }
	});

	m_cvControlsVertecies.push_back
		({
			{ -0.8f, -0.55f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_cpControlsMesh = new CHUDMesh(m_nvControlsIndicies, m_cvControlsVertecies, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/Controls.dds");
	m_cpControlsMesh->GetTransparent() = true;
	m_cpControlsMesh->GetRender() = false;
	GRAPHICS->AddRenderMesh(m_cpControlsMesh);

	TranslateLocalY(m_cpControlsMesh->GetTransformMatrix(), -0.1f);

	m_vszF1Textures.push_back(L"../Game/Assets/Art/2D/Menu/Controls.dds");
	//m_vszF1Textures.push_back(L"../Game/Assets/Art/2D/Menu/CombatScreen.dds");
	//m_vszF1Textures.push_back(L"../Game/Assets/Art/2D/Menu/StaminaScreen.dds");
	//m_vszF1Textures.push_back(L"../Game/Assets/Art/2D/Menu/ChestScreen.dds");
	m_vszF1Textures.push_back(L"../Game/Assets/Art/2D/Menu/CraftingScreen.dds");
	m_vszF1Textures.push_back(L"../Game/Assets/Art/2D/Menu/MapScreen.dds");
	m_vszF1Textures.push_back(L"../Game/Assets/Art/2D/Menu/NightScreen.dds");

	m_vszScreenNames.push_back(L"");
	//m_vszScreenNames.push_back(L"     Player Combat");
	//m_vszScreenNames.push_back(L"     Player Stamina");
	//m_vszScreenNames.push_back(L"      Loot Chests");
	m_vszScreenNames.push_back(L"   Items and Storage");
	m_vszScreenNames.push_back(L"    Mapping The Maze");
	m_vszScreenNames.push_back(L" Night Time and Your Bed");

#pragma endregion

#pragma region Loading Arrows
	vector<TVertex2D> leftArrowVerts;
	vector<unsigned int> leftArrowIndices;

	leftArrowIndices.push_back(0);
	leftArrowIndices.push_back(1);
	leftArrowIndices.push_back(2);

	leftArrowIndices.push_back(0);
	leftArrowIndices.push_back(2);
	leftArrowIndices.push_back(3);

	leftArrowVerts.push_back
		({
			{ -0.9f, 0.0f, -1.0f, 1 },
			{ 0, 0 }
	});

	leftArrowVerts.push_back
		({
			{ -0.8f, 0.0f, -1.0f, 1 },
			{ 1, 0 }
	});

	leftArrowVerts.push_back
		({
			{ -0.8f, -0.2f, -1.0f, 1 },
			{ 1, 1 }
	});

	leftArrowVerts.push_back
		({
			{ -0.9f, -0.2f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_cpLeftArrowMesh = new CHUDMesh(leftArrowIndices, leftArrowVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/LeftKey.dds");
	GRAPHICS->AddRenderMesh(m_cpLeftArrowMesh);
	m_cpLeftArrowMesh->GetRender() = false;

	vector<TVertex2D> rightArrowVerts;
	vector<unsigned int> rightArrowIndices;

	rightArrowIndices.push_back(0);
	rightArrowIndices.push_back(1);
	rightArrowIndices.push_back(2);

	rightArrowIndices.push_back(0);
	rightArrowIndices.push_back(2);
	rightArrowIndices.push_back(3);

	rightArrowVerts.push_back
		({
			{ 0.8f, 0.0f, -1.0f, 1 },
			{ 0, 0 }
	});

	rightArrowVerts.push_back
		({
			{ 0.9f, 0.0f, -1.0f, 1 },
			{ 1, 0 }
	});

	rightArrowVerts.push_back
		({
			{ 0.9f, -0.2f, -1.0f, 1 },
			{ 1, 1 }
	});

	rightArrowVerts.push_back
		({
			{ 0.8f, -0.2f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_cpRightArrowMesh = new CHUDMesh(rightArrowIndices, rightArrowVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/RightKey.dds");
	GRAPHICS->AddRenderMesh(m_cpRightArrowMesh);
	m_cpRightArrowMesh->GetRender() = false;
#pragma endregion

	m_tDirectLight = new TDirectLight
		({
			{ 1, -0.75f, 0.5f, 1 },
			{ 1, 1, 1 }
	});
	GRAPHICS->SetDirectionalLight(m_tDirectLight);

	m_cpAnimationManager = new CAnimationManager();

	m_cpObjectManager = new CObjectManager();

	m_cpAssetManager = CAssetManager::GetInstance();

	m_cpThePlayer = new CPlayer(m_cpObjectManager, m_cpAnimationManager);

	m_cpTheCamera = new CCamera(0.0f, 250.0f, -350.0f, m_cpObjectManager);
	m_cpTheCamera->SetParent(m_cpThePlayer);
	m_cpObjectManager->AddObject(m_cpTheCamera, CObjectManager::Dynamic);
	m_cpThePlayer->AddChild(m_cpTheCamera);
	((CPlayer*)m_cpThePlayer)->GetCamera() = m_cpTheCamera;

	m_cpParticleManager = new CParticleManager(m_cpObjectManager);
	unsigned int unTestParticle = m_cpParticleManager->LoadEmitter("TestPArticleExport.xml");
	m_cpParticleManager->GetEmitter(unTestParticle)->GetPosition(0) = 0.0f;
	m_cpParticleManager->GetEmitter(unTestParticle)->GetPosition(1) = 100.0f;
	m_cpParticleManager->GetEmitter(unTestParticle)->GetPosition(2) = 0.0f;
	m_cpParticleManager->GetEmitter(unTestParticle)->ePlay();
	m_pcDayNight = new CDayNight((CCamera*)m_cpTheCamera, m_cpParticleManager);

	//	//Initalize Message Manager
	CMessageManager::GetInstance()->Initialize(&MessageProc);
	//
	//	// Input Manager
	m_cpInputManager = CInputManager::GetInstance();
	m_cpInputManager->RegisterKey("Pause", eKey::escape);
	m_cpInputManager->RegisterKey("Controls", eKey::F1);
	m_cpInputManager->RegisterKey("Ctrl", eKey::Control);
	m_cpInputManager->RegisterKey("TeleportToExit", eKey::NumPad1);
	m_cpInputManager->RegisterKey("FastForward", eKey::NumPad2);
	m_cpInputManager->RegisterKey("MenuRight", eKey::RightArrow);
	m_cpInputManager->RegisterKey("MenuLeft", eKey::LeftArrow);
	m_cpInputManager->RegisterKey("Quit", eKey::Q);

	m_cpInputManager->RegisterKey("UpgradeWeapon", eKey::NumPad7);
	m_cpInputManager->RegisterKey("UpgradeShield", eKey::NumPad8);
	m_cpInputManager->RegisterKey("IncreaseMaxHealth", eKey::NumPad9);

	//bridge teleport set up
	m_unCurrentBridgeID = 0;

	((CPlayer*)m_cpThePlayer)->SetDayNight(m_pcDayNight);
	((CPlayer*)m_cpThePlayer)->GetHUD()->SetDayNight(m_pcDayNight);

	m_cpBed = new CBed(m_pcDayNight, m_cpObjectManager);
	m_cpBed->SetHUD(((CPlayer*)m_cpThePlayer)->GetHUD());
	//m_cpCraftingTable = new CCrafting();
	//reinterpret_cast<CPlayer*>(m_cpThePlayer)->SetCraftingTable(m_cpCraftingTable);

	m_cpStorageCrate = new CStorageCrate();
	reinterpret_cast<CPlayer*>(m_cpThePlayer)->SetStorageCrate(m_cpStorageCrate);

	AudioSystemWwise::Get()->RegisterListener(m_cpThePlayer, "Player");
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::STOPBACKGROUND);
	CreateSkybox();

	m_cpCutsceneManager = new CCutsceneManager((CPlayer*)m_cpThePlayer);

	m_cpCutsceneManager->SetCamera(m_cpTheCamera);
	//TEST CODE  Leave Commented Out
	unsigned int unStartCutscene = m_cpCutsceneManager->LoadCutscene("CutsceneTest.xml");

	m_cpCutsceneManager->PlayCutscene(unStartCutscene);
	m_cpBed->SetCutsceneManager(m_cpCutsceneManager);
	m_cpMaze = new CMaze;
	m_cpMaze->LoadMaze(m_cpObjectManager, m_pcDayNight);
	m_cpSpawnControl = new CSpawnControl(m_cpMaze, m_cpObjectManager, m_cpAnimationManager, m_pcDayNight);



	CMessageManager::GetInstance()->Update();

	std::vector<IObject*> vDoors = m_cpGameState->m_cpObjectManager->FindObjectsWithTag("Door", CObjectManager::eObjectType::Static);
	for (unsigned int i = 0; i < vDoors.size(); i++)
	{
		reinterpret_cast<CDoorObject*>(vDoors[i])->SetDayNight(m_pcDayNight);
	}
	m_vpExits = m_cpGameState->m_cpObjectManager->FindObjectsWithTag("ExitZone", CObjectManager::eObjectType::Static);
	m_vpBushes = m_cpGameState->m_cpObjectManager->FindObjectsWithTag("Bush", CObjectManager::eObjectType::Static);
	for (unsigned int i = 0; i < m_vpBushes.size(); i++)
	{
		reinterpret_cast<CBush*>(m_vpBushes[i])->SetDayNight(m_pcDayNight);
	}
	m_vpTrees = m_cpGameState->m_cpObjectManager->FindObjectsWithTag("Tree", CObjectManager::eObjectType::Static);
	for (unsigned int i = 0; i < m_vpTrees.size(); i++)
	{
		reinterpret_cast<CTree*>(m_vpTrees[i])->SetDayNight(m_pcDayNight);
	}
	Traps = m_cpGameState->m_cpObjectManager->FindObjectsWithTag("Trap5x3", CObjectManager::eObjectType::Static);
	//controller disconnect pop up set up
	DCControllerInit();

	m_vpCrackedWalls = m_cpGameState->m_cpObjectManager->FindObjectsWithTag("CrackedWall", CObjectManager::eObjectType::Static);

	//exit keys
	for (unsigned int i = 0; i < m_cpMaze->GetExitKeys().size(); i++)
	{
		m_cpMaze->GetExitKeys()[i]->Activate();
		m_vpExitKeys.push_back(m_cpMaze->GetExitKeys()[i]);
	}

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

	m_cGameOverScreen = new CHUDMesh(nSplashIndices, tSplashVertices, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D());

	m_cGameOverScreen->SetTexture(L"../Game/Assets/Art/2D/Textures/Black.dds");
	m_cGameOverScreen->GetTransparent() = true;
	m_cGameOverScreen->GetTransparencyOverride() = 0.0f;

	GRAPHICS->AddRenderMesh(m_cGameOverScreen);
	//XMFLOAT4X4 mat;
	//XMMATRIX matrix = XMMatrixTranslation(0.0f, 0.0f, 1000.0f);
	//XMStoreFloat4x4(&mat, matrix);
	//CMinotaurSpawn* prefab = new CMinotaurSpawn("MinotaurPenW", m_cpObjectManager, m_pcDayNight);
	//prefab->SetWorldMatrix(mat);
	//prefab->AddToObjectManager();

	//randomize exits
	//int exitID = rand() % m_vpExits.size();
	//((CExitDoorObject*)(((CPrefabObject*)m_vpExits[exitID])->GetExitDoorObject()))->MakeActive();

}

void CGameplayState::Exit()
{
	CMessageManager::GetInstance()->Shutdown();
	AudioSystemWwise::Get()->UnRegisterListener(m_cpThePlayer);
	CNavGraphManager::GetReference().Clear();
	GRAPHICS->ClearScene();

	SAFE_DELETE(m_cpSkyBoxMesh);
	SAFE_DELETE(m_cpBed);
	SAFE_DELETE(m_cpCraftingTable);
	SAFE_DELETE(m_cWeaponChest);
	SAFE_DELETE(m_cShieldChest1);
	SAFE_DELETE(m_cShieldChest2);
	SAFE_DELETE(m_cShieldChest3);
	SAFE_DELETE(m_cHealthChest1);
	SAFE_DELETE(m_cHealthChest2);
	SAFE_DELETE(m_cHealthChest3);
	SAFE_DELETE(m_cpMaze);
	SAFE_DELETE(m_cpSpawnControl);
	SAFE_DELETE(m_cpObjectManager);
	SAFE_DELETE(m_cpTheCamera);
	SAFE_DELETE(m_cpThePlayer);
	SAFE_DELETE(m_cpAnimationManager);
	SAFE_DELETE(m_cpCutsceneManager);
	SAFE_DELETE(m_cpTextFrame);
	SAFE_DELETE(m_cpControlsMesh);
	SAFE_DELETE(m_cpParticleManager);
	SAFE_DELETE(m_pcDayNight);
	SAFE_DELETE(m_tDirectLight);
	SAFE_DELETE(m_cpControllerPopUpMesh);
	SAFE_DELETE(m_cpSkybox);
	SAFE_DELETE(m_cpLeftArrowMesh);
	SAFE_DELETE(m_cpRightArrowMesh);
	SAFE_DELETE(m_cpStorageCrate);

	VECTOR_DELETE_ALL(m_vpExitKeys);
	VECTOR_DELETE_ALL(m_vpPausedControls);

	//SAFE_DELETE(pLoadingControlsMesh);
	//m_tThreadData.bIsFinishedLoading = false;

}

CGameplayState::CGameplayState()
{
	m_cpSkyBoxMesh = nullptr;
	m_cpBed = nullptr;
	m_cpCraftingTable = nullptr;
	m_cWeaponChest = nullptr;
	m_cShieldChest1 = nullptr;
	m_cShieldChest2 = nullptr;
	m_cShieldChest3 = nullptr;
	m_cHealthChest1 = nullptr;
	m_cHealthChest2 = nullptr;
	m_cHealthChest3 = nullptr;
	m_cpMaze = nullptr;
	m_cpSpawnControl = nullptr;
	m_cpObjectManager = nullptr;
	m_cpTheCamera = nullptr;
	m_cpThePlayer = nullptr;
	m_cpAnimationManager = nullptr;
	m_cpCutsceneManager = nullptr;
	m_cpTextFrame = nullptr;
	m_cpControlsMesh = nullptr;
	m_cpParticleManager = nullptr;
	m_pcDayNight = nullptr;
	m_tDirectLight = nullptr;
	m_cpControllerPopUpMesh = nullptr;
	m_cpSkybox = nullptr;
	m_cpLeftArrowMesh = nullptr;
	m_cpRightArrowMesh = nullptr;
	m_cpStorageCrate = nullptr;

	m_cpGameState = this;

}

/*****************************************************************
* Update()				Updates the GameplayState
*
* Ins:					None
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    09/17/2015
* Mod. Initials:	    NH
*****************************************************************/
void CGameplayState::Update()
{
	if (GetForegroundWindow() != hWindow)
		m_bPaused = true;

	if (!m_bMusicPlaying)
	{
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::DAYTIMEBIRDS);
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::DAYTIMESONG);
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::DAYTIMEWIND);
		m_bMusicPlaying = true;
	}

	if (((CPlayer*)m_cpThePlayer)->GetHUD()->GetCurrentDay() >= DAY_LIMIT)
	{
		m_fGameOverScreenTimer -= DELTA_TIME();
		((CPlayer*)m_cpThePlayer)->GetHUD()->bOff = true;

		if (m_fGameOverScreenTimer >= 0.0f)
			m_cGameOverScreen->GetTransparencyOverride() += DELTA_TIME() / 4.0f;
		else if (m_fGameOverScreenTimer <= 0.0f && m_bGameOver == false)
		{
			m_cGameOverScreen->GetTransparencyOverride() = 1.0f;
			bLose = true;
			m_bGameOver = true;
		}
	}
	else if (((CPlayer*)m_cpThePlayer)->GetHUD()->GetCurrentDay() == DAY_LIMIT - 1 && m_fDayPromptTimer >= 0.0f)
	{
		m_fDayPromptTimer -= DELTA_TIME();
		GRAPHICS->RenderText(L"Last Day!", { 0, 450.0f }, { 1, 1, 1, 1 }, { 1.25f, 1.25f }, Center);
	}

	if (m_fObjectiveTimer >= 0.0f)
	{
		m_fObjectiveTimer -= DELTA_TIME();
		GRAPHICS->RenderText(L"Find an Exit to Escape!", { 0, 650.0f }, { 1, 1, 1, 1 }, { 1, 1 }, Center);
		GRAPHICS->RenderText((L"You only have " + to_wstring(DAY_LIMIT - ((CPlayer*)m_cpThePlayer)->GetHUD()->GetCurrentDay() + 1) + L" days!").c_str(), { 0, 450.0f }, { 1, 1, 1, 1 }, { 1, 1 }, Center);
	}

	if (m_bPaused == false && m_bControllerPopUp == false && m_bControlsON == false)
	{
		m_cpAnimationManager->Update();
		m_cpObjectManager->UpdateAll();
		m_cpSpawnControl->Update();
		m_pcDayNight->Update();
		m_pcDayNight->UpdateDirectionalLight(m_tDirectLight);
		m_pcDayNight->UpdateSkybox(m_cpSkybox);
		CMessageManager::GetInstance()->Update();
		m_cpParticleManager->Update();

		//update the player's hud's time of day
		((CPlayer*)m_cpThePlayer)->GetHUD()->SetWorldTime(m_pcDayNight->GetTimeOfDay());

	}
	//DEBUG_UTIL::DrawNavGraph(CNavGraphManager::GetReference().GetNavGraph("MinotaurNavGraph"));
	//DEBUG_UTIL::DrawNavGraph(CNavGraphManager::GetReference().GetNavGraph("SkeletonNavGraph"));


	m_cpCutsceneManager->Update();
	//::DrawNavGraph(CNavGraphManager::GetReference().GetNavGraph("MinotaurNavGraph"));
}

void CGameplayState::ReInitialize()
{
	//static objects
	for (unsigned int i = 0; i < m_cpObjectManager->GetList(CObjectManager::Static).size(); i++)
		m_cpObjectManager->GetList(CObjectManager::Static)[i]->AddToRenderer();

	//dynamic objects
	for (unsigned int i = 0; i < m_cpObjectManager->GetList(CObjectManager::Dynamic).size(); i++)
		m_cpObjectManager->GetList(CObjectManager::Dynamic)[i]->AddToRenderer();

	//AI objects
	for (unsigned int i = 0; i < m_cpObjectManager->GetList(CObjectManager::AI).size(); i++)
		m_cpObjectManager->GetList(CObjectManager::AI)[i]->AddToRenderer();

	//inventory reint
	((CPlayer*)m_cpThePlayer)->GetInventory()->AddToRenderer();

	//skybox
	if (m_cpSkybox != nullptr)
		GRAPHICS->AddRenderMesh(m_cpSkybox);

	//world map reint
	((CPlayer*)m_cpThePlayer)->GetWorldMap()->AddToRenderer();

	//pause menu background
	if (m_cpTextFrame != nullptr)
		GRAPHICS->AddRenderMesh(m_cpTextFrame);

	for (unsigned int i = 0; i < m_vpPausedControls.size(); i++)
	{
		if (m_vpPausedControls[i] != nullptr)
			GRAPHICS->AddRenderMesh(m_vpPausedControls[i]);
	}

	//f1 menus
	if (m_cpControlsMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_cpControlsMesh);

	if (m_cpRightArrowMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_cpRightArrowMesh);

	if (m_cpLeftArrowMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_cpLeftArrowMesh);

	//controller DC pop-up
	if (m_cpControllerPopUpMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_cpControllerPopUpMesh);

	//game over texture
	if (m_cGameOverScreen != nullptr)
		GRAPHICS->AddRenderMesh(m_cGameOverScreen);

	//HUD
	((CPlayer*)m_cpThePlayer)->GetHUD()->AddToRenderer();

	//directional light
	if (m_tDirectLight != nullptr)
		GRAPHICS->SetDirectionalLight(m_tDirectLight);

	m_cpSpawnControl->AddToRenderer();

	bLose = false;
}
