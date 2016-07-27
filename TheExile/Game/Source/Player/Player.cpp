
#include "Player.h"

#include "../../../Engine/Core/Collision/Collision.h"
#include "../../../Engine/Object Manager/ObjectManager.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include <iostream>
#include "../../../Engine/Renderer/Renderer.h"
#include "../../../Engine/Renderer/HUDMesh.h"
#include "../../../Engine/Core/Collision/Collider.h"
#include "../../../Engine/Core/Collision/Capsule.h"
#include "../../../Engine/Core/State Machine/StateMachine.h"
#include "../../../Engine/Core/State Machine/States/GameplayState.h"
#include "../../../Engine/Core/Messages/WinConditionMessage.h"
#include "../../../Engine/Animation/AnimationManager.h"
#include "../../../Engine/Animation/AnimationMesh.h"
#include "../../../Engine/Object Manager/TestObject.h"
#include "../../../Engine/Core/Messages/AddObjectMessage.h"
#include "../../../Engine/Core/Messages/RemoveObjectMessage.h"
#include "../../../Engine/Particles/ParticleManager.h"
#include "../../../Engine/Particles/Emitter.h"
#include "../../../Game/Source/Agents/Skeleton/Skeleton.h"
#include "../HUD/WorldMap.h"
#include "../Base/Inventory.h"
#include "../Base/Crafting.h"
#include "Weapon.h"
#include "../Camera/Camera.h"
#include "../Environment/ItemCrate.h"

#include "../Environment/CrackedWall.h"
#include "../Environment/Bridge.h"
#include "../Environment/GraveStone.h"
#include "../Traps/FireTrap.h"
#include "../Traps/SpikeTrap.h"
#include "../Traps/SpinningBlade.h"
#include "../../../Engine/Object Manager/ExitDoorObject.h"

#include "../../../Engine/Object Manager/DoorObject.h"

#include "../HUD/HUD.h"
#include "Attack.h"
#include "Dagger.h"
#include "../Environment/Bone.h"
#include "../Environment/Herb.h"
#include "../Environment/Wood.h"
#include "../Environment/BedObject.h"
#include "../Environment/Torch.h"
#include "../Environment/Gunpowder.h"
#include "../Base/StorageCrate.h"
#include "../Environment/ExitKey.h"
#include "../../../Engine/Core/Messages/ExitKeyFoundMessage.h"
#include "../Environment/DayNight.h"

#if _DEBUG
#endif
//#include "../../../../Core/DebugUtil.h"

#define MAX_SPEED 800
#define MOVE_SPEED 100
#define FRICTION 100
#define KILL_VELOCITY 100
#define JUMP_SPEED 750
#define GRAVITY -1500.0f

#define LIGHT_ATTACK_STAMINA -20.0f
#define STAMINA_DRAUGHT 2.0f
#define STAMINA_REGEN_RATE 10.0f
#define MAX_HEALTH_LEVEL 3

#define BLINK_TIME 0.175f

#define DODGE_TIME 1.0f
#define HEAVY_ATTACK_TIME 1.5f
#define LIGHT_ATTACK_TIME 1.25
#define SPAWN_HEIGHT 10.0f
#define LEFT_HAND "RightHand"
#define RIGHT_HAND ""

#define HEALTHPOT_HEAL 50

bool NearlyEqual(float a, float b);

