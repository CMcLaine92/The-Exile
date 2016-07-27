#include "Maze.h"
#include "TileMap.h"
#include "../../../Engine/AI/CNavGraph.h"
#include "../../../Engine/AI/Node.h"
#include "../../../Engine/AI/Edge.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "LevelPrefab.h"
#include "../../../Engine/AI/NavGraphManager.h"
#include "../../../Engine/Object Manager/ObjectManager.h"
#include "../../../Engine/Object Manager/WallObject.h"
#include "../../../Engine/Object Manager/FloorObject.h"
#include "../../../Engine/Object Manager/DoorObject.h"
#include "../../../Engine/Object Manager/ExitDoorObject.h"
#include "../../../Engine/Object Manager/PitObject.h"
#include "../../../Engine/Object Manager/PitWallObject.h"
#include "../../../Engine/Object Manager/SpawnerObject.h"
#include "../Environment/CrackedWall.h"
#include "../Environment/Bush.h"
#include "../Environment/Tree.h"
#include "../Environment/Torch.h"
#include "../Traps/FireTrap.h"
#include "../Traps/SpikeTrap.h"
#include "../Traps/SpinningBlade.h"
#include "../Environment/BedObject.h"
#include "../../../Engine/Object Manager/MinotaurSpawn.h"

#include "../../../Engine/Object Manager/PrefabObject.h"
#include "../Environment/ExitKey.h"

#include <fstream>
CMaze::CMaze()
{
	/*m_cpTileMap = new CTileMap(int2(45, 45));
	m_unNumShortCuts = 10;
	m_unNumMinSpawns = 10;
	m_unNumSkeletonSpawn = 80;
	m_unNumOrcSpawns = 15;
	m_unNumBigPits = 1;
	m_unNumSmallPits = 40;
	m_unTilesProcessed = 0;*/
	exitDoor = nullptr;
	srand((unsigned int)time(0));

}

CMaze::~CMaze()
{
	ClearMaze();
	
	/*delete m_cpTileMap;
	m_cpTileMap = nullptr;
	for (size_t i = 0; i < levelPieces.size(); i++)
	{
	delete levelPieces[i];
	levelPieces[i] = nullptr;
	}
	levelPieces.clear();*/
}

