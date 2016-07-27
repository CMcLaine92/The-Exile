/***********************************************
* Filename:  		LevelLoader.cpp
* Date:      		8/7/2015
* Mod. Date: 		9/2/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Loads the entire level
************************************************/

#include "LevelLoader.h"
#include <fbxsdk/fileio/fbxiosettings.h>

#include "../Asset Manager/AssetManager.h"
#include "../../Game/Source/Player/Player.h"
#include "../Animation/Mesh.h"
#include "../../Engine/Renderer/Renderer.h"
#include "../../Engine/AI/CNavGraph.h"
#include "../../Engine/AI/NavGraphManager.h"
#include "../../Engine/Object Manager/ObjectManager.h"

#include "../Object Manager/WallObject.h"
#include "../Object Manager/FloorObject.h"
#include "../Object Manager/PitObject.h"
#include "../Object Manager/PitWallObject.h"
#include "../Object Manager/DoorObject.h"
#include "../Object Manager/ExitDoorObject.h"
#include "../Object Manager/SpawnerObject.h"
#include "../Object Manager/WaypointObject.h"
#include "../../Game/Source/Environment/CrackedWall.h"
#include "../../Game/Source/Environment/Torch.h"
#include "../../Game/Source/Environment/LightWeaponChest.h"
#include "../../Game/Source/Environment/HeavyWeaponChest.h"
#include "../../Game/Source/Environment/HealthUpgradeChest.h"
#include "../../Game/Source/Environment/Bush.h"
#include "../../Game/Source/Environment/Tree.h"
#include "../../Game/Source/Traps/SpikeTrap.h"
#include "../../Game/Source/Traps/SpinningBlade.h"
#include "../../Game/Source/Traps/FireTrap.h"


/*	PRIVATE FUNCTIONS	*/