CPlayer::CPlayer(CObjectManager * _manager, CAnimationManager * _animManager) : IObject("Player")
{
	m_cObjectManager = _manager;

	m_fFriction = FRICTION;
	m_bIsBuilding = false;
	m_bCancelBuild = false;
	m_fTotalPlayTime = 0.0f;

	m_cpDayNight = nullptr;
	m_bLastNight = false;

	m_bExitKeyFound = false;

	// Drop Shadow Creation //
	CMesh cDropShadow;

	cDropShadow.GetIndices().push_back(0);
	cDropShadow.GetIndices().push_back(1);
	cDropShadow.GetIndices().push_back(2);

	cDropShadow.GetIndices().push_back(0);
	cDropShadow.GetIndices().push_back(2);
	cDropShadow.GetIndices().push_back(3);

	cDropShadow.GetVertices().push_back
		({
			{ -100.0f, 3.0f, 100.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 0, 0 }
	});

	cDropShadow.GetVertices().push_back
		({
			{ 100.0f, 3.0f, 100.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 1, 0 }
	});

	cDropShadow.GetVertices().push_back
		({
			{ 100.0f, 3.0f, -100.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 1, 1 }
	});

	cDropShadow.GetVertices().push_back
		({
			{ -100.0f, 3.0f, -100.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 0, 1 }
	});

	m_cDropShadow = new CRenderMesh(&cDropShadow, GRAPHICS->GetStandardVS(), GRAPHICS->GetStandardPS());
	m_cDropShadow->SetTexture(L"../Game/Assets/Art/2D/Textures/Shadow.dds");
	m_cDropShadow->GetTransparent() = true;

	GRAPHICS->AddRenderMesh(m_cDropShadow);

#pragma region Keyboard Input Registers

	Input = CInputManager::GetInstance();

	//debug keys
	Input->RegisterKey("CheatModifier", eKey::Control);

	//numpad keys
	Input->RegisterKey("GodMode", eKey::NumPad0);
	//numpad1 = exit	
	//numpad2 = speed up time 
	Input->RegisterKey("Wood Plank Cheat", eKey::NumPad3);
	Input->RegisterKey("Bomb Cheat", eKey::NumPad4);
	Input->RegisterKey("Health Potion Cheat", eKey::NumPad5);
	Input->RegisterKey("Bridge Cheat", eKey::NumPad6);
	//Input->RegisterKey("UpgradeWeapon", eKey::NumPad7);
	//Input->RegisterKey("UpgradeShield", eKey::NumPad8);
	//Input->RegisterKey("IncreaseMaxHealth", eKey::NumPad9);

	//num keys
	Input->RegisterKey("ExitKey Cheat", eKey::Num6);
	Input->RegisterKey("Herb Cheat", eKey::Num7);
	Input->RegisterKey("Wood Cheat", eKey::Num8);
	Input->RegisterKey("Bone Cheat", eKey::Num9);
	Input->RegisterKey("Gunpowder Cheat", eKey::Num0);

	//player actions
	Input->RegisterKey("MoveForward", eKey::W);
	Input->RegisterKey("MoveLeft", eKey::A);
	Input->RegisterKey("MoveBackward", eKey::S);
	Input->RegisterKey("MoveRight", eKey::D);
	Input->RegisterKey("Jump", eKey::Spacebar);
	Input->RegisterKey("LightAttack", eKey::LeftMouse);
	Input->RegisterKey("HeavyAttack", eKey::RightMouse);
	Input->RegisterKey("Interact", eKey::F);
	Input->RegisterKey("Inventory", eKey::B);
	//Input->RegisterKey("Crafting", eKey::C);
	Input->RegisterKey("Map", eKey::Tab);
	Input->RegisterKey("HealthPotion", eKey::Q);
	Input->RegisterKey("Block", eKey::MiddleMouse);
	Input->RegisterKey("SwitchWeapon", eKey::E);
	//Input->RegisterKey("SwitchSword", eKey::Num1);
	//Input->RegisterKey("SwitchAxe", eKey::Num2);
	//Input->RegisterKey("SwitchSpear", eKey::Num3);
	Input->RegisterKey("OpenDoor", eKey::P);

#pragma endregion

	XMStoreFloat4x4(&m_mWorld,/* XMMatrixScaling(0.15f, 0.15f, 0.15f) **/ XMMatrixTranslation(0.0f, SPAWN_HEIGHT, 0.0f));
	m_tSpawnPoint = XMFLOAT3(0.0f, SPAWN_HEIGHT, 0.0f);
	//XMStoreFloat4x4(&m_mWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_mWorld), XMMatrixScaling(0.5f, 0.1f, 0.1f)));
	m_MousePrevPos = { 0, 0 };


	CMesh* playerMesh = CAssetManager::GetInstance()->GetPrefabMesh("PlayerMesh");

	AddCollider(new CCollider(true, new CAABB(float3(m_mWorld._41, 90.0f, m_mWorld._43), float3(90.0f, 90.0f, 90.0f)), false, false));

	CCapsule* playerCapsule = new CCapsule(XMFLOAT3(0.0f, SPAWN_HEIGHT + 45.0f, 0.0f), XMFLOAT3(0.0f, SPAWN_HEIGHT + 135.0f, 0.0f), 45.0f);
	CCollider* playerCollider = new CCollider(true, playerCapsule);
	AddCollider(playerCollider);

	m_pAnimationManager = _animManager;

	//hud
	m_cpTheHUD = new CHUD();
	m_nHealth = 100;

	//Creating Spear Matrix
	XMMATRIX mSpear = XMMatrixIdentity();
	XMMATRIX mSpearRotation = XMMatrixRotationAxis(XMLoadFloat3(&XMFLOAT3(1.0f, 0.0f, 0.0f)), RADIANS(90.0f));
	XMStoreFloat4x4(&m_mSpearLocal, mSpear * mSpearRotation);

	//Mesh and animations
	m_cpRenderMesh = new CAnimationMesh(playerMesh, GRAPHICS->GetSkinningVS(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, m_sPlayerTexture.c_str());
	m_cpRenderMesh->SetEdgeGlow(true);
	m_cpRenderMesh->SetNormals(L"../Game/Assets/Art/2D/Textures/PlayerNormal.dds");
	//((CAnimationMesh*)m_cpRenderMesh)->m_cMesh = (CAssetManager::GetInstance()->GetMesh(CAssetManager::PlayerMesh)[0]);
	m_cObjectManager->AddObject(this, CObjectManager::Dynamic);
	GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	CMesh* playerSword = CAssetManager::GetInstance()->GetPrefabMesh("PlayerSword");
	CMesh* playerAxe = CAssetManager::GetInstance()->GetPrefabMesh("PlayerAxe");
	CMesh* playerSpear = CAssetManager::GetInstance()->GetPrefabMesh("PlayerSpear");

	m_cSword = new CWeapon(playerSword, L"../Game/Assets/Art/2D/Textures/FailBot_Sword2.dds", nullptr, 25, LIGHT_ATTACK_TIME, "Dagger");
	m_cAxe = new CWeapon(playerAxe, L"../Game/Assets/Art/2D/Textures/AxeDiffuse.dds", L"../Game/Assets/2D/Normal Maps/AxeNormal.dds", 50, HEAVY_ATTACK_TIME, "Axe");
	m_cSpear = new CWeapon(playerSpear, L"../Game/Assets/Art/2D/Textures/FailBot_Sword2.dds", nullptr, 25, HEAVY_ATTACK_TIME, "Spear");

	m_cLightAttack = new CAttack(this, playerSword, &m_cSword->GetRenderMesh()->GetPositionMatrix(), 20, "PlayerAttack");
	m_cSword->SetAttack(m_cLightAttack);

	m_cAxeAttack = new CAttack(this, playerAxe, &m_cAxe->GetRenderMesh()->GetPositionMatrix(), 34, "PlayerAttack");
	m_cAxe->SetAttack(m_cAxeAttack);

	m_cSpearAttack = new CAttack(this, playerSpear, &m_cAxe->GetRenderMesh()->GetPositionMatrix(), 34, "PlayerAttack");
	m_cSpear->SetAttack(m_cSpearAttack);

	CAddObjectMessage * msg2 = new CAddObjectMessage(m_cLightAttack, CObjectManager::Dynamic);
	msg2->Send();

	CAddObjectMessage * msg3 = new CAddObjectMessage(m_cAxeAttack, CObjectManager::Dynamic);
	msg3->Send();

	CAddObjectMessage * msg4 = new CAddObjectMessage(m_cSpearAttack, CObjectManager::Dynamic);
	msg4->Send();

	m_cCurrentAttack = m_cLightAttack;



	//shieldWorld *= XMMatrixRotationY(-37.0f);




	AttachToJoint(m_cSword->GetRenderMesh(), LEFT_HAND);
	AttachToJoint(m_cAxe->GetRenderMesh(), LEFT_HAND);
	AttachToJoint(m_cSpear->GetRenderMesh(), LEFT_HAND);
	m_cSword->Activate();
	m_cCurrentWeapon = m_cSword;

	m_vpAnimations = new CAnimation[eAnimCount];
	//Adding animations and animation specific values
	m_vpAnimations[eIdleAnim] = CAssetManager::GetInstance()->GetAnimation("PlayerIdle");
	m_vpAnimations[eIdleAnim].SetLooping(true);

	m_vpAnimations[eRunAnim] = CAssetManager::GetInstance()->GetAnimation("PlayerRunForward");
	m_vpAnimations[eRunAnim].SetLooping(true);

	m_vpAnimations[eHeavyRightAttackAnim] = CAssetManager::GetInstance()->GetAnimation("PlayerSwingDown");
	m_vpAnimations[eHeavyRightAttackAnim].SetScale(0.15f);

	m_vpAnimations[eHeavyLeftAttackAnim] = CAssetManager::GetInstance()->GetAnimation("PlayerSwingRight");
	m_vpAnimations[eHeavyLeftAttackAnim].SetScale(0.15f);

	m_vpAnimations[eLightRightAttackAnim] = CAssetManager::GetInstance()->GetAnimation("PlayerSwingUp");
	m_vpAnimations[eLightRightAttackAnim].SetScale(0.15f);

	m_vpAnimations[eLightLeftAttackAnim] = CAssetManager::GetInstance()->GetAnimation("PlayerSwingRight");
	m_vpAnimations[eLightLeftAttackAnim].SetScale(0.15f);

	m_vpAnimations[eHitAnim] = CAssetManager::GetInstance()->GetAnimation("PlayerHurt1");

	m_vpAnimations[eDeathAnim] = CAssetManager::GetInstance()->GetAnimation("PlayerDeathForward");
	m_vpAnimations[eJumpAnim] = CAssetManager::GetInstance()->GetAnimation("PlayerRunningJump");
	m_vpAnimations[eJumpAnim].SetScale(1.25);

	for (unsigned int animation = 0; animation < eAnimCount; animation++)
	{
		m_vpAnimations[animation].SetAnimMesh((CAnimationMesh*)m_cpRenderMesh);
	}

	m_pAnimationManager->AddAnimation(&m_vpAnimations[eIdleAnim]);
	m_vpCurrAnim = eIdleAnim;

	// Inventory
	m_cpTheInventory = new CInventory();

	//Map
	m_cpMap = new CWorldMap(this);

	//audio
	m_cpAudio = m_cpAudio->Get();



	m_cpParticleManager = new CParticleManager(m_cObjectManager);
	m_unParticles[eBloodParticle] = m_cpParticleManager->LoadEmitter("BloodDrop.xml");
	m_unParticles[eBloodString] = m_cpParticleManager->LoadEmitter("BloodString.xml");
	m_unParticles[eBloodMist] = m_cpParticleManager->LoadEmitter("BloodMist.xml");
	m_unParticles[eBloodMist2] = m_cpParticleManager->LoadEmitter("BloodMist.xml");
	m_unParticles[eSwordParticle] = m_cpParticleManager->LoadEmitter("SwordSlash.xml");
	m_unParticles[eXSlashParticleLeft] = m_cpParticleManager->LoadEmitter("XSlashLeft.xml");
	m_unParticles[eXSlashParticleRight] = m_cpParticleManager->LoadEmitter("XSlashRight.xml");

	m_vpGraveStones.push_back(new CGraveStone(m_cObjectManager, this));
	m_vpGraveStones[0]->SetID(0);

	m_unParticles[eHealthPotionYellow] = m_cpParticleManager->LoadEmitter("HealthPotionYellow.xml");
	m_cpParticleManager->GetEmitter(m_unParticles[eHealthPotionYellow])->GetParent() = this;
	m_unParticles[eHealthPotionGreen] = m_cpParticleManager->LoadEmitter("HealthPotionGreen.xml");
	m_cpParticleManager->GetEmitter(m_unParticles[eHealthPotionGreen])->GetParent() = this;

	vector<TVertex2D> TextVerts;
	vector<unsigned int> TextInd;

	TextInd.push_back(0);
	TextInd.push_back(1);
	TextInd.push_back(2);

	TextInd.push_back(0);
	TextInd.push_back(2);
	TextInd.push_back(3);

	float fStartX = -0.75f;
	float fStartY = 0.55f;
	float fWidth = 1.5f;
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

	m_fExitDoorPromptTimer = 0.0f;
	m_bExitDoorPrompt = false;


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

	m_cpControllerDoorPopUp = new CHUDMesh(tempInd, tempVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Door_Controller_PopUp.dds");
	m_cpControllerDoorPopUp->GetRender() = false;
	m_cpControllerDoorPopUp->GetTransparent() = true;
	TranslateLocalY(m_cpControllerDoorPopUp->GetTransformMatrix(), 0.4f);
	GRAPHICS->AddRenderMesh(m_cpControllerDoorPopUp);

#pragma endregion

}

CPlayer::~CPlayer()
{
	SAFE_DELETE(m_cDropShadow);
	SAFE_DELETE(m_cpTheHUD);
	SAFE_DELETE(m_cLightAttack)
		SAFE_DELETE(m_cAxeAttack);
	SAFE_DELETE(m_cSpearAttack);
	SAFE_DELETE(m_cpParticleManager);
	SAFE_DELETE(m_cSword);
	SAFE_DELETE(m_cHeavyWeapon);
	SAFE_DELETE(m_cpTheInventory);
	SAFE_DELETE(m_cpMap);
	SAFE_DELETE(m_cAxe);
	SAFE_DELETE(m_cAxeAttack);
	SAFE_DELETE(m_cpTextFrame);
	SAFE_DELETE(m_cSpear);
	SAFE_DELETE(m_cpControllerDoorPopUp);

	delete[] m_vpAnimations;
	m_vpAnimations = nullptr;
	VECTOR_DELETE_ALL(m_vpGraveStones);
	VECTOR_DELETE_ALL(m_vpOldGraves);


}

void CPlayer::Update()
{
	m_fTotalPlayTime += DELTA_TIME();

	unsigned int unActiveTombestoneCounter = 0;
	for (unsigned int i = 0; i < m_vpGraveStones.size(); i++)
	{
		if (m_vpGraveStones[i]->GetActive() == true)
		{
			unActiveTombestoneCounter++;
		}
	}

	if (unActiveTombestoneCounter == 0)
	{
		m_cpMap->DeActivateTombstones();
	}

	// Update Pickup Animations //


	if (GetPosition()->y < -900.0f)
	{
		TakeDamage(500);
	}


	if (m_bActiveUpdate == false)
		return;

	//Update Weapon Position
	if (!m_bIsBlocking && !m_bStaminaDraught && !m_bAttacking)
	{
		AdjustStamina(STAMINA_REGEN_RATE * DELTA_TIME());
	}

	m_cpParticleManager->Update();
	if (m_fInvincibleTimer > 0.0f && m_bInvincible)
	{
		m_fBlinkTimer -= DELTA_TIME();
		if (m_fBlinkTimer < 0.0f)
		{
			m_fBlinkTimer = BLINK_TIME;
			DamageBlink();
		}

		m_fInvincibleTimer -= DELTA_TIME();
		if (m_fInvincibleTimer < 0.0f)
		{
			m_bInvincible = false;

			m_cpRenderMesh->SetTexture(m_sPlayerTexture.c_str());
		}
	}

	if (!m_bPlayerAlive)
	{
		m_f3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_fRespawnTimer -= DELTA_TIME();
		if (m_fRespawnTimer < 0.0f)
		{
			XMFLOAT4X4 oldWorldMAtrix = m_mWorld;
			SetPosition(m_tSpawnPoint);
			for (unsigned int collider = 0; collider < m_pvColliders.size(); collider++)
			{
				XMFLOAT3 f3VelocityUpdate;
				f3VelocityUpdate.x = m_mWorld._41 - oldWorldMAtrix._41;
				f3VelocityUpdate.y = m_mWorld._42 - oldWorldMAtrix._42;
				f3VelocityUpdate.z = m_mWorld._43 - oldWorldMAtrix._43;

				m_pvColliders[collider]->UpdatePosition(f3VelocityUpdate);
			}
			m_nHealth = 100;
			m_bPlayerAlive = true;
		}
	}
	else
	{
		//if (Input->IsKeyPressed("SwitchSword"))
		//{
		//	SwitchWeapon(m_cSword);
		//}
		//
		//if (Input->IsKeyPressed("SwitchAxe") && m_eCurrentWeaponLevel > eShortSword)
		//{
		//	SwitchWeapon(m_cAxe);
		//}
		//
		//if (Input->IsKeyPressed("SwitchSpear") && m_eCurrentWeaponLevel > eSword)
		//{
		//	SwitchWeapon(m_cSpear);
		//}

		if (Input->IsKeyPressed("SwitchWeapon") || Input->IsButtonPressed("Weapon Switch"))
		{
			//if current weapon is the sword and the axe is unlocked, then switch to the axe
			if (m_cCurrentWeapon->GetString() == m_cSword->GetString() && m_eCurrentWeaponLevel == eAxe)
			{
				SwitchWeapon(m_cAxe);
			}
			else if (m_cCurrentWeapon->GetString() == m_cAxe->GetString())//if current weapon is axe, switch to sword
			{
				SwitchWeapon(m_cSword);
			}

			//if (m_cCurrentWeapon->GetString() == m_cSword->GetString() && m_eCurrentWeaponLevel > eShortSword)
			//{
			//	SwitchWeapon(m_cAxe);
			//}
			//else if (m_cCurrentWeapon->GetString() == m_cAxe->GetString())
			//{
			//	if (m_eCurrentWeaponLevel > eSword)
			//		SwitchWeapon(m_cSpear);
			//	else
			//		SwitchWeapon(m_cSword);
			//}
			//else if (m_cCurrentWeapon->GetString() == m_cSpear->GetString())
			//{
			//	SwitchWeapon(m_cSword);
			//}
		}

		BlockLogic();

		//Movement controls
		Movement();

		//Mouse turn controls
		if (Input->GetControllerBeingUsed() == true)
		{
			JoystickTurn();
		}
		else
		{
			MouseTurn();
		}


		//Attack controls and logic
		Attacking();

		if (Input->IsKeyPressed("Inventory") || Input->IsButtonPressed("Inventory"))
		{
			if (m_cpTheInventory->IsOpen())
			{
				m_cpTheInventory->Close();
			}
			else if (/*!m_cpCraftingTable->IsOpen() &&*/ m_cpStorageCrate->IsOpen() == false)
			{
				m_cpMap->Close();
				m_cpTheInventory->Open();
			}
		}
		if (Input->IsKeyPressed("Map") || Input->IsButtonPressed("Map"))
		{
			if (m_cpMap->IsOpen())
			{
				m_cpMap->Close();
			}
			else
			{
				m_cpStorageCrate->Close();
				//m_cpCraftingTable->Close();
				m_cpTheInventory->Close();
				m_cpMap->Open();
			}
		}
		if (Input->IsKeyPressed("HealthPotion") || Input->IsButtonPressed("Health Potion"))
		{
			if (m_cpTheInventory->GetStock(CInventory::HealthPotion) > 0)
			{
				AudioSystemWwise::Get()->PostEvent(AK::EVENTS::HEALTHPOTION, *GetPosition());
				m_cpTheInventory->RemoveFromInventory(CInventory::HealthPotion, 1);
				m_nHealth += HEALTHPOT_HEAL;
				if (m_nHealth > m_nMaxHealth)
					m_nHealth = m_nMaxHealth;

				PlayParticle(eHealthPotionYellow, { 0, 75, 0 });
				PlayParticle(eHealthPotionGreen, { 0, 75, 0 });
			}
		}
	}

	Animation();



	//hud update
	m_cpTheHUD->Update(m_nHealth, m_nMaxHealth, m_cpTheInventory->GetStock(CInventory::eItems::HealthPotion), 0.0f/* m_fStamina*/);

	//Debug Input
	DebugModeInput();

	for (unsigned int emitter = 0; emitter < eParticleCount; emitter++)
	{
		m_cpParticleManager->GetEmitter(m_unParticles[emitter])->GetPosition(0) = GetPosition()->x;
		m_cpParticleManager->GetEmitter(m_unParticles[emitter])->GetPosition(1) = GetPosition()->y;
		m_cpParticleManager->GetEmitter(m_unParticles[emitter])->GetPosition(2) = GetPosition()->z;
	}

	//Update Map
	m_cpMap->Update();
	m_cpTheInventory->Update();

	if (m_bIsStunned)
	{
		m_fStunBlinkTimer -= DELTA_TIME();
		if (m_fStunBlinkTimer < 0.0f)
		{
			m_fStunBlinkTimer = BLINK_TIME;
			StunBlink();
		}

		m_fStunTimer -= DELTA_TIME();
		if (m_fStunTimer < 0.0f)
		{
			m_bIsStunned = false;
			m_cCamera->GetLocked() = false;

			m_cpRenderMesh->SetTexture(m_sPlayerTexture.c_str());
		}
	}

	// Prompt Updates //
	if (m_bHavenExitPrompt) // Haven door prompt
	{
		m_bHavenExitPrompt = false;
		m_cpTextFrame->GetRender() = true;
		GRAPHICS->RenderText(L"Press 'P' to open door", { 0, 550 }, { 0, 0, 0, 0 }, { 1, 1 }, Center);
	}
	else if (m_bExitDoorPrompt) // Exit door prompt
	{
		m_bExitDoorPrompt = false;
		m_cpTextFrame->GetRender() = true;
		GRAPHICS->RenderText(L"Door Locked. Find the key.", { 0, 540 }, { 0, 0, 0, 0 }, { 0.85f, 0.8f }, Center);

		if (m_bLockSoundPlayed == false)
		{
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::MENUENTER, { 0, 0, 0 });
			m_bLockSoundPlayed = true;
		}
	}
	else if (m_cpTextFrame->GetRender())
		m_cpTextFrame->GetRender() = false;
	else
		m_bLockSoundPlayed = false;

	//door pop up for controller
	if (m_cpControllerDoorPopUp->GetRender() == true)
	{
		m_cpControllerDoorPopUp->GetRender() = false;
	}

#if _DEBUG
	////DEBUG
	CCapsule* pCapsule = (CCapsule*)m_pvColliders[1]->GetBounds();
	CAABB* pAABB = (CAABB*)m_pvColliders[0]->GetBounds();

	//XMFLOAT3 f3LineAbove = pCapsule->GetEnd();
	//f3LineAbove.y += 300.0f;
	//GRAPHICS->DrawLine(f3LineAbove, pCapsule->GetStart(), XMFLOAT3(0, 1, 0));
	DEBUG_UTIL::DrawAABBCollider(pAABB->Min(), pAABB->Max());
	DEBUG_UTIL::DrawCapsuleCollider(pCapsule->GetStart(), pCapsule->GetEnd(), pCapsule->GetRadius());
#endif

	//DEBUG::DrawWorldAxis(*GetPosition());
	//CCapsule* pCapsule = (CCapsule*)m_pvColliders[1]->GetBounds();
	//CAABB* pAABB = (CAABB*)m_pvColliders[0]->GetBounds();

	////XMFLOAT3 f3LineAbove = pCapsule->GetEnd();
	////f3LineAbove.y += 300.0f;
	////GRAPHICS->DrawLine(f3LineAbove, pCapsule->GetStart(), XMFLOAT3(0, 1, 0));
	//DEBUG::DrawAABBCollider(pAABB->Min(), pAABB->Max());
	//DEBUG::DrawCapsuleCollider(pCapsule->GetStart(), pCapsule->GetEnd(), pCapsule->GetRadius());

	SnapPosition(m_cDropShadow->GetTransformMatrix(), m_mWorld);

	TRay tRay;

	tRay.f3Direction.x = 0;
	tRay.f3Direction.y = -1;
	tRay.f3Direction.z = 0;
	tRay.f3Start.x = m_mWorld.m[3][0];
	tRay.f3Start.y = m_mWorld.m[3][1] + 5.0f;
	tRay.f3Start.z = m_mWorld.m[3][2];
	tRay.fDistance = 2000.0f;

	float fHitTime = 0.0f;
	std::vector<IObject*> cObjectList = m_cObjectManager->GetQuadTree()->GetNearbyObjects(m_mWorld.m[3][0] - 300.0f, m_mWorld.m[3][2] - 300.0f, m_mWorld.m[3][0] + 300.0f, m_mWorld.m[3][2] + 300.0f);

	LineSegmentIntersection(tRay, cObjectList, fHitTime);
	fHitTime -= 5.0f;
	m_cDropShadow->GetTransformMatrix().m[3][1] = m_mWorld.m[3][1] - fHitTime;
}

//void CPlayer::UpdatePosition()
//{
//
//	XMFLOAT4X4 oldWorldMAtrix = m_mWorld;
//
//	XMMATRIX d3dTranslateZ = XMMatrixTranslation(m_f3Velocity.x, m_f3Velocity.y, m_f3Velocity.z);
//	XMStoreFloat4x4(&m_mWorld, XMMatrixMultiply(d3dTranslateZ, XMLoadFloat4x4(&m_mWorld)));
//	m_cpRenderMesh->GetPositionMatrix() = m_mWorld;
//
//	m_pvColliders[0]->UpdatePosition(m_pvColliders[0]->GetResetVelocity());
//	XMFLOAT3 f3VelocityUpdate;
//	f3VelocityUpdate.x = m_mWorld._41 - oldWorldMAtrix._41;
//	f3VelocityUpdate.y = m_mWorld._42 - oldWorldMAtrix._42;
//	f3VelocityUpdate.z = m_mWorld._43 - oldWorldMAtrix._43;
//
//	m_pvColliders[0]->UpdatePosition(f3VelocityUpdate);
//
//	CCapsule* pCapsule = (CCapsule*)m_pvColliders[1]->GetBounds();
//
//	XMFLOAT3 f3NewStart = pCapsule->GetStart();
//	XMFLOAT3 f3NewEnd = pCapsule->GetEnd();
//
//	f3NewStart.x = f3NewEnd.x = m_mWorld._41;
//	f3NewStart.z = f3NewEnd.z = m_mWorld._43;
//
//	pCapsule->SetStart(f3NewStart);
//	pCapsule->SetEnd(f3NewEnd);
//
//}

void CPlayer::CollisionResponse(IObject * _cCollidedObject)
{
	if (_cCollidedObject->GetTag() == "WeaponChest"){
		UnlockWeapon();
		_cCollidedObject->GetRenderMesh()->GetRender() = false;
		_cCollidedObject->GetColliders()[0]->SetActive(false);

		CItemPickupAnimation* cAnimation = new CItemPickupAnimation(CAssetManager::GetInstance()->GetPrefabMesh("PlayerAxeSmall"), L"../Game/Assets/Art/2D/Textures/AxeDiffuse.dds", this, m_cCamera);
		m_cAnimations.push_back(cAnimation);
	}
	else if (_cCollidedObject->GetTag() == "Prop")
	{
		CapsuleToCapsule(this, m_pvColliders[1], _cCollidedObject->GetColliders()[1]);
	
	}
	else if (_cCollidedObject->GetTag() == "Wood")
	{
		if (((CWood*)(_cCollidedObject))->IsActive())
		{
			((CWood*)(_cCollidedObject))->DeActivate();
			m_cpTheInventory->AddToInventory(CInventory::Wood, 1);

		}
	}
	else if (_cCollidedObject->GetTag() == "ShieldChest"){
		//UpgradeShield();
		_cCollidedObject->GetRenderMesh()->GetRender() = false;
		_cCollidedObject->GetColliders()[0]->SetActive(false);
	}

	else if (_cCollidedObject->GetTag() == "HealthChest"){
		if (m_nCurrentHealthLevel < MAX_HEALTH_LEVEL)
		{
			m_nCurrentHealthLevel++;
			m_nMaxHealth += 25;
			m_nHealth += 25;
			m_cpTheHUD->SetMaxHealth(m_nMaxHealth);
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::HEALTHUPGRADE, *GetPosition());
			_cCollidedObject->GetRenderMesh()->GetRender() = false;
			_cCollidedObject->GetColliders()[0]->SetActive(false);
		}
	}
	else if (_cCollidedObject->GetTag() == "ItemChest")
	{

		CItemCrate* crate = (CItemCrate*)_cCollidedObject;
		crate->SetActive(false);
		int itemId = 5;
		int itemNum = 3;
		CItemPickupAnimation* cAnimation = nullptr;
		switch (itemId)
		{
		case 0: // Bone
			m_cpTheInventory->AddToInventory(CInventory::eItems::Bones, itemNum);

			cAnimation = new CItemPickupAnimation(CAssetManager::GetInstance()->GetPrefabMesh("BonePile"), L"../Game/Assets/Art/2D/Textures/Failbot_BoneLoot.dds", this, m_cCamera);
			m_cAnimations.push_back(cAnimation);
			break;
		case 1:// Wood
			m_cpTheInventory->AddToInventory(CInventory::eItems::Wood, itemNum);

			cAnimation = new CItemPickupAnimation(CAssetManager::GetInstance()->GetPrefabMesh("WoodPile"), L"../Game/Assets/Art/2D/Textures/Failbot_Wood.dds", this, m_cCamera);
			m_cAnimations.push_back(cAnimation);
			break;
		case 2://Herb
			m_cpTheInventory->AddToInventory(CInventory::eItems::Herb, itemNum);

			cAnimation = new CItemPickupAnimation(CAssetManager::GetInstance()->GetPrefabMesh("Herb"), L"../Game/Assets/Art/2D/HUD/HealthBarGreen.dds", this, m_cCamera);
			m_cAnimations.push_back(cAnimation);
			break;
		case 3://Gun Poweder
			m_cpTheInventory->AddToInventory(CInventory::eItems::Gunpowder, itemNum);

			cAnimation = new CItemPickupAnimation(CAssetManager::GetInstance()->GetPrefabMesh("GunPowder"), L"../Game/Assets/Art/2D/Textures/Failbot_Gunpowder.dds", this, m_cCamera);
			m_cAnimations.push_back(cAnimation);
			break;
		case 4: //Bomb
		case 5: //Health Potion
			itemNum = 1;
			if (itemId == 4)
			{
				m_cpTheInventory->AddToInventory(CInventory::eItems::Bomb, itemNum);

				cAnimation = new CItemPickupAnimation(CAssetManager::GetInstance()->GetPrefabMesh("WallBomb"), L"../Game/Assets/Art/2D/Textures/Failbot_WallBomb.dds", this, m_cCamera);
				m_cAnimations.push_back(cAnimation);
			}
			else
			{
				m_cpTheInventory->AddToInventory(CInventory::eItems::HealthPotion, itemNum);
				cAnimation = new CItemPickupAnimation(CAssetManager::GetInstance()->GetPrefabMesh("HealthPotion"), L"../Game/Assets/Art/2D/Textures/HealthPotion.dds", this, m_cCamera);
				m_cAnimations.push_back(cAnimation);
			}
			break;
		}


	}

	else if ((_cCollidedObject->GetTag() == "SkeletonAttack" || _cCollidedObject->GetTag() == "OrcMelee") && !m_bInvincible)
	{
		((CAttack*)_cCollidedObject)->SetIsActive(false);
		TakeDamage(((CAttack*)_cCollidedObject)->GetDamage());
		Stun(0.3f);
	}

	else if (_cCollidedObject->GetTag() == "Floor")
	{

		if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
		{
			OUTPUT("Collided With Floor\n");
			CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[1]);
			m_bIsGrounded = true;
		}

	}
	else if (_cCollidedObject->GetTag() == "TrapPrefab")
	{
		if (m_fFriction != FRICTION)
			m_fFriction = FRICTION;
		for (size_t i = 1; i < _cCollidedObject->GetColliders().size(); i++)
		{
			if (_cCollidedObject->GetColliders()[i]->GetCollided())
			{
				if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::AABB)
				{
					if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
					{
						CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
						m_bIsGrounded = true;
					}
					else
						CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);

					_cCollidedObject->GetColliders()[i]->SetCollided(false);
				}
				else if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::Plane)
				{
					if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
					{
						CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
						_cCollidedObject->GetColliders()[i]->SetCollided(false);
						m_bIsGrounded = true;
					}
				}
			}
		}
	}
	else if (_cCollidedObject->GetTag() == "SafeHaven")
	{
		if (m_fFriction != FRICTION)
			m_fFriction = FRICTION;
		for (size_t i = 1; i < _cCollidedObject->GetColliders().size(); i++)
		{
			if (_cCollidedObject->GetColliders()[i]->GetCollided())
			{
				if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::AABB)
				{
					CAABB* bounds = (CAABB*)(_cCollidedObject->GetColliders()[i]->GetBounds());
					if ((bounds->GetExtents().x == 500.0f &&
						bounds->GetExtents().y == 300.0f &&
						bounds->GetExtents().z == 500.0f) == false)
						CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);

					_cCollidedObject->GetColliders()[i]->SetCollided(false);
				}
				else if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::Plane)
				{
					if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
					{
						CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
						_cCollidedObject->GetColliders()[i]->SetCollided(false);
						m_bIsGrounded = true;
					}
				}
			}
		}
	}
	/*else if (_cCollidedObject->GetTag() == "MinotaurSpawn")
	{
	if (m_fFriction != FRICTION)
	m_fFriction = FRICTION;
	for (size_t i = 1; i < _cCollidedObject->GetColliders().size(); i++)
	{
	if (_cCollidedObject->GetColliders()[i]->GetCollided())
	{
	if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::AABB)
	{
	CAABB* bounds = (CAABB*)(_cCollidedObject->GetColliders()[i]->GetBounds());

	CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);

	_cCollidedObject->GetColliders()[i]->SetCollided(false);
	}
	else if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::Plane)
	{
	if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
	{
	CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
	_cCollidedObject->GetColliders()[i]->SetCollided(false);
	m_bIsGrounded = true;
	}
	}
	}
	}
	}*/
	else if (_cCollidedObject->GetTag() == "Wall" || _cCollidedObject->GetTag() == "Enemy Cave Wall" || _cCollidedObject->GetTag() == "MinotaurSpawn")
	{
		if (m_fFriction != FRICTION)
			m_fFriction = FRICTION;
		for (size_t i = 1; i < _cCollidedObject->GetColliders().size(); i++)
		{
			if (_cCollidedObject->GetColliders()[i]->GetCollided())
			{
				if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::AABB)
				{
					CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
					_cCollidedObject->GetColliders()[i]->SetCollided(false);
				}
				else if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::Plane)
				{
					if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
					{
						CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
						_cCollidedObject->GetColliders()[i]->SetCollided(false);
						m_bIsGrounded = true;
					}
				}
			}
		}
	}
	else if (_cCollidedObject->GetTag() == "CaveSpawn")
	{
		if (m_fFriction != FRICTION)
			m_fFriction = FRICTION;

		CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
	}
	else if (_cCollidedObject->GetTag() == "CrackedWall")
	{
		// For the last 4 colliders (Bomb colliders)
		for (size_t i = _cCollidedObject->GetColliders().size() - 4; i < _cCollidedObject->GetColliders().size(); i++)
		{
			if ((_cCollidedObject->GetColliders()[i])->GetCollided() && _cCollidedObject->GetColliders()[i]->GetType() == Bounds::AABB)
			{
				CCrackedWall* currWall = reinterpret_cast<CCrackedWall*>(_cCollidedObject);

				if (!currWall->IsActive())
				{
					if (m_cpTheInventory->GetStock(CInventory::Bomb) >= 1)
					{
						currWall->DrawAvailable();

						if (Input->IsKeyPressed("Interact") || Input->IsButtonPressed("Interact"))
						{
							currWall->Activate(true, (int)(i - (_cCollidedObject->GetColliders().size() - 4)));
							m_cpTheInventory->RemoveFromInventory(CInventory::Bomb, 1);
							m_cpMap->AddDestroyedWallIcon(currWall->GetPosition());
							break;
						}
					}
					else
					{
						if (m_cpTheInventory->IsOpen() == false && m_cpMap->IsOpen() == false)
						{
							currWall->DrawUnAvailable();
						}
					}
				}
				_cCollidedObject->GetColliders()[i]->SetCollided(false);
			}
		}

		for (size_t i = 0; i < _cCollidedObject->GetColliders().size() - 4; i++)
		{
			if ((_cCollidedObject->GetColliders()[i])->GetCollided())
			{
				if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::AABB)
				{
					CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
					_cCollidedObject->GetColliders()[i]->SetCollided(false);
				}
				else if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::Plane)
				{
					CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
					_cCollidedObject->GetColliders()[i]->SetCollided(false);
					m_bIsGrounded = true;
				}
			}
		}
	}
	else if (_cCollidedObject->GetTag() == "Bush")
	{
		if (m_fFriction != FRICTION)
			m_fFriction = FRICTION;

		CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
	}
	else if (_cCollidedObject->GetTag() == "Door")
	{
		if (_cCollidedObject->GetColliders()[0]->GetCollided())
		{
			CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
			_cCollidedObject->GetColliders()[0]->SetCollided(false);
			OUTPUT("First Collider\n");
		}
		if (_cCollidedObject->GetColliders()[1]->GetCollided() && m_cpTheHUD->GetWorldTime() > 180.0f)
		{
			OUTPUT("Second Collider\n");

			if (Input->GetControllerBeingUsed() == true)
			{
				if (m_cpTheInventory->IsOpen() == false && m_cpMap->IsOpen() == false)
					m_cpControllerDoorPopUp->GetRender() = true;
			}
			else
			{
				if (m_cpTheInventory->IsOpen() == false && m_cpMap->IsOpen() == false)
					m_bHavenExitPrompt = true;
			}

			if (Input->IsKeyDown("OpenDoor") || Input->IsButtonPressed("Interact"))
				(reinterpret_cast<CDoorObject*>(_cCollidedObject))->TempOpen();

		}
		_cCollidedObject->GetColliders()[1]->SetCollided(false);
	}

	else if (_cCollidedObject->GetTag() == "PitWall")
	{
		if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
		{
			CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
			m_bIsGrounded = true;
		}
		else
		{
			CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
		}
	}

	else if (_cCollidedObject->GetTag() == "SmallPit")
	{
		if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
		{
			for (size_t i = 1; i < _cCollidedObject->GetColliders().size(); i++)
			{
				CCollider* tested = _cCollidedObject->GetColliders()[i];
				if (tested->GetCollided())
				{
					if (tested->GetType() == Bounds::Plane)
					{
						if (((CPlane*)(tested->GetBounds()))->GetCenter().y < 0.0f)
						{
							CapsuleToFloor(this, m_pvColliders[1], tested);
							tested->SetCollided(false);
							m_bIsGrounded = true;
						}
					}
					else
					{
						CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
						_cCollidedObject->GetColliders()[i]->SetCollided(false);
						m_bIsGrounded = true;
					}
				}
			}
		}
		else if (m_f3Velocity.y < 0.0f)
		{
			for (size_t i = 1; i < _cCollidedObject->GetColliders().size() - 2; i++)
			{
				if (_cCollidedObject->GetColliders()[i]->GetCollided())
				{
					CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
					_cCollidedObject->GetColliders()[i]->SetCollided(false);
				}

			}
		}
	}

	else if (_cCollidedObject->GetTag() == "BigPit")
	{
		if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
		{
			for (size_t i = 1; i < _cCollidedObject->GetColliders().size(); i++)
			{
				CCollider* tested = _cCollidedObject->GetColliders()[i];
				if (tested->GetCollided())
				{
					CapsuleToFloor(this, m_pvColliders[1], tested);
					tested->SetCollided(false);
					m_bIsGrounded = true;
				}
			}
		}
		else if (m_f3Velocity.y < 0.0f)
		{
			for (size_t i = 1; i < _cCollidedObject->GetColliders().size() - 1; i++)
			{
				CCollider* collider = _cCollidedObject->GetColliders()[i];
				if (collider->GetCollided())
				{
					CapsuleToWall(this, m_pvColliders[1], collider);
					collider->SetCollided(false);
				}
			}
		}
	}
	else if (_cCollidedObject->GetTag() == "ExitDoor")
	{
		CExitDoorObject* exitDoor = (CExitDoorObject*)_cCollidedObject;

		CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);

		if (exitDoor->GetActive() && m_bExitKeyFound == true)
		{
			CWinConditionMessage* msg = new CWinConditionMessage(true);
			msg->Send();
		}
		else
		{
			if (m_cpTheInventory->IsOpen() == false && m_cpMap->IsOpen() == false)
				m_bExitDoorPrompt = true;
		}

	}
	else if (_cCollidedObject->GetTag() == "SpikeTrap")
	{
		cout << "Spike Trap Hit\n";
		CSpikeTrap* curTrap = reinterpret_cast<CSpikeTrap*>(_cCollidedObject);
		if (curTrap->GetActive() || curTrap->GetReset())
		{
			SetWorldVelocity({ 0.0f, 0.0f, 0.0f });
		}

		if ((curTrap->GetActive() || curTrap->GetReset()) && !m_bIsGrounded)
		{
			TakeDamage(curTrap->GetTrapDamage());
		}

		else if (curTrap->GetActive())
		{
			TakeDamage(curTrap->GetTrapDamage());
		}
		else
		{
			CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
		}
	}
	else if (_cCollidedObject->GetTag() == "SpinningBlade")
	{
		cout << "Spinning Bladd Hit\n";
		CSpinningBlade* curTrap = reinterpret_cast<CSpinningBlade*>(_cCollidedObject);

		XMVECTOR mathPlayer = XMVector3Normalize(XMLoadFloat3(GetPosition()) - XMLoadFloat3(curTrap->GetPosition()));

		m_fFriction = 20.0f;
		mathPlayer *= 500.0f;

		XMFLOAT3 toPlayer; XMStoreFloat3(&toPlayer, mathPlayer);

		SetWorldVelocity(toPlayer);

		TakeDamage(curTrap->GetTrapDamage());
	}
	else if (_cCollidedObject->GetTag() == "FireTrap")
	{
		cout << "FireTrap Hit\n";
		CFireTrap* curTrap = reinterpret_cast<CFireTrap*>(_cCollidedObject);

		if (curTrap->GetActive())
		{
			TakeDamage(curTrap->GetTrapDamage());
		}

	}
	else if (_cCollidedObject->GetTag() == "ExitZone")
	{
		if (m_fFriction != FRICTION)
			m_fFriction = FRICTION;
		for (size_t i = 1; i < _cCollidedObject->GetColliders().size(); i++)
		{
			if (_cCollidedObject->GetColliders()[i]->GetCollided())
			{
				if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::AABB)
				{
					CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
					_cCollidedObject->GetColliders()[i]->SetCollided(false);

				}
				else if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::Plane)
				{
					if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
					{
						CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
						_cCollidedObject->GetColliders()[i]->SetCollided(false);
						m_bIsGrounded = true;
					}
				}
			}
		}
	}
	else if (_cCollidedObject->GetTag() == "Bridge")
	{
		CBridge* curBridge = reinterpret_cast<CBridge*>(_cCollidedObject);
		curBridge->SetCollided(true);

		m_cpMap->BridgeCollided();

		if (curBridge->IsActive() && curBridge->IsIntact())
		{
			if (_cCollidedObject->GetColliders()[1]->GetCollided())
			{
				if (_cCollidedObject->GetColliders()[1]->GetCollided())
					_cCollidedObject->GetColliders()[1]->SetCollided(false);
			}

			for (size_t i = 2; i < 10; i++)
			{
				if (_cCollidedObject->GetColliders()[i]->GetCollided())
				{
					if (GetPosition()->y < 20.0f)
					{
						CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
						_cCollidedObject->GetColliders()[i]->SetCollided(false);
						m_bIsGrounded = true;
					}
					else
					{
						CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
						_cCollidedObject->GetColliders()[i]->SetCollided(false);
					}
				}
			}
			for (size_t i = 10; i < _cCollidedObject->GetColliders().size(); i++)
			{
				if (_cCollidedObject->GetColliders()[i]->GetCollided())
				{

					CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
					_cCollidedObject->GetColliders()[i]->SetCollided(false);
				}
			}
		}
		else if (m_cpTheInventory->GetStock(CInventory::Bridge) >= 8)
		{
			if (Input->IsKeyDown("Interact") || Input->IsButtonDown("Interact"))
			{
				if (m_bCancelBuild == true)
				{
					m_bIsBuilding = false;
					curBridge->InterruptBuild();
					m_bCancelBuild = false;
				}
				else
				{
					m_bIsBuilding = true;
				}

				if (curBridge->BuildProcess() == true)
				{
					if (curBridge->GetRepairable() == false)
					{
						m_cpTheInventory->RemoveFromInventory(CInventory::Bones, 6);
						m_cpTheInventory->RemoveFromInventory(CInventory::Bridge, 8);
					}
					else
					{
						m_cpTheInventory->RemoveFromInventory(CInventory::Bridge, 8);
					}

					AudioSystemWwise::Get()->PostEvent(AK::EVENTS::CRAFT);
					Input->VibrateController(CONTROLLER_SOFT_VIBRATE, CONTROLLER_SOFT_VIBRATE, 0.75f);

					curBridge->Activate();
					m_cpMap->BridgeBuilt();
				}

				/*
				if (curBridge->BuildProcess() == true && curBridge->GetRepairable() == false)
				{
				m_cpTheInventory->RemoveFromInventory(CInventory::Bones, 6);
				m_cpTheInventory->RemoveFromInventory(CInventory::Bridge, 8);

				AudioSystemWwise::Get()->PostEvent(AK::EVENTS::CRAFT);
				Input->VibrateController(CONTROLLER_SOFT_VIBRATE, CONTROLLER_SOFT_VIBRATE, 0.75f);

				curBridge->Activate();
				}
				else if (curBridge->BuildProcess() == true && curBridge->GetRepairable() == true)
				{
				m_cpTheInventory->RemoveFromInventory(CInventory::Bridge, 8);

				AudioSystemWwise::Get()->PostEvent(AK::EVENTS::CRAFT);
				Input->VibrateController(CONTROLLER_SOFT_VIBRATE, CONTROLLER_SOFT_VIBRATE, 0.75f);

				curBridge->Activate();
				}
				*/
			}
			else
			{
				m_bIsBuilding = false;

				curBridge->InterruptBuild();

				if (m_cpTheInventory->IsOpen() == false && m_cpMap->IsOpen() == false)
					curBridge->DrawAvailableIndicator();
			}

			/*if (curBridge->GetRepairable() == false)
			{
			//if (Input->IsKeyPressed("Interact") || Input->IsButtonPressed("Interact"))
			//{
			//	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::CRAFT);
			//	curBridge->Activate();
			//
			//	m_cpTheInventory->RemoveFromInventory(CInventory::Bones, 6);
			//	m_cpTheInventory->RemoveFromInventory(CInventory::Bridge, 8);
			//
			//	Input->VibrateController(CONTROLLER_SOFT_VIBRATE, CONTROLLER_SOFT_VIBRATE, 0.75f);
			//}
			//else
			//{
			//	curBridge->DrawAvailableIndicator();
			//}
			}
			else
			{
			//if (Input->IsKeyPressed("Interact") || Input->IsButtonPressed("Interact"))
			//{
			//	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::CRAFT);
			//	curBridge->Activate();
			//
			//	m_cpTheInventory->RemoveFromInventory(CInventory::Bridge, 8);
			//
			//	Input->VibrateController(CONTROLLER_SOFT_VIBRATE, CONTROLLER_SOFT_VIBRATE, 0.75f);
			//}
			//else
			//{
			//	curBridge->DrawAvailableIndicator();
			//}
			}*/
		}
		else
		{
			if (m_cpTheInventory->IsOpen() == false && m_cpMap->IsOpen() == false)
				curBridge->DrawUnAvailableIndicator();
		}
	}
	else if (_cCollidedObject->GetTag() == "Herb")
	{
		((CHerb*)(_cCollidedObject))->DeActivate();
		m_cpTheInventory->AddToInventory(CInventory::Herb, 1);
		CItemPickupAnimation* cAnimation = new CItemPickupAnimation(CAssetManager::GetInstance()->GetPrefabMesh("Herb"), L"../Game/Assets/Art/2D/HUD/HealthBarGreen.dds", this, m_cCamera);
		m_cAnimations.push_back(cAnimation);
	}
	else if (_cCollidedObject->GetTag() == "Bed")
	{
		CBed* theBed = reinterpret_cast<CBed*>(_cCollidedObject);
		CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);

		if (m_cpTheHUD->GetCurrentDay() == 4 && m_cpDayNight->IsNight() == true)
			m_bLastNight = true;


		if (theBed->IsAvailable() && m_bIsGrounded == true && m_bLastNight == false)
		{
			if (m_cpTheInventory->IsOpen() == false && m_cpMap->IsOpen() == false)
				theBed->DrawAvailableIndicator();

			if (Input->IsKeyPressed("Interact") || Input->IsButtonPressed("Interact"))
			{
				theBed->SetToMorning();
				m_nHealth = m_nMaxHealth;

				if (m_cpDayNight != nullptr)
				{
					if (m_cpDayNight->IsNight() == true)
						m_cpTheHUD->IncrementDayCounter();
				}
			}
		}
		else
		{
			theBed->DrawUnAvailableIndicator();
		}

	}
	else if (_cCollidedObject->GetTag() == "PressurePlate")
	{
		_cCollidedObject->CollisionResponse(this);
	}
	else if (_cCollidedObject->GetTag() == "Tree")
	{
		CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
	}

	else if (_cCollidedObject->GetTag() == "Bone")
	{
		((CBone*)(_cCollidedObject))->DeActivate();

		if (m_cpTheHUD->GetWorldTime() > 180.0f)
			m_cpTheInventory->AddToInventory(CInventory::Bones, 2); // Night
		else
			m_cpTheInventory->AddToInventory(CInventory::Bones, 1); // Day

		CItemPickupAnimation* cAnimation = new CItemPickupAnimation(CAssetManager::GetInstance()->GetPrefabMesh("BonePile"), L"../Game/Assets/Art/2D/Textures/Failbot_BoneLoot.dds", this, m_cCamera);
		m_cAnimations.push_back(cAnimation);
	}
	else if (_cCollidedObject->GetTag() == "Gunpowder")
	{
		((CGunpowder*)(_cCollidedObject))->DeActivate();

		if (m_cpTheHUD->GetWorldTime() > 180.0f)
			m_cpTheInventory->AddToInventory(CInventory::Gunpowder, 2); // Night
		else
			m_cpTheInventory->AddToInventory(CInventory::Gunpowder, 1); // Day

		CItemPickupAnimation* cAnimation = new CItemPickupAnimation(CAssetManager::GetInstance()->GetPrefabMesh("GunPowder"), L"../Game/Assets/Art/2D/Textures/Failbot_Gunpowder.dds", this, m_cCamera);
		m_cAnimations.push_back(cAnimation);

	}
	//else if (_cCollidedObject->GetTag() == "CraftingTable")
	//{
	//	CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
	//	m_cpCraftingTable->SetAtTable(true);
	//
	//	if (!m_cpCraftingTable->IsOpen())
	//		reinterpret_cast<CCrafting*>(_cCollidedObject)->DrawIndicator();
	//
	//	if (Input->IsKeyPressed("Crafting") || Input->IsButtonPressed("Interact"))
	//	{
	//		if (!m_cpCraftingTable->IsOpen())
	//		{
	//			m_cpCraftingTable->Open();
	//		}
	//		else
	//			m_cpCraftingTable->Close();
	//
	//		m_cpTheInventory->Close();
	//	}
	//}
	else if (_cCollidedObject->GetTag() == "Torch")
	{
		((CTorch*)(_cCollidedObject))->ActivateTorch();
	}
	else if (_cCollidedObject->GetTag() == "GraveStone")
	{
		if (m_bPlayerAlive)
		{
			CGraveStone* curStone = reinterpret_cast<CGraveStone*>(_cCollidedObject);
			m_cpTheInventory->AddGraveStoneStock(curStone->GetDropedStock());
			curStone->DeActivate();

			m_cpMap->RemoveTombstoneIcon(curStone->GetID());

			for (auto iter = m_vpGraveStones.begin(); iter != m_vpGraveStones.end(); iter++)
			{
				if (curStone->GetID() == (*iter)->GetID())
				{
					m_vpOldGraves.push_back((*iter)->GetRenderMesh());
					m_vpGraveStones.erase(iter);
					break;
				}
			}

			CItemPickupAnimation* cAnimation = new CItemPickupAnimation(CAssetManager::GetInstance()->GetPrefabMesh("LootPile"), L"../Game/Assets/Art/2D/Textures/Failbot_LootPile.dds", this, m_cCamera);
			m_cAnimations.push_back(cAnimation);
		}
	}
	else if (_cCollidedObject->GetTag() == "StorageCrate")
	{
		CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);

		if (m_cpTheInventory->IsOpen() == false && m_cpMap->IsOpen() == false)
		{
			((CStorageCrate*)_cCollidedObject)->DrawIndicator();
			((CStorageCrate*)_cCollidedObject)->SetAtTable();
		}

		if (Input->IsKeyPressed("Interact") || Input->IsButtonPressed("Interact"))
		{
			if (((CStorageCrate*)_cCollidedObject)->IsOpen() == false)
			{
				((CStorageCrate*)_cCollidedObject)->Open(m_cpTheInventory);

				m_cpTheInventory->Close();
				m_cpMap->Close();
			}
			else
			{
				((CStorageCrate*)_cCollidedObject)->Close();
			}
		}

	}
	else if (_cCollidedObject->GetTag() == "ExitKey")
	{
		if (m_bExitKeyFound == false)
		{
			//find a key pick up audio
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::LOOT, { 0, 0, 0 });

			m_bExitKeyFound = true;
			((CExitKey*)_cCollidedObject)->DeAactive();

			m_cpTheHUD->ExitKeyFound();
			m_cpMap->ExitKeyFound();

			//send message to deactivate all other keys
			CExitKeyFoundMessage* msg = new CExitKeyFoundMessage(true);
			msg->Send();
		}
	}

}

