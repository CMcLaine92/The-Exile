/***********************************************
* Filename:  		CBridge.cpp
* Date:      		09/13/2015
* Mod. Date:
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			Handles both transperant bridge and collidable bridge.
************************************************/
#include "Bridge.h"
#include "../../../Engine/Animation/Mesh.h"
#include "../../../Engine/Core/Input/InputManager.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Core/Messages/AddObjectMessage.h"
#include "../../../Engine/Core/Messages/RemoveObjectMessage.h"
#include "../../../Engine/Renderer/HUDMesh.h"
#include "../../../Engine/Renderer/Renderer.h"

#define RESET_DELAY 2.0f
#define GRAVITY -1500.0f
#define BUILD_TIMER 3.0f
#define BUILDING_BAR_LEFT -0.3f

void CBridge::FindOrientation(bool vertical)
{

	if (vertical)
	{
		//RotateLocalY(m_mWorld, 90.0f);

		float xExtent = ((CAABB*)m_pvColliders[0]->GetBounds())->GetExtents().x;
		float yExtent = 100.0f;
		float zExtent = ((CAABB*)m_pvColliders[0]->GetBounds())->GetExtents().z - 20.0f;
		((CAABB*)m_pvColliders[0]->GetBounds())->SetExtents(XMFLOAT3{ xExtent, yExtent, zExtent + 180.0f });


		XMFLOAT3 leftCenter = *GetPosition();
		leftCenter.x += xExtent - 10.0f;
		leftCenter.y += 120.0f;

		CAABB* leftRail = new CAABB(leftCenter, XMFLOAT3{ 30.0f, 200, zExtent - 20.0f });
		AddCollider(new CCollider(false, leftRail, true, false));

		XMFLOAT3 rightCenter = *GetPosition();
		rightCenter.x -= xExtent - 10.0f;
		rightCenter.y += 120.0f;

		CAABB* rightRail = new CAABB(rightCenter, XMFLOAT3{ 30.0f, 200, zExtent - 20.0f });
		AddCollider(new CCollider(false, rightRail, true, false));
	}
	else
	{
		float xExtent = ((CAABB*)m_pvColliders[0]->GetBounds())->GetExtents().x;
		float yExtent = 100.0f;
		float zExtent = ((CAABB*)m_pvColliders[0]->GetBounds())->GetExtents().z - 20.0f;
		((CAABB*)m_pvColliders[0]->GetBounds())->SetExtents(XMFLOAT3{ xExtent + 180.0f, yExtent, zExtent });


		XMFLOAT3 leftCenter = *GetPosition();
		leftCenter.z += zExtent - 10.0f;
		leftCenter.y += 120.0f;

		CAABB* leftRail = new CAABB(leftCenter, XMFLOAT3{ xExtent - 20.0f, 200, 30.0f });
		AddCollider(new CCollider(false, leftRail, true, false));

		XMFLOAT3 rightCenter = *GetPosition();
		rightCenter.z -= zExtent - 10.0f;
		rightCenter.y += 120.0f;

		CAABB* rightRail = new CAABB(rightCenter, XMFLOAT3{ xExtent - 20.0f, 200, 30.0f });
		AddCollider(new CCollider(false, rightRail, true, false));
	}

}