/*****************************************************************
* ProcessLevelNode()	Processes a single fbx node and loads in appropiate data
*
* Ins:					node
*
* Outs:					meshes
*
* Returns:				bool
*
* Mod. Date:		    09/02/2015
* Mod. Initials:	    NH
*****************************************************************/
bool CLevelLoader::ProcessLevelNode(FbxNode* node, vector<CMesh>& meshes)
{
	FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute != NULL)
	{
		if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			CMesh* tempMesh = new CMesh;
			FbxMesh* attributeMesh = (FbxMesh*)attribute;
			CAssetManager * assetManager = CAssetManager::GetInstance();

			bool errorCheck = assetManager->LoadMesh(attributeMesh, *tempMesh);
			if (errorCheck == false)
			{
				attribute->Destroy();
				return false;
			}

			//now get prefix and apply texture, collision, etc...
			const char* objName = node->GetName();
			std::string prefix = "    ";

			for (unsigned int i = 0; i < 4; i++)
			{
				prefix[i] = objName[i];
			}

			if (strcmp(prefix.c_str(), "Wal_") == 0)//wall prefab
			{
				tempMesh->ConvertVertices();

				CWallObject* tempWall = new CWallObject("Wall");
				tempWall->AddCollider(new CCollider(false, Bounds::AABB, tempMesh->GetVertices()));
				tempWall->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Wall.dds"));
				tempWall->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
				tempWall->GetRenderMesh()->GetSpecular() = 0.5f;

				m_cpEnvironmentObjects.push_back(tempWall);

				m_cpObjectManager->AddObject(tempWall, CObjectManager::eObjectType::Static);
			}
			else if (strcmp(prefix.c_str(), "Flr_") == 0)//floor prefab
			{
				tempMesh->ConvertVertices();

				CFloorObject* tempFloor = new CFloorObject("Floor");
				tempFloor->AddCollider(new CCollider(false, Bounds::Plane, tempMesh->GetVertices()));
				tempFloor->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Ground.dds"));
				tempFloor->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
				tempFloor->GetRenderMesh()->GetSpecular() = 1.5f;

				m_cpEnvironmentObjects.push_back(tempFloor);

				m_cpObjectManager->AddObject(tempFloor, CObjectManager::eObjectType::Static);
			}
			else if (strcmp(prefix.c_str(), "Grd_") == 0)//ground prefab(around pits)
			{
				tempMesh->ConvertVertices();
				
				/*
				float f3Min[3] =
				{
					FLT_MAX,
					FLT_MAX,
					FLT_MAX
				};

				float f3Max[3] =
				{
					-FLT_MAX,
					-FLT_MAX,
					-FLT_MAX
				};

				for (unsigned int i = 0; i < tempMesh->GetVertices().size(); i++)
				{
					for (unsigned int xyz = 0; xyz < 3; xyz++)
					{
						if (tempMesh->GetVertices()[i].m_fPosition[xyz] < f3Min[xyz])
						{
							f3Min[xyz] = tempMesh->GetVertices()[i].m_fPosition[xyz];
						}
						if (tempMesh->GetVertices()[i].m_fPosition[xyz] > f3Max[xyz])
						{
							f3Max[xyz] = tempMesh->GetVertices()[i].m_fPosition[xyz];
						}
					}
				}

				XMFLOAT3 f3Center;
				f3Center.x = (f3Min[0] + f3Max[0]) / 2.0f;
				f3Center.y = (f3Min[1] + f3Max[1]) / 2.0f;
				f3Center.z = (f3Min[2] + f3Max[2]) / 2.0f;

				f3Center.y -= 20.0f;

				XMFLOAT3 f3Extents;
				f3Extents.x = f3Max[0] - f3Center.x;
				f3Extents.y = f3Max[1] - f3Center.y;
				f3Extents.z = f3Max[2] - f3Center.z;


				CBounds* tempBounds = new CAABB(f3Center, f3Extents);
				*/

				CPitWallObject* tempPitWall = new CPitWallObject("PitWall");
				//tempPitWall->AddCollider(new CCollider(false, tempBounds, true));
				tempPitWall->AddCollider(new CCollider(false, Bounds::AABB, tempMesh->GetVertices()));
				tempPitWall->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Ground.dds"));
				tempPitWall->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
				tempPitWall->GetRenderMesh()->GetSpecular() = 1.5f;

				//try moving collider down
				//XMFLOAT3 newCenter = ((CAABB*)(tempPitWall->GetColliders()[0]->GetBounds()))->GetCenter();
				//newCenter.y -= 20.0f;
				//((CAABB*)(tempPitWall->GetColliders()[0]->GetBounds()))->SetCenter(newCenter);

				m_cpEnvironmentObjects.push_back(tempPitWall);

				m_cpObjectManager->AddObject(tempPitWall, CObjectManager::eObjectType::Static);
			}
			else if (strcmp(prefix.c_str(), "Dor_") == 0)//door prefab
			{
				//tempMesh->ConvertVertices();
				//
				//CDoorObject* tempDoor = new CDoorObject("Door");
				////tempDoor->
				////tempDoor->
				//
				//m_cpEnvironmentObjects.push_back(tempDoor);
				//
				//m_cpObjectManager->AddObject(tempDoor, CObjectManager::eObjectType::Dynamic);
			}
			else if (strcmp(prefix.c_str(), "Pit_") == 0)//pit bottom prefab
			{
				tempMesh->ConvertVertices();

				CPitObject* tempPit = new CPitObject("Pit");
				tempPit->AddCollider(new CCollider(false, Bounds::Plane, tempMesh->GetVertices()));
				tempPit->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Ground.dds"));
				tempPit->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
				tempPit->GetRenderMesh()->GetSpecular() = 1.5f;

				m_cpEnvironmentObjects.push_back(tempPit);

				m_cpObjectManager->AddObject(tempPit, CObjectManager::eObjectType::Static);
			}
			else if (strcmp(prefix.c_str(), "BPT_") == 0)//big pit bottom prefab
			{
				tempMesh->ConvertVertices();

				CPitObject* tempBigPit = new CPitObject("BigPit");
				tempBigPit->AddCollider(new CCollider(false, Bounds::Plane, tempMesh->GetVertices()));
				tempBigPit->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Ground.dds"));
				tempBigPit->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
				tempBigPit->GetRenderMesh()->GetSpecular() = 1.5f;

				m_cpEnvironmentObjects.push_back(tempBigPit);
				m_cpBigPitObjects.push_back(tempBigPit);

				m_cpObjectManager->AddObject(tempBigPit, CObjectManager::eObjectType::Static);

				//now create teleporter for this bridge location
				XMFLOAT3 teleportPoint = { 0.0f, 0.0f, 0.0f };

				if (((CPlane*)(tempBigPit->GetColliders()[0]->GetBounds()))->GetExtents().x > ((CPlane*)(tempBigPit->GetColliders()[0]->GetBounds()))->GetExtents().z)
				{
					teleportPoint.x = ((CPlane*)(tempBigPit->GetColliders()[0]->GetBounds()))->GetCenter().x + 1000.0f;
					teleportPoint.y = ((CPlane*)(tempBigPit->GetColliders()[0]->GetBounds()))->GetCenter().y + 1000.0f;
					teleportPoint.z = ((CPlane*)(tempBigPit->GetColliders()[0]->GetBounds()))->GetCenter().z;
				}
				else
				{
					teleportPoint.x = ((CPlane*)(tempBigPit->GetColliders()[0]->GetBounds()))->GetCenter().x;
					teleportPoint.y = ((CPlane*)(tempBigPit->GetColliders()[0]->GetBounds()))->GetCenter().y + 1000.0f;
					teleportPoint.z = ((CPlane*)(tempBigPit->GetColliders()[0]->GetBounds()))->GetCenter().z + 1000.0f;
				}

				CWaypointObject* bridgeTeleport = new CWaypointObject("BridgeTeleporter");
				bridgeTeleport->SetPosition(teleportPoint);

				m_cpBridgeTeleporters.push_back(bridgeTeleport);
			}
			else if (strcmp(prefix.c_str(), "Ext_") == 0)//exit door
			{
				m_cvExitDoorMeshes.push_back(*tempMesh);
			}
			else if (strcmp(prefix.c_str(), "GFL_") == 0)//grass floor
			{
				tempMesh->ConvertVertices();

				CFloorObject* tempGrassFloor = new CFloorObject("SafeHavenFloor");
				tempGrassFloor->AddCollider(new CCollider(false, Bounds::Plane, tempMesh->GetVertices()));
				tempGrassFloor->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Ground_Grass.dds"));

				m_cpEnvironmentObjects.push_back(tempGrassFloor);

				m_cpObjectManager->AddObject(tempGrassFloor, CObjectManager::eObjectType::Static);
			}
			else if (strcmp(prefix.c_str(), "Skl_") == 0)//skeleton spawn point
			{
				tempMesh->ConvertVertices();
				XMFLOAT3 tempPos = GetAABBCentroid(tempMesh->GetVertices());

				CSpawnerObject* tempSpawner = new CSpawnerObject("SkeletonSpawn");
				tempSpawner->GetSpawnPosition() = tempPos;

				m_cpSkeletonSpawnObjects.push_back(tempSpawner);
			}
			else if (strcmp(prefix.c_str(), "Orc_") == 0)//orc spawn point
			{
				tempMesh->ConvertVertices();
				XMFLOAT3 tempPos = GetAABBCentroid(tempMesh->GetVertices());
				
				CSpawnerObject* tempSpawner = new CSpawnerObject("OrcSpawn");
				tempSpawner->GetSpawnPosition() = tempPos;

				
				m_cpOrcSpawnObjects.push_back(tempSpawner);
			}
			else if (strcmp(prefix.c_str(), "ECS_") == 0)//enemy cave spawners
			{
			}
			else if (strcmp(prefix.c_str(), "Min_") == 0)//minotaur spawn point
			{
				tempMesh->ConvertVertices();
				XMFLOAT3 tempPos = GetAABBCentroid(tempMesh->GetVertices());

				CSpawnerObject* tempSpawner = new CSpawnerObject("MinotaurSpawn");
				tempSpawner->GetSpawnPosition() = tempPos;


				m_cpMinotaurSpawnObjects.push_back(tempSpawner);
			}
			else if (strcmp(prefix.c_str(), "Mpt_") == 0)//minotaur waypoints
			{
#pragma region Minotaur Waypoints

				string tempName = "                             ";
				for (int i = 4; i < 30; i++)
				{
					tempName[i] = objName[i];

					if ((int)objName[i] == 'e')
					{
						break;
					}
				}

				//get waypoint ID
				int firstNum = (int)tempName[4] - 48;
				int secondNum = (int)tempName[5] - 48;
				int thirdNum = (int)tempName[6] - 48;

				int WayPointID = (firstNum * 100) + (secondNum * 10) + thirdNum;

				//get adjacent waypoint IDs
				int AdjacentIDs[4] = { -1, -1, -1, -1 };
				int nextAdjacent = 0;

				for (int i = 7; i < 30; i++)
				{
					if ((int)tempName[i] == '_')
					{
						nextAdjacent++;
						continue;
					}
					if ((int)tempName[i] == 'e')
					{
						break;
					}

					int firstNum = (int)tempName[i] - 48;
					int secondNum = (int)tempName[i + 1] - 48;
					int thirdNum = (int)tempName[i + 2] - 48;

					AdjacentIDs[nextAdjacent] = (firstNum * 100) + (secondNum * 10) + thirdNum;
					if (AdjacentIDs[nextAdjacent] >= 147)
					{
						break;
					}
					i += 2;
				}

				//create node
				tempMesh->ConvertVertices();
				XMFLOAT3 tempPos = GetAABBCentroid(tempMesh->GetVertices());
				CNode* tempNode = new CNode(WayPointID, tempPos);

				//create and add edges
				for (unsigned int i = 0; i < 4; i++)
				{
					if (AdjacentIDs[i] == -1)
					{
						continue;
					}

					tempNode->addEdge(new CEdge(1.0f, AdjacentIDs[i]));
				}

				//add node to navgraph
				m_cpMinotaurNavGraph->AddNode(tempNode);

#pragma endregion
			}
			else if (strcmp(prefix.c_str(), "CWL_") == 0)//cracked wall location
			{
				//tempMesh->ConvertVertices();

				//CCrackedWall* tempCrackedWall = new CCrackedWall("CrackedWall");
				//tempCrackedWall->AddCollider(new CCollider(false, Bounds::AABB, tempMesh->GetVertices()));
				//tempCrackedWall->SetRenderMesh(new CRenderMesh(tempMesh->GetIndices(), tempMesh->GetVertices(), GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr));

				//m_cpEnvironmentObjects.push_back(tempCrackedWall);

				//m_cpObjectManager->AddObject(tempCrackedWall, CObjectManager::eObjectType::Static);
			}
			else if (strcmp(prefix.c_str(), "LWC_") == 0)//light weapon upgrade chest location
			{
			}
			else if (strcmp(prefix.c_str(), "HWC_") == 0)//heavy weapon upgrade chest location
			{
			}
			else if (strcmp(prefix.c_str(), "HUC_") == 0)//health upgrade chest location
			{
			}
			else if (strcmp(prefix.c_str(), "Tor_") == 0)//torch location
			{
#pragma region Torches

				std::string tempName = "            ";
				for (unsigned int i = 4; i < 12; i++)
				{
					tempName[i] = objName[i];
				}

				XMFLOAT3 tiltDirection = { 0, 0, 0 };

				//get x value
				int secondNum = (int)tempName[5] - 48;
				if (tempName[4] == 'n')
				{
					tiltDirection.x = -((float)secondNum);
				}
				else
				{
					tiltDirection.x = (float)secondNum;
				}

				//get y value
				secondNum = (int)tempName[8] - 48;
				if (tempName[7] == 'n')
				{
					tiltDirection.y = -((float)secondNum);
				}
				else
				{
					tiltDirection.y = (float)secondNum;
				}

				//get z value
				secondNum = (int)tempName[11] - 48;
				if (tempName[10] == 'n')
				{
					tiltDirection.z = -((float)secondNum);
				}
				else
				{
					tiltDirection.z = (float)secondNum;
				}

				tempMesh->ConvertVertices();
				XMFLOAT3 tempPos = GetAABBCentroid(tempMesh->GetVertices());

				m_cpTorchObjects.push_back(new CTorch(m_cpObjectManager, tempPos, tiltDirection));

#pragma endregion
			}
			else if (strcmp(prefix.c_str(), "STL_") == 0)//pressure plate spike trap
			{
				tempMesh->ConvertVertices();
				XMFLOAT3 tempPos = GetAABBCentroid(tempMesh->GetVertices());

				m_cpSpikeTrapObjects.push_back(new CSpikeTrap(tempPos, true, 0.0f, 1.0f));
			}
			else if (strcmp(prefix.c_str(), "AST_") == 0)//alternating spike trap
			{
				std::string tempName = "          ";
				for (unsigned int i = 4; i < 9; i++)
				{
					tempName[i] = objName[i];
				}

				int firstNum = (int)tempName[4] - 48;
				int secondNum = (int)tempName[5] - 48;

				float startTime = (float)firstNum + ((float)secondNum / 10.0f);

				firstNum = (int)tempName[7] - 48;
				secondNum = (int)tempName[8] - 48;

				float offsetTime = (float)firstNum + ((float)secondNum / 10.0f);

				tempMesh->ConvertVertices();
				XMFLOAT3 tempPos = GetAABBCentroid(tempMesh->GetVertices());

				m_cpAltSpikeTrapObjects.push_back(new CSpikeTrap(tempPos, false, startTime, offsetTime));
			}
			else if (strcmp(prefix.c_str(), "SBT_") == 0)//spinning blade trap
			{
				std::string tempName = "        ";
				for (unsigned int i = 4; i < 7; i++)
				{
					tempName[i] = objName[i];
				}

				int firstNum = (int)tempName[4] - 48;
				int secondNum = (int)tempName[5] - 48;

				float rotationTime = (float)firstNum + ((float)secondNum / 10.0f);

				tempMesh->ConvertVertices();
				XMFLOAT3 tempPos = GetAABBCentroid(tempMesh->GetVertices());

				m_cpSpinningBladeObjects.push_back(new CSpinningBlade(tempPos, rotationTime));
			}
			else if (strcmp(prefix.c_str(), "FTE_") == 0)//fire trap emmiter
			{
#pragma region Fire Trap

				std::string tempName = "            ";
				for (unsigned int i = 4; i < 12; i++)
				{
					tempName[i] = objName[i];
				}

				XMFLOAT3 fireDirection = { 0, 0, 0 };

				int secondNum = (int)tempName[5] - 48;
				if (tempName[4] == 'n')
				{
					fireDirection.x = -((float)secondNum);
				}
				else
				{
					fireDirection.x = (float)secondNum;
				}

				secondNum = (int)tempName[8] - 48;
				if (tempName[7] == 'n')
				{
					fireDirection.y = -((float)secondNum);
				}
				else
				{
					fireDirection.y = (float)secondNum;
				}

				secondNum = (int)tempName[11] - 48;
				if (tempName[10] == 'n')
				{
					fireDirection.z = -((float)secondNum);
				}
				else
				{
					fireDirection.z = (float)secondNum;
				}

				tempMesh->ConvertVertices();
				XMFLOAT3 tempPos = GetAABBCentroid(tempMesh->GetVertices());

				m_cpFireTrapObjects.push_back(new CFireTrap(m_cpObjectManager, tempPos, fireDirection));

#pragma endregion
			}
			else if (strcmp(prefix.c_str(), "Tre_") == 0)//cuttable trees
			{
			}
			else if (strcmp(prefix.c_str(), "Bsh_") == 0)//bushes
			{
				tempMesh->ConvertVertices();
				XMFLOAT3 tempPos = GetAABBCentroid(tempMesh->GetVertices());

				m_cpBushObjects.push_back(new CBush(m_cpObjectManager, tempPos));
			}
			else if (strcmp(prefix.c_str(), "EDT_") == 0)//exit door teleporters
			{
				m_cvExitTeleporterMeshes.push_back(*tempMesh);
			}
			else if (strcmp(prefix.c_str(), "Inv_") == 0)//invisible walls
			{
				tempMesh->ConvertVertices();

				CWallObject* tempWall = new CWallObject("Wall");
				tempWall->AddCollider(new CCollider(false, Bounds::AABB, tempMesh->GetVertices()));
				tempWall->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader()));
				tempWall->GetRenderMesh()->GetRender() = false;

				m_cpInvisibleWallObjects.push_back(tempWall);

				m_cpObjectManager->AddObject(tempWall, CObjectManager::eObjectType::Static);
			}
			else if (strcmp(prefix.c_str(), "Kil_") == 0)//kill floor
			{
				tempMesh->ConvertVertices();

				CFloorObject* tempFloor = new CFloorObject("KillFloor");
				tempFloor->AddCollider(new CCollider(false, Bounds::Plane, tempMesh->GetVertices()));
				tempFloor->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader()));
				tempFloor->GetRenderMesh()->GetRender() = false;

				m_cpKillFloorObjects.push_back(tempFloor);

				m_cpObjectManager->AddObject(tempFloor, CObjectManager::eObjectType::Static);
			}
			else
			{
				meshes.push_back(*tempMesh);
			}

			attributeMesh->Destroy();
			delete tempMesh;
		}
	}

	for (int i = 0; i < node->GetChildCount(); i++)
	{
		ProcessLevelNode(node->GetChild(i), meshes);
	}

	return true;
}