void CPlayer::MouseTurn()
{
	float fDeltaX = Input->GetMouseDirection().x;
	float fDeltaY = Input->GetMouseDirection().y;

	float sensitivity = SENSITIVITY;
	if (m_bAttacking)
	{
		if (m_cLightAttack->GetIsActive())
		{
			sensitivity /= 2.0f;
		}
		else
		{
			sensitivity /= 5.0f;
		}
	}

	if (m_bIsStunned)
	{
		sensitivity = 0.0f;
	}

	// Rotate Camera //
	if (fDeltaX || fDeltaY)
	{
		XMFLOAT3 f3WorldVelocity;
		if (!m_bIsGrounded)
		{
			f3WorldVelocity = GetWorldVelocity();
		}

		fDeltaX *= sensitivity;
		fDeltaY *= sensitivity;

		// Store Position //
		float fMatrixPosX = m_mWorld.m[3][0];
		float fMatrixPosY = m_mWorld.m[3][1];
		float fMatrixPosZ = m_mWorld.m[3][2];
		float fMatrixPosW = m_mWorld.m[3][3];

		// Set Position To World Center // Global rotation becomes same as local
		m_mWorld.m[3][0] = 0;
		m_mWorld.m[3][1] = 0;
		m_mWorld.m[3][2] = 0;
		m_mWorld.m[3][3] = 0;

		// Rotate Globally On Y-Axis //
		float fAngleY = RADIANS(fDeltaX);
		XMMATRIX d3dRotateY = XMMatrixRotationY(fAngleY);
		XMStoreFloat4x4(&m_mWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_mWorld), d3dRotateY));

		//if (m_fCameraAngle + fDeltaY <  90 &&
		//	m_fCameraAngle + fDeltaY > -90) // No x-axis 360's
		//{
		//	// Update Angle //
		//	m_fCameraAngle += fDeltaY;
		//
		//	// Rotate Locally On X-Axis //
		//	float fAngleX = RADIANS(fDeltaY);
		//	XMMATRIX d3dRotateX = XMMatrixRotationX(fAngleX);
		//	XMStoreFloat4x4(&m_mWorld, XMMatrixMultiply(d3dRotateX, XMLoadFloat4x4(&m_mWorld)));
		//}

		// Restore Position //
		m_mWorld.m[3][0] = fMatrixPosX;
		m_mWorld.m[3][1] = fMatrixPosY;
		m_mWorld.m[3][2] = fMatrixPosZ;
		m_mWorld.m[3][3] = fMatrixPosW;

		if (!m_bIsGrounded)
		{
			SetWorldVelocity(f3WorldVelocity);
		}
	}
}