CBridge::CBridge(XMFLOAT3 position, bool vertical) : IObject("Bridge")
{
	XMFLOAT3 newLocation = position;
	newLocation.y = -32.0f;
	SetPosition(newLocation);
	//SetPosition(XMFLOAT3{ 0, 0, 0 });
	CMesh* ubase = nullptr;
	CMesh* base = nullptr;
	CMesh* plank = nullptr;
	m_nNumFallingPlanks = 0;
	m_fNextFallTimer = 0.0f;
	m_nFallingIndex = 0;
	m_bActive = false;
	m_bCollided = false;
	m_bCollapsing = false;
	m_bFalling = false;
	m_bIntact = false;
	m_fDelayTimer = RESET_DELAY;
	m_fBuildTimer = 0.0f;
	m_bBuildingBar = false;

	

	m_bHasBeenBuilt = false;


	if (vertical)
	{
		ubase = CAssetManager::GetInstance()->GetPrefabMesh("UnbuiltBridgeBaseV");
		base = CAssetManager::GetInstance()->GetPrefabMesh("BridgeBaseV");
	}
	else
	{
		ubase = CAssetManager::GetInstance()->GetPrefabMesh("UnbuiltBridgeBaseH");
		base = CAssetManager::GetInstance()->GetPrefabMesh("BridgeBaseH");
	}

	plank = CAssetManager::GetInstance()->GetPrefabMesh("Plank");

	m_cpBuiltBase = new CRenderMesh(base, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Failbot_Bridge.dds");
	m_cpRenderMesh = new CRenderMesh(ubase, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Failbot_Bridge.dds");
	AddCollider(new CCollider(true, Bounds::AABB, ubase->GetVertices(), true, false));
	AddCollider(new CCollider(true, new CAABB(base->GetAABBs()[1].center, base->GetAABBs()[1].extents), true, false));


	float startPos = 700.0f;
	for (size_t i = 0; i < 8; i++)
	{
		m_tPlanks[i].renderMesh = new CRenderMesh(plank, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Plank.dds");
		m_tPlanks[i].renderMesh->GetRender() = false;
		if (vertical)
		{
			RotateLocalY(m_tPlanks[i].renderMesh->GetPositionMatrix(), 90.0f);

			m_tPlanks[i].renderMesh->GetPositionMatrix()._43 = startPos;

			CMesh::TAABB aabb = plank->GetAABBs()[0];
			float temp = aabb.extents.x;
			aabb.center.z = startPos;
			aabb.center.y = newLocation.y;
			aabb.extents.x = aabb.extents.z;
			aabb.extents.z = temp;
			m_tPlanks[i].collider = new CCollider(true, new CAABB(aabb.center, aabb.extents), true, false);
			AddCollider(m_tPlanks[i].collider);

		}
		else
		{
			m_tPlanks[i].renderMesh->GetPositionMatrix()._41 = startPos;

			CMesh::TAABB aabb = plank->GetAABBs()[0];
			aabb.center.x = startPos;
			aabb.center.y = newLocation.y;
			m_tPlanks[i].collider = new CCollider(true, new CAABB(aabb.center, aabb.extents), true);
			AddCollider(m_tPlanks[i].collider);
		}
		startPos -= 200.0f;
		GRAPHICS->AddRenderMesh(m_tPlanks[i].renderMesh);
	}
	FindOrientation(vertical);



	GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	GRAPHICS->AddRenderMesh(m_cpBuiltBase);
	m_cpBuiltBase->GetRender() = false;

	((CAABB*)m_pvColliders[0]->GetBounds())->SetCenter({ m_mWorld._41, 0, m_mWorld._43 });

#pragma region TextFrame

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
			{ -0.5f, 0.6f, -1.0f, 1 },
			{ 0, 0 }
	});

	ScreenVerts.push_back // Top Right
		({
			{ 0.5f, 0.6f, -1.0f, 1 },
			{ 1, 0 }
	});

	ScreenVerts.push_back // Bot Right
		({
			{ 0.5f, 0.0f, -1.0f, 1 },
			{ 1, 1 }
	});

	ScreenVerts.push_back // Bot Left
		({
			{ -0.5f, 0.0f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_cpTextFrame = new CHUDMesh(ScreenIndicies, ScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/Failbot_TextBackground.dds");
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

	m_cpControllerPopUp = new CHUDMesh(tempInd, tempVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Build_Controller_PopUp.dds");
	m_cpControllerPopUp->GetRender() = false;
	m_cpControllerPopUp->GetTransparent() = true;
	TranslateLocalY(m_cpControllerPopUp->GetTransformMatrix(), 0.4f);
	GRAPHICS->AddRenderMesh(m_cpControllerPopUp);

#pragma endregion

#pragma region Building Bar

	tempVerts.clear();

	tempVerts.push_back // Top Left
		({
			{ BUILDING_BAR_LEFT, 0.1f, -1.0f, 1 },
			{ 0, 0 }
	});

	tempVerts.push_back // Top Right
		({
			{ 0.3f, 0.1f, -1.0f, 1 },
			{ 1, 0 }
	});

	tempVerts.push_back // Bot Right
		({
			{ 0.3f, -0.1f, -1.0f, 1 },
			{ 1, 1 }
	});

	tempVerts.push_back // Bot Left
		({
			{ BUILDING_BAR_LEFT, -0.1f, -1.0f, 1 },
			{ 0, 1 }
	});

	m_fBarLength = 0.6f;

	m_cpBuildingBar = new CHUDMesh(tempInd, tempVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Inventory/Failbot_Craft_Bar.dds");
	m_cpBuildingBar->GetRender() = false;
	m_cpBuildingBar->GetTransparent() = true;
	TranslateLocalY(m_cpBuildingBar->GetTransformMatrix(), 0.25f);
	GRAPHICS->AddRenderMesh(m_cpBuildingBar);

#pragma endregion

}

void CBridge::Collapse()
{
	if (m_bCollapsing) // If the bridge starts collapsing
	{
		m_fNextFallTimer -= DELTA_TIME(); //increment next fall timer
		if (m_fNextFallTimer < 0.0f) //if we're ready to make the next plank fall
		{
			m_tPlanks[m_nFallingIndex].falling = true;
			m_nNumFallingPlanks++;
			m_bFalling = true;
			m_fNextFallTimer = 0.25f;
			m_nFallingIndex++;
			if (m_nFallingIndex > 8)
			{
				m_bCollapsing = false;
				m_nFallingIndex = 0;
			}
		}
	}
	if (m_bFalling) //if there are any falling planks
	{
		for (int i = 0; i < 8; i++)
		{
			if (m_tPlanks[i].falling)
			{

				/*m_tPlanks[i].falling = false;
				m_tPlanks[i].renderMesh->GetRender() = false;
				m_nNumFallingPlanks--;*/


				float newY = m_tPlanks[i].renderMesh->GetPositionMatrix()._42 + GRAVITY * DELTA_TIME();

				if (newY >= -700.0f) // if the planks is hasnt hit the floor yet, update it mesh and collider
				{
					m_tPlanks[i].renderMesh->GetPositionMatrix()._42 = newY;
					XMFLOAT3 center = ((CAABB*)m_tPlanks[i].collider->GetBounds())->GetCenter();
					((CAABB*)m_tPlanks[i].collider->GetBounds())->SetCenter((XMFLOAT3(center.x, newY, center.z)));
				}
				else //otherwise, stop it from falling and decrement the number of falling planks
				{
					m_tPlanks[i].falling = false;
					m_nNumFallingPlanks--;
				}
			}
		}
		if (m_nNumFallingPlanks = 0)  //if there are no more falling planks after this update, stop running the falling code
			m_bFalling = false;
	}
}

void CBridge::SetWorldMatrix(const XMFLOAT4X4 world)
{
	m_mWorld = world;
	m_cpBuiltBase->GetPositionMatrix() = world;
	m_cpRenderMesh->GetPositionMatrix() = world;
	for (size_t i = 0; i < 8; i++)
	{
		m_tPlanks[i].renderMesh->GetPositionMatrix()._41 += world._41;
		m_tPlanks[i].renderMesh->GetPositionMatrix()._42 += world._42;
		m_tPlanks[i].renderMesh->GetPositionMatrix()._43 += world._43;
	}

}

CBridge::~CBridge()
{
	for (int i = 0; i < 8; i++)
	{
		SAFE_DELETE(m_tPlanks[i].renderMesh);
		//SAFE_DELETE(m_tPlanks[i].collider);

		OUTPUT("Delete No: " + to_string(i) + '\n');
	}
	//

	
	SAFE_DELETE(m_cpTextFrame);
	OUTPUT("Delete No: " + to_string(9) + '\n');
	//SAFE_DELETE(m_cpControllerPopUp);
	OUTPUT("Delete No: " + to_string(10) + '\n');

	SAFE_DELETE(m_cpBuiltBase);
	OUTPUT("Delete No: " + to_string(11) + '\n');

	SAFE_DELETE(m_cpBuildingBar);
	OUTPUT("Delete No: " + to_string(12) + '\n');

}

void CBridge::Update()
{
	if (m_bCollided == true && m_bActive == false)
	{
		if (CInputManager::GetInstance()->GetControllerBeingUsed() == true)
		{
			m_cpControllerPopUp->GetRender() = true;
			m_cpTextFrame->GetRender() = false;
		}
		else
		{
			m_cpControllerPopUp->GetRender() = false;
			m_cpTextFrame->GetRender() = true;
		}
	}

	if (m_bCollided == false)
	{
		m_cpTextFrame->GetRender() = false;
		m_cpControllerPopUp->GetRender() = false;
	}
	m_bCollided = false;

	Collapse();


	//building bar
	if (m_bBuildingBar == true)
	{
		m_fBuildTimer += DELTA_TIME();
		if (m_fBuildTimer < BUILD_TIMER)
		{
			if (m_bHasBeenBuilt == false)
			{
				GRAPHICS->RenderText(L"F to Build", { 0.0f, 500.0f }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ .5f, .5f }, eTextMod::Center);
			}
			else
			{
				GRAPHICS->RenderText(L"F to Rebuild", { 0.0f, 500.0f }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ .5f, .5f }, eTextMod::Center);
			}

			m_cpTextFrame->GetVertex2DArr()[0].m_fPosition[0] = -0.4f;
			m_cpTextFrame->GetVertex2DArr()[1].m_fPosition[0] = 0.4f;
			m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[0] = 0.4f;
			m_cpTextFrame->GetVertex2DArr()[3].m_fPosition[0] = -0.4f;

			m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[1] = 0.0f;
			m_cpTextFrame->GetVertex2DArr()[3].m_fPosition[1] = 0.0f;

			float fRatio = m_fBuildTimer / BUILD_TIMER;
			m_cpBuildingBar->GetVertex2DArr()[1].m_fPosition[0] = BUILDING_BAR_LEFT + (m_fBarLength * fRatio);
			m_cpBuildingBar->GetVertex2DArr()[2].m_fPosition[0] = BUILDING_BAR_LEFT + (m_fBarLength * fRatio);
		}
	}


	/*for (int i = 0; i < m_pvColliders.size(); i++)
	{
		CAABB* aabb = ((CAABB*)m_pvColliders[i]->GetBounds());
		DEBUG_UTIL::DrawAABBCollider(aabb->Min(), aabb->Max());
	}*/
}

void CBridge::Destroy()
{
	if (m_bIntact)
	{
		m_bCollapsing = true;
		m_bIntact = false;
		m_bActive = false;
	}
}

void CBridge::Activate()
{
	if (m_bHasBeenBuilt == true)
	{
		Rebuild();
	}

	m_cpRenderMesh->GetRender() = false;
	m_cpRenderMesh->GetTransparencyOverride() = 0.0f;

	GRAPHICS->RemoveRenderMesh(m_cpRenderMesh);

	m_cpBuiltBase->GetRender() = true;
	for (int i = 0; i < 8; i++)
	{
		m_tPlanks[i].renderMesh->GetRender() = true;

	}

	m_bIntact = true;
	m_bActive = true;
	m_cpRenderMesh->SetTexture(L"../Game/Assets/Art/2D/Textures/Failbot_Bridge.dds");
	m_cpTextFrame->GetRender() = false;
	m_cpControllerPopUp->GetRender() = false;

	m_bHasBeenBuilt = true;

	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::CRAFT);
	CInputManager::GetInstance()->VibrateController(CONTROLLER_SOFT_VIBRATE, CONTROLLER_SOFT_VIBRATE, 0.75f);
}

void CBridge::Rebuild()
{
	
	for (size_t i = 0; i < 8; i++)
	{
		
		m_tPlanks[i].renderMesh->GetRender() = true;
		XMFLOAT3 center = ((CAABB*)(m_tPlanks[i].collider->GetBounds()))->GetCenter();
		m_tPlanks[i].renderMesh->SetPosition({ center.x, 0.0f, center.z });
		((CAABB*)(m_tPlanks[i].collider->GetBounds()))->SetCenter({ center.x, 0.0f, center.z });
		m_tPlanks[i].falling = false;

	}
	m_bIntact = true;

}

void CBridge::DrawAvailableIndicator()
{
	if (m_bHasBeenBuilt == true)//repair
	{
		if (CInputManager::GetInstance()->GetControllerBeingUsed())
		{
			m_cpControllerPopUp->GetRender() = true;
			m_cpControllerPopUp->SetTexture(L"../Game/Assets/Art/2D/Textures/Rebuild_Controller_PopUp.dds");
		}
		else
		{
			GRAPHICS->RenderText(L"F to Rebuild", { 0.0f, 500.0f }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ .5f, .5f }, eTextMod::Center);

			m_cpTextFrame->GetVertex2DArr()[0].m_fPosition[0] = -0.3f;
			m_cpTextFrame->GetVertex2DArr()[1].m_fPosition[0] = 0.3f;
			m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[0] = 0.3f;
			m_cpTextFrame->GetVertex2DArr()[3].m_fPosition[0] = -0.3f;

			m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[1] = 0.3f;
			m_cpTextFrame->GetVertex2DArr()[3].m_fPosition[1] = 0.3f;

			m_cpTextFrame->GetRender() = true;
		}
	}
	else//build for first time
	{
		if (CInputManager::GetInstance()->GetControllerBeingUsed())
		{
			m_cpControllerPopUp->GetRender() = true;
			m_cpControllerPopUp->SetTexture(L"../Game/Assets/Art/2D/Textures/Build_Controller_PopUp.dds");
		}
		else
		{
			GRAPHICS->RenderText(L"F to Build", { 0.0f, 500.0f }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ .5f, .5f }, eTextMod::Center);

			//m_cpTextFrame->GetVertex2DArr()[0].m_fPosition[0] = -0.4f;
			//m_cpTextFrame->GetVertex2DArr()[1].m_fPosition[0] = 0.4f;
			//m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[0] = 0.4f;
			//m_cpTextFrame->GetVertex2DArr()[3].m_fPosition[0] = -0.4f;
			//
			//m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[1] = 0.0f;
			//m_cpTextFrame->GetVertex2DArr()[3].m_fPosition[1] = 0.0f;

			m_cpTextFrame->GetVertex2DArr()[0].m_fPosition[0] = -0.3f;
			m_cpTextFrame->GetVertex2DArr()[1].m_fPosition[0] = 0.3f;
			m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[0] = 0.3f;
			m_cpTextFrame->GetVertex2DArr()[3].m_fPosition[0] = -0.3f;

			m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[1] = 0.3f;
			m_cpTextFrame->GetVertex2DArr()[3].m_fPosition[1] = 0.3f;

			m_cpTextFrame->GetRender() = true;
		}
	}

	m_cpRenderMesh->SetTexture(L"../Game/Assets/Art/2D/Textures/Failbot_Green_Bridge.dds");
}

void CBridge::DrawUnAvailableIndicator()
{
	if (m_bHasBeenBuilt == true)
	{
		GRAPHICS->RenderText(L"Not Enough Wood Planks to Repair", { 0.0f, 500.0f }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ 0.5f, 0.5f }, eTextMod::Center);
		GRAPHICS->RenderText(L"Requires: 8 Wood Planks", { 0.0f, 400.0f }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ 0.5f, 0.5f }, eTextMod::Center);
	}
	else
	{
		GRAPHICS->RenderText(L"Not Enough Resources to Build", { 0.0f, 500.0f }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ 0.5f, 0.5f }, eTextMod::Center);
		GRAPHICS->RenderText(L"Requires: 6 Bones and 8 Wood Planks", { 0.0f, 400.0f }, XMFLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ 0.5f, 0.5f }, eTextMod::Center);
	}

	m_cpRenderMesh->SetTexture(L"../Game/Assets/Art/2D/Textures/Failbot_Red_Bridge.dds");

	m_cpTextFrame->GetVertex2DArr()[0].m_fPosition[0] = -0.7f;
	m_cpTextFrame->GetVertex2DArr()[1].m_fPosition[0] = 0.7f;
	m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[0] = 0.7f;
	m_cpTextFrame->GetVertex2DArr()[3].m_fPosition[0] = -0.7f;

	m_cpTextFrame->GetVertex2DArr()[2].m_fPosition[1] = 0.2f;
	m_cpTextFrame->GetVertex2DArr()[3].m_fPosition[1] = 0.2f;
	m_cpTextFrame->GetRender() = true;

}