XMFLOAT3 CLevelLoader::GetAABBCentroid(std::vector<TVertex> verts)
{
	float f3Min[3] =
	{
		FLT_MAX,
		FLT_MAX,
		FLT_MAX
	};
	float f3Max[3] =
	{
		-FLT_MAX,
		-FLT_MAX,
		-FLT_MAX
	};

	for (unsigned int i = 0; i < verts.size(); i++)
	{
		for (unsigned int xyz = 0; xyz < 3; xyz++)
		{
			if (verts[i].m_fPosition[xyz] < f3Min[xyz])
			{
				f3Min[xyz] = verts[i].m_fPosition[xyz];
			}
			if (verts[i].m_fPosition[xyz] > f3Max[xyz])
			{
				f3Max[xyz] = verts[i].m_fPosition[xyz];
			}
		}
	}
	XMFLOAT3 f3Center;
	f3Center.x = (f3Min[0] + f3Max[0]) / 2.0f;
	f3Center.y = (f3Min[1] + f3Max[1]) / 2.0f;
	f3Center.z = (f3Min[2] + f3Max[2]) / 2.0f;

	return f3Center;
}


/*	PUBLIC FUNCTIONS	*/

/*****************************************************************
* CLevelLoader()		The constructor for the level loader
*
* Ins:					None
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    08/16/2015
* Mod. Initials:	    NH
*****************************************************************/
CLevelLoader::CLevelLoader(CObjectManager* manager)
{
	m_cpMinotaurNavGraph = new CNavGraph();

	m_cpObjectManager = manager;

}