void CPlayer::Movement()
{
	if (m_bIsGrounded && !m_bIsStunned)
	{
		bool isMovingRight = m_f3Velocity.x > 0;
		bool isMovingForward = m_f3Velocity.z > 0;
		bool isMovingX = false;
		bool isMovingZ = false;

		//SPENCER_TODO - need to check if player is touching the ground or not
		XMFLOAT4 pos = (XMFLOAT4)m_mWorld.m[3];

		if ((Input->IsKeyPressed("Jump") || Input->IsButtonPressed("Jump Action")) && m_bIsGrounded && !m_bAttacking /*&& !m_cpCraftingTable->IsOpen()*/)
		{
			m_f3Velocity.y += JUMP_SPEED;
			m_f3JumpVelocity = GetWorldVelocity();
			m_bIsGrounded = false;

			m_cpAudio->PostEvent(AK::EVENTS::PLAYERJUMP, *GetPosition());

			AnimationTransition(m_vpCurrAnim, eJumpAnim);
		}



#pragma region Controller Input

		if (Input->IsAxisDown("Left JS Pos Y"))//forward
		{
			if (m_fFriction != FRICTION)
				m_fFriction = FRICTION;

			if (!isMovingForward)
			{
				m_f3Velocity.z = 0.0f;
			}

			float temp = Input->GetRegisteredAxisValue("Left JS Pos Y");
			m_f3Velocity.z = MAX_SPEED * (temp / DEADZONE_MAX);
			//m_f3Velocity.z += MOVE_SPEED * (temp / DEADZONE_MAX);

			isMovingZ = true;
		}
		if (Input->IsAxisDown("Left JS Neg Y"))//backward
		{
			if (m_fFriction != FRICTION)
				m_fFriction = FRICTION;

			if (!isMovingForward)
			{
				m_f3Velocity.z = 0.0f;
			}

			float temp = Input->GetRegisteredAxisValue("Left JS Neg Y");
			m_f3Velocity.z = MAX_SPEED * (temp / DEADZONE_MAX);
			//m_f3Velocity.z -= MOVE_SPEED * (-temp / DEADZONE_MAX);

			isMovingZ = true;
		}
		if (Input->IsAxisDown("Left JS Pos X"))//rightward
		{
			if (m_fFriction != FRICTION)
				m_fFriction = FRICTION;

			if (!isMovingForward)
			{
				m_f3Velocity.x = 0.0f;
			}

			float temp = Input->GetRegisteredAxisValue("Left JS Pos X");
			m_f3Velocity.x = MAX_SPEED * (temp / DEADZONE_MAX);
			//m_f3Velocity.x += MOVE_SPEED * (temp / DEADZONE_MAX);

			isMovingX = true;
		}
		if (Input->IsAxisDown("Left JS Neg X"))//leftward
		{
			if (m_fFriction != FRICTION)
				m_fFriction = FRICTION;

			if (!isMovingForward)
			{
				m_f3Velocity.x = 0.0f;
			}

			float temp = Input->GetRegisteredAxisValue("Left JS Neg X");
			m_f3Velocity.x = MAX_SPEED * (temp / DEADZONE_MAX);
			//m_f3Velocity.x -= MOVE_SPEED * (-temp / DEADZONE_MAX);

			isMovingX = true;
		}

#pragma endregion



		if (Input->IsKeyDown("MoveForward"))
		{
			if (m_fFriction != FRICTION)
				m_fFriction = FRICTION;

			if (!isMovingForward)
			{
				m_f3Velocity.z = 0.0f;
			}

			m_f3Velocity.z += MOVE_SPEED;
			isMovingZ = true;
		}

		if (Input->IsKeyDown("MoveLeft"))
		{
			if (m_fFriction != FRICTION)
				m_fFriction = FRICTION;

			if (isMovingRight)
			{
				m_f3Velocity.x = 0.0f;
			}

			m_f3Velocity.x -= MOVE_SPEED;
			isMovingX = true;
		}

		if (Input->IsKeyDown("MoveBackward"))
		{
			if (m_fFriction != FRICTION)
				m_fFriction = FRICTION;

			if (isMovingForward)
			{
				m_f3Velocity.z = 0.0f;
			}

			m_f3Velocity.z -= MOVE_SPEED;
			isMovingZ = true;
		}

		if (Input->IsKeyDown("MoveRight"))
		{
			if (m_fFriction != FRICTION)
				m_fFriction = FRICTION;

			if (!isMovingRight)
			{
				m_f3Velocity.x = 0.0f;
			}

			m_f3Velocity.x += MOVE_SPEED;
			isMovingX = true;
		}

#if _DEBUG
		if (GetAsyncKeyState('O'))
		{

			XMFLOAT4X4 oldWorldMAtrix = m_mWorld;
			SetPosition(XMFLOAT3(0, 0, 0));
			for (unsigned int collider = 0; collider < m_pvColliders.size(); collider++)
			{
				XMFLOAT3 f3VelocityUpdate;
				f3VelocityUpdate.x = m_mWorld._41 - oldWorldMAtrix._41;
				f3VelocityUpdate.y = m_mWorld._42 - oldWorldMAtrix._42;
				f3VelocityUpdate.z = m_mWorld._43 - oldWorldMAtrix._43;

				m_pvColliders[collider]->UpdatePosition(f3VelocityUpdate);
			}

		}
#endif

		//Cap the speed to MAX_SPEED
		if (abs(m_f3Velocity.x) > MAX_SPEED)
		{
			m_f3Velocity.x /= abs(m_f3Velocity.x);
			m_f3Velocity.x *= MAX_SPEED;
		}

		if (abs(m_f3Velocity.z) > MAX_SPEED)
		{
			m_f3Velocity.z /= abs(m_f3Velocity.z);
			m_f3Velocity.z *= MAX_SPEED;
		}

		isMovingRight = m_f3Velocity.x > 0;
		isMovingForward = m_f3Velocity.z > 0;
		bool hasXVelocity = abs(m_f3Velocity.x) > 0;
		bool hasZVelocity = abs(m_f3Velocity.z) > 0;

		//Slow down if not trying to move
		if (!isMovingX)
		{
			if (hasXVelocity)
			{
				float xVel = abs(m_f3Velocity.x);
				if (isMovingRight)
				{
					m_f3Velocity.x -= m_fFriction;
				}
				else
				{
					m_f3Velocity.x += m_fFriction;
				}

				if (abs(m_f3Velocity.x) > xVel)
					m_f3Velocity.x = xVel;

				//If moving very slowly, set speed to 0
				if (abs(m_f3Velocity.x) < KILL_VELOCITY)
				{
					m_f3Velocity.x = 0.0f;
				}
			}
		}

		if (!isMovingZ)
		{
			if (hasZVelocity)
			{
				float zVel = abs(m_f3Velocity.z);
				if (isMovingForward)
				{
					m_f3Velocity.z -= m_fFriction;
				}
				else
				{
					m_f3Velocity.z += m_fFriction;
				}

				if (abs(m_f3Velocity.z) > zVel)
					m_f3Velocity.z = zVel;

				//If moving very slowly, set speed to 0
				if (abs(m_f3Velocity.z) < KILL_VELOCITY)
				{
					m_f3Velocity.z = 0.0f;
				}
			}
		}
	}



	if (m_bAttacking)
	{
		if (m_cLightAttack->GetIsActive())
		{
			m_f3Velocity.x /= 1.5f;
			m_f3Velocity.z /= 1.5f;
		}
		else
		{
			m_f3Velocity.x = 0.0f;
			m_f3Velocity.z = 0.0f;
		}
	}

	m_f3Velocity.y += GRAVITY * DELTA_TIME();
	m_f3JumpVelocity.y += GRAVITY * DELTA_TIME();

	if (NearlyEqual(abs(m_f3Velocity.x), MAX_SPEED) && NearlyEqual(abs(m_f3Velocity.z), MAX_SPEED))
	{
		XMVECTOR oldVel = XMLoadFloat3(&m_f3Velocity);
		float yVel = m_f3Velocity.y;
		oldVel *= 0.707f;
		oldVel.m128_f32[1] = yVel;
		XMStoreFloat3(&m_f3Velocity, oldVel);
	}

	if (m_bStaminaDraught)
	{
		m_f3Velocity.x /= 2.0f;
		m_f3Velocity.z /= 2.0f;
	}

	//This breaks aerial movement
	if (!m_bIsGrounded)
		SetWorldVelocity(m_f3JumpVelocity);

	CCapsule* pCapsule = (CCapsule*)m_pvColliders[1]->GetBounds();

	XMFLOAT3 f3NewStart = pCapsule->GetStart();
	XMFLOAT3 f3NewEnd = pCapsule->GetEnd();

	f3NewStart.x = f3NewEnd.x = m_mWorld._41;
	f3NewStart.z = f3NewEnd.z = m_mWorld._43;

	pCapsule->SetStart(f3NewStart);
	pCapsule->SetEnd(f3NewEnd);

	CAABB* boxCollider = reinterpret_cast<CAABB*>(m_pvColliders[0]->GetBounds());
	XMFLOAT3 center = boxCollider->GetCenter();
	center.x = f3NewStart.x;
	center.z = f3NewStart.z;
	boxCollider->SetCenter(center);

	for (unsigned int collider = 0; collider < m_pvColliders.size(); collider++)
	{
		m_pvColliders[collider]->UpdatePosition({ GetWorldVelocity().x * DELTA_TIME(), GetWorldVelocity().y * DELTA_TIME(), GetWorldVelocity().z * DELTA_TIME() });
	}
}

