/***********************************************
* Filename:  		HUD.cpp
* Date:      		8/18/2015
* Mod. Date: 		9/08/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			This is the player HUD class
************************************************/

#include "HUD.h"
#include "../../../Engine/Renderer/Renderer.h"
#include "../../../Game/Source/Environment/DayNight.h"


/*****************************************************************
* CHUD()				The constructor for the HUD
*
* Ins:					None
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    09/08/2015
* Mod. Initials:	    NH
*****************************************************************/
CHUD::CHUD()
{
	m_nPlayerHealth = 100;
	m_nMaxHealth = 100;
	m_fWorldTime = 0.0f;
	m_bHealthFlashON = false;
	m_bLightFlashON = false;
	m_fHealthFlashTimer = 0.0f;
	m_unDayCounter = 1;
	m_fHealthBarOffset = HEALTH_BAR_START;
	m_cpDayNight = nullptr;

#pragma region Health Bar Outline Set Up

	m_tHealthBar.m_nvIndicies.push_back(0);
	m_tHealthBar.m_nvIndicies.push_back(1);
	m_tHealthBar.m_nvIndicies.push_back(2);

	m_tHealthBar.m_nvIndicies.push_back(0);
	m_tHealthBar.m_nvIndicies.push_back(2);
	m_tHealthBar.m_nvIndicies.push_back(3);

	m_tHealthBar.m_cvVertecies.push_back
		({
			{ 0.0f, 0.3f, -0.5f, 1 }, 
			{ 0, 0 }
	});

	m_tHealthBar.m_cvVertecies.push_back
		({
			{ m_nMaxHealth / 100.0f, 0.3f, -0.5f, 1 },
			{ 1, 0 }
	});

	m_tHealthBar.m_cvVertecies.push_back
		({
			{ m_nMaxHealth / 100.0f, -0.3f, -0.5f, 1 },
			{ 1, 1 }
	});

	m_tHealthBar.m_cvVertecies.push_back
		({
			{ 0.0f, -0.3f, -0.5f, 1 }, 
			{ 0, 1 }
	});

	m_tHealthBar.m_cpRenderMesh = new CHUDMesh(m_tHealthBar.m_nvIndicies, m_tHealthBar.m_cvVertecies, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/HealthBarOutline.dds");
	m_tHealthBar.m_cpRenderMesh->GetTransparent() = true;
	GRAPHICS->AddRenderMesh(m_tHealthBar.m_cpRenderMesh);

	TranslateLocalX(m_tHealthBar.m_cpRenderMesh->GetPositionMatrix(), -0.825f); 
	TranslateLocalY(m_tHealthBar.m_cpRenderMesh->GetPositionMatrix(), 0.7f); 

#pragma endregion

#pragma region Health Bar Fill Set Up

	m_tHealthFill.m_nvIndicies.push_back(0);
	m_tHealthFill.m_nvIndicies.push_back(1);
	m_tHealthFill.m_nvIndicies.push_back(2);

	m_tHealthFill.m_nvIndicies.push_back(0);
	m_tHealthFill.m_nvIndicies.push_back(2);
	m_tHealthFill.m_nvIndicies.push_back(3);

	m_tHealthFill.m_cvVertecies.push_back
		({
			{ 0.0f, 0.06f, -1.0f, 1 }, 
			{ 0, 0 }
	});

	m_tHealthFill.m_cvVertecies.push_back
		({
			{ HEALTH_BAR_START, 0.06f, -1.0f, 1 },
			{ 1, 0 }
	});

	m_tHealthFill.m_cvVertecies.push_back
		({
			{ HEALTH_BAR_START, -0.06f, -1.0f, 1 },
			{ 1, 1 }
	});

	m_tHealthFill.m_cvVertecies.push_back
		({
			{ 0.0f, -0.06f, -1.0f, 1 }, 
			{ 0, 1 }
	});

	m_tHealthFill.m_cpRenderMesh = new CHUDMesh(m_tHealthFill.m_nvIndicies, m_tHealthFill.m_cvVertecies, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/HealthBarGreen.dds");
	m_tHealthFill.m_cpRenderMesh->GetTransparent() = true;
	GRAPHICS->AddRenderMesh(m_tHealthFill.m_cpRenderMesh);

	TranslateLocalX(m_tHealthFill.m_cpRenderMesh->GetPositionMatrix(), -0.7495f); 
	TranslateLocalY(m_tHealthFill.m_cpRenderMesh->GetPositionMatrix(), 0.8349f); 

#pragma endregion

#pragma region Stamina Bar Fill Set Up
	m_tStaminaFill.m_nvIndicies.push_back(0);
	m_tStaminaFill.m_nvIndicies.push_back(1);
	m_tStaminaFill.m_nvIndicies.push_back(2);

	m_tStaminaFill.m_nvIndicies.push_back(0);
	m_tStaminaFill.m_nvIndicies.push_back(2);
	m_tStaminaFill.m_nvIndicies.push_back(3);

	m_tStaminaFill.m_cvVertecies.push_back
		({
			{ 0.0f, -0.07f, -1.0f, 1 },
			{ 0, 0 }
	});

	m_tStaminaFill.m_cvVertecies.push_back
		({
			{ HEALTH_BAR_XOFFSET, -0.07f, -1.0f, 1 },
			{ 1, 0 }
	});

	m_tStaminaFill.m_cvVertecies.push_back
		({
			{ HEALTH_BAR_XOFFSET, -0.19f, -1.0f, 1 },
			{ 1, 1 }
	});

	m_tStaminaFill.m_cvVertecies.push_back
		({
			{ 0.0f, -0.19f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_tStaminaFill.m_cpRenderMesh = new CHUDMesh(m_tStaminaFill.m_nvIndicies, m_tStaminaFill.m_cvVertecies, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/StunTexture.dds");
	m_tStaminaFill.m_cpRenderMesh->GetTransparent() = true;
	//GRAPHICS->AddRenderMesh(m_tStaminaFill.m_cpRenderMesh);

	TranslateLocalX(m_tStaminaFill.m_cpRenderMesh->GetPositionMatrix(), -0.7495f);
	TranslateLocalY(m_tStaminaFill.m_cpRenderMesh->GetPositionMatrix(), 0.8349f);
#pragma endregion

#pragma region Health Vignette

	m_tHealthVignette.m_nvIndicies.push_back(0);
	m_tHealthVignette.m_nvIndicies.push_back(1);
	m_tHealthVignette.m_nvIndicies.push_back(2);

	m_tHealthVignette.m_nvIndicies.push_back(0);
	m_tHealthVignette.m_nvIndicies.push_back(2);
	m_tHealthVignette.m_nvIndicies.push_back(3);

	m_tHealthVignette.m_cvVertecies.push_back
		({
			{ -2.0f, 1.25f, -1.0f, 1 },
			{ 0, 0 }
	});

	m_tHealthVignette.m_cvVertecies.push_back
		({
			{ 2.0f, 1.25f, -1.0f, 1 },
			{ 1, 0 }
	});

	m_tHealthVignette.m_cvVertecies.push_back
		({
			{ 2.0f, -1.25f, -1.0f, 1 },
			{ 1, 1 }
	});

	m_tHealthVignette.m_cvVertecies.push_back
		({
			{ -2.0f, -1.25f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_tHealthVignette.m_cpRenderMesh = new CHUDMesh(m_tHealthVignette.m_nvIndicies, m_tHealthVignette.m_cvVertecies, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/HealthVignette.dds");
	m_tHealthVignette.m_cpRenderMesh->GetTransparent() = true;
	GRAPHICS->AddRenderMesh(m_tHealthVignette.m_cpRenderMesh);
	m_tHealthVignette.m_cpRenderMesh->GetRender() = false;

#pragma endregion

#pragma region Circle Image Set Up

	m_tCircle.m_nvIndicies.push_back(0);
	m_tCircle.m_nvIndicies.push_back(1);
	m_tCircle.m_nvIndicies.push_back(2);

	m_tCircle.m_nvIndicies.push_back(0);
	m_tCircle.m_nvIndicies.push_back(2);
	m_tCircle.m_nvIndicies.push_back(3);

	m_tCircle.m_cvVertecies.push_back//top left
		({
			{ -0.25f, 0.5f, -1.0f, 1 },
			{ 0, 0 }
	});

	m_tCircle.m_cvVertecies.push_back//top right
		({
			{ 0.5f, 0.5f, -1.0f, 1 },
			{ 1, 0 }
	});

	m_tCircle.m_cvVertecies.push_back//bottom right
		({
			{ 0.5f, -0.25f, -1.0f, 1 },
			{ 1, 1 }
	});

	m_tCircle.m_cvVertecies.push_back//bottom left
		({
			{ -0.25f, -0.25f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_tCircle.m_cpRenderMesh = new CHUDMesh(m_tCircle.m_nvIndicies, m_tCircle.m_cvVertecies, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/ClockOutline.dds");
	m_tCircle.m_cpRenderMesh->GetTransparent() = true;
	GRAPHICS->AddRenderMesh(m_tCircle.m_cpRenderMesh);

	TranslateLocalY(m_tCircle.m_cpRenderMesh->GetPositionMatrix(), 0.505f); //Move up
	TranslateLocalX(m_tCircle.m_cpRenderMesh->GetPositionMatrix(), 0.505f); //Move right


#pragma endregion

#pragma region Icons Image Set Up

	m_tIcons.m_nvIndicies.push_back(0);
	m_tIcons.m_nvIndicies.push_back(1);
	m_tIcons.m_nvIndicies.push_back(2);

	m_tIcons.m_nvIndicies.push_back(0);
	m_tIcons.m_nvIndicies.push_back(2);
	m_tIcons.m_nvIndicies.push_back(3);

	m_tIcons.m_cvVertecies.push_back//top left
		({
			{ -0.75f, 0.75f, -1.0f, 1 },
			{ 0, 0 }
	});

	m_tIcons.m_cvVertecies.push_back//top right
		({
			{ 0.75f, 0.75f, -1.0f, 1 },
			{ 1, 0 }
	});

	m_tIcons.m_cvVertecies.push_back//bottom right
		({
			{ 0.75f, -0.75f, -1.0f, 1 },
			{ 1, 1 }
	});

	m_tIcons.m_cvVertecies.push_back//bottom left
		({
			{ -0.75f, -0.75f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_tIcons.m_cpRenderMesh = new CHUDMesh(m_tIcons.m_nvIndicies, m_tIcons.m_cvVertecies, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/SunAndMoonIcons.dds");
	m_tIcons.m_cpRenderMesh->GetTransparent() = true;

	XMStoreFloat4x4(&m_tIcons.m_cpRenderMesh->GetTransformMatrix(), XMMatrixIdentity());
	TranslateLocalX(m_tIcons.m_cpRenderMesh->GetPositionMatrix(), 1.025f); //Move right
	TranslateLocalY(m_tIcons.m_cpRenderMesh->GetPositionMatrix(), 1.03f); //Move up
	RotateLocalZ(m_tIcons.m_cpRenderMesh->GetPositionMatrix(), 90.0f);//start of day

	GRAPHICS->AddRenderMesh(m_tIcons.m_cpRenderMesh);

#pragma endregion

#pragma region Inventory Image Set Up

	m_tInventory.m_nvIndicies.push_back(0);
	m_tInventory.m_nvIndicies.push_back(1);
	m_tInventory.m_nvIndicies.push_back(2);

	m_tInventory.m_nvIndicies.push_back(0);
	m_tInventory.m_nvIndicies.push_back(2);
	m_tInventory.m_nvIndicies.push_back(3);

	m_tInventory.m_cvVertecies.push_back
		({
			{ -0.1f, 0.15f, -1.0f, 1 },
			{ 0, 0 }
	});

	m_tInventory.m_cvVertecies.push_back
		({
			{ 0.1f, 0.15f, -1.0f, 1 },
			{ 1, 0 }
	});

	m_tInventory.m_cvVertecies.push_back
		({
			{ 0.1f, -0.15f, -1.0f, 1 },
			{ 1, 1 }
	});

	m_tInventory.m_cvVertecies.push_back
		({
			{ -0.1f, -0.15f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_tInventory.m_cpRenderMesh = new CHUDMesh(m_tInventory.m_nvIndicies, m_tInventory.m_cvVertecies, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/BackPack.dds");
	m_tInventory.m_cpRenderMesh->GetTransparent() = true;

	TranslateLocalX(m_tInventory.m_cpRenderMesh->GetPositionMatrix(), 0.8f); //Move right
	TranslateLocalY(m_tInventory.m_cpRenderMesh->GetPositionMatrix(), -0.7f); //Move down

	GRAPHICS->AddRenderMesh(m_tInventory.m_cpRenderMesh);

#pragma endregion

#pragma region Health Potions Set Up

	m_tHealthPotionIcon.m_nvIndicies.push_back(0);
	m_tHealthPotionIcon.m_nvIndicies.push_back(1);
	m_tHealthPotionIcon.m_nvIndicies.push_back(2);

	m_tHealthPotionIcon.m_nvIndicies.push_back(0);
	m_tHealthPotionIcon.m_nvIndicies.push_back(2);
	m_tHealthPotionIcon.m_nvIndicies.push_back(3);

	m_tHealthPotionIcon.m_cvVertecies.push_back
		({
			{ -0.15f, 0.2f, -1.0f, 1 },
			{ 0, 0 }
	});

	m_tHealthPotionIcon.m_cvVertecies.push_back
		({
			{ 0.15f, 0.2f, -1.0f, 1 },
			{ 1, 0 }
	});

	m_tHealthPotionIcon.m_cvVertecies.push_back
		({
			{ 0.15f, -0.2f, -1.0f, 1 },
			{ 1, 1 }
	});

	m_tHealthPotionIcon.m_cvVertecies.push_back
		({
			{ -0.15f, -0.2f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_tHealthPotionIcon.m_cpRenderMesh = new CHUDMesh(m_tHealthPotionIcon.m_nvIndicies, m_tHealthPotionIcon.m_cvVertecies, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/HealthPotion.dds");
	m_tHealthPotionIcon.m_cpRenderMesh->GetTransparent() = true;

	TranslateLocalX(m_tHealthPotionIcon.m_cpRenderMesh->GetPositionMatrix(), -0.9f); 
	TranslateLocalY(m_tHealthPotionIcon.m_cpRenderMesh->GetPositionMatrix(), 0.85f); 

	GRAPHICS->AddRenderMesh(m_tHealthPotionIcon.m_cpRenderMesh);

#pragma endregion

#pragma region Exit Key Icon Set Up

	m_tExitKeyIcon.m_nvIndicies.push_back(0);
	m_tExitKeyIcon.m_nvIndicies.push_back(1);
	m_tExitKeyIcon.m_nvIndicies.push_back(2);
	m_tExitKeyIcon.m_nvIndicies.push_back(0);
	m_tExitKeyIcon.m_nvIndicies.push_back(2);
	m_tExitKeyIcon.m_nvIndicies.push_back(3);

	m_tExitKeyIcon.m_cvVertecies.push_back//top left
		({
			{ 0.0f, 0.0f, -1.0f, 1 },
			{ 0, 0 }
	});

	m_tExitKeyIcon.m_cvVertecies.push_back//top right
		({
			{ 0.2f, 0.0f, -1.0f, 1 },
			{ 1, 0 }
	});

	m_tExitKeyIcon.m_cvVertecies.push_back//bottom right
		({
			{ 0.2f, -0.2f, -1.0f, 1 },
			{ 1, 1 }
	});

	m_tExitKeyIcon.m_cvVertecies.push_back//bottom left
		({
			{ 0.0f, -0.2f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_tExitKeyIcon.m_cpRenderMesh = new CHUDMesh(m_tExitKeyIcon.m_nvIndicies, m_tExitKeyIcon.m_cvVertecies, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/ExitKey.dds");
	m_tExitKeyIcon.m_cpRenderMesh->GetTransparent() = true;
	m_tExitKeyIcon.m_cpRenderMesh->GetRender() = false;
	TranslateLocalX(m_tExitKeyIcon.m_cpRenderMesh->GetPositionMatrix(), 0.7f);
	TranslateLocalY(m_tExitKeyIcon.m_cpRenderMesh->GetPositionMatrix(), -0.3f);
	GRAPHICS->AddRenderMesh(m_tExitKeyIcon.m_cpRenderMesh);

#pragma endregion

}

/*****************************************************************
* ~CHUD()				The destructor for the HUD
*
* Ins:					None
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    09/08/2015
* Mod. Initials:	    NH
*****************************************************************/
CHUD::~CHUD()
{
	delete m_tHealthBar.m_cpRenderMesh;
	delete m_tHealthFill.m_cpRenderMesh;
	delete m_tHealthVignette.m_cpRenderMesh;
	delete m_tStaminaFill.m_cpRenderMesh;

	delete m_tCircle.m_cpRenderMesh;
	delete m_tIcons.m_cpRenderMesh;

	delete m_tInventory.m_cpRenderMesh;

	delete m_tHealthPotionIcon.m_cpRenderMesh;

	delete m_tExitKeyIcon.m_cpRenderMesh;
}

/*****************************************************************
* Update()				Updates all elements of the HUD
*
* Ins:					playerHealth
*						numHealthPotions
*						worldTime
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    09/08/2015
* Mod. Initials:	    NH
*****************************************************************/
void CHUD::Update(int playerHealth, int maxHealth, int numHealthPotions, float stamina)
{
#pragma region Player Health Potions

	string potionsString = to_string(numHealthPotions);
	wstring wPotionsString(potionsString.begin(), potionsString.end());

	if (bOff == false)
		GRAPHICS->RenderText(wPotionsString.c_str(), { -850.0f, 750.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.65f, 0.65f }, Center);

#pragma endregion

#pragma region Player Health Bar

	if (m_nPlayerHealth != playerHealth)
	{
		if (playerHealth <= 0)
		{
			m_nPlayerHealth = 0;
		}
		else
		{
			m_nPlayerHealth = playerHealth;
		}

		if (m_nPlayerHealth > LOW_HEALTH)
		{
			m_bHealthFlashON = false;
			m_bLightFlashON = false;
			m_tHealthFill.m_cpRenderMesh->SetTexture(L"../Game/Assets/Art/2D/HUD/HealthBarGreen.dds");
			m_tHealthVignette.m_cpRenderMesh->GetRender() = false;
		}

		//player has low health so change health bar to flash
		if (m_nPlayerHealth <= LOW_HEALTH)
		{
			if (m_bHealthFlashON == false)
			{
				//change health bar texture to red
				m_tHealthFill.m_cpRenderMesh->SetTexture(L"../Game/Assets/Art/2D/HUD/HealthBarFlash.dds");

				//add vignette to screen
				m_tHealthVignette.m_cpRenderMesh->GetRender() = true;

				m_bHealthFlashON = true;
			}
		}

		//update the verts of the health fill
		for (unsigned int i = 0; i < m_tHealthFill.m_cvVertecies.size(); i++)
		{
			if (m_tHealthFill.m_cvVertecies[i].m_fTexCoord[0] == 1)
			{
				float VertOffsetX = float((m_nPlayerHealth) / (float)m_nMaxHealth) * m_fHealthBarOffset;
				m_tHealthFill.m_cpRenderMesh->GetVertex2DArr()[i].m_fPosition[0] = VertOffsetX;
			}
		}
	}

	if (m_bHealthFlashON == true)
	{
		m_fHealthFlashTimer += DELTA_TIME();

		if (m_fHealthFlashTimer > FLASH_TIME)
		{
			m_fHealthFlashTimer = 0.0f;
			m_bLightFlashON = !m_bLightFlashON;

			if (m_bLightFlashON == true)//display the light red flash
			{
				m_tHealthFill.m_cpRenderMesh->SetTexture(L"../Game/Assets/Art/2D/HUD/HealthBarFlash2.dds");
			}
			else//display the dark flash
			{
				m_tHealthFill.m_cpRenderMesh->SetTexture(L"../Game/Assets/Art/2D/HUD/HealthBarFlash.dds");
			}
		}
	}


#pragma endregion

#pragma region Day and Night

	//sun and moon icons
	XMStoreFloat4x4(&m_tIcons.m_cpRenderMesh->GetTransformMatrix(), XMMatrixIdentity());
	TranslateLocalX(m_tIcons.m_cpRenderMesh->GetPositionMatrix(), 1.025f); //Move right
	TranslateLocalY(m_tIcons.m_cpRenderMesh->GetPositionMatrix(), 1.03f); //Move up
	
	RotateLocalZ(m_tIcons.m_cpRenderMesh->GetTransformMatrix(), -(m_fWorldTime / 2.0f) + 90);

	if (bOff == false)
	{
		//day counter
		string dayCounterString = to_string(m_unDayCounter);
		wstring wDayCounterString(dayCounterString.begin(), dayCounterString.end());

		XMFLOAT4 f4Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		if (m_unDayCounter > 3)//if its the last day
		{
			f4Color = { 1.0f, 0.0f, 0.0f, 1.0f };//change color to red
			if (m_cpDayNight != nullptr)
			{
				if (m_cpDayNight->IsDay() == true)
					GRAPHICS->RenderText(L"Last Day", { 900.0f, 985.0f }, f4Color, { 0.35f, 0.3f }, Center);
				else if (m_cpDayNight->IsNight() == true)
					GRAPHICS->RenderText(L"Last Night", { 895.0f, 985.0f }, f4Color, { 0.35f, 0.3f }, Center);
			}
			else
			{
				GRAPHICS->RenderText(L"Last Day", { 900.0f, 985.0f }, f4Color, { 0.35f, 0.3f }, Center);
			}

		}
		else
		{
			if (m_cpDayNight != nullptr)
			{
				if (m_cpDayNight->IsDay() == true)
					GRAPHICS->RenderText(L"Day", { 910.0f, 985.0f }, f4Color, { 0.45f, 0.4f }, Center);
				else if (m_cpDayNight->IsNight() == true)
					GRAPHICS->RenderText(L"Night", { 905.0f, 985.0f }, f4Color, { 0.45f, 0.4f }, Center);
			}
			else
			{
				GRAPHICS->RenderText(L"Day", { 910.0f, 985.0f }, f4Color, { 0.45f, 0.4f }, Center);
			}

		}

		GRAPHICS->RenderText(wDayCounterString.c_str(), { 910.0f, 925.0f }, f4Color, { 0.85f, 0.8f }, Center);
	}

#pragma endregion
	
#pragma region Player Stamina. COMMENTED OUT

	/*
	if (m_fStamina != stamina)//there is a change in health since last frame
	{
		if (stamina <= 0.99f)
		{
			m_fStamina = 0;
		}
		else
		{
			m_fStamina = stamina;
		}

		//player has low health so change health bar to flash
		//if (m_fStamina <= LOW_HEALTH)
		//{
		//	if (m_bHealthFlashON == false)
		//	{
		//		//change health bar texture to red
		//		m_tStaminaFill.m_cpRenderMesh->SetTexture(L"../Game/Assets/Art/2D/HUD/HealthBarFlash.dds");
		//
		//		//add vignette to screen
		//		GRAPHICS->AddRenderMesh(m_tHealthVignette.m_cpRenderMesh);
		//
		//		m_bHealthFlashON = true;
		//	}
		//}

		//update the verts of the health fill
		for (unsigned int i = 0; i < m_tStaminaFill.m_cvVertecies.size(); i++)
		{
			if (m_tStaminaFill.m_cvVertecies[i].m_fTexCoord[0] == 1)
			{
				float VertOffsetX = float((m_fStamina) / 100.0f) * HEALTH_BAR_XOFFSET;
				m_tStaminaFill.m_cpRenderMesh->GetVertex2DArr()[i].m_fPosition[0] = VertOffsetX;
			}
		}
	}
	*/
#pragma endregion

}

/*****************************************************************
* SetWorldTime()		Sets the world time
*
* Ins:					time
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    09/18/2015
* Mod. Initials:	    NH
*****************************************************************/
void CHUD::SetWorldTime(float time)
{
	m_fWorldTime = time;
}

/*****************************************************************
* SetDayCounter()		Sets the day counter
*
* Ins:					day
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    09/28/2015
* Mod. Initials:	    NH
*****************************************************************/
void CHUD::SetDayCounter(int day)
{
	m_unDayCounter = day;
}

/*****************************************************************
* IncrementDayCounter()		Adds 1 to the day counter
*
* Ins:						None
*
* Outs:						None
*
* Returns:					None
*
* Mod. Date:				09/28/2015
* Mod. Initials:			NH
*****************************************************************/
void CHUD::IncrementDayCounter()
{
	m_unDayCounter++;
}

void CHUD::SetMaxHealth(int _nMaxHealth)
{
	m_nMaxHealth = _nMaxHealth;

	float XOffset = 1.0f;
	if (m_nMaxHealth == 125)
	{
		XOffset += HEALTH_OUTLINE_RATIO;
	}
	else if (m_nMaxHealth == 150)
	{
		XOffset += HEALTH_OUTLINE_RATIO * 2;
	}
	else if (m_nMaxHealth == 175)
	{
		XOffset += HEALTH_OUTLINE_RATIO * 3;
	}

	m_tHealthBar.m_cpRenderMesh->GetVertex2DArr()[1].m_fPosition[0] = XOffset;
	m_tHealthBar.m_cpRenderMesh->GetVertex2DArr()[2].m_fPosition[0] = XOffset;

	m_fHealthBarOffset += HEALTH_BAR_XOFFSET;
	TranslateLocalX(m_tHealthFill.m_cpRenderMesh->GetTransformMatrix(), 0.00825f);

}

void CHUD::ExitKeyFound()
{
	m_tExitKeyIcon.m_cpRenderMesh->GetRender() = true;
}

void CHUD::AddToRenderer()
{
	bOff = false;

	if (m_tHealthBar.m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_tHealthBar.m_cpRenderMesh);
	
	if (m_tHealthFill.m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_tHealthFill.m_cpRenderMesh);

	if (m_tHealthVignette.m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_tHealthVignette.m_cpRenderMesh);

	if (m_tHealthPotionIcon.m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_tHealthPotionIcon.m_cpRenderMesh);


	if (m_tCircle.m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_tCircle.m_cpRenderMesh);

	if (m_tIcons.m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_tIcons.m_cpRenderMesh);

	if (m_tInventory.m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_tInventory.m_cpRenderMesh);

	if (m_tExitKeyIcon.m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_tExitKeyIcon.m_cpRenderMesh);
}