int temp1 = 0;
int temp2 = 0;
int temp3 = 0;
void CMaze::ClearMaze()
{
	temp1 = m_pvLevelPrefabs.size();
	temp2 = CrackedWallNodes.size();
	temp3 = BridgeWallNodes.size();

	VECTOR_DELETE_ALL(m_pvLevelPrefabs);
	VECTOR_DELETE_ALL(CrackedWallNodes);
	VECTOR_DELETE_ALL(BridgeWallNodes);

	m_pvLevelPrefabs.clear();
	CrackedWallNodes.clear();
	BridgeWallNodes.clear();

	levelData.clear();

	
	
}
void CMaze::GenerateMaze()
{

}
void CMaze::BuildNavData()
{
	CNavGraph* minNavGraph = new CNavGraph;
	CNavGraph* SkelOrcNavGraph = new CNavGraph;




	for (size_t i = 0; i < navData.size(); i++)
	{
		//CNode* newNode = nullptr;
		switch (navData[i].type)
		{
		case SKELETON_IGNORE:
		case PATH_NODE:
			minNavGraph->GetNodes().push_back(new CNode(minNavGraph->GetNodes().size(), navData[i].position));
			break;
		case CWALL_NODE:
			CrackedWallNodes.push_back(new CNode(CrackedWallNodes.size(), navData[i].position));
			break;
		case PIT_NODE:
			BridgeWallNodes.push_back(new CNode(BridgeWallNodes.size(), navData[i].position));
			break;

		default:
			break;
		}

	}

	for (int _node = 0; _node < (int)minNavGraph->GetNodes().size(); _node++)
	{
		CNode* currNode = minNavGraph->GetNodes()[_node];

		for (int other = 0; other < (int)minNavGraph->GetNodes().size(); other++)
		{
			CNode* otherNode = minNavGraph->GetNodes()[other];
			if (currNode == otherNode)
				continue;

			if ((otherNode->GetPosition().x == currNode->GetPosition().x && fabs(otherNode->GetPosition().z - currNode->GetPosition().z) == 1000)
				|| (otherNode->GetPosition().z == currNode->GetPosition().z && fabs(otherNode->GetPosition().x - currNode->GetPosition().x) == 1000))
			{
				CEdge* edge = new CEdge(1.0f, other);
				currNode->addEdge(edge);
			}
		}
	}

	vector<TTri> triangles;
	int triIndex = 0;
	for (size_t i = 0; i < navData.size(); i++)
	{

		switch (navData[i].type)
		{
		case CAVE_NODE:
		case PATH_NODE:
		case MIN_PATROL_IGNORE:
		{
			TTri tri1;
			TTri tri2;

			float3 points[4];
			points[0] = float3(navData[i].position.x - 500, 0.0f, navData[i].position.z - 500);
			points[1] = float3(navData[i].position.x + 500, 0.0f, navData[i].position.z - 500);
			points[2] = float3(navData[i].position.x - 500, 0.0f, navData[i].position.z + 500);
			points[3] = float3(navData[i].position.x + 500, 0.0f, navData[i].position.z + 500);

			tri1.verts[0] = points[0];
			tri1.verts[1] = points[1];
			tri1.verts[2] = points[2];

			tri2.verts[0] = points[2];
			tri2.verts[1] = points[1];
			tri2.verts[2] = points[3];

			tri1.centroid = float3((tri1.verts[0].x + tri1.verts[1].x + tri1.verts[2].x) / 3, 0.0f, (tri1.verts[0].z + tri1.verts[1].z + tri1.verts[2].z) / 3);
			tri2.centroid = float3((tri2.verts[0].x + tri2.verts[1].x + tri2.verts[2].x) / 3, 0.0f, (tri2.verts[0].z + tri2.verts[1].z + tri2.verts[2].z) / 3);
			triangles.push_back(tri1);
			triangles.push_back(tri2);
			triIndex += 2;
		}
		break;

		default:
			break;
		}
	}

	for (int _tri = 0; _tri < (int)triangles.size(); _tri++)
	{
		TTri* currTri = &triangles[_tri];

		for (int other = 0; other < (int)triangles.size(); other++)
		{
			TTri* otherTri = &triangles[other];
			if (currTri == otherTri)
				continue;
			int shared = 0;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (currTri->verts[i].x == otherTri->verts[j].x &&
						currTri->verts[i].y == otherTri->verts[j].y &&
						currTri->verts[i].z == otherTri->verts[j].z
						)
					{
						shared++;
						break;
					}
				}
				if (shared == 2)
				{
					currTri->edges.push_back(other);
					break;
				}
			}
		}
	}
	for (size_t i = 0; i < triangles.size(); i++)
	{

		CNode* node = new CNode(SkelOrcNavGraph->GetNodes().size(), float3(triangles[i].centroid.x, 0.0f, triangles[i].centroid.z));
		for (size_t currEdge = 0; currEdge < triangles[i].edges.size(); currEdge++)
		{
			CEdge* edge = new CEdge(1.0f, triangles[i].edges[currEdge]);
			node->addEdge(edge);
		}
		SkelOrcNavGraph->GetNodes().push_back(node);
	}

	CNavGraphManager::GetReference().AddNavGraph("SkeletonNavGraph", SkelOrcNavGraph);
	CNavGraphManager::GetReference().AddNavGraph("MinotaurNavGraph", minNavGraph);
}
bool CMaze::LoadPathingData(const char* filePath)
{
	ifstream file(filePath);
	if (file.is_open())
	{
		char delim;
		int type = -1;
		int x = 0, y = 0;

		PrefabData tileData;
		do
		{
			file >> type;
			file >> delim;
			tileData.type = (ePrefabType)type;
			tileData.position.x = (x * 1000.0f + 500.0f) - 50000.0f;
			tileData.position.y = 0.0f;
			tileData.position.z = (y * 1000.0f + 500.0f) - 50000.0f;

			if (type != -1)
				navData.push_back(tileData);

			x++;

			if (x == 100)
			{
				x = 0;
				y++;
			}

		} while (file.eof() == false);
		file.close();
		return true;
	}
	return false;
}
bool CMaze::LoadLayer(const char* filePath)
{
	ifstream file(filePath);
	if (file.is_open())
	{
		char delim;
		int type = -1;
		int x = 0, y = 0;

		PrefabData tileData;
		do
		{

			file >> type;
			file >> delim;
			tileData.type = (ePrefabType)type;
			tileData.position.x = (x * 1000.0f + 500.0f) - 50000.0f;
			tileData.position.y = 0.0f;
			tileData.position.z = (y * 1000.0f + 500.0f) - 50000.0f;

			levelData.push_back(tileData);

			x++;

			if (x == 100)
			{
				x = 0;
				y++;
			}

		} while (file.eof() == false);
		file.close();
		return true;
	}
	return false;
}
bool CMaze::LoadMaze(CObjectManager* objectManager , CDayNight* dayNight)
{

	LoadLayer("../Game/Assets/Scripts/Config/Maze_Base.csv");
	ProcessLayer(objectManager, dayNight);
	LoadPathingData("../Game/Assets/Scripts/Config/Maze_Pathing.csv");
	//ProcessLayer(objectManager);
	BuildNavData();
	LoadLayer("../Game/Assets/Scripts/Config/Maze_Spawns.csv");
	ProcessLayer(objectManager, dayNight);




	return true;
}