/*****************************************************************
* TakeDamage()			Subtracts the damage from the player's health
*						and checks for death
*
* Ins:					int _damage
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    SS
*****************************************************************/
void CPlayer::TakeDamage(int _damage)
{
	if (m_bGodMode == true)
	{
		return;
	}

	if (m_bIsBuilding == true)
	{
		m_bCancelBuild = true;
	}

	if (m_nHealth > 0 && !m_bInvincible)
	{
		if (_damage > 0)
		{
			PlayParticle(m_unParticles[eBloodParticle], { 0.0f, 110.0f, 0.0f });
			PlayParticle(m_unParticles[eBloodString], { 0.0f, 110.0f, 0.0f });
			PlayParticle(m_unParticles[eBloodMist], { 0.0f, 110.0f, 0.0f });
			PlayParticle(m_unParticles[eBloodMist2], { 0.0f, 120.0f, 0.0f });


			m_cpParticleManager->GetEmitter(m_unParticles[eSwordParticle])->Stop();
			m_cpParticleManager->GetEmitter(m_unParticles[eXSlashParticleLeft])->Stop();
			m_cpParticleManager->GetEmitter(m_unParticles[eXSlashParticleRight])->Stop();

			m_nHealth -= _damage;
			Input->VibrateController(CONTROLLER_SOFT_VIBRATE, CONTROLLER_SOFT_VIBRATE, 0.25f);

			if (m_nHealth > 50)
				m_cpAudio->PostEvent(AK::EVENTS::PLAYERHURTHIGHHEALTH, *GetPosition());
			else
				m_cpAudio->PostEvent(AK::EVENTS::PLAYERHURTLOWHEALTH, *GetPosition());

			AnimationTransition(m_vpCurrAnim, eHitAnim);

			if (m_nHealth <= 0)
			{
				HandleDeath();
			}
		}

		m_bInvincible = true;
		m_fInvincibleTimer = m_vpAnimations[eHitAnim].GetDuration();

		if (m_bAttacking)
		{
			m_cLightAttack->SetIsActive(false);
		}

		m_cCamera->Shake();
	}
}