/*****************************************************************
* ~CLevelLoader()		The destructor for the level loader
*
* Ins:					None
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    09/02/2015
* Mod. Initials:	    NH
*****************************************************************/
CLevelLoader::~CLevelLoader()
{
	//all environment objects loaded in
	for (unsigned int i = 0; i < m_cpEnvironmentObjects.size(); i++)
	{
		delete m_cpEnvironmentObjects[i];
		m_cpEnvironmentObjects[i] = nullptr;
	}

#pragma region Spawners, Waypoints, Teleport Point Clean Up

	//skeleton spawners
	for (unsigned int i = 0; i < m_cpSkeletonSpawnObjects.size(); i++)
	{
		delete m_cpSkeletonSpawnObjects[i];
		m_cpSkeletonSpawnObjects[i] = nullptr;
	}

	//orc spawners
	for (unsigned int i = 0; i < m_cpOrcSpawnObjects.size(); i++)
	{
		delete m_cpOrcSpawnObjects[i];
		m_cpOrcSpawnObjects[i] = nullptr;
	}

	//minotaur spawners
	for (unsigned int i = 0; i < m_cpMinotaurSpawnObjects.size(); i++)
	{
		delete m_cpMinotaurSpawnObjects[i];
		m_cpMinotaurSpawnObjects[i] = nullptr;
	}

	//the exit door teleporter
	if (m_cpTheExitTeleporter)
	{
		delete m_cpTheExitTeleporter;
		m_cpTheExitTeleporter = nullptr;
	}

	//bridge teleporters
	for (unsigned int i = 0; i < m_cpBridgeTeleporters.size(); i++)
	{
		delete m_cpBridgeTeleporters[i];
		m_cpBridgeTeleporters[i] = nullptr;
	}

#pragma endregion

#pragma region Trap Object Clean Up

	//pressure plate spike trap
	for (unsigned int i = 0; i < m_cpSpikeTrapObjects.size(); i++)
	{
		delete m_cpSpikeTrapObjects[i];
		m_cpSpikeTrapObjects[i] = nullptr;
	}
	m_cpSpikeTrapObjects.clear();

	//alternating spike trap
	for (unsigned int i = 0; i < m_cpAltSpikeTrapObjects.size(); i++)
	{
		delete m_cpAltSpikeTrapObjects[i];
		m_cpAltSpikeTrapObjects[i] = nullptr;
	}
	m_cpAltSpikeTrapObjects.clear();

	//spinning blade trap
	for (unsigned int i = 0; i < m_cpSpinningBladeObjects.size(); i++)
	{
		delete m_cpSpinningBladeObjects[i];
		m_cpSpinningBladeObjects[i] = nullptr;
	}
	m_cpSpinningBladeObjects.clear();

	//fire trap
	for (unsigned int i = 0; i < m_cpFireTrapObjects.size(); i++)
	{
		delete m_cpFireTrapObjects[i];
		m_cpFireTrapObjects[i] = nullptr;
	}
	m_cpFireTrapObjects.clear();

#pragma endregion

#pragma region Other Object Clean Up

	//torches
	for (unsigned int i = 0; i < m_cpTorchObjects.size(); i++)
	{
		delete m_cpTorchObjects[i];
		m_cpTorchObjects[i] = nullptr;
	}

	//bushes
	for (unsigned int i = 0; i < m_cpBushObjects.size(); i++)
	{
		delete m_cpBushObjects[i];
		m_cpBushObjects[i] = nullptr;
	}

	//invisible walls
	for (unsigned int i = 0; i < m_cpInvisibleWallObjects.size(); i++)
	{
		delete m_cpInvisibleWallObjects[i];
		m_cpInvisibleWallObjects[i] = nullptr;
	}

	//kill floor
	for (unsigned int i = 0; i < m_cpKillFloorObjects.size(); i++)
	{
		delete m_cpKillFloorObjects[i];
		m_cpKillFloorObjects[i] = nullptr;
	}

#pragma endregion

}

