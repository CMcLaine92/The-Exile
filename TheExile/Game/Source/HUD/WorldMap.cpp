#include "WorldMap.h"
#include "../../../Engine/Renderer/HUDMesh.h"
#include "../../../Engine/Renderer/Renderer.h"
#include "../../../Engine/Object Manager/IObject.h"
#include <iostream>
#include "../Base/TileMap.h"
#include "../Base/Tile.h"
#include "../../../Engine/Core/Input/InputManager.h"

//#define NUM_ROWS (40)
//#define NUM_COLUMNS (40)
//#define IMAGE_SIZE (1.8f)
//#define MAP_IMAGE_RATIO (0.67578125f)
//#define MAP_SIZE  (IMAGE_SIZE * MAP_IMAGE_RATIO)
//#define WORLD_SIZE (33000.0f)
//#define MAP_LEFT (-0.9f)
//#define MAP_RIGHT (MAP_LEFT + MAP_SIZE)
//#define MAP_TOP (0.8f)
//#define MAP_BOTTOM (MAP_TOP - MAP_SIZE)
//#define PLAYER_INDICATOR_SIZE (0.025f)
//#define PLAYER_INDICATOR_START_TOP (-0.1525f)
//#define PLAYER_INDICATOR_START_LEFT (0.0025f)
#define PLAYER_INDICATOR_OFFSET (0.0f)//(0.015f)
//#define NUM_TILES_X (60)
//#define NUM_TILES_Y (60)

#define NUM_ROWS (35)
#define NUM_COLUMNS (35)
#define IMAGE_SIZE (1.8f)
#define MAP_IMAGE_RATIO (.67578125f)
#define MAP_SIZE  (IMAGE_SIZE * MAP_IMAGE_RATIO)
#define WORLD_SIZE (27000.0f)
#define MAP_LEFT (-0.9f)
#define MAP_RIGHT (MAP_LEFT + MAP_SIZE)
#define MAP_TOP (0.8f)
#define MAP_BOTTOM (MAP_TOP - MAP_SIZE)
//#define PLAYER_INDICATOR_SIZE (0.03f)
//#define PLAYER_INDICATOR_START_TOP (-0.105f)//(-0.095f)
//#define PLAYER_INDICATOR_START_LEFT (-0.009f)
#define NUM_TILES_X (60)
#define NUM_TILES_Y (60)

