
#include "LoadState.h"
#include "GameplayState.h"
#include "../StateMachine.h"
#include "../../Input/InputManager.h"
#include "../../../Animation/Mesh.h"
#include "../../../Renderer/HUDMesh.h"
#include "../../../Object Manager/FloorObject.h"
#include "../../../Object Manager/ObjectManager.h"
#include "../../../../Game/Source/Camera/Camera.h"
#include "../../../../Game/Source/Environment/BedObject.h"
#include "../../../../Game/Source/Environment/Bridge.h"
#include "../../../../Game/Source/Environment/CrackedWall.h"
#include "../../../../Game/Source/Base/Inventory.h"
#include "../../../../Game/Source/Base/StorageCrate.h"
#include "../../../../Game/Source/Environment/Torch.h"
#include "../../../../Game/Source/Player/Player.h"
#include "../../../../Game/Source/HUD/WorldMap.h"
#include "../../../../Game/Source/HUD/HUD.h"


#include <fstream>
#include <sstream>
#include <ShlObj.h>

#define NUM_OPTIONS 2

void CLoadState::Enter()
{
	CInputManager::GetInstance()->RegisterKey("Enter", eKey::Enter);
	CInputManager::GetInstance()->RegisterKey("Escape", eKey::escape);
	CInputManager::GetInstance()->RegisterKey("Right", eKey::RightArrow, eKey::D);
	CInputManager::GetInstance()->RegisterKey("Left", eKey::LeftArrow, eKey::A);

	m_nCursor = 0;

	BuildPlanes();

	m_bToGameplay = false;
	m_cpCameraObject->AddCollider(new CCollider(false, Bounds::Plane, m_cpCameraMesh->GetVertices(), false));

	//set camera up
	m_cpObjManager = new CObjectManager();
	m_cpObjManager->AddObject(m_cpCameraObject, CObjectManager::Static);
	m_cpTheCamera = new CCamera(0.0f, 0.0f, -2.5f);
	m_cpTheCamera->SetParent(m_cpCameraObject);
	m_cpObjManager->AddObject(m_cpTheCamera, CObjectManager::Dynamic);
	m_cpCameraObject->AddChild(m_cpTheCamera);
}
void CLoadState::Exit()
{
GRAPHICS->RemoveRenderMesh(m_cpBackground);
GRAPHICS->RemoveRenderMesh(m_cpContinue);
GRAPHICS->RemoveRenderMesh(m_cpNewGame);
GRAPHICS->RemoveRenderMesh(m_cpContinueArrow);
GRAPHICS->RemoveRenderMesh(m_cpNewGameArrow);

	SAFE_DELETE(m_cpCameraObject);
	SAFE_DELETE(m_cpTheCamera);
	SAFE_DELETE(m_cpCameraMesh);

	

	SAFE_DELETE(m_cpBackground);
	SAFE_DELETE(m_cpContinue);
	SAFE_DELETE(m_cpNewGame);
	SAFE_DELETE(m_cpContinueArrow);
	SAFE_DELETE(m_cpNewGameArrow);

	SAFE_DELETE(m_cpObjManager);

}
CLoadState::CLoadState()
{
	m_cpCameraObject = nullptr;
	m_cpTheCamera = nullptr;
	m_cpCameraMesh = nullptr;
	m_cpBackground = nullptr;
	m_cpContinue = nullptr;
	m_cpNewGame = nullptr;
	m_cpContinueArrow = nullptr;
	m_cpNewGameArrow = nullptr;
	m_cpObjManager = nullptr;
}