/*****************************************************************
* LoadLevel()			Returns the only instance of the MessageManager
*
* Ins:					szFilePath
*
* Outs:					None
*
* Returns:				bool
*
* Mod. Date:		    09/02/2015
* Mod. Initials:	    NH
*****************************************************************/
bool CLevelLoader::LoadLevel(string file_name)
{

#pragma region FBX initialize

	// Get an FBX manager
	FbxManager* manager = FbxManager::Create();
	if (manager == 0)
	{
		return false;
	}

	// Create IO settings
	FbxIOSettings* io_settings = FbxIOSettings::Create(manager, IOSROOT);
	if (io_settings == 0)
	{
		manager->Destroy();
		return false;
	}

	manager->SetIOSettings(io_settings);

	// Create importer
	FbxImporter* importer = FbxImporter::Create(manager, "");
	if (importer == 0)
	{
		io_settings->Destroy();
		manager->Destroy();

		return false;
	}

	// Initialize importer
	if (importer->Initialize(file_name.c_str(), -1, io_settings) == false)
	{
		io_settings->Destroy();
		manager->Destroy();
		importer->Destroy();
		return false;
	}

	// Create a scene
	FbxScene* scene = FbxScene::Create(manager, "myScene");
	if (scene == 0)
	{
		io_settings->Destroy();
		manager->Destroy();
		importer->Destroy();
		return false;
	}

	// Load the scene with contents from the file
	if (importer->Import(scene) == false)
	{
		io_settings->Destroy();
		manager->Destroy();
		importer->Destroy();
		scene->Destroy();
		return false;
	}

	// No longer need the importer

	// Traverse the scene
	FbxNode* root_node = scene->GetRootNode();
	if (ProcessLevelNode(root_node, m_cvMeshes) == false)
	{
		importer->Destroy();
		io_settings->Destroy();
		root_node->Destroy();
		scene->Destroy();
		manager->Destroy();
		return false;
	}

	importer->Destroy();
	io_settings->Destroy();
	root_node->Destroy();
	scene->Destroy();
	manager->Destroy();
	


#pragma endregion

#pragma region Exit Door Objects
	srand((unsigned int)(CURRENT_TIME()));
	unsigned int ExitDoorIndex = rand() % 4;

	//for (unsigned int i = 0; i < m_cvExitDoorMeshes.size(); i++)
	//{
	//	m_cvExitDoorMeshes[i].ConvertVertices();
	//	m_cvExitTeleporterMeshes[i].ConvertVertices();

	//	if (i == ExitDoorIndex)
	//	{
	//		//this is the exit door
	//		m_cpTheExitDoor = new CExitDoorObject("ExitDoor");
	//		m_cpTheExitDoor->AddCollider(new CCollider(true, Bounds::AABB, m_cvExitDoorMeshes[i].GetVertices()));
	//		m_cpTheExitDoor->SetRenderMesh(new CRenderMesh(&m_cvExitDoorMeshes[i], GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Door.dds"));

	//		m_cpEnvironmentObjects.push_back(m_cpTheExitDoor);
	//		m_cpObjectManager->AddObject(m_cpTheExitDoor, CObjectManager::eObjectType::Dynamic);

	//		//set up teleporter
	//		m_cpTheExitTeleporter = new CWaypointObject("ExitTeleporter");
	//		m_cpTheExitTeleporter->SetPosition(GetAABBCentroid(m_cvExitTeleporterMeshes[i].GetVertices()));

	//		continue;
	//	}

	//	//thse are not exit doors anymore. now they are standard walls
	//	CWallObject* newWall = new CWallObject("Wall");
	//	newWall->AddCollider(new CCollider(false, Bounds::AABB, m_cvExitDoorMeshes[i].GetVertices()));
	//	newWall->SetRenderMesh(new CRenderMesh(&m_cvExitDoorMeshes[i], GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Wall.dds"));

	//	m_cpEnvironmentObjects.push_back(newWall);
	//	m_cpObjectManager->AddObject(newWall, CObjectManager::eObjectType::Static);
	//}

#pragma endregion

	CNavGraphManager::GetReference().AddNavGraph("MinotaurNavGraph", m_cpMinotaurNavGraph);

	return true;
}