/*****************************************************************
* HandleDeath()			All logic that happens right when player
*						death occurs
*
* Ins:					None
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    SS
*****************************************************************/
void CPlayer::HandleDeath()
{
	m_bPlayerAlive = false;

	m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eDeathAnim]);
	m_vpCurrAnim = eDeathAnim;

	m_fRespawnTimer = m_vpAnimations[eDeathAnim].GetDuration();

	Input->VibrateController(CONTROLLER_HARD_VIBRATE, CONTROLLER_HARD_VIBRATE, 1.0f);

	m_cpAudio->PostEvent(AK::EVENTS::PLAYERDEATH, *GetPosition());

	//gravestone
	unsigned int unTotalResources = m_cpTheInventory->GetStock(CInventory::eItems::Bones) + m_cpTheInventory->GetStock(CInventory::eItems::Wood) + m_cpTheInventory->GetStock(CInventory::eItems::Herb) +
		m_cpTheInventory->GetStock(CInventory::eItems::Gunpowder) + m_cpTheInventory->GetStock(CInventory::eItems::Bomb) + m_cpTheInventory->GetStock(CInventory::eItems::Bridge) + m_cpTheInventory->GetStock(CInventory::eItems::HealthPotion);

	if (unTotalResources > 0)
	{
		if (m_vpGraveStones.size() == 1 && m_vpGraveStones[0]->GetActive() == false)
		{
			m_vpGraveStones[0]->SetDropedStock(m_cpTheInventory->GetTotalStock());
			m_vpGraveStones[0]->Activate();

			m_cpMap->AddTombstoneIcon(0);
		}
		else
		{
			CGraveStone* tempGrave = new CGraveStone(m_cObjectManager, this);
			tempGrave->SetDropedStock(m_cpTheInventory->GetTotalStock());
			tempGrave->Activate();
			tempGrave->SetID(m_vpGraveStones.size());
			m_vpGraveStones.push_back(tempGrave);

			m_cpMap->AddTombstoneIcon(tempGrave->GetID());
		}
	}

	m_cpTheInventory->ResetStock();
}

void CPlayer::HeavyAttack()
{
}

void CPlayer::LightAttack(bool isRight)
{
	float r = LightAttackTimer / m_cCurrentWeapon->GetAttackTime();
	if (m_bIsGrounded && r < 0.5f /*&& m_fStamina > 0.0f*/)
	{
		XMVECTOR forward = XMLoadFloat3(GetZAxis());
		XMVECTOR pos = XMLoadFloat3(GetPosition());
		XMVECTOR forwardPoint = pos + (forward * 90);
		XMFLOAT3 particlePoint;  XMStoreFloat3(&particlePoint, forwardPoint);

		if (!isRight)
		{
			m_vpAnimations[eLightLeftAttackAnim].SetScale(m_vpAnimations[eLightLeftAttackAnim].GetDuration() / m_cCurrentWeapon->GetAttackTime());
			AnimationTransition(m_vpCurrAnim, eLightLeftAttackAnim);
			m_cCurrentAttack->Attack(m_cCurrentWeapon->GetDamage());
			m_bAttackingRight = false;

			/*m_cpParticleManager->GetEmitter(m_unParticles[eXSlashParticleLeft])->Stop();
			m_cpParticleManager->GetEmitter(m_unParticles[eXSlashParticleLeft])->GetPosition(0) = particlePoint.x;
			m_cpParticleManager->GetEmitter(m_unParticles[eXSlashParticleLeft])->GetPosition(1) = particlePoint.y + 120;
			m_cpParticleManager->GetEmitter(m_unParticles[eXSlashParticleLeft])->GetPosition(2) = particlePoint.z;
			m_cpParticleManager->GetEmitter(m_unParticles[eXSlashParticleLeft])->ePlay();*/
		}
		else
		{
			m_vpAnimations[eLightRightAttackAnim].SetScale(m_vpAnimations[eLightRightAttackAnim].GetDuration() / m_cCurrentWeapon->GetAttackTime());
			AnimationTransition(m_vpCurrAnim, eLightRightAttackAnim);
			m_cCurrentAttack->Attack(m_cCurrentWeapon->GetDamage());
			m_bAttackingRight = true;

			/*m_cpParticleManager->GetEmitter(m_unParticles[eXSlashParticleRight])->Stop();
			m_cpParticleManager->GetEmitter(m_unParticles[eXSlashParticleRight])->GetPosition(0) = particlePoint.x;
			m_cpParticleManager->GetEmitter(m_unParticles[eXSlashParticleRight])->GetPosition(1) = particlePoint.y + 120;
			m_cpParticleManager->GetEmitter(m_unParticles[eXSlashParticleRight])->GetPosition(2) = particlePoint.z;
			m_cpParticleManager->GetEmitter(m_unParticles[eXSlashParticleRight])->ePlay();*/
		}

		LightAttackTimer = m_cCurrentWeapon->GetAttackTime();
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYERSWING);
		m_bAttacking = true;

		AdjustStamina(LIGHT_ATTACK_STAMINA);
	}
}