void CBridge::InterruptBuild()
{
	m_bBuildingBar = false;
	m_fBuildTimer = 0.0f;

	//reset building bar
	m_cpBuildingBar->GetRender() = false;
	m_cpBuildingBar->GetVertex2DArr()[1].m_fPosition[0] = BUILDING_BAR_LEFT;
	m_cpBuildingBar->GetVertex2DArr()[2].m_fPosition[0] = BUILDING_BAR_LEFT;

}

bool CBridge::BuildProcess()
{
	m_bBuildingBar = true;
	m_cpBuildingBar->GetRender() = true;

	if (m_fBuildTimer >= BUILD_TIMER)
	{
		m_fBuildTimer = 0.0f;
		m_bBuildingBar = false;

		m_cpBuildingBar->GetRender() = false;
		m_cpBuildingBar->GetVertex2DArr()[1].m_fPosition[0] = BUILDING_BAR_LEFT;
		m_cpBuildingBar->GetVertex2DArr()[2].m_fPosition[0] = BUILDING_BAR_LEFT;
		return true;
	}

	return false;
}

void CBridge::AddToRenderer()
{
	if (m_cpRenderMesh != nullptr)
		GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	if (m_cpBuiltBase != nullptr)
		GRAPHICS->AddRenderMesh(m_cpBuiltBase);

	if (m_cpTextFrame != nullptr)
		GRAPHICS->AddRenderMesh(m_cpTextFrame);

	if (m_cpControllerPopUp != nullptr)
		GRAPHICS->AddRenderMesh(m_cpControllerPopUp);

	if (m_cpBuildingBar != nullptr)
		GRAPHICS->AddRenderMesh(m_cpBuildingBar);

	for (unsigned int i = 0; i < 8; i++)
	{
		if (m_tPlanks->renderMesh != nullptr)
			GRAPHICS->AddRenderMesh(m_tPlanks->renderMesh);
	}
}