/*	ACCESSORS	*/

/*****************************************************************
* GetLevelObjects()		Returns all objects loaded from the level.
*						Not including spawners, waypoints, teleport points
*
* Ins:					None
*
* Outs:					None
*
* Returns:				vector<IObject*>
*
* Mod. Date:		    08/13/2015
* Mod. Initials:	    NH
*****************************************************************/
vector<IObject*> CLevelLoader::GetEnvironmentObjects()
{
	return m_cpEnvironmentObjects;
}

/*****************************************************************
* GetSkeletonSpawnerObjects()	Returns all skeleton spawners
*
* Ins:							None
*
* Outs:							None
*
* Returns:						vector<CSpawnerObject*>
*
* Mod. Date:					08/16/2015
* Mod. Initials:				NH
*****************************************************************/
vector<CSpawnerObject*> CLevelLoader::GetSkeletonSpawnerObjects()
{
	return m_cpSkeletonSpawnObjects;
}

/*****************************************************************
* GetOrcSpawnerObjects()		Returns all orc spawners
*
* Ins:							None
*
* Outs:							None
*
* Returns:						vector<CSpawnerObject*>
*
* Mod. Date:					09/02/2015
* Mod. Initials:				NH
*****************************************************************/
vector<CSpawnerObject*> CLevelLoader::GetOrcSpawnerObjects()
{
	return m_cpOrcSpawnObjects;
}