void CPlayer::Attacking()
{
	if (LightAttackTimer > 0.0f && m_bAttacking && !m_bIsStunned)
	{
		LightAttackTimer -= DELTA_TIME();
		if (LightAttackTimer < 0.0f)
		{
			m_bAttacking = false;
			m_cCurrentAttack->SetIsActive(false);
			m_fSpearDistance = 0.0f;
		}
	}
	else
	{
		if (Input->IsKeyPressed("LightAttack") || Input->IsTriggerPressed("Left Attack"))
		{
			LightAttack(false);
		}
		else if (Input->IsKeyPressed("HeavyAttack") || Input->IsTriggerPressed("Right Attack"))
		{
			LightAttack(true);
		}
	}
}

void CPlayer::Animation()
{
	if (m_bPlayerAlive)
	{
		XMFLOAT3 vel = m_f3Velocity;
		vel.y = 0.0f;
		float mag = XMVector3Length(XMLoadFloat3(&vel)).m128_f32[2];
		if (m_bIsGrounded)
		{


			if (mag > KILL_VELOCITY)
			{
				if (!m_bAttacking && m_vpCurrAnim != eRunAnim && !m_bInvincible)
					AnimationTransition(m_vpCurrAnim, eRunAnim);
			}
			else
			{
				if (!m_bAttacking && m_vpCurrAnim != eIdleAnim && !m_bInvincible)
					AnimationTransition(m_vpCurrAnim, eIdleAnim);
			}

		}
		else
		{
			//if (m_vpCurrAnim != eFallAnim)
			//{
			//
			//}
		}
	}

	if (m_vpCurrAnim == eRunAnim)
	{
		if (NearlyEqual(m_vpAnimations[m_vpCurrAnim].GetCurrTime(), 0.05f))
		{
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYERFOOTSTEP, *GetPosition());
		}

		if (NearlyEqual(m_vpAnimations[m_vpCurrAnim].GetCurrTime(), 0.5f))
		{
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYERFOOTSTEP, *GetPosition());
		}
	}
}

void CPlayer::AnimationTransition(EPlayerAnimation _from, EPlayerAnimation _to)
{
	switch (_from)
	{
	case CPlayer::eIdleAnim:

		switch (_to)
		{
		case CPlayer::eRunAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eRunAnim]);
			break;
		case CPlayer::eHeavyRightAttackAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eHeavyRightAttackAnim]);
			break;
		case CPlayer::eHeavyLeftAttackAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eHeavyLeftAttackAnim]);
			break;
		case CPlayer::eLightRightAttackAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eLightRightAttackAnim]);
			break;
		case CPlayer::eLightLeftAttackAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eLightLeftAttackAnim]);
			break;
		case CPlayer::eDeathAnim:
			break;
		case CPlayer::eJumpAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eJumpAnim]);
			break;
		case CPlayer::eHitAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eHitAnim]);
			break;
		}
		break;

	case CPlayer::eRunAnim:

		switch (_to)
		{
		case CPlayer::eIdleAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eIdleAnim]);
			break;
		case CPlayer::eHeavyRightAttackAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eHeavyRightAttackAnim]);
			break;
		case CPlayer::eHeavyLeftAttackAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eHeavyLeftAttackAnim]);
			break;
		case CPlayer::eLightRightAttackAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eLightRightAttackAnim]);
			break;
		case CPlayer::eLightLeftAttackAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eLightLeftAttackAnim]);
			break;
		case CPlayer::eDeathAnim:
			break;
		case CPlayer::eJumpAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eJumpAnim]);
			break;
		case CPlayer::eHitAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eHitAnim]);
			break;
		}
		break;

	case CPlayer::eHeavyLeftAttackAnim:

		switch (_to)
		{
		case CPlayer::eIdleAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eIdleAnim]);
			break;
		case CPlayer::eRunAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eRunAnim]);
			break;
		case CPlayer::eDeathAnim:
			break;
		case CPlayer::eHitAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eHitAnim]);
			break;
		}
		break;

	case CPlayer::eHeavyRightAttackAnim:

		switch (_to)
		{
		case CPlayer::eIdleAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eIdleAnim]);
			break;
		case CPlayer::eRunAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eRunAnim]);
			break;
		case CPlayer::eDeathAnim:
			break;
		case CPlayer::eHitAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eHitAnim]);
			break;
		}
		break;

	case CPlayer::eLightLeftAttackAnim:

		switch (_to)
		{
		case CPlayer::eIdleAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eIdleAnim]);
			break;
		case CPlayer::eRunAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eRunAnim]);
			break;
		case CPlayer::eLightLeftAttackAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eLightLeftAttackAnim]);

			break;
		case CPlayer::eDeathAnim:
			break;
		case CPlayer::eHitAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eHitAnim]);
			break;
		}
		break;

	case CPlayer::eLightRightAttackAnim:

		switch (_to)
		{
		case CPlayer::eIdleAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eIdleAnim]);
			break;
		case CPlayer::eRunAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eRunAnim]);
			break;
		case CPlayer::eDeathAnim:
			break;
		case CPlayer::eHitAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eHitAnim]);
			break;
		}
		break;

	case CPlayer::eDeathAnim:

		switch (_to)
		{
		case CPlayer::eIdleAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eIdleAnim]);
			break;
		case CPlayer::eRunAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eRunAnim]);
			break;
		}
		break;

		break;
	case CPlayer::eJumpAnim:

		switch (_to)
		{
		case CPlayer::eIdleAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eIdleAnim]);
			break;
		case CPlayer::eRunAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eRunAnim]);
			break;
		case CPlayer::eDeathAnim:
			break;
		case CPlayer::eJumpAnim:
			break;
		case CPlayer::eHitAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eHitAnim]);
			break;
		}
		break;

		break;
	case CPlayer::eHitAnim:

		switch (_to)
		{
		case CPlayer::eIdleAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eIdleAnim]);
			break;
		case CPlayer::eRunAnim:
			m_pAnimationManager->Switch(&m_vpAnimations[m_vpCurrAnim], &m_vpAnimations[eRunAnim]);
			break;
		}
		break;
	}

	m_vpCurrAnim = _to;
}

void CPlayer::DebugModeInput()
{
	Input = CInputManager::GetInstance();

	if (Input->IsKeyDown("CheatModifier") && Input->IsKeyPressed("GodMode"))
	{
		m_bGodMode = !m_bGodMode;
		if (m_bGodMode == true)
		{
			m_nHealth = 100;
			cout << "God Mode ON\n";
		}
		else
		{
			cout << "God Mode OFF\n";
		}
	}

	if (Input->IsKeyDown("CheatModifier") && Input->IsKeyPressed("Wood Plank Cheat"))
	{
		m_cpTheInventory->AddToInventory(CInventory::Bridge, 1);
	}
	if (Input->IsKeyDown("CheatModifier") && Input->IsKeyPressed("Bomb Cheat"))
	{
		m_cpTheInventory->AddToInventory(CInventory::Bomb, 1);
	}
	if (Input->IsKeyDown("CheatModifier") && Input->IsKeyPressed("Health Potion Cheat"))
	{
		m_cpTheInventory->AddToInventory(CInventory::HealthPotion, 1);
	}
	if (Input->IsKeyDown("CheatModifier") && Input->IsKeyPressed("Bridge Cheat"))
	{
		m_cpTheInventory->AddToInventory(CInventory::Bridge, 8);
		m_cpTheInventory->AddToInventory(CInventory::Bones, 6);
	}

	//if (Input->IsKeyDown("CheatModifier") && Input->IsKeyPressed("UpgradeWeapon"))
	//{
	//	UnlockWeapon();
	//}
	//if (Input->IsKeyDown("CheatModifier") && Input->IsKeyPressed("UpgradeShield"))
	//{
	//	UpgradeShield();
	//}
	//if (Input->IsKeyDown("CheatModifier") && Input->IsKeyPressed("IncreaseMaxHealth"))
	//{
	//	if (m_nCurrentHealthLevel < MAX_HEALTH_LEVEL)
	//	{
	//		m_nCurrentHealthLevel++;
	//		m_nMaxHealth += 25;
	//		m_nHealth += 25;
	//		m_cpTheHUD->SetMaxHealth(m_nMaxHealth);
	//		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::HEALTHUPGRADE, *GetPosition());
	//	}
	//}

	if (Input->IsKeyDown("CheatModifier") && Input->IsKeyPressed("ExitKey Cheat"))
	{
		if (m_bExitKeyFound == false)
		{
			m_bExitKeyFound = true;
			m_cpTheHUD->ExitKeyFound();
			m_cpMap->ExitKeyFound();

			CExitKeyFoundMessage* msg = new CExitKeyFoundMessage(true);
			msg->Send();
		}
	}

	if (Input->IsKeyDown("CheatModifier") && Input->IsKeyPressed("Herb Cheat"))
	{
		m_cpTheInventory->AddToInventory(CInventory::Herb, 1);
	}
	if (Input->IsKeyDown("CheatModifier") && Input->IsKeyPressed("Wood Cheat"))
	{
		m_cpTheInventory->AddToInventory(CInventory::Wood, 1);
	}
	if (Input->IsKeyDown("CheatModifier") && Input->IsKeyPressed("Bone Cheat"))
	{
		m_cpTheInventory->AddToInventory(CInventory::Bones, 1);
	}
	if (Input->IsKeyDown("CheatModifier") && Input->IsKeyPressed("Gunpowder Cheat"))
	{
		m_cpTheInventory->AddToInventory(CInventory::Gunpowder, 1);
	}

}

bool NearlyEqual(float a, float b)
{
	return (a < b + 0.021f) && (a > b - 0.02f);
}

void CPlayer::PlayParticle(unsigned int _toPlay, XMFLOAT3 _relativePos)
{
	m_cpParticleManager->GetEmitter(_toPlay)->Stop();
	m_cpParticleManager->GetEmitter(_toPlay)->GetPosition(0) = GetPosition()->x + _relativePos.x;
	m_cpParticleManager->GetEmitter(_toPlay)->GetPosition(1) = GetPosition()->y + _relativePos.y;
	m_cpParticleManager->GetEmitter(_toPlay)->GetPosition(2) = GetPosition()->z + _relativePos.z;
	m_cpParticleManager->GetEmitter(_toPlay)->ePlay();
}

CHUD* CPlayer::GetHUD()
{
	return m_cpTheHUD;
}

void CPlayer::UpdatePosition()
{




	IObject::UpdatePosition();

	XMFLOAT4X4 joint = ((CAnimationMesh*)m_cpRenderMesh)->GetJointMatrix("Hips");

	XMMATRIX mat1 = XMLoadFloat4x4(&m_mWorld);
	XMMATRIX mat2 = XMLoadFloat4x4(&joint);

	XMStoreFloat4x4(&joint, XMMatrixMultiply(mat2, mat1));
	float3 pos = float3(joint._41, joint._42, joint._43);


	m_pvColliders[0]->GetBounds()->SetCenter(pos);
	m_pvColliders[1]->GetBounds()->SetCenter(pos);

	CAABB* aabb = (CAABB*)(m_pvColliders[0]->GetBounds());
	CCapsule* capsule = (CCapsule*)(m_pvColliders[1]->GetBounds());

	//DEBUG_UTIL::DrawAABBCollider(aabb->Min(), aabb->Max());
	//DEBUG_UTIL::DrawCapsuleCollider(capsule->GetStart(), capsule->GetEnd(), capsule->GetRadius());

	for (unsigned int i = 0; i < (unsigned int)m_cAnimations.size(); i++)
	{
		if (m_cAnimations[i]->m_bAlive)
			m_cAnimations[i]->Update();
		else
			VECTOR_DELETE(m_cAnimations, m_cAnimations[i]);
	}



	if (m_cCurrentWeapon == m_cSpear && m_bAttacking)
	{
		m_fSpearDistance += 120.0f * DELTA_TIME();
		XMFLOAT4X4 * spearMatrix = &m_cSpear->GetRenderMesh()->GetPositionMatrix();
		XMMATRIX m_mParent = XMLoadFloat4x4(&GetWorldMatrix());
		XMMATRIX local = XMLoadFloat4x4(&m_mSpearLocal);
		XMMATRIX velocity = XMMatrixTranslation(0.0f, 90.0f, m_mSpearLocal._43 + m_fSpearDistance);
		XMMATRIX newLocal = XMMatrixMultiply(local, velocity);
		newLocal = newLocal * m_mParent;
		XMStoreFloat4x4(spearMatrix, newLocal);
	}
}

void  CPlayer::Stun(float _fStunTime)
{
	m_fStunTimer = _fStunTime;
	m_bIsStunned = true;
	AnimationTransition(m_vpCurrAnim, eHitAnim);
	m_cCamera->GetLocked() = true;
	m_f3Velocity = { 0, m_f3Velocity.y, 0 };
}