void CMaze::ProcessLayer(CObjectManager* objectManager, CDayNight* dayNight)
{

	for (size_t i = 0; i < levelData.size(); i++)
	{
		CPrefabObject* prefab = nullptr;


		XMFLOAT4X4 mat = { 1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			levelData[i].position.x, levelData[i].position.y, levelData[i].position.z, 1.0f };

		switch (levelData[i].type)
		{

		case 57:
		case NWALL:
		{
			int rnd = (rand() % 5) + 1;
			char num = '0' + (char)rnd;
			string name = "Wall";
			name += num;
			prefab = new CPrefabObject(name.c_str(), "Wall", objectManager);
		}
		break;
		case GWALL:
		{
			int rnd = (rand() % 5) + 1;
			char num = '0' + (char)rnd;
			string name = "Wall";
			name += num;
			prefab = new CPrefabObject(name.c_str(), "Wall", objectManager);
			
			prefab->GetPrefabRenderMeshes()[0]->SetTexture(L"../Game/Assets/Art/2D/Textures/GreenWall_Diffuse.dds");
		}
		break;
		case CWALL:
			prefab = new CCrackedWall("CrackedWall", objectManager);
			break;
		case ECWALL:
			prefab = new CPrefabObject("EnemyCaveWall", "Wall", objectManager);
			m_vOrcSpawns.push_back(mat);
			break;
		case FLOOR:
			prefab = new CPrefabObject("Floor", "Floor", objectManager);
			break;

		case GFLOOR:
		{
			
			prefab = new CPrefabObject("GrassFloor1", "Floor", objectManager);
		}
		break;
		case HCHEST_U:
			prefab = new CPrefabObject("HChest_U", "Floor", objectManager);
			break;
		case HCHEST_R:
			prefab = new CPrefabObject("HChest_R", "Floor", objectManager);
			break;
		case HCHEST_D:
			prefab = new CPrefabObject("HChest_D", "Floor", objectManager);
			break;
		case HCHEST_L:
			prefab = new CPrefabObject("HChest_L", "Floor", objectManager);
			break;
		case WEAPCHEST_U:
			prefab = new CPrefabObject("WChest_U", "Floor", objectManager);
			break;
		case WEAPCHEST_R:
			prefab = new CPrefabObject("WChest_R", "Floor", objectManager);
			break;
		case WEAPCHEST_D:
			prefab = new CPrefabObject("WChest_D", "Floor", objectManager);
			break;
		case WEAPCHEST_L:
			prefab = new CPrefabObject("WChest_L", "Floor", objectManager);
			break;
		case SHLDCHEST_U:
			prefab = new CPrefabObject("SChest_U", "Floor", objectManager);
			break;
		case SHLDCHEST_R:
			prefab = new CPrefabObject("SChest_R", "Floor", objectManager);
			break;
		case SHLDCHEST_D:
			prefab = new CPrefabObject("SChest_D", "Floor", objectManager);
			break;
		case SHLDCHEST_L:
			prefab = new CPrefabObject("SChest_L", "Floor", objectManager);
			break;
		case STORCH_U:
			prefab = new CPrefabObject("TorchWall_Up", "Wall", objectManager);

			break;
		case STORCH_R:
			prefab = new CPrefabObject("TorchWall_Right", "Wall", objectManager);

			break;
		case STORCH_D:
			prefab = new CPrefabObject("TorchWall_Down", "Wall", objectManager);

			break;
		case STORCH_L:
			prefab = new CPrefabObject("TorchWall_Left", "Wall", objectManager);

			break;
		case DTORCH_V:
			prefab = new CPrefabObject("DoubleTorchWall_V", "Wall", objectManager);

			break;
		case DTORCH_H:
			prefab = new CPrefabObject("DoubleTorchWall_H", "Wall", objectManager);

			break;
		case SAFE_HAVEN:
			prefab = new CPrefabObject("SafeHaven", "SafeHaven", objectManager);
			break;
		case EXIT_U: //Good
			prefab = new CPrefabObject("ExitZoneDown", "ExitZone", objectManager);

			break;
		case EXIT_R:
			prefab = new CPrefabObject("ExitZoneUp", "ExitZone", objectManager);

			break;
		case MIN_SPAWN_S:
		{
			prefab = new CMinotaurSpawn("MinotaurPenN", objectManager, dayNight);
			m_vMinSpawns.push_back(mat);
			//exitDoor = new CExitDoorObject(position,"ExitDoor");
			//objectManager->AddObject(exitDoor, CObjectManager::eObjectType::Static);
		}
			
			break;
		case MIN_SPAWN_E:
		{
			prefab = new CMinotaurSpawn("MinotaurPenE", objectManager, dayNight);
			m_vMinSpawns.push_back(mat);

			//exitDoor = new CExitDoorObject(position,"ExitDoor");
			//objectManager->AddObject(exitDoor, CObjectManager::eObjectType::Static);
		}

		break;
		case MIN_SPAWN_N:
		{
			prefab = new CMinotaurSpawn("MinotaurPenS", objectManager, dayNight);
			m_vMinSpawns.push_back(mat);

			//exitDoor = new CExitDoorObject(position,"ExitDoor");
			//objectManager->AddObject(exitDoor, CObjectManager::eObjectType::Static);
		}

		break;
		case MIN_SPAWN_W:
		{
			prefab = new CMinotaurSpawn("MinotaurPenW", objectManager, dayNight);
			m_vMinSpawns.push_back(mat);

			//exitDoor = new CExitDoorObject(position,"ExitDoor");
			//objectManager->AddObject(exitDoor, CObjectManager::eObjectType::Static);
		}

		break;
		case EXIT_D:
			prefab = new CPrefabObject("ExitZoneLeft", "ExitZone", objectManager);

			break;
		case EXIT_L:
			prefab = new CPrefabObject("ExitZoneRight", "ExitZone", objectManager);

			break;
		case SPIT_V:
			prefab = new CPrefabObject("SmallPitV", "SmallPit", objectManager);

			break;
		case SPIT_H:
			prefab = new CPrefabObject("SmallPitH", "SmallPit", objectManager);

			break;
		case BPIT_V:
			prefab = new CPrefabObject("BigPitV", "BigPit", objectManager);

			break;
		case BPIT_H:
			prefab = new CPrefabObject("BigPitH", "BigPit", objectManager);

			break;
		case T_ThreeXOne_H:
		{
			int rnd = (rand() % 8) + 1;
			char num = '0' + (char)rnd;
			string name = "Trap1x3_H";
			name += num;
			prefab = new CPrefabObject(name.c_str(), "TrapPrefab", objectManager);

		}
		break;
		case T_ThreeXOne_V:
		{
			int rnd = (rand() % 8) + 1;
			char num = '0' + (char)rnd;
			string name = "Trap1x3_V";
			name += num;
			prefab = new CPrefabObject(name.c_str(), "TrapPrefab", objectManager);

		}
		break;
		case T_ThreeXThree_V:
		{
			int rnd = (rand() % 6) + 1;
			char num = '0' + (char)rnd;
			string name = "Trap3x3_V";
			name += num;
			prefab = new CPrefabObject(name.c_str(), "TrapPrefab", objectManager);
		}

			break;
		case T_ThreeXThree_H:
		{
			int rnd = (rand() % 6) + 1;
			char num = '0' + (char)rnd;
			string name = "Trap3x3_H";
			name += num;
			prefab = new CPrefabObject(name.c_str(), "TrapPrefab", objectManager);
		}

			break;
		case T_ThreeXFive_V:
		{
			int rnd = (rand() % 5) + 1;
			char num = '0' + (char)rnd;
			string name = "Trap5x3_V";
			name += num;
			prefab = new CPrefabObject(name.c_str(), "TrapPrefab", objectManager);

		}
			break;
		case T_ThreeXFive_H:
		{
			int rnd = (rand() % 5) + 1;
			char num = '0' + (char)rnd;
			string name = "Trap5x3_H";
			name += num;
			prefab = new CPrefabObject(name.c_str(), "TrapPrefab", objectManager);

		}

			break;
		case SKEL_SPAWN:
			m_vSkelSpawns.push_back(mat);
			break;
		case GZ_SKEL_SPAWN:
			m_vGZSkelSpawns.push_back(mat);
			break;
		
		case BUSH_SPAWN:
		{
			int rnd = rand() % 2;
			if (rnd == 0)
				prefab = new CPrefabObject("BushFloor1", "Floor", objectManager);
			else
				prefab = new CPrefabObject("BushFloor2", "Floor", objectManager);

		}
			
			break;
		case TREE_SPAWNER:
		{
			int rnd = rand() % 2;
			if (rnd == 0)
				prefab = new CPrefabObject("TreeFloor1", "Floor", objectManager);
			else
				prefab = new CPrefabObject("TreeFloor2", "Floor", objectManager);

		}
			break;
		case EXIT_KEY:
		{
			XMFLOAT3 tempPos = { levelData[i].position.x, levelData[i].position.y, levelData[i].position.z };
			m_vpExitKeys.push_back(new CExitKey(tempPos, objectManager));
		}
			break;

		default:
			break;
		}
		if (prefab)
		{
			prefab->SetWorldMatrix(mat);
			prefab->AddToObjectManager();

			m_pvLevelPrefabs.push_back(prefab);
		}

	}
	levelData.clear();

}