/*****************************************************************
* GetMinotaurSpawnerObjects()	Returns all minotaur spawners
*
* Ins:							None
*
* Outs:							None
*
* Returns:						vector<CSpawnerObject*>
*
* Mod. Date:					09/02/2015
* Mod. Initials:				NH
*****************************************************************/
vector<CSpawnerObject*> CLevelLoader::GetMinotaurSpawnerObjects()
{
	return m_cpMinotaurSpawnObjects;
}

/*****************************************************************
* GetMinotaurWaypoints()	Returns all minotaur waypoints
*
* Ins:						None
*
* Outs:						None
*
* Returns:					CNavGraph*
*
* Mod. Date:				09/02/2015
* Mod. Initials:			NH
*****************************************************************/
CNavGraph* CLevelLoader::GetMinotaurNavGraph()
{
	return m_cpMinotaurNavGraph;
}

/*****************************************************************
* GetCrackedWallObjects()	Returns all cracked wall objects
*
* Ins:						None
*
* Outs:						None
*
* Returns:					vector<CCrackedWall*>
*
* Mod. Date:				09/02/2015
* Mod. Initials:			NH
*****************************************************************/
vector<XMFLOAT3> CLevelLoader::GetCrackedWallObjects()
{
	return m_cpCrackedWallObjects;
}