void CPlayer::DamageBlink()
{
	if (m_bBlinkRed)
	{
		m_cpRenderMesh->SetTexture(m_sPlayerTexture.c_str());
		m_sCurrentTexture = m_sPlayerTexture.c_str();
	}
	else
	{
		m_cpRenderMesh->SetTexture(m_sDamageTexture.c_str());
		m_sCurrentTexture = m_sDamageTexture.c_str();
	}

	m_bBlinkRed = !m_bBlinkRed;
}

void CPlayer::StunBlink()
{
	if (m_bBlinkYellow)
	{
		m_cpRenderMesh->SetTexture(m_sPlayerTexture.c_str());
		m_sCurrentTexture = m_sPlayerTexture.c_str();
	}
	else
	{
		m_cpRenderMesh->SetTexture(m_sStunTexture.c_str());
		m_sCurrentTexture = m_sStunTexture.c_str();
	}

	m_bBlinkYellow = !m_bBlinkYellow;
}

void CPlayer::BlockLogic()
{
	//m_bIsBlocking = ((Input->IsKeyDown("Block") || Input->IsButtonDown("Block")) && !m_bAttacking && !m_bIsStunned && m_bIsGrounded && !m_bStaminaDraught && m_eCurrentShield != eNone);
	//m_cShieldRender->GetRender() = m_bIsBlocking;
	//
	//if (m_bIsBlocking)
	//{
	//	XMFLOAT4X4 world = GetWorldMatrix();
	//	XMFLOAT4X4 local = m_f4x4ShieldMatrix;
	//	XMMATRIX shieldWorld = XMLoadFloat4x4(&world);
	//	XMMATRIX shieldLocal = XMLoadFloat4x4(&local);
	//	shieldWorld = shieldLocal * shieldWorld;
	//	shieldWorld *= XMMatrixTranslation(0.0f, 90.0f, 0.0f);
	//	XMStoreFloat4x4(&m_cShieldRender->GetPositionMatrix(), shieldWorld);
	//}
}

void CPlayer::AdjustStamina(float _fStamDam)
{
	//m_fStamina += _fStamDam;
	//if (m_fStamina > 100.0f)
	//	m_fStamina = 100.0f;
	//
	//
	//if (m_fStamina < 0.0f)
	//{
	//	m_fStamina = 0.0f;
	//	m_fStaminaDraughtTimer = STAMINA_DRAUGHT;
	//	m_bStaminaDraught = true;
	//}
}

void CPlayer::SwitchWeapon(CWeapon * _nextWep)
{
	if (_nextWep && _nextWep != m_cCurrentWeapon)
	{
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::SWITCHWEAPON, *GetPosition());
		m_cCurrentWeapon->Deactivate();
		_nextWep->Activate();
		m_cCurrentWeapon = _nextWep;
		m_cCurrentAttack = m_cCurrentWeapon->GetAttack();
	}
}

bool CPlayer::UpgradeShield()
{
	CMesh* mesh = nullptr;
	wstring texture;
	bool switched = false;

	/*switch (m_eCurrentShield)
	{
	case CPlayer::eNone:
	m_eCurrentShield = eWoodShield;
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::HEALTHUPGRADE, *GetPosition());
	texture = L"../Game/Assets/Art/2D/Textures/Shield2_Diffuse.dds";
	mesh = &CAssetManager::GetInstance()->GetMesh(CAssetManager::SkeletonShield)[0];
	switched = true;
	return true;
	break;
	case CPlayer::eWoodShield:
	m_eCurrentShield = eMetalShield;
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::HEALTHUPGRADE, *GetPosition());
	texture = L"../Game/Assets/Art/2D/Textures/Shield2_Diffuse.dds";
	mesh = &CAssetManager::GetInstance()->GetMesh(CAssetManager::SkeletonShield)[0];
	switched = true;
	return true;
	break;
	case CPlayer::eMetalShield:
	m_eCurrentShield = eObsidianFlameShield;
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::HEALTHUPGRADE, *GetPosition());
	texture = L"../Game/Assets/Art/2D/Textures/Shield2_Diffuse.dds";
	mesh = &CAssetManager::GetInstance()->GetMesh(CAssetManager::SkeletonShield)[0];
	switched = true;
	return true;
	break;
	}
	*/
	return false;

	//if (switched)
	//{
	//	//	CItemPickupAnimation itemAnimation(mesh, texture, this, m_cCamera);
	//	//	m_vcPickupAnimations.push(itemAnimation);
	//}
}

void CPlayer::BlockHandling(IObject * _enemy)
{
	//SPENCER_TODO - Fix once we have other enemies
	IObject * enemy = _enemy;
	((CSkeleton*)(enemy))->Blocked();

	switch (m_eCurrentShield)
	{

	case CPlayer::eObsidianFlameShield:
	{
		((CSkeleton*)(enemy))->TakeDamage(10);
		//SPENCER TODO - add sound for obsidian shield flame noise
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::OBSSHIELD, *GetPosition());
	}
	case CPlayer::eMetalShield:
	{
		float xVal, yVal, zVal;
		xVal = enemy->GetPosition()->x - GetPosition()->x;
		yVal = enemy->GetPosition()->y - GetPosition()->y;
		zVal = enemy->GetPosition()->z - GetPosition()->z;

		XMVECTOR toEnemy; toEnemy.m128_f32[0] = xVal; toEnemy.m128_f32[1] = 0.0f; toEnemy.m128_f32[2] = zVal;
		toEnemy = XMVector3Normalize(toEnemy);

		XMFLOAT3 direction; XMStoreFloat3(&direction, toEnemy);

		((CSkeleton*)(enemy))->Knockback(direction, 300.0f);
		((CSkeleton*)(enemy))->SetIsGrounded(false);
		if (m_eCurrentShield == eMetalShield)
		{
			//SPENCER TODO - add sound for metal shield noise
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::METALSHIELD, *GetPosition());
		}
	}
	case CPlayer::eWoodShield:
	{
		TakeDamage(0);
		if (m_eCurrentShield == eWoodShield)
		{
			//SPENCER TODO - add sound for wood shield noise
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::WOODSHIELD, *GetPosition());
		}
	}
	break;

	}

}

bool CPlayer::UnlockWeapon()
{
	//if no weapon upgrade has been recieved yet
	if (m_eCurrentWeaponLevel == eSword)
	{
		m_eCurrentWeaponLevel = eAxe;
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::HEALTHUPGRADE, *GetPosition());
		return true;
	}

	return false;
}

bool CPlayer::UpgradeHealth()
{
	if (m_nCurrentHealthLevel < MAX_HEALTH_LEVEL)
	{
		m_nCurrentHealthLevel++;
		m_nMaxHealth += 25;
		m_nHealth += 25;
		m_cpTheHUD->SetMaxHealth(m_nMaxHealth);
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::HEALTHUPGRADE, *GetPosition());

		return true;
	}

	return false;
}

void CPlayer::SetCraftingTable(CCrafting* _table)
{
	m_cpCraftingTable = _table;
	m_cpCraftingTable->SetCurrentInventory(m_cpTheInventory);
}

void CPlayer::JoystickTurn()
{
	if (Input->IsAxisDown("Right JS Pos X"))//turn left
	{
		float sensitivity = SENSITIVITY;
		if (m_bAttacking)
		{
			if (m_cLightAttack->GetIsActive())
			{
				sensitivity /= 2.0f;
			}
			else
			{
				sensitivity /= 5.0f;
			}
		}

		if (m_bIsStunned)
		{
			sensitivity = 0.0f;
		}

		XMFLOAT3 f3WorldVelocity;
		if (!m_bIsGrounded)
		{
			f3WorldVelocity = GetWorldVelocity();
		}

		// Store Position //
		float fMatrixPosX = m_mWorld.m[3][0];
		float fMatrixPosY = m_mWorld.m[3][1];
		float fMatrixPosZ = m_mWorld.m[3][2];
		float fMatrixPosW = m_mWorld.m[3][3];

		// Set Position To World Center // Global rotation becomes same as local
		m_mWorld.m[3][0] = 0;
		m_mWorld.m[3][1] = 0;
		m_mWorld.m[3][2] = 0;
		m_mWorld.m[3][3] = 0;

		// Rotate Globally On Y-Axis //
		float axisValue = Input->GetRegisteredAxisValue("Right JS Pos X");
		//XMMATRIX d3dRotateY = XMMatrixRotationY(RADIANS((axisValue / AXIS_DEADZONE_LERP_X_VALUE) * sensitivity));
		//XMStoreFloat4x4(&m_mWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_mWorld), d3dRotateY));
		RotateGlobalY(m_mWorld, (axisValue / AXIS_DEADZONE_LERP_X_VALUE) * sensitivity);

		// Restore Position //
		m_mWorld.m[3][0] = fMatrixPosX;
		m_mWorld.m[3][1] = fMatrixPosY;
		m_mWorld.m[3][2] = fMatrixPosZ;
		m_mWorld.m[3][3] = fMatrixPosW;

		if (!m_bIsGrounded)
		{
			SetWorldVelocity(f3WorldVelocity);
		}

	}
	else if (Input->IsAxisDown("Right JS Neg X"))//turn right
	{
		float sensitivity = SENSITIVITY;
		if (m_bAttacking)
		{
			if (m_cLightAttack->GetIsActive())
			{
				sensitivity /= 2.0f;
			}
			else
			{
				sensitivity /= 5.0f;
			}
		}

		if (m_bIsStunned)
		{
			sensitivity = 0.0f;
		}

		XMFLOAT3 f3WorldVelocity;
		if (!m_bIsGrounded)
		{
			f3WorldVelocity = GetWorldVelocity();
		}

		// Store Position //
		float fMatrixPosX = m_mWorld.m[3][0];
		float fMatrixPosY = m_mWorld.m[3][1];
		float fMatrixPosZ = m_mWorld.m[3][2];
		float fMatrixPosW = m_mWorld.m[3][3];

		// Set Position To World Center // Global rotation becomes same as local
		m_mWorld.m[3][0] = 0;
		m_mWorld.m[3][1] = 0;
		m_mWorld.m[3][2] = 0;
		m_mWorld.m[3][3] = 0;

		// Rotate Globally On Y-Axis //
		float axisValue = Input->GetRegisteredAxisValue("Right JS Neg X");
		//XMMATRIX d3dRotateY = XMMatrixRotationY(RADIANS(((axisValue / AXIS_DEADZONE_LERP_X_VALUE) * sensitivity)));
		//XMStoreFloat4x4(&m_mWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_mWorld), d3dRotateY));
		RotateGlobalY(m_mWorld, (axisValue / AXIS_DEADZONE_LERP_X_VALUE) * sensitivity);

		// Restore Position //
		m_mWorld.m[3][0] = fMatrixPosX;
		m_mWorld.m[3][1] = fMatrixPosY;
		m_mWorld.m[3][2] = fMatrixPosZ;
		m_mWorld.m[3][3] = fMatrixPosW;

		if (!m_bIsGrounded)
		{
			SetWorldVelocity(f3WorldVelocity);
		}

	}

}

void CPlayer::AddToRenderer()
{
	if (m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	for (unsigned int i = 0; i < m_vpAttachments.size(); i++)
	{
		if (m_vpAttachments[i].mesh != nullptr)
			GRAPHICS->AddRenderMesh(m_vpAttachments[i].mesh);
	}

	if (m_cpTextFrame != nullptr)
		GRAPHICS->AddRenderMesh(m_cpTextFrame);

	if (m_cpControllerDoorPopUp != nullptr)
		GRAPHICS->AddRenderMesh(m_cpControllerDoorPopUp);

	if (m_cShieldRender != nullptr)
		GRAPHICS->AddRenderMesh(m_cShieldRender);

	for (unsigned int i = 0; i < m_vpOldGraves.size(); i++)
	{
		if (m_vpOldGraves[i] != nullptr)
			GRAPHICS->AddRenderMesh(m_vpOldGraves[i]);
	}

	m_unParticles[eBloodParticle] = m_cpParticleManager->LoadEmitter("BloodDrop.xml");
	m_unParticles[eBloodString] = m_cpParticleManager->LoadEmitter("BloodString.xml");
	m_unParticles[eBloodMist] = m_cpParticleManager->LoadEmitter("BloodMist.xml");
	m_unParticles[eBloodMist2] = m_cpParticleManager->LoadEmitter("BloodMist.xml");
	m_unParticles[eSwordParticle] = m_cpParticleManager->LoadEmitter("SwordSlash.xml");
	m_unParticles[eXSlashParticleLeft] = m_cpParticleManager->LoadEmitter("XSlashLeft.xml");
	m_unParticles[eXSlashParticleRight] = m_cpParticleManager->LoadEmitter("XSlashRight.xml");
	m_unParticles[eHealthPotionYellow] = m_cpParticleManager->LoadEmitter("HealthPotionYellow.xml");
	m_unParticles[eHealthPotionGreen] = m_cpParticleManager->LoadEmitter("HealthPotionGreen.xml");
}