CLoadState::~CLoadState()
{
	
}
EInputResult CLoadState::Input()
{
	 prevCursor = m_nCursor;

	if (CInputManager::GetInstance()->IsKeyPressed("Escape") || CInputManager::GetInstance()->IsButtonPressed("Back"))
	{
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::MENUENTER, XMFLOAT3(0, 0, 0));
		return eRemove;
	}
	if (CInputManager::GetInstance()->IsKeyPressed("Enter") || CInputManager::GetInstance()->IsButtonPressed("Jump Action"))
	{
		GRAPHICS->ClearScene();
		CGameplayState::GetInstance()->Enter();
		m_bToGameplay = true;
		if (m_nCursor == 0) // Continue
			Load();
		return eRemove;
	}
	 if (CInputManager::GetInstance()->IsKeyPressed("Left") || CInputManager::GetInstance()->IsButtonPressed("Map") || CInputManager::GetInstance()->IsAxisPressed("Left JS Pos X"))
	{
		m_nCursor--;
		if (m_nCursor < 0)
			m_nCursor = NUM_OPTIONS - 1;

	}
	 else if (CInputManager::GetInstance()->IsKeyPressed("Right") || CInputManager::GetInstance()->IsButtonPressed("Inventory") || CInputManager::GetInstance()->IsAxisPressed("Left JS Neg X"))
	{
		m_nCursor++;
		if (m_nCursor >= NUM_OPTIONS)
			m_nCursor = 0;
	}

	

	return eContinue;
}
void CLoadState::Update()
{

	

	if( m_nCursor != prevCursor )
	{
		if( m_nCursor == 0 ) // Continue
		{
			m_cpContinue->SetTexture( L"../Game/Assets/Art/2D/Menu/Failbot_CNT_Selected.dds" );
			m_cpNewGame->SetTexture( L"../Game/Assets/Art/2D/Menu/Failbot_NG_Normal.dds" );
			m_cpContinueArrow->GetRender() = true;
			m_cpNewGameArrow->GetRender() = false;
		}
		if( m_nCursor == 1 ) // Continue
		{
			m_cpContinue->SetTexture( L"../Game/Assets/Art/2D/Menu/Failbot_CNT_Normal.dds" );
			m_cpNewGame->SetTexture( L"../Game/Assets/Art/2D/Menu/Failbot_NG_Selected.dds" );
			m_cpContinueArrow->GetRender() = false;
			m_cpNewGameArrow->GetRender() = true;
		}
	}

	
}

void CLoadState::Load()
{
	ifstream binIn( GetFilePath() + "Save_01.bin", ios_base::binary );
	TSaveData inData;

	//CStateMachine::GetInstance()->AddState( CStateMachine::eGameplayState );

	CGameplayState* curState = CGameplayState::GetInstance();
	
	CPlayer* curPlayer = reinterpret_cast<CPlayer*>( curState->GetPlayer() );
	vector<IObject*> vpBridges = curState->GetObjectManager()->FindObjectsWithTag( "Bridge", CObjectManager::Static );
	vector<IObject*> vpWalls = curState->GetObjectManager()->FindObjectsWithTag( "CrackedWall", CObjectManager::Static );

	if( binIn.is_open() )
	{

		int nVecSize;
		int nReadInt;
		bool bReadBool;

		// Bridges
		binIn.read( (char*)&nVecSize, sizeof( int ) );

		for( int i = 0; i < nVecSize; i++ )
		{
			binIn.read( (char*)&bReadBool, sizeof( bool ) );

			if( bReadBool )
			{
				reinterpret_cast<CBridge*>( vpBridges[i] )->Activate();
			}
		}

		// Walls
		binIn.read( (char*)&nVecSize, sizeof( int ) );

		for( int i = 0; i < nVecSize; i++ )
		{
			binIn.read( (char*)&bReadBool, sizeof( bool ) );
			if( bReadBool )
			{
				reinterpret_cast<CCrackedWall*>( vpWalls[i] )->Activate(false, -1);

			}
		}

		// m_vbMapFog
		binIn.read( (char*)&nVecSize, sizeof( int ) );

		for( int i = 0; i < nVecSize; i++ )
		{
			binIn.read( (char*)&bReadBool, sizeof( bool ) );
			curPlayer->GetWorldMap()->GetFog()[i].bIsActive = bReadBool;
			curPlayer->GetWorldMap()->GetFog()[i].pcMesh->GetRender() = bReadBool;
		}

		// Inventory
		binIn.read( (char*)&nVecSize, sizeof( int ) );

		for( int i = 0; i < nVecSize; i++ )
		{
			binIn.read( (char*)&nReadInt, sizeof( int ) );
			curPlayer->GetInventory()->AddToInventory( ( CInventory::eItems )i, nReadInt );

			//inData.m_vnInventory.push_back( bReadBool );
		}

		// Upgrades

		int wepIn, shiIn, healthIn;
		binIn.read( (char*)&wepIn, sizeof( int ) );
		binIn.read( (char*)&shiIn, sizeof( int ) );
		binIn.read( (char*)&healthIn, sizeof( int ) );
		curPlayer->SetUpgrades( wepIn, shiIn, healthIn );

		// Days
		binIn.read( (char*)&nReadInt, sizeof( int ) );
		curPlayer->GetHUD()->SetDayCounter( nReadInt );


		// Storage Crate //
		for (auto iter = curPlayer->GetStorageCrate()->GetStock().begin(); iter != curPlayer->GetStorageCrate()->GetStock().end(); iter++)
			binIn.read((char*)&(iter->second), sizeof(int));

		// Exit Key //
		char bKeyFound;
		binIn.read((char*)&bKeyFound, sizeof(char));

		if (bKeyFound == 1)
		{
			curPlayer->GetHUD()->ExitKeyFound();
			curPlayer->GetKeyFound() = true;
		}

		// Torches //
		vector<IObject*> m_cTorchList = curState->GetObjectManager()->FindObjectsWithTag("Torch", CObjectManager::eObjectType::Static);
		for (unsigned int i = 0; i < m_cTorchList.size(); i++)
		{
			char state = false;
			binIn.read((char*)&state, sizeof(char));


			if (state == 1)
			{
				((CTorch*)m_cTorchList[i])->ActivateTorch();
				OUTPUT("On\n");
			}
		}


		// Total Time //
		float fTotalTime = 0.0f;
		binIn.read((char*)&fTotalTime, sizeof(float));
		curPlayer->SetTotalPlayTime(fTotalTime);

		binIn.close();
	}

}