/*****************************************************************
* GetBigPitObjects()	Returns all big pit objects
*
* Ins:					None
*
* Outs:					None
*
* Returns:				vector<CPitObject*>
*
* Mod. Date:		    09/02/2015
* Mod. Initials:	    NH
*****************************************************************/
vector<CPitObject*> CLevelLoader::GetBigPitObjects()
{
	return m_cpBigPitObjects;
}

/*****************************************************************
* GetBridgeTeleporterPosition()	Returns all bridge teleporters
*
* Ins:						None
*
* Outs:						None
*
* Returns:					XMFLOAT3
*
* Mod. Date:				09/13/2015
* Mod. Initials:			NH
*****************************************************************/
XMFLOAT3 CLevelLoader::GetBridgeTeleporterPosition(unsigned int ID)
{
	if (ID >= m_cpBridgeTeleporters.size())
	{
		return XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	XMFLOAT3 temp;
	temp.x = m_cpBridgeTeleporters[ID]->GetPosition()->x;
	temp.y = m_cpBridgeTeleporters[ID]->GetPosition()->y;
	temp.z = m_cpBridgeTeleporters[ID]->GetPosition()->z;
	return temp;
}

/*****************************************************************
* GetExitTeleport()		Returns the waypoint at which the exit door is located
*
* Ins:					None
*
* Outs:					None
*
* Returns:				CWaypointObject*
*
* Mod. Date:		    09/06/2015
* Mod. Initials:	    NH
*****************************************************************/
XMFLOAT3 CLevelLoader::GetExitTeleportPosition() const
{
	return *m_cpTheExitTeleporter->GetPosition();
}

void CLevelLoader::ActivateExitDoor(float moveTime)
{
	m_cpTheExitDoor->SetMovementTime(moveTime);
}