void CWorldMap::CreateFog()
{
	float fFogSizeX = MAP_SIZE / NUM_COLUMNS;
	float fFogSizeY = MAP_SIZE / NUM_ROWS;

	for (unsigned int row = 0; row < NUM_ROWS; row++)
	{
		for (unsigned int col = 0; col < NUM_COLUMNS; col++)
		{
			std::vector<TVertex2D> m_vScreenVerts;
			std::vector<unsigned int> m_vScreenIndicies;

			m_vScreenIndicies.push_back(0);
			m_vScreenIndicies.push_back(1);
			m_vScreenIndicies.push_back(2);

			m_vScreenIndicies.push_back(0);
			m_vScreenIndicies.push_back(2);
			m_vScreenIndicies.push_back(3);
			float fFogleft = MAP_LEFT + fFogSizeX * col;
			float fFogTop = MAP_TOP - fFogSizeY * row;
			m_vScreenVerts.push_back // Top Left
				({
					{ fFogleft, fFogTop, -1.0f, 1 },
					{ 0, 0 }
			});

			m_vScreenVerts.push_back // Top Right
				({
					{ fFogleft + fFogSizeX, fFogTop, -1.0f, 1 },
					{ 1, 0 }
			});

			m_vScreenVerts.push_back // Bot Right
				({
					{ fFogleft + fFogSizeX, fFogTop - fFogSizeY, -1.0f, 1 },
					{ 1, 1 }
			});

			m_vScreenVerts.push_back // Bot Left
				({
					{ fFogleft, fFogTop - fFogSizeY, -1.0f, 1 },
					{ 0, 1 }
			});
			TFog tFog;
			tFog.pcMesh = new CHUDMesh(m_vScreenIndicies, m_vScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/Fog.dds");
			tFog.pcMesh->GetRender() = true;
			tFog.pcMesh->GetTransparent() = true;
			TranslateLocalY(tFog.pcMesh->GetTransformMatrix(), -0.15f);
			GRAPHICS->AddRenderToTextureMap(tFog.pcMesh);
			tFog.fLeft = fFogleft;
			tFog.fTop = fFogTop;
			tFog.fRight = fFogleft + fFogSizeX;
			tFog.fBottom = fFogTop - fFogSizeY;
			m_vFog.push_back(tFog);
		}
	}

}

CWorldMap::CWorldMap(IObject* pcPlayer)
{
	CInputManager::GetInstance()->RegisterKey("Escape", eKey::escape);
	m_pcPlayer = pcPlayer;
	std::vector<TVertex2D> m_vScreenVerts;
	std::vector<unsigned int> m_vScreenIndicies;

	m_bKeyFound = false;
	m_bCollidedBridge = false;
	m_bBridgeBuilt = false;

#pragma region The Map

	m_vScreenIndicies.push_back(0);
	m_vScreenIndicies.push_back(1);
	m_vScreenIndicies.push_back(2);

	m_vScreenIndicies.push_back(0);
	m_vScreenIndicies.push_back(2);
	m_vScreenIndicies.push_back(3);

	m_vScreenVerts.push_back // Top Left
		({
			{ MAP_LEFT, MAP_TOP, -1.0f, 1 },
			{ 0, 0 }
	});

	m_vScreenVerts.push_back // Top Right
		({
			{ MAP_LEFT + IMAGE_SIZE, MAP_TOP, -1.0f, 1 },
			{ 1, 0 }
	});

	m_vScreenVerts.push_back // Bot Right
		({
			{ MAP_LEFT + IMAGE_SIZE, MAP_TOP - IMAGE_SIZE, -1.0f, 1 },
			{ 1, 1 }
	});

	m_vScreenVerts.push_back // Bot Left
		({
			{ MAP_LEFT, MAP_TOP - IMAGE_SIZE, -1.0f, 1 },
			{ 0, 1 }
	});

	m_pcMap = new CHUDMesh(m_vScreenIndicies, m_vScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/MapV1.dds");
	m_pcMap->GetTransparent() = true;
	m_pcMap->GetRender() = true;

	TranslateLocalY(m_pcMap->GetTransformMatrix(), -0.15f);

	GRAPHICS->AddRenderToTextureMap(m_pcMap);

#pragma endregion

	CreateFog();

	//overlay
	m_pcOverlay = new CHUDMesh(m_vScreenIndicies, m_vScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/MapOverlay_Keyboard.dds");
	m_pcOverlay->GetTransparent() = true;
	m_pcOverlay->GetRender() = true;
	TranslateLocalY(m_pcOverlay->GetTransformMatrix(), -0.15f);
	GRAPHICS->AddRenderToTextureMap(m_pcOverlay);

#pragma region Total Map

	m_vScreenVerts.clear();
	m_vScreenVerts.push_back // Top Left
		({
			{ -1, 1, -1.0f, 1 },
			{ 0, 0 }
	});

	m_vScreenVerts.push_back // Top Right
		({
			{ 1, 1, -1.0f, 1 },
			{ 1, 0 }
	});

	m_vScreenVerts.push_back // Bot Right
		({
			{ 1, -1, -1.0f, 1 },
			{ 1, 1 }
	});

	m_vScreenVerts.push_back // Bot Left
		({
			{ -1, -1, -1.0f, 1 },
			{ 0, 1 }
	});

	m_pcTotalMap = new CHUDMesh(m_vScreenIndicies, m_vScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, GRAPHICS->GetMapShaderResourceView());
	m_pcTotalMap->GetTransparent() = true;
	m_pcTotalMap->GetRender() = false;
	m_pcTotalMap->GetTransparencyOverride() = .75f;
	GRAPHICS->AddRenderMesh(m_pcTotalMap);

#pragma endregion

#pragma region Player indicator

	m_vScreenVerts.clear();
	m_vScreenVerts.push_back // Top Left
		({
			{ -0.015f, 0.015f, -1.0f, 1 },
			{ 0, 0 }
	});

	m_vScreenVerts.push_back // Top Right
		({
			{ 0.015f, 0.015f, -1.0f, 1 },
			{ 1, 0 }
	});

	m_vScreenVerts.push_back // Bot Right
		({
			{ 0.015f, -0.015f, -1.0f, 1 },
			{ 1, 1 }
	});

	m_vScreenVerts.push_back // Bot Left
		({
			{ -0.015f, -0.015f, -1.0f, 1 },
			{ 0, 1 }
	});
	m_pcPlayerIndicator = new CHUDMesh(m_vScreenIndicies, m_vScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/PlayerIndicator.dds");
	m_pcPlayerIndicator->GetTransparent() = true;
	m_pcPlayerIndicator->GetRender() = false;

#pragma endregion

#pragma region Tombstone Icons 

	m_umTombstoneIcons.insert(pair<unsigned int, CHUDMesh*>(0, new CHUDMesh(m_vScreenIndicies, m_vScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/Tombstone_Icon.dds")));
	m_umTombstoneIcons[0]->GetTransparent() = true;
	m_umTombstoneIcons[0]->GetRender() = false;
	GRAPHICS->AddRenderMesh(m_umTombstoneIcons[0]);

#pragma endregion

	GRAPHICS->AddRenderMesh(m_pcPlayerIndicator);

}

CWorldMap::~CWorldMap()
{
	SAFE_DELETE(m_pcMap);
	SAFE_DELETE(m_pcOverlay);
	SAFE_DELETE(m_pcPlayerIndicator);
	SAFE_DELETE(m_pcTotalMap);

	for (unsigned int i = 0; i < m_vFog.size(); i++)
		SAFE_DELETE(m_vFog[i].pcMesh);

	for (unsigned int i = 0; i < m_umTombstoneIcons.size(); i++)
		SAFE_DELETE(m_umTombstoneIcons[i]);

	for (unsigned int i = 0; i < m_vpOldTombs.size(); i++)
		SAFE_DELETE(m_vpOldTombs[i]);

	for (unsigned int i = 0; i < m_vpDestroyedWalls.size(); i++)
		SAFE_DELETE(m_vpDestroyedWalls[i]);

	m_pcPlayer = nullptr;
}

void CWorldMap::Update()
{
	if (CInputManager::GetInstance()->IsKeyPressed("Escape"))
		Close();

	bool bRenderMap = false;

#pragma region Player Indicator

	float fDistanceRatioX = ((m_pcPlayer->GetPosition()->x / WORLD_SIZE) * MAP_SIZE) * 0.5f;
	float fDistanceRatioZ = ((m_pcPlayer->GetPosition()->z / WORLD_SIZE) * MAP_SIZE) * 0.5f;
	m_pcPlayerIndicator->GetPositionMatrix()._41 = ((MAP_LEFT + MAP_RIGHT) * 0.5f) + (fDistanceRatioX - PLAYER_INDICATOR_OFFSET) + 0.006f;
	m_pcPlayerIndicator->GetPositionMatrix()._42 = ((MAP_BOTTOM + MAP_TOP) * 0.5f) + (fDistanceRatioZ + PLAYER_INDICATOR_OFFSET) - 0.125f;

	//RotateLocalZ(m_pcPlayerIndicator->GetPositionMatrix(), 10.0f);
	//icons z to players y for rotation
	m_pcPlayerIndicator->GetPositionMatrix()._11 = m_pcPlayer->GetWorldMatrix()._11;
	m_pcPlayerIndicator->GetPositionMatrix()._12 = m_pcPlayer->GetWorldMatrix()._13;
	m_pcPlayerIndicator->GetPositionMatrix()._21 = m_pcPlayer->GetWorldMatrix()._31;
	m_pcPlayerIndicator->GetPositionMatrix()._22 = m_pcPlayer->GetWorldMatrix()._33;

#pragma endregion

#pragma region Objectives

	if (m_bIsOpen == true)
	{
		if (m_bKeyFound == false)//first objective(find key)
		{
			GRAPHICS->RenderText(L"Objective:  Use a bomb to\nenter a green zone where\na key can be found.", { 350.0f, -200.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.35f, 0.3f }, Left);
		}
		else if (m_bKeyFound == true && m_bBridgeBuilt == false && m_bCollidedBridge == true)//second objective(bridge)
		{
			GRAPHICS->RenderText(L"Objective:  Build a bridge\nto get closer to an exit.", { 350.0f, -200.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.35f, 0.3f }, Left);
		}
		else if (m_bKeyFound == true)//third objective(find exit)
		{
			GRAPHICS->RenderText(L"Objective:  Find an exit\ndoor and use the key to\nopen it.", { 350.0f, -200.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.35f, 0.3f }, Left);
		}
	}

#pragma endregion

	float fMappedXDistance = (m_pcPlayer->GetPosition()->x / WORLD_SIZE);
	float fMappedYDistance = (m_pcPlayer->GetPosition()->z / WORLD_SIZE);
	fMappedXDistance += 1.0f;
	fMappedYDistance += 1.0f;
	fMappedXDistance *= .5f;
	fMappedYDistance *= .5f;
	if (fMappedXDistance < 0.0f)
	{
		fMappedXDistance = 0.0f;
	}
	if (fMappedYDistance < 0.0f)
	{
		fMappedYDistance = 0.0f;
	}
	fMappedYDistance = 1.0f - fMappedYDistance;
	if (fMappedYDistance < 0.0f)
	{
		fMappedYDistance = 0.0f;
	}

	unsigned int unIndexX = (unsigned int)(fMappedXDistance * NUM_COLUMNS);
	unsigned int unIndexY = (unsigned int)(fMappedYDistance * NUM_ROWS);
	if (unIndexX >= NUM_COLUMNS)
	{
		unIndexX = NUM_COLUMNS - 1;
	}
	if (unIndexY >= NUM_ROWS)
	{
		unIndexY = NUM_ROWS - 1;
	}
	if (m_vFog[unIndexY*NUM_COLUMNS + unIndexX].bIsActive)
	{
		m_vFog[unIndexY*NUM_COLUMNS + unIndexX].bIsActive = false;
		m_vFog[unIndexY*NUM_COLUMNS + unIndexX].pcMesh->GetRender() = false;
		bRenderMap = true;
	}

	//if (unIndexX > 0 && unIndexX < NUM_COLUMNS - 1 && unIndexY > 0 && unIndexY < NUM_ROWS - 1)
	//{
	//	m_vFog[(unIndexY+1)*NUM_COLUMNS + unIndexX + 1].bIsActive = false;
	//	m_vFog[(unIndexY+1)*NUM_COLUMNS + unIndexX + 1].pcMesh->GetRender() = false;
	//	m_vFog[(unIndexY+1)*NUM_COLUMNS + unIndexX - 1].bIsActive = false;
	//	m_vFog[(unIndexY+1)*NUM_COLUMNS + unIndexX - 1].pcMesh->GetRender() = false;
	//
	//	m_vFog[(unIndexY - 1) * NUM_COLUMNS + unIndexX + 1].bIsActive = false;
	//	m_vFog[(unIndexY - 1) * NUM_COLUMNS + unIndexX + 1].pcMesh->GetRender() = false;
	//	m_vFog[(unIndexY - 1) * NUM_COLUMNS + unIndexX - 1].bIsActive = false;
	//	m_vFog[(unIndexY - 1) * NUM_COLUMNS + unIndexX - 1].pcMesh->GetRender() = false;
	//}
	if (unIndexX > 0 && unIndexX < NUM_COLUMNS - 1)
	{
		if (m_vFog[unIndexY*NUM_COLUMNS + unIndexX + 1].bIsActive)
		{
			m_vFog[unIndexY*NUM_COLUMNS + unIndexX + 1].bIsActive = false;
			m_vFog[unIndexY*NUM_COLUMNS + unIndexX + 1].pcMesh->GetRender() = false;
			bRenderMap = true;
		}
		if (m_vFog[unIndexY*NUM_COLUMNS + unIndexX - 1].bIsActive)
		{
			m_vFog[unIndexY*NUM_COLUMNS + unIndexX - 1].bIsActive = false;
			m_vFog[unIndexY*NUM_COLUMNS + unIndexX - 1].pcMesh->GetRender() = false;
			bRenderMap = true;
		}

		//m_vFog[unIndexY*NUM_COLUMNS + unIndexX + 2].bIsActive = false;
		//m_vFog[unIndexY*NUM_COLUMNS + unIndexX + 2].pcMesh->GetRender() = false;
		//m_vFog[unIndexY*NUM_COLUMNS + unIndexX - 2].bIsActive = false;
		//m_vFog[unIndexY*NUM_COLUMNS + unIndexX - 2].pcMesh->GetRender() = false;
	}
	if (unIndexY > 0 && unIndexY < NUM_ROWS - 1)
	{
		if (m_vFog[(unIndexY - 1)*NUM_COLUMNS + unIndexX].bIsActive)
		{
			m_vFog[(unIndexY - 1)*NUM_COLUMNS + unIndexX].bIsActive = false;
			m_vFog[(unIndexY - 1)*NUM_COLUMNS + unIndexX].pcMesh->GetRender() = false;
			bRenderMap = true;
		}
		if (m_vFog[(unIndexY + 1)*NUM_COLUMNS + unIndexX].bIsActive)
		{
			m_vFog[(unIndexY + 1)*NUM_COLUMNS + unIndexX].bIsActive = false;
			m_vFog[(unIndexY + 1)*NUM_COLUMNS + unIndexX].pcMesh->GetRender() = false;
			bRenderMap = true;
		}

		//m_vFog[(unIndexY - 2)*NUM_COLUMNS + unIndexX].bIsActive = false;
		//m_vFog[(unIndexY - 2)*NUM_COLUMNS + unIndexX].pcMesh->GetRender() = false;
		//m_vFog[(unIndexY + 2)*NUM_COLUMNS + unIndexX].bIsActive = false;
		//m_vFog[(unIndexY + 2)*NUM_COLUMNS + unIndexX].pcMesh->GetRender() = false;
	}

	if (bRenderMap)
	{
		GRAPHICS->RenderToTextureMap();
	}
	if (m_pcTotalMap->GetRender())
	{
		m_pcTotalMap->SetTexture(GRAPHICS->GetMapShaderResourceView());
	}

}

void CWorldMap::Open()
{
	if (CInputManager::GetInstance()->GetControllerBeingUsed() == true)
	{
		m_pcOverlay->SetTexture(L"../Game/Assets/Art/2D/HUD/MapOverlay_Controller.dds");
	}
	else
	{
		m_pcOverlay->SetTexture(L"../Game/Assets/Art/2D/HUD/MapOverlay_Keyboard.dds");
	}

	GRAPHICS->RenderToTextureMap();
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::OPENINV);
	m_bIsOpen = true;
	m_pcPlayerIndicator->GetRender() = true;
	m_pcTotalMap->GetRender() = true;
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::OPENINV);

	if (m_bTomstoneActive == true)
	{
		for (auto iter = m_umTombstoneIcons.begin(); iter != m_umTombstoneIcons.end(); iter++)
		{
			(*iter).second->GetRender() = true;
		}
	}

	for (unsigned int i = 0; i < m_vpDestroyedWalls.size(); i++)
	{
		m_vpDestroyedWalls[i]->GetRender() = true;
	}

}

void CWorldMap::Close()
{
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::OPENINV);
	m_bIsOpen = false;
	m_pcPlayerIndicator->GetRender() = false;
	m_pcTotalMap->GetRender() = false;
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::OPENINV);

	if (m_bTomstoneActive == true)
	{
		for (auto iter = m_umTombstoneIcons.begin(); iter != m_umTombstoneIcons.end(); iter++)
		{
			(*iter).second->GetRender() = false;
		}
	}

	for (unsigned int i = 0; i < m_vpDestroyedWalls.size(); i++)
	{
		m_vpDestroyedWalls[i]->GetRender() = false;
	}

}

void CWorldMap::Initialize(CTileMap* pcTileMap)
{
	//std::vector<TVertex2D> m_vScreenVerts;
	//std::vector<unsigned int> m_vScreenIndicies;
	////Position Size
	//float fTileSizeX = MAP_SIZE / (float)NUM_TILES_X;
	//float fTileSizeY = MAP_SIZE / (float)NUM_TILES_Y;
	////Position Start
	//float fStartX = -1.0f;
	//float fStartY = 1.0f;
	////TexCoord Starts
	//float fTexCoordU = 0.0f;
	//float fTexCoordV = 0.0f;
	////TexCoord Increments
	//float fTextureTileWidth = .15625f;
	//float fTextureTileHeight = .3125f;

	//unsigned int unTileIndex = 0;
	//for (unsigned int unX = 0; unX < pcTileMap->Size().x; unX++)
	//{
	//	for (unsigned int unY = 0; unY < pcTileMap->Size().y; unY++)
	//	{
	//		fStartX = MAP_LEFT + fTileSizeX*unX;
	//		fStartY = MAP_TOP - fTileSizeY*unY;
	//		m_vScreenVerts.clear();
	//		CTile* cTile = pcTileMap->GetTile(unX, unY);
	//		//Setup UVs based on Tile			
	//		fTexCoordU = cTile->GetUVS().x;
	//		fTexCoordV = cTile->GetUVS().y;

	//		//Set Verts up
	//		m_vScreenVerts.push_back // Top Left
	//			({
	//				{ fStartX, fStartY, 0, 1 },
	//				{ fTexCoordU + PALLET_ADJUST, fTexCoordV + PALLET_ADJUST }
	//		});

	//		m_vScreenVerts.push_back // Top Right
	//			({
	//				{ fStartX + fTileSizeX, fStartY, 0, 1 },
	//				{ fTexCoordU + fTextureTileWidth - PALLET_ADJUST, fTexCoordV + PALLET_ADJUST }
	//		});

	//		m_vScreenVerts.push_back // Bot Right
	//			({
	//				{ fStartX + fTileSizeX, fStartY - fTileSizeY, 0, 1 },
	//				{ fTexCoordU + fTextureTileWidth - PALLET_ADJUST, fTexCoordV + fTextureTileHeight  - PALLET_ADJUST}
	//		});

	//		m_vScreenVerts.push_back // Bot Left
	//			({
	//				{ fStartX, fStartY - fTileSizeY, 0, 1 },
	//				{ fTexCoordU + PALLET_ADJUST, fTexCoordV + fTextureTileHeight -PALLET_ADJUST}
	//		});

	//		CHUDMesh* pcTempHudMesh = new CHUDMesh(m_vScreenIndicies, m_vScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/Pallet.dds");
	//		pcTempHudMesh->GetRender() = true;
	//		pcTempHudMesh->GetTransparent() = true;
	//		GRAPHICS->AddRenderToTextureMap(pcTempHudMesh);
	//		m_vpcTiles.push_back(pcTempHudMesh);
	//	}
	//}

	//CreateFog();

	//m_pcOverlay = new CHUDMesh(m_vScreenIndicies, m_vScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/MapOverlay.dds");
	//m_pcOverlay->GetTransparent() = true;
	//m_pcOverlay->GetRender() = true;
	//GRAPHICS->AddRenderToTextureMap(m_pcOverlay);
}

void CWorldMap::AddTombstoneIcon(unsigned int ID)
{
	if (ID >= m_umTombstoneIcons.size())//add a new one
	{
		vector<TVertex2D> m_vScreenVerts;
		vector<unsigned int> m_vScreenIndicies;

		m_vScreenIndicies.push_back(0);
		m_vScreenIndicies.push_back(1);
		m_vScreenIndicies.push_back(2);
		m_vScreenIndicies.push_back(0);
		m_vScreenIndicies.push_back(2);
		m_vScreenIndicies.push_back(3);

		m_vScreenVerts.push_back // Top Left
			({
				{ -0.015f, 0.015f, -1.0f, 1 },
				{ 0, 0 }
		});

		m_vScreenVerts.push_back // Top Right
			({
				{ 0.015f, 0.015f, -1.0f, 1 },
				{ 1, 0 }
		});

		m_vScreenVerts.push_back // Bot Right
			({
				{ 0.015f, -0.015f, -1.0f, 1 },
				{ 1, 1 }
		});

		m_vScreenVerts.push_back // Bot Left
			({
				{ -0.015f, -0.015f, -1.0f, 1 },
				{ 0, 1 }
		});

		m_umTombstoneIcons.insert(pair<unsigned int, CHUDMesh*>(ID, new CHUDMesh(m_vScreenIndicies, m_vScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/Tombstone_Icon.dds")));
		m_umTombstoneIcons[ID]->GetTransparent() = true;
		m_umTombstoneIcons[ID]->GetRender() = false;
		GRAPHICS->AddRenderMesh(m_umTombstoneIcons[ID]);
	}

	m_umTombstoneIcons[ID]->GetPositionMatrix() = m_pcPlayerIndicator->GetPositionMatrix();
	m_bTomstoneActive = true;
}

void CWorldMap::RemoveTombstoneIcon(unsigned int ID)
{
	for (auto iter = m_umTombstoneIcons.begin(); iter != m_umTombstoneIcons.end(); iter++)
	{
		if ((*iter).first == ID)
		{
			m_vpOldTombs.push_back((*iter).second);
			m_umTombstoneIcons.erase((*iter).first);
			break;
		}
	}
}

void CWorldMap::DeActivateTombstones()
{
	m_bTomstoneActive = false;
}

void CWorldMap::AddDestroyedWallIcon(const XMFLOAT3* pos)
{
	std::vector<TVertex2D> m_vScreenVerts;
	std::vector<unsigned int> m_vScreenIndicies;

	m_vScreenIndicies.push_back(0);
	m_vScreenIndicies.push_back(1);
	m_vScreenIndicies.push_back(2);
	m_vScreenIndicies.push_back(0);
	m_vScreenIndicies.push_back(2);
	m_vScreenIndicies.push_back(3);

	m_vScreenVerts.push_back // Top Left
		({
			{ -0.015f, 0.015f, -1.0f, 1 },
			{ 0, 0 }
	});

	m_vScreenVerts.push_back // Top Right
		({
			{ 0.015f, 0.015f, -1.0f, 1 },
			{ 1, 0 }
	});

	m_vScreenVerts.push_back // Bot Right
		({
			{ 0.015f, -0.015f, -1.0f, 1 },
			{ 1, 1 }
	});

	m_vScreenVerts.push_back // Bot Left
		({
			{ -0.015f, -0.015f, -1.0f, 1 },
			{ 0, 1 }
	});

	CHUDMesh* tempMesh = new CHUDMesh(m_vScreenIndicies, m_vScreenVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/HUD/DestroyedWallIcon.dds");
	tempMesh->GetTransparent() = true;

	if (m_bIsOpen == true)
		tempMesh->GetRender() = true;
	else
		tempMesh->GetRender() = false;

	TranslateGlobalX(tempMesh->GetPositionMatrix(), (MAP_LEFT + MAP_RIGHT) * 0.5f); //Move left
	TranslateGlobalY(tempMesh->GetPositionMatrix(), (MAP_BOTTOM + MAP_TOP) * 0.5f); //Move down

	float fDistanceRatioX = ((pos->x / WORLD_SIZE) * MAP_SIZE) * 0.5f;
	float fDistanceRatioZ = ((pos->z / WORLD_SIZE) * MAP_SIZE) * 0.5f;
	tempMesh->GetPositionMatrix()._41 = (MAP_LEFT + MAP_RIGHT) * 0.5f + (fDistanceRatioX - PLAYER_INDICATOR_OFFSET);
	tempMesh->GetPositionMatrix()._42 = (MAP_BOTTOM + MAP_TOP) * 0.5f + (fDistanceRatioZ + PLAYER_INDICATOR_OFFSET) - 0.125f;

	GRAPHICS->AddRenderMesh(tempMesh);
	m_vpDestroyedWalls.push_back(tempMesh);
}

void CWorldMap::AddToRenderer()
{
	if (m_pcMap != nullptr)
		GRAPHICS->AddRenderToTextureMap(m_pcMap);

	for (unsigned int i = 0; i < m_vFog.size(); i++)
	{
		if (m_vFog[i].pcMesh != nullptr)
			GRAPHICS->AddRenderToTextureMap(m_vFog[i].pcMesh);
	}

	if (m_pcOverlay != nullptr)
		GRAPHICS->AddRenderToTextureMap(m_pcOverlay);

	if (m_pcTotalMap != nullptr)
		GRAPHICS->AddRenderMesh(m_pcTotalMap);

	for (unsigned int i = 0; i < m_vpOldTombs.size(); i++)
	{
		if (m_vpOldTombs[i] != nullptr)
			GRAPHICS->AddRenderMesh(m_vpOldTombs[i]);
	}

	for (unsigned int i = 0; i < m_vpDestroyedWalls.size(); i++)
	{
		if (m_vpDestroyedWalls[i] != nullptr)
			GRAPHICS->AddRenderMesh(m_vpDestroyedWalls[i]);
	}

	if (m_pcPlayerIndicator != nullptr)
		GRAPHICS->AddRenderMesh(m_pcPlayerIndicator);

}

void CWorldMap::ExitKeyFound()
{
	m_bKeyFound = true;
}

void CWorldMap::BridgeCollided()
{
	m_bCollidedBridge = true;
}

void CWorldMap::BridgeBuilt()
{
	m_bBridgeBuilt = true;
}