string CLoadState::GetFilePath()
{
	HRESULT hr;
	std::ostringstream stringStream;
	char path[MAX_PATH];
	LPWSTR wszPath = NULL;
	size_t   size;

	// Get the path to the app data folder
	hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath );

	// Convert from LPWSTR to char[]
	wcstombs_s( &size, path, MAX_PATH, wszPath, MAX_PATH );

	// Convert char types
	if( hr == S_OK )
		stringStream << path;
	std::string filePath = stringStream.str();

	// Add the company and game information
	filePath += "\\" + std::string( "Failbot" ) + "\\" + std::string( "The Exile" ) + "\\" + std::string( "Save" ) + "\\";

	// Create our directory
	SHCreateDirectoryExA( NULL, filePath.c_str(), 0 );

	return filePath;
}

void CLoadState::BuildPlanes()
{
	vector<unsigned int> nvIndicies;

	nvIndicies.push_back( 0 );
	nvIndicies.push_back( 1 );
	nvIndicies.push_back( 2 );

	nvIndicies.push_back( 0 );
	nvIndicies.push_back( 2 );
	nvIndicies.push_back( 3 );

#pragma region Background

	vector<TVertex2D> cvBackground;

	//top left
	cvBackground.push_back
		( {
				{ -1.0f, 0.8f, -1, 1 },
				{ 0, 0 }
	} );

	//top right
	cvBackground.push_back
		( {
				{ 1.0f, 0.8f, -1, 1 },
				{ 1, 0 }
	} );

	//bottom right
	cvBackground.push_back
		( {
				{ 1.0f, -1.2f, -1, 1 },
				{ 1, 1 }
	} );

	//bottom left
	cvBackground.push_back
		( {
				{ -1.0f, -1.2f, -1, 1 },
				{ 0, 1 }
	} );


	m_cpBackground = new CHUDMesh( nvIndicies, cvBackground, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/FX/MazeScreen.dds" );
	m_cpBackground->GetTransparent() = true;
	TranslateLocalY( m_cpBackground->GetTransformMatrix(), 0.2f );
	GRAPHICS->AddRenderMesh( m_cpBackground );


#pragma endregion

#pragma region Icons

	// -1,1   1,1
	// -1,-1  1,-1
	vector<TVertex2D> cv2DVerts;

	float fContinueX = -0.55f;
	float fContinueY = 0.25f;

	float fContinueWidth = 0.5f;
	float fContinueHeight = 0.25f;

	//top left
	cv2DVerts.push_back
		( {
				{ fContinueX, fContinueY, -1, 1 },
				{ 0, 0 }
	} );

	//top right
	cv2DVerts.push_back
		( {
				{ fContinueX + fContinueWidth, fContinueY, -1, 1 },
				{ 1, 0 }
	} );

	//bottom right
	cv2DVerts.push_back
		( {
				{ fContinueX + fContinueWidth, fContinueY - fContinueHeight, -1, 1 },
				{ 1, 1 }
	} );

	//bottom left
	cv2DVerts.push_back
		( {
				{ fContinueX, fContinueY - fContinueHeight, -1, 1 },
				{ 0, 1 }
	} );


	m_cpContinue = new CHUDMesh( nvIndicies, cv2DVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/Failbot_CNT_Selected.dds" );
	m_cpContinue->GetTransparent() = true;
	GRAPHICS->AddRenderMesh( m_cpContinue );

	float fOffsetX = 0.8f;
	for( size_t i = 0; i < cv2DVerts.size(); i++ )
	{
		cv2DVerts[i].m_fPosition[0] += fOffsetX;
	}

	m_cpNewGame = new CHUDMesh( nvIndicies, cv2DVerts, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/Failbot_NG_Normal.dds" );
	m_cpNewGame->GetTransparent() = true;

	GRAPHICS->AddRenderMesh( m_cpNewGame );


#pragma endregion


#pragma region Arrows

	// -1,1   1,1
	// -1,-1  1,-1
	vector<TVertex2D> cv2DArrowVert;
	// -0.7 y = 0.25
	fContinueX = -0.85f;
	fContinueY = 0.25f;
	fContinueWidth = 0.3f;
	fContinueHeight = 0.25f;

	//top left
	cv2DArrowVert.push_back
		( {
				{ fContinueX, fContinueY, -1, 1 },
				{ 0, 0 }
	} );

	//top right
	cv2DArrowVert.push_back
		( {
				{ fContinueX + fContinueWidth, fContinueY, -1, 1 },
				{ 1, 0 }
	} );

	//bottom right
	cv2DArrowVert.push_back
		( {
				{ fContinueX + fContinueWidth, fContinueY - fContinueHeight, -1, 1 },
				{ 1, 1 }
	} );

	//bottom left
	cv2DArrowVert.push_back
		( {
				{ fContinueX, fContinueY - fContinueHeight, -1, 1 },
				{ 0, 1 }
	} );

	m_cpContinueArrow = new CHUDMesh( nvIndicies, cv2DArrowVert, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/Failbot_LoadArrow.dds" );
	m_cpContinueArrow->GetTransparent() = true;
	GRAPHICS->AddRenderMesh( m_cpContinueArrow );

	fOffsetX = 0.8f;
	for( size_t i = 0; i < cv2DVerts.size(); i++ )
	{
		cv2DArrowVert[i].m_fPosition[0] += fOffsetX;
	}

	m_cpNewGameArrow = new CHUDMesh( nvIndicies, cv2DArrowVert, GRAPHICS->GetVertexShader2D(), GRAPHICS->GetPixelShader2D(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Menu/Failbot_LoadArrow.dds" );
	m_cpNewGameArrow->GetTransparent() = true;
	m_cpNewGameArrow->GetRender() = false;
	GRAPHICS->AddRenderMesh( m_cpNewGameArrow );

#pragma endregion

	m_cpCameraMesh = new CMesh;
	m_cpCameraMesh->GetIndices().push_back( 0 );
	m_cpCameraMesh->GetIndices().push_back( 1 );
	m_cpCameraMesh->GetIndices().push_back( 2 );

	m_cpCameraMesh->GetIndices().push_back( 0 );
	m_cpCameraMesh->GetIndices().push_back( 2 );
	m_cpCameraMesh->GetIndices().push_back( 3 );

	//top left
	m_cpCameraMesh->GetVertices().push_back
		( {
				{ -1.75, 1, -1, 1 },
				{ 0, 0, -1, 0 },
				{ 0, 0 }
	} );

	//top right
	m_cpCameraMesh->GetVertices().push_back
		( {
				{ 1.75, 1, -1, 1 },
				{ 0, 0, -1 },
				{ 1, 0 }
	} );

	//bottom right
	m_cpCameraMesh->GetVertices().push_back
		( {
				{ 1.75, -1, -1, 1 },
				{ 0, 0, -1 },
				{ 1, 1 }
	} );

	//bottom left
	m_cpCameraMesh->GetVertices().push_back
		( {
				{ -1.75, -1, -1, 1 },
				{ 0, 0, -1 },
				{ 0, 1 }
	} );

	m_cpCameraObject = new CFloorObject( "Controls Camera Object" );
	m_cpCameraObject->SetRenderMesh( new CRenderMesh( m_cpCameraMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader() ) );
	m_cpCameraObject->GetWorldMatrix() =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

}