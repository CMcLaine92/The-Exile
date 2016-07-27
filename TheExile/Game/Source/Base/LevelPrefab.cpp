/***********************************************
* Filename:  		LevelPrefab.cpp
* Date:      		10/2/2015
* Mod. Date: 		10/2/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a level prefab object
************************************************/

#include "LevelPrefab.h"
#include <fbxsdk/fileio/fbxiosettings.h>

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

#include <fstream>



/*	PRIVATE FUNCTIONS	*/

bool CLevelPrefab::FBXLoadPrefab(FbxNode* node, vector<CMesh*> meshes)
{
//	FbxNodeAttribute* attribute = node->GetNodeAttribute();
//
//	if (attribute != NULL)
//	{
//		if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
//		{
//			//get name prefix for this object from the fbx node
//			const char* objName = node->GetName();
//			std::string prefix = "    ";
//
//			for (unsigned int i = 0; i < 4; i++)
//			{
//				prefix[i] = objName[i];
//			}
//
//			if (strcmp(prefix.c_str(), "Wal_") == 0)//wall object
//			{
//#pragma region Wall Object
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == WALL_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, prefix));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						//create the wall object.
//						CWallObject* tempWall = new CWallObject("Wall");
//						tempWall->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh(prefix), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Wall.dds"));
//						tempWall->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//						tempWall->GetRenderMesh()->GetSpecular() = 0.5f;
//						tempWall->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//						tempWall->SetWorldMatrix(GetMeshLocalMatrix(node));
//						m_cpObjects.push_back(tempWall);
//
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					//create the wall object.
//					CWallObject* tempWall = new CWallObject("Wall");
//					tempWall->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Wall.dds"));
//					tempWall->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//					tempWall->GetRenderMesh()->GetSpecular() = 0.5f;
//					tempWall->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//					tempWall->SetWorldMatrix(GetMeshLocalMatrix(node));
//					m_cpObjects.push_back(tempWall);
//
//					//populate the load data with a new object type
//					TObjectMeshData wallObject;
//					wallObject.m_eMeshType = WALL_OBJECT;
//					wallObject.m_unMeshCount = 1;
//					wallObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(wallObject.m_unMeshCount, prefix));
//					wallObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(wallObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(wallObject);
//
//					//populate assetmanger map
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "ROW_") == 0)//render only wall
//			{
//#pragma region Render Only Wall Object
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == RENDER_ONLY_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, prefix));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						//create the wall object.
//						CWallObject* tempWall = new CWallObject("Render Only Wall");
//						tempWall->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh(prefix), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Wall.dds"));
//						tempWall->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//						tempWall->GetRenderMesh()->GetSpecular() = 0.5f;
//						tempWall->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//						tempWall->SetWorldMatrix(GetMeshLocalMatrix(node));
//						m_cpObjects.push_back(tempWall);
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					//create the wall object.
//					CWallObject* tempWall = new CWallObject("Render Only Wall");
//					tempWall->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Wall.dds"));
//					tempWall->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//					tempWall->GetRenderMesh()->GetSpecular() = 0.5f;
//					tempWall->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//					tempWall->SetWorldMatrix(GetMeshLocalMatrix(node));
//					m_cpObjects.push_back(tempWall);
//
//					//populate the load data with a new object type
//					TObjectMeshData wallObject;
//					wallObject.m_eMeshType = RENDER_ONLY_OBJECT;
//					wallObject.m_unMeshCount = 1;
//					wallObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(wallObject.m_unMeshCount, prefix));
//					wallObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(wallObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(wallObject);
//
//					//populate assetmanger map
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "INV_") == 0)//invisible wall
//			{
//#pragma region Invisible Wall Object
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == INVISIBLE_WALL_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, prefix));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						//create the object
//						CWallObject* tempWall = new CWallObject("Wall");
//						tempWall->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh(prefix), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS()));
//						tempWall->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//						tempWall->GetRenderMesh()->GetRender() = false;
//						tempWall->SetWorldMatrix(GetMeshLocalMatrix(node));
//						m_cpObjects.push_back(tempWall);
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					//create the wall object.
//					CWallObject* tempWall = new CWallObject("Wall");
//					tempWall->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS()));
//					tempWall->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//					tempWall->GetRenderMesh()->GetRender() = false;
//					tempWall->SetWorldMatrix(GetMeshLocalMatrix(node));
//					m_cpObjects.push_back(tempWall);
//
//					//populate the load data with a new object type
//					TObjectMeshData wallObject;
//					wallObject.m_eMeshType = INVISIBLE_WALL_OBJECT;
//					wallObject.m_unMeshCount = 1;
//					wallObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(wallObject.m_unMeshCount, prefix));
//					wallObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(wallObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(wallObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "TOR_") == 0)//torch object spawn point
//			{
//#pragma region Torch Object
//
//				std::string tempName = "            ";
//				for (unsigned int i = 4; i < 12; i++)
//				{
//					tempName[i] = objName[i];
//				}
//
//				XMFLOAT3 tiltDirection = { 0, 0, 0 };
//
//				//get x value
//				int secondNum = (int)tempName[5] - 48;
//				if (tempName[4] == 'n')
//				{
//					tiltDirection.x = -((float)secondNum);
//				}
//				else
//				{
//					tiltDirection.x = (float)secondNum;
//				}
//
//				//get y value
//				secondNum = (int)tempName[8] - 48;
//				if (tempName[7] == 'n')
//				{
//					tiltDirection.y = -((float)secondNum);
//				}
//				else
//				{
//					tiltDirection.y = (float)secondNum;
//				}
//
//				//get z value
//				secondNum = (int)tempName[11] - 48;
//				if (tempName[10] == 'n')
//				{
//					tiltDirection.z = -((float)secondNum);
//				}
//				else
//				{
//					tiltDirection.z = (float)secondNum;
//				}
//
//				//set torch id
//				int torchID = -1;
//				if (tiltDirection.x > tiltDirection.z && tiltDirection.z == 0.0f)//pos X
//				{
//					torchID = 3;
//				}
//				else if (tiltDirection.x < tiltDirection.z && tiltDirection.z == 0.0f)//neg x
//				{
//					torchID = 1;
//				}
//				else if (tiltDirection.z > tiltDirection.x && tiltDirection.x == 0.0f)//pos Z
//				{
//					torchID = 0;
//				}
//				else if (tiltDirection.z < tiltDirection.x && tiltDirection.x == 0.0f)//neg z
//				{
//					torchID = 2;
//				}
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == TORCH_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, tempName));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					//populate the load data with a new object type
//					TObjectMeshData torchObject;
//					torchObject.m_eMeshType = TORCH_OBJECT;
//					torchObject.m_unMeshCount = 1;
//					torchObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(torchObject.m_unMeshCount, tempName));
//					torchObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(torchObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(torchObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//				//create the spawner object
//				CSpawnerObject* tempTorch = new CSpawnerObject("Torch Spawner");
//				XMFLOAT3 tempPos = { GetMeshLocalMatrix(node)._41, GetMeshLocalMatrix(node)._42, GetMeshLocalMatrix(node)._43 };
//				tempTorch->SetAttributes(tempPos, tiltDirection);
//				tempTorch->SetWorldMatrix(GetMeshLocalMatrix(node));
//				m_umWallTorchIDs[torchID] = tempTorch;
//				m_cpObjects.push_back(tempTorch);
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "FLR_") == 0)//Floor object
//			{
//#pragma region Floor Object
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == FLOOR_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, prefix));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						//create the object
//						CFloorObject* tempFloor = new CFloorObject("Floor");
//						tempFloor->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh(prefix), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Ground.dds"));
//						tempFloor->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//						tempFloor->GetRenderMesh()->GetSpecular() = 1.5f;
//						tempFloor->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//						tempFloor->SetWorldMatrix(GetMeshLocalMatrix(node));
//						m_cpObjects.push_back(tempFloor);
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					CFloorObject* tempFloor = new CFloorObject("Floor");
//					tempFloor->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Ground.dds"));
//					tempFloor->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//					tempFloor->GetRenderMesh()->GetSpecular() = 1.5f;
//					tempFloor->SetWorldMatrix(GetMeshLocalMatrix(node));
//					m_cpObjects.push_back(tempFloor);
//
//					//populate the load data with a new object type
//					TObjectMeshData floorObject;
//					floorObject.m_eMeshType = FLOOR_OBJECT;
//					floorObject.m_unMeshCount = 1;
//					floorObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(floorObject.m_unMeshCount, prefix));
//					floorObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(floorObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(floorObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "GFL_") == 0)//grass floor
//			{
//#pragma region Grass Floor Object
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == GRASS_FLOOR_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, prefix));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						//create the object
//						CFloorObject* tempFloor = new CFloorObject("SafeHavenFloor");
//						tempFloor->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh(prefix), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Ground_Grass.dds"));
//						tempFloor->GetRenderMesh()->GetSpecular() = 1.5f;
//						tempFloor->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//						tempFloor->SetWorldMatrix(GetMeshLocalMatrix(node));
//						m_cpObjects.push_back(tempFloor);
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					CFloorObject* tempFloor = new CFloorObject("SafeHavenFloor");
//					tempFloor->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Ground_Grass.dds"));
//					tempFloor->GetRenderMesh()->GetSpecular() = 1.5f;
//					tempFloor->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//					tempFloor->SetWorldMatrix(GetMeshLocalMatrix(node));
//					m_cpObjects.push_back(tempFloor);
//
//					//populate the load data with a new object type
//					TObjectMeshData floorObject;
//					floorObject.m_eMeshType = GRASS_FLOOR_OBJECT;
//					floorObject.m_unMeshCount = 1;
//					floorObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(floorObject.m_unMeshCount, prefix));
//					floorObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(floorObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(floorObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "CWL_") == 0)//Cracked wall object. has cracked wall texture
//			{
//#pragma region Cracked Wall Object
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == CRACKED_WALL_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, prefix));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						//create the object
//						CCrackedWall* tempCrackedWall = new CCrackedWall("Cracked Wall");
//						tempCrackedWall->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh(prefix), GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr));
//						tempCrackedWall->GetRenderMesh()->GetSpecular() = 0.5f;
//						tempCrackedWall->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//						tempCrackedWall->SetWorldMatrix(GetMeshLocalMatrix(node));
//						m_cpObjects.push_back(tempCrackedWall);
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					CCrackedWall* tempCrackedWall = new CCrackedWall("Cracked Wall");
//					tempCrackedWall->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr));
//					tempCrackedWall->GetRenderMesh()->GetSpecular() = 0.5f;
//					tempCrackedWall->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//					tempCrackedWall->SetWorldMatrix(GetMeshLocalMatrix(node));
//
//					m_cpObjects.push_back(tempCrackedWall);
//
//					//populate the load data with a new object type
//					TObjectMeshData wallObject;
//					wallObject.m_eMeshType = CRACKED_WALL_OBJECT;
//					wallObject.m_unMeshCount = 1;
//					wallObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(wallObject.m_unMeshCount, prefix));
//					wallObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(wallObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(wallObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "ECW_") == 0)//enemies do not check collision with this object type
//			{
//#pragma region Enemy Cave Wall Object
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == ENEMY_CAVE_WALL_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, prefix));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						//create the object
//						CWallObject* tempCavedWall = new CWallObject("Enemy Cave Wall");
//						tempCavedWall->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh(prefix), GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr));
//						tempCavedWall->GetRenderMesh()->GetSpecular() = 0.5f;
//						tempCavedWall->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//						tempCavedWall->SetWorldMatrix(GetMeshLocalMatrix(node));
//						m_cpObjects.push_back(tempCavedWall);
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					CWallObject* tempCavedWall = new CWallObject("Enemy Cave Wall");
//					tempCavedWall->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr));
//					tempCavedWall->GetRenderMesh()->GetSpecular() = 0.5f;
//					tempCavedWall->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//					tempCavedWall->SetWorldMatrix(GetMeshLocalMatrix(node));
//					m_cpObjects.push_back(tempCavedWall);
//
//					//populate the load data with a new object type
//					TObjectMeshData wallObject;
//					wallObject.m_eMeshType = ENEMY_CAVE_WALL_OBJECT;
//					wallObject.m_unMeshCount = 1;
//					wallObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(wallObject.m_unMeshCount, prefix));
//					wallObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(wallObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(wallObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "PIT_") == 0)//small pit plane
//			{
//#pragma region Small Pit Object
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == SMALL_PIT_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, prefix));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						//create the object
//						CPitObject* tempPit = new CPitObject("Pit");
//						tempPit->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh(prefix), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Ground.dds"));
//						tempPit->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//						tempPit->GetRenderMesh()->GetSpecular() = 1.5f;
//						tempPit->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//						tempPit->SetWorldMatrix(GetMeshLocalMatrix(node));
//						m_cpObjects.push_back(tempPit);
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					CPitObject* tempPit = new CPitObject("Pit");
//					tempPit->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Ground.dds"));
//					tempPit->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//					tempPit->GetRenderMesh()->GetSpecular() = 1.5f;
//					tempPit->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//					tempPit->SetWorldMatrix(GetMeshLocalMatrix(node));
//
//					m_cpObjects.push_back(tempPit);
//
//					//populate the load data with a new object type
//					TObjectMeshData pitObject;
//					pitObject.m_eMeshType = SMALL_PIT_OBJECT;
//					pitObject.m_unMeshCount = 1;
//					pitObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(pitObject.m_unMeshCount, prefix));
//					pitObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(pitObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(pitObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "PWL_") == 0)//pit wall
//			{
//#pragma region Pit Wall Object
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == PIT_WALL_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, prefix));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						CPitWallObject* tempPitWall = new CPitWallObject("PitWall");
//						tempPitWall->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh(prefix), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Ground.dds"));
//						tempPitWall->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//						tempPitWall->GetRenderMesh()->GetSpecular() = 1.5f;
//						tempPitWall->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//						tempPitWall->SetWorldMatrix(GetMeshLocalMatrix(node));
//						m_cpObjects.push_back(tempPitWall);
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					CPitWallObject* tempPitWall = new CPitWallObject("PitWall");
//					tempPitWall->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Ground.dds"));
//					tempPitWall->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//					tempPitWall->GetRenderMesh()->GetSpecular() = 1.5f;
//					tempPitWall->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//					tempPitWall->SetWorldMatrix(GetMeshLocalMatrix(node));
//
//					m_cpObjects.push_back(tempPitWall);
//
//					//populate the load data with a new object type
//					TObjectMeshData wallObject;
//					wallObject.m_eMeshType = PIT_WALL_OBJECT;
//					wallObject.m_unMeshCount = 1;
//					wallObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(wallObject.m_unMeshCount, prefix));
//					wallObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(wallObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(wallObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "BPT_") == 0)//big pit plane
//			{
//#pragma region Big Pit Object
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == BIG_PIT_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, prefix));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						//create the object
//						CPitObject* tempBigPit = new CPitObject("BigPit");
//						tempBigPit->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh(prefix), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Ground.dds"));
//						tempBigPit->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//						tempBigPit->GetRenderMesh()->GetSpecular() = 1.5f;
//						tempBigPit->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//						tempBigPit->SetWorldMatrix(GetMeshLocalMatrix(node));
//						m_cpObjects.push_back(tempBigPit);
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					CPitObject* tempBigPit = new CPitObject("BigPit");
//					tempBigPit->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Ground.dds"));
//					tempBigPit->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//					tempBigPit->GetRenderMesh()->GetSpecular() = 1.5f;
//					tempBigPit->GetRenderMesh()->GetTransformMatrix() = GetMeshLocalMatrix(node);
//					tempBigPit->SetWorldMatrix(GetMeshLocalMatrix(node));
//
//					m_cpObjects.push_back(tempBigPit);
//
//					//populate the load data with a new object type
//					TObjectMeshData wallObject;
//					wallObject.m_eMeshType = BIG_PIT_OBJECT;
//					wallObject.m_unMeshCount = 1;
//					wallObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(wallObject.m_unMeshCount, prefix));
//					wallObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(wallObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(wallObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "BSH_") == 0)//bush object
//			{
//#pragma region Bush Spawner
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == BUSH_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, prefix));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					//populate the load data with a new object type
//					TObjectMeshData bushObject;
//					bushObject.m_eMeshType = BUSH_OBJECT;
//					bushObject.m_unMeshCount = 1;
//					bushObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(bushObject.m_unMeshCount, prefix));
//					bushObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(bushObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(bushObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//				//creaet the spawner
//				CSpawnerObject* temp = new CSpawnerObject("Bush Spawner");
//				XMFLOAT3 tempPos = { GetMeshLocalMatrix(node)._41, GetMeshLocalMatrix(node)._42, GetMeshLocalMatrix(node)._43 };
//				temp->SetAttributes(tempPos, { 0, 0, 0 });
//				temp->SetWorldMatrix(GetMeshLocalMatrix(node));
//				m_cpObjects.push_back(temp);
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "TRE_") == 0)//tree object
//			{
//#pragma region Tree Spawner
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == TREE_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, prefix));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					//populate the load data with a new object type
//					TObjectMeshData treeObject;
//					treeObject.m_eMeshType = TREE_OBJECT;
//					treeObject.m_unMeshCount = 1;
//					treeObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(treeObject.m_unMeshCount, prefix));
//					treeObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(treeObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(treeObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//				//creaet the spawner
//				CSpawnerObject* temp = new CSpawnerObject("Tree Spawner");
//				XMFLOAT3 tempPos = { GetMeshLocalMatrix(node)._41, GetMeshLocalMatrix(node)._42, GetMeshLocalMatrix(node)._43 };
//				temp->SetAttributes(tempPos, { 0, 0, 0 });
//				temp->SetWorldMatrix(GetMeshLocalMatrix(node));
//				m_cpObjects.push_back(temp);
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "CHT_") == 0)//chest spawner
//			{
//#pragma region Chest Spawner
//
//				std::string tempName = "            ";
//				for (unsigned int i = 4; i < 12; i++)
//				{
//					tempName[i] = objName[i];
//				}
//
//				XMFLOAT3 faceDirection = { 0, 0, 0 };
//
//				int secondNum = (int)tempName[5] - 48;
//				if (tempName[4] == 'n')
//				{
//					faceDirection.x = -((float)secondNum);
//				}
//				else
//				{
//					faceDirection.x = (float)secondNum;
//				}
//
//				secondNum = (int)tempName[8] - 48;
//				if (tempName[7] == 'n')
//				{
//					faceDirection.y = -((float)secondNum);
//				}
//				else
//				{
//					faceDirection.y = (float)secondNum;
//				}
//
//				secondNum = (int)tempName[11] - 48;
//				if (tempName[10] == 'n')
//				{
//					faceDirection.z = -((float)secondNum);
//				}
//				else
//				{
//					faceDirection.z = (float)secondNum;
//				}
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == CHEST_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, tempName));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					//populate the load data with a new object type
//					TObjectMeshData chestObject;
//					chestObject.m_eMeshType = CHEST_OBJECT;
//					chestObject.m_unMeshCount = 1;
//					chestObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(chestObject.m_unMeshCount, tempName));
//					chestObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(chestObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(chestObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//				//create the spawner
//				CSpawnerObject* temp = new CSpawnerObject("Chest Spawner");
//				XMFLOAT3 tempPos = { GetMeshLocalMatrix(node)._41, GetMeshLocalMatrix(node)._42, GetMeshLocalMatrix(node)._43 };
//				temp->SetAttributes(tempPos, faceDirection);
//				temp->SetWorldMatrix(GetMeshLocalMatrix(node));
//				m_cpObjects.push_back(temp);
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "Dor_") == 0)//door 
//			{
//#pragma region Door Object
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == DOOR_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, prefix));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						//create the object
//						CDoorObject* tempExitDoor = new CDoorObject("Door");
//						tempExitDoor->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh(prefix), GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Door.dds"));
//						tempExitDoor->SetWorldMatrix(GetMeshLocalMatrix(node));
//						m_cpObjects.push_back(tempExitDoor);
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					CDoorObject* tempExitDoor = new CDoorObject("Door");
//					tempExitDoor->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Door.dds"));
//					tempExitDoor->SetWorldMatrix(GetMeshLocalMatrix(node));
//					m_cpObjects.push_back(tempExitDoor);
//
//					//populate the load data with a new object type
//					TObjectMeshData doorObject;
//					doorObject.m_eMeshType = DOOR_OBJECT;
//					doorObject.m_unMeshCount = 1;
//					doorObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(doorObject.m_unMeshCount, prefix));
//					doorObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(doorObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(doorObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "EXT_") == 0)
//			{
//#pragma region Exit Door Object
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == EXIT_DOOR_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, prefix));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						//create the object
//						CExitDoorObject* tempExitDoor = new CExitDoorObject("ExitDoor");
//						tempExitDoor->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh(prefix), GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Door.dds"));
//						tempExitDoor->SetWorldMatrix(GetMeshLocalMatrix(node));
//						m_cpObjects.push_back(tempExitDoor);
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					CExitDoorObject* tempExitDoor = new CExitDoorObject("ExitDoor");
//					tempExitDoor->SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Door.dds"));
//					tempExitDoor->SetWorldMatrix(GetMeshLocalMatrix(node));
//					m_cpObjects.push_back(tempExitDoor);
//
//					//populate the load data with a new object type
//					TObjectMeshData wallObject;
//					wallObject.m_eMeshType = EXIT_DOOR_OBJECT;
//					wallObject.m_unMeshCount = 1;
//					wallObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(wallObject.m_unMeshCount, prefix));
//					wallObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(wallObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(wallObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "STL_") == 0)//spike trap
//			{
//#pragma region Spike Trap Object
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == SPIKE_TRAP_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, prefix));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					//populate the load data with a new object type
//					TObjectMeshData wallObject;
//					wallObject.m_eMeshType = SPIKE_TRAP_OBJECT;
//					wallObject.m_unMeshCount = 1;
//					wallObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(wallObject.m_unMeshCount, prefix));
//					wallObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(wallObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(wallObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//				//create the spawner
//				CSpawnerObject* temp = new CSpawnerObject("Spike Trap Spawner");
//				XMFLOAT3 tempPos = { GetMeshLocalMatrix(node)._41, GetMeshLocalMatrix(node)._42, GetMeshLocalMatrix(node)._43 };
//				temp->SetAttributes(tempPos, 0.0f, 1.0f);
//				temp->SetWorldMatrix(GetMeshLocalMatrix(node));
//				m_cpObjects.push_back(temp);
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "AST_") == 0)//alt spike trap
//			{
//#pragma region Alt Spike Trap Object
//
//				std::string tempName = "          ";
//				for (unsigned int i = 4; i < 9; i++)
//				{
//					tempName[i] = objName[i];
//				}
//
//				int firstNum = (int)tempName[4] - 48;
//				int secondNum = (int)tempName[5] - 48;
//
//				float startTime = (float)firstNum + ((float)secondNum / 10.0f);
//
//				firstNum = (int)tempName[7] - 48;
//				secondNum = (int)tempName[8] - 48;
//
//				float offsetTime = (float)firstNum + ((float)secondNum / 10.0f);
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == ALT_SPIKE_TRAP_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, tempName));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					//populate the load data with a new object type
//					TObjectMeshData wallObject;
//					wallObject.m_eMeshType = ALT_SPIKE_TRAP_OBJECT;
//					wallObject.m_unMeshCount = 1;
//					wallObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(wallObject.m_unMeshCount, tempName));
//					wallObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(wallObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(wallObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//				//create the spawner
//				CSpawnerObject* temp = new CSpawnerObject("Alt Spike Trap Spawner");
//				XMFLOAT3 tempPos = { GetMeshLocalMatrix(node)._41, GetMeshLocalMatrix(node)._42, GetMeshLocalMatrix(node)._43 };
//				temp->SetAttributes(tempPos, startTime, offsetTime);
//				temp->SetWorldMatrix(GetMeshLocalMatrix(node));
//				m_cpObjects.push_back(temp);
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "FTE_") == 0)//fire trap
//			{
//#pragma region Fire Trap Object
//
//				std::string tempName = "            ";
//				for (unsigned int i = 4; i < 12; i++)
//				{
//					tempName[i] = objName[i];
//				}
//
//				XMFLOAT3 fireDirection = { 0, 0, 0 };
//
//				int secondNum = (int)tempName[5] - 48;
//				if (tempName[4] == 'n')
//				{
//					fireDirection.x = -((float)secondNum);
//				}
//				else
//				{
//					fireDirection.x = (float)secondNum;
//				}
//
//				secondNum = (int)tempName[8] - 48;
//				if (tempName[7] == 'n')
//				{
//					fireDirection.y = -((float)secondNum);
//				}
//				else
//				{
//					fireDirection.y = (float)secondNum;
//				}
//
//				secondNum = (int)tempName[11] - 48;
//				if (tempName[10] == 'n')
//				{
//					fireDirection.z = -((float)secondNum);
//				}
//				else
//				{
//					fireDirection.z = (float)secondNum;
//				}
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == FIRE_TRAP_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, tempName));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					//populate the load data with a new object type
//					TObjectMeshData wallObject;
//					wallObject.m_eMeshType = FIRE_TRAP_OBJECT;
//					wallObject.m_unMeshCount = 1;
//					wallObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(wallObject.m_unMeshCount, tempName));
//					wallObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(wallObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(wallObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//				//create the spawner
//				CSpawnerObject* temp = new CSpawnerObject("Fire Trap Spawner");
//				XMFLOAT3 tempPos = { GetMeshLocalMatrix(node)._41, GetMeshLocalMatrix(node)._42, GetMeshLocalMatrix(node)._43 };
//				temp->SetAttributes(tempPos, fireDirection);
//				temp->SetWorldMatrix(GetMeshLocalMatrix(node));
//				m_cpObjects.push_back(temp);
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "SPT_") == 0)//spinning blade trap
//			{
//#pragma region Spinning Blade Object
//
//				std::string tempName = "         ";
//				for (unsigned int i = 4; i < 9; i++)
//				{
//					tempName[i] = objName[i];
//				}
//
//				int firstNum = (int)tempName[4] - 48;
//				int secondNum = (int)tempName[5] - 48;
//
//				float rotationSpeed = (float)firstNum + ((float)secondNum / 10.0f);
//
//				int ID = (int)tempName[7] - 48;
//
//				//loop through the load data vector to see if this object type has been loaded
//				bool objectExists = false;
//				for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//				{
//					//if the object type = a wall object type. then this wall object type has been loaded in alrdy.
//					if (m_vtObjectLoadData[i].m_eMeshType == SPINNING_BLADE_OBJECT)
//					{
//						m_vtObjectLoadData[i].m_unMeshCount++;
//						m_vtObjectLoadData[i].m_umMeshFullNameMap.insert(pair<unsigned int, string>(m_vtObjectLoadData[i].m_unMeshCount, tempName));
//						m_vtObjectLoadData[i].m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(m_vtObjectLoadData[i].m_unMeshCount, GetMeshLocalMatrix(node)));
//
//						objectExists = true;
//						break;
//					}
//				}
//
//				if (objectExists == false)
//				{
//					//create a temp mesh
//					CMesh* tempMesh = new CMesh;
//					FbxMesh* attributeMesh = (FbxMesh*)attribute;
//
//					bool errorCheck = CAssetManager::GetInstance()->LoadMesh(attributeMesh, tempMesh);
//					if (errorCheck == false)
//					{
//						return false;
//					}
//					attribute->Destroy();
//
//					//get the local matrix for this mesh
//					tempMesh->GetName() = prefix;
//					tempMesh->GetLocalMatrix() = GetMeshLocalMatrix(node);
//					tempMesh->ConvertVertices();
//					m_cpMeshes.push_back(tempMesh);
//
//					//populate the load data with a new object type
//					TObjectMeshData wallObject;
//					wallObject.m_eMeshType = SPINNING_BLADE_OBJECT;
//					wallObject.m_unMeshCount = 1;
//					wallObject.m_umMeshFullNameMap.insert(pair<unsigned int, string>(wallObject.m_unMeshCount, tempName));
//					wallObject.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(wallObject.m_unMeshCount, GetMeshLocalMatrix(node)));
//					m_vtObjectLoadData.push_back(wallObject);
//
//					CAssetManager::GetInstance()->AddPrefabMesh(prefix, tempMesh);
//				}
//
//
//				if (ID == 0)//it is a non moving spinning blade
//				{
//					CSpawnerObject* tempTrap = new CSpawnerObject("Spinning Blade Spawner");
//					XMFLOAT3 tempPos = { GetMeshLocalMatrix(node)._41, GetMeshLocalMatrix(node)._42, GetMeshLocalMatrix(node)._43 };
//					tempTrap->SetAttributes(tempPos, rotationSpeed, { 0, 0, 0 }, { 0, 0, 0 }, 0);
//					tempTrap->SetWorldMatrix(GetMeshLocalMatrix(node));
//					m_cpObjects.push_back(tempTrap);
//
//				}
//				else//it is a moving spinning blade
//				{
//					CSpawnerObject* tempSpinningBlade = new CSpawnerObject("Moving Spinning Blade Spanwer");
//					tempSpinningBlade->SetWorldMatrix(GetMeshLocalMatrix(node));
//
//					m_umSBRotationSpeedMap.insert(pair<int, float>(ID, rotationSpeed));
//					m_umSpinningBladeMap.insert(pair<int, IObject*>(ID, tempSpinningBlade));
//				}
//
//#pragma endregion
//
//			}
//			else if (strcmp(prefix.c_str(), "SBP_") == 0)//spinning blade move point
//			{
//#pragma region Spinning Blade Move Points
//
//				std::string tempName = "        ";
//				for (unsigned int i = 4; i < 8; i++)
//				{
//					tempName[i] = objName[i];
//				}
//
//				int ID = (int)tempName[4] - 48;
//				int adjID = (int)tempName[6] - 48;
//
//				m_umSBMovePointsIDs.insert(pair<int, int>(ID, adjID));
//
//				XMFLOAT3 tempPos = { GetMeshLocalMatrix(node)._41, GetMeshLocalMatrix(node)._42, GetMeshLocalMatrix(node)._43 };
//				m_umSBMovePointsMap.insert(pair<int, XMFLOAT3>(ID, tempPos));
//
//#pragma endregion
//
//			}
//
//		}
//
//	}
//
//	for (int i = 0; i < node->GetChildCount(); i++)
//	{
//		FBXLoadPrefab(node->GetChild(i), meshes);
//	}
//
	return true;
}

XMFLOAT4X4 CLevelPrefab::GetMeshLocalMatrix(FbxNode* node)
{
	FbxDouble3 translation = node->LclTranslation.Get();
	FbxDouble3 rotation = node->LclRotation.Get();
	FbxDouble3 scaling = node->LclScaling.Get();

	XMFLOAT3 translate = XMFLOAT3((float)translation.mData[0], (float)translation.mData[1], (float)translation.mData[2]);
	XMFLOAT3 scale = XMFLOAT3((float)scaling.mData[0], (float)scaling.mData[1], (float)scaling.mData[2]);
	XMFLOAT3 rotate = XMFLOAT3((float)rotation.mData[0], (float)rotation.mData[1], (float)rotation.mData[2]);


	float rotateX_Radians = RADIANS(rotate.x);
	float rotateY_Radians = RADIANS(rotate.y);
	float rotateZ_Radians = RADIANS(rotate.z);

	XMMATRIX worldMat = XMMatrixIdentity();
	worldMat = XMMatrixMultiply(worldMat, XMMatrixScaling(scale.x, scale.y, scale.z));
	worldMat = XMMatrixMultiply(worldMat, XMMatrixRotationX(rotateX_Radians));
	worldMat = XMMatrixMultiply(worldMat, XMMatrixRotationY(rotateY_Radians));
	worldMat = XMMatrixMultiply(worldMat, XMMatrixRotationZ(rotateZ_Radians));
	worldMat = XMMatrixMultiply(worldMat, XMMatrixTranslation(translate.x, translate.y, translate.z));

	XMFLOAT4X4 meshMatrix;
	XMStoreFloat4x4(&meshMatrix, worldMat);

	return meshMatrix;
}

XMFLOAT3 CLevelPrefab::GetAABBCentroid(std::vector<TVertex> verts)
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

CLevelPrefab::CLevelPrefab(string file_name, ePrefabType type, CObjectManager* objectManger)
{
//	m_nPrefabType = type;
//	m_szFBXpath = file_name;
//	m_cpObjectManager = objectManger;
//
//	m_mWorlMatrix =
//	{
//		1, 0, 0, 0,
//		0, 1, 0, 0,
//		0, 0, 1, 0,
//		0, 0, 0, 1
//	};
//
//#pragma region Prefab Object Loading
//
//	//check if binary exists. if so load in that instead of the fbx
//	if (CAssetManager::GetInstance()->BinaryExists(m_szFBXpath))
//	{
//		//call load binary function
//		PrefabBinaryMeshLoad(m_szFBXpath, m_cpMeshes);
//	}
//	else
//	{
//#pragma region FBX Initialize
//
//		// Get an FBX manager
//		FbxManager* manager = FbxManager::Create();
//		if (manager == 0)
//		{
//			return;
//		}
//
//		// Create IO settings
//		FbxIOSettings* io_settings = FbxIOSettings::Create(manager, IOSROOT);
//		if (io_settings == 0)
//		{
//			return;
//		}
//
//		manager->SetIOSettings(io_settings);
//
//		// Create importer
//		FbxImporter* importer = FbxImporter::Create(manager, "");
//		if (importer == 0)
//		{
//			return;
//		}
//
//		// Initialize importer
//		if (importer->Initialize(file_name.c_str(), -1, io_settings) == false)
//		{
//			return;
//		}
//
//		// Create a scene
//		FbxScene* scene = FbxScene::Create(manager, "PrefabScene");
//		if (scene == 0)
//		{
//			return;
//		}
//
//		// Load the scene with contents from the file
//		if (importer->Import(scene) == false)
//		{
//			return;
//		}
//
//		// No longer need the importer
//		importer->Destroy();
//
//#pragma endregion
//
//		//traverse the scene based on the prefab type
//		FbxNode* root_node = scene->GetRootNode();
//
//		//fbx loader
//		if (FBXLoadPrefab(root_node, m_cpMeshes) == false)
//		{
//			return;
//		}
//
//		if (m_nPrefabType == TRAP_SECTION_PREFAB)
//		{
//#pragma region Spinning Blade Waypoint Connection
//
//			//iterate through all the spinning blade spawners and set there waypoints
//			for (std::unordered_map<int, IObject*>::iterator iter = m_umSpinningBladeMap.begin(); iter != m_umSpinningBladeMap.end(); iter++)
//			{
//				float rotationSpeed = 0.0f;
//
//				//iterate to get the rotation speed for this spinning blade
//				for (std::unordered_map<int, float>::iterator i = m_umSBRotationSpeedMap.begin(); i != m_umSBRotationSpeedMap.end(); i++)
//				{
//					//if the spinning blade id == the id of this iterator
//					if ((*iter).first == (*i).first)//get the rotation speed
//					{
//						rotationSpeed = (*i).second;
//						break;
//					}
//				}
//
//				XMFLOAT3 startPoint = { 0, 0, 0 };
//				XMFLOAT3 endPoint = { 0, 0, 0 };
//
//				for (std::unordered_map<int, XMFLOAT3>::iterator k = m_umSBMovePointsMap.begin(); k != m_umSBMovePointsMap.end(); k++)
//				{
//					//if the waypoint ID == the spinning blade ID
//					if ((*k).first == (*iter).first)//get the start point
//					{
//						startPoint = (*k).second;
//
//						int adjID = 0;
//						//now find the adjacent waypoint
//						for (std::unordered_map<int, int>::iterator j = m_umSBMovePointsIDs.begin(); j != m_umSBMovePointsIDs.end(); j++)
//						{
//							//get the adjacent ID of the starting point
//							if ((*k).first == (*j).first)
//							{
//								adjID = (*j).second;
//								break;
//							}
//						}
//
//						//now get the adj waypoint
//						for (std::unordered_map<int, XMFLOAT3>::iterator j = m_umSBMovePointsMap.begin(); j != m_umSBMovePointsMap.end(); j++)
//						{
//							//if the starting waypoints adjID == this waypoints ID. then they are adjacent
//							if ((*j).first == adjID)//get the endpoint
//							{
//								endPoint = (*j).second;
//								break;
//							}
//						}
//
//						break;
//					}
//
//				}
//
//				CSpawnerObject* tempTrap = new CSpawnerObject("Spinning Blade Spawner");
//				tempTrap->SetAttributes(startPoint, rotationSpeed, startPoint, endPoint, (*iter).first);
//				m_cpObjects.push_back(tempTrap);
//
//				delete (*iter).second;
//			}
//
//			//populate loaddata struct for binary loading
//			for (unsigned int i = 0; i < m_cpObjects.size(); i++)
//			{
//				if (m_cpObjects[i]->GetTag() == "Spinning Blade Spawner")
//				{
//					for (unsigned int k = 0; k < m_vtObjectLoadData.size(); k++)
//					{
//						if (m_vtObjectLoadData[k].m_eMeshType == SPINNING_BLADE_OBJECT)
//						{
//							unsigned int ID = ((CSpawnerObject*)(m_cpObjects[i]))->GetID();
//							XMFLOAT3 StartPos = ((CSpawnerObject*)(m_cpObjects[i]))->GetStartPoint();
//							XMFLOAT3 EndPos = ((CSpawnerObject*)(m_cpObjects[i]))->GetEndPoint();
//
//							m_vtObjectLoadData[k].m_umSBStartPointMap.insert(pair<unsigned int, XMFLOAT3>(ID, StartPos));
//							m_vtObjectLoadData[k].m_umSBEndPointMap.insert(pair<unsigned int, XMFLOAT3>(ID, EndPos));
//
//						}
//					}
//				}
//			}
//
//
//#pragma endregion
//		}
//
//		//save a binary version
//		PrefabBinaryMeshSave(m_szFBXpath, m_cpMeshes);
//
//		io_settings->Destroy();
//		root_node->Destroy();
//		scene->Destroy();
//		manager->Destroy();
//	}
//
//#pragma endregion
//
}
//
CLevelPrefab::~CLevelPrefab()
{

	for (unsigned int i = 0; i < m_cpObjects.size(); i++)
	{
		if (m_cpObjects[i] != nullptr)
		{
			delete m_cpObjects[i];
			m_cpObjects[i] = nullptr;
		}
	}

	//delete m_cpObjectManager;

	for (std::unordered_map<int, IObject*>::iterator i = m_umWallTorchIDs.begin(); i != m_umWallTorchIDs.end(); i++)
	{
		delete (*i).second;
		(*i).second = nullptr;
	}

	for (unsigned int i = 0; i < m_cpMeshes.size(); i++)
	{
		if (m_cpMeshes[i] != nullptr)
		{
			delete m_cpMeshes[i];
			m_cpMeshes[i] = nullptr;
		}
	}

}

void CLevelPrefab::Update()
{

}

void CLevelPrefab::SwitchPrefabTexture(const wchar_t* texture_path)
{
	for (unsigned int i = 0; i < m_cpObjects.size(); i++)
	{
		m_cpObjects[i]->GetRenderMesh()->SetTexture(texture_path);
	}
}

CLevelPrefab* CLevelPrefab::CopyPrefab()
{
	return new CLevelPrefab(this->m_szFBXpath, this->m_nPrefabType, this->m_cpObjectManager);
}

void CLevelPrefab::PrefabBinaryMeshLoad(string _filePath, std::vector<CMesh*>& _outMesh)
{
	ifstream fin;
	fin.open(_filePath.c_str(), std::ios_base::binary);

	if (fin.is_open())
	{
		//read in the load data struct info
		unsigned int loadDataSize;
		fin.read((char*)&loadDataSize, sizeof(unsigned int));
		for (unsigned int i = 0; i < loadDataSize; i++)
		{
			TObjectMeshData loadData;

			//get mesh type
			unsigned int type;
			fin.read((char*)&type, sizeof(unsigned int));
			loadData.m_eMeshType = (eObjectMeshType)type;

			//get mesh count
			unsigned int numMeshes;
			fin.read((char*)&numMeshes, sizeof(unsigned int));
			loadData.m_unMeshCount = numMeshes;

			//get full naems map size
			unsigned int fullNameMapSize;
			fin.read((char*)&fullNameMapSize, sizeof(unsigned int));

			//get local matrix map size
			unsigned int matrixMapSize;
			fin.read((char*)&matrixMapSize, sizeof(unsigned int));

			//full name map
			for (unsigned int k = 0; k < fullNameMapSize; k++)
			{
				//the mesh ID
				unsigned int ID;
				fin.read((char*)&ID, sizeof(unsigned int));

				//get mesh full name length
				unsigned int fullNameSize;
				fin.read((char*)&fullNameSize, sizeof(unsigned int));

				//get mesh full name
				char* meshFullName = new char[fullNameSize + 1];
				fin.read(meshFullName, fullNameSize);
				meshFullName[fullNameSize] = '\0';
				loadData.m_umMeshFullNameMap.insert(pair<unsigned int, string>(ID, meshFullName));
				delete[] meshFullName;
				meshFullName = nullptr;

			}

			//matrix map
			for (unsigned int k = 0; k < matrixMapSize; k++)
			{
				//the mesh ID
				unsigned int ID;
				fin.read((char*)&ID, sizeof(unsigned int));

				//get mesh matrix
				XMFLOAT4X4 tempMatrix;
				for (unsigned int x = 0; x < 4; x++)
				{
					for (unsigned int y = 0; y < 4; y++)
					{
						float temp;
						fin.read((char*)&temp, sizeof(float));
						tempMatrix.m[x][y] = temp;
					}
				}

				loadData.m_umMeshMatrixMap.insert(pair<unsigned int, XMFLOAT4X4>(ID, tempMatrix));
			}

			if (loadData.m_eMeshType == SPINNING_BLADE_OBJECT)
			{
				//size of start point map
				unsigned int StartPointMapSize;
				fin.read((char*)&StartPointMapSize, sizeof(unsigned int));

				//size of end point map
				unsigned int EndPointMapSize;
				fin.read((char*)&EndPointMapSize, sizeof(unsigned int));

				//map of spinning blade start points
				for (unsigned int iter = 0; iter < StartPointMapSize; iter++)
				{
					unsigned int wayPointID;
					fin.read((char*)&wayPointID, sizeof(unsigned int));

					XMFLOAT3 startPos;
					fin.read((char*)&startPos.x, sizeof(float));
					fin.read((char*)&startPos.y, sizeof(float));
					fin.read((char*)&startPos.z, sizeof(float));

					loadData.m_umSBStartPointMap.insert(pair<unsigned int, XMFLOAT3>(wayPointID, startPos));
				}

				//map of spinning balde end points
				for (unsigned int iter = 0; iter < EndPointMapSize; iter++)
				{
					unsigned int wayPointID;
					fin.read((char*)&wayPointID, sizeof(unsigned int));

					XMFLOAT3 endPos;
					fin.read((char*)&endPos.x, sizeof(float));
					fin.read((char*)&endPos.y, sizeof(float));
					fin.read((char*)&endPos.z, sizeof(float));

					loadData.m_umSBEndPointMap.insert(pair<unsigned int, XMFLOAT3>(wayPointID, endPos));
				}
			}

			m_vtObjectLoadData.push_back(loadData);
		}

		// Number of Meshes - unsigned int
		unsigned int numMeshes;
		fin.read((char*)&numMeshes, sizeof(unsigned int));
		_outMesh.resize(numMeshes);

		for (unsigned int mesh = 0; mesh < numMeshes; mesh++)
		{
			CMesh* tempMesh = new CMesh;

			// Size of name - unsigned int
			unsigned int meshNameSize;
			fin.read((char*)&meshNameSize, sizeof(unsigned int));

			// Name of mesh - string
			char* meshName = new char[meshNameSize + 1];

			fin.read(meshName, meshNameSize);
			meshName[4] = '\0';
			tempMesh->GetName() = meshName;
			delete[] meshName;
			meshName = nullptr;


			// Number of vertices - unsigned int
			unsigned int numVertices;
			fin.read((char*)&numVertices, sizeof(unsigned int));
			tempMesh->GetVertices().resize(numVertices);

			// Number of indices - unsigned int
			unsigned int numIndices;
			fin.read((char*)&numIndices, sizeof(unsigned int));
			tempMesh->GetIndices().resize(numIndices);


			// TVertex information - vector<TVertex>
			vector<TVertex> tVerts;
			for (unsigned int vert = 0; vert < numVertices; vert++)
			{
				TVertex vertex;
				fin.read((char*)&vertex, sizeof(TVertex));
				tempMesh->GetVertices()[vert] = vertex;
			}

			// Indices - vector<unsigned int>
			vector<unsigned int> indices;
			for (unsigned int index = 0; index < numIndices; index++)
			{
				unsigned int ind;
				fin.read((char*)&ind, sizeof(unsigned int));
				tempMesh->GetIndices()[index] = ind;
			}


			XMFLOAT4X4 tempMatrix;
			for (unsigned int i = 0; i < 4; i++)
			{
				for (unsigned int k = 0; k < 4; k++)
				{
					float temp;
					fin.read((char*)&temp, sizeof(float));
					tempMatrix.m[i][k] = temp;
				}
			}
			tempMesh->GetLocalMatrix() = tempMatrix;

			_outMesh[mesh] = tempMesh;
			CAssetManager::GetInstance()->AddPrefabMesh(tempMesh->GetName(), *tempMesh);
		}
	}

	fin.close();

	BinaryLoadPrefab();
}

void CLevelPrefab::PrefabBinaryMeshSave(std::string _filePath, std::vector<CMesh*> _saveMesh)
{
	//ofstream fout;
	//fout.open(_filePath.c_str(), std::ios_base::binary);

	//if (fout.is_open())
	//{
	//	//save all the load data
	//	unsigned int loadDataSize = m_vtObjectLoadData.size();
	//	fout.write((char*)&loadDataSize, sizeof(unsigned int));
	//	for (unsigned int i = 0; i < loadDataSize; i++)
	//	{
	//		//mesh type
	//		unsigned int type = m_vtObjectLoadData[i].m_eMeshType;
	//		fout.write((char*)&type, sizeof(unsigned int));

	//		//mesh count
	//		unsigned int numMeshes = m_vtObjectLoadData[i].m_unMeshCount;
	//		fout.write((char*)&numMeshes, sizeof(unsigned int));

	//		//size of full names map
	//		unsigned int fullNameSize = m_vtObjectLoadData[i].m_umMeshFullNameMap.size();
	//		fout.write((char*)&fullNameSize, sizeof(unsigned int));

	//		//size of world matrix map
	//		unsigned int matrixMapSize = m_vtObjectLoadData[i].m_umMeshMatrixMap.size();
	//		fout.write((char*)&matrixMapSize, sizeof(unsigned int));

	//		//map of full names
	//		for (auto iter = m_vtObjectLoadData[i].m_umMeshFullNameMap.begin(); iter != m_vtObjectLoadData[i].m_umMeshFullNameMap.end(); iter++)
	//		{
	//			//the mesh ID
	//			unsigned int ID = (*iter).first;
	//			fout.write((char*)&ID, sizeof(unsigned int));

	//			//the name length
	//			unsigned int nameLength = (*iter).second.length();
	//			fout.write((char*)&nameLength, sizeof(unsigned int));

	//			//the name
	//			const char* fullName = (*iter).second.c_str();
	//			fout.write((char*)fullName, (*iter).second.size());
	//		}

	//		//map of local matrix 
	//		for (auto iter = m_vtObjectLoadData[i].m_umMeshMatrixMap.begin(); iter != m_vtObjectLoadData[i].m_umMeshMatrixMap.end(); iter++)
	//		{
	//			//the mesh matrix ID
	//			unsigned int ID = (*iter).first;
	//			fout.write((char*)&ID, sizeof(unsigned int));

	//			//the matrix
	//			XMFLOAT4X4 tempMatrix = (*iter).second;
	//			for (unsigned int x = 0; x < 4; x++)
	//			{
	//				for (unsigned int y = 0; y < 4; y++)
	//				{
	//					float temp = tempMatrix.m[x][y];
	//					fout.write((char*)&temp, sizeof(float));
	//				}
	//			}
	//		}

	//		if (m_vtObjectLoadData[i].m_eMeshType == SPINNING_BLADE_OBJECT)
	//		{
	//			//size of start point map
	//			unsigned int StartPointMapSize = m_vtObjectLoadData[i].m_umSBStartPointMap.size();
	//			fout.write((char*)&StartPointMapSize, sizeof(unsigned int));

	//			//size of end point map
	//			unsigned int EndPointMapSize = m_vtObjectLoadData[i].m_umSBEndPointMap.size();
	//			fout.write((char*)&EndPointMapSize, sizeof(unsigned int));

	//			//map of spinning blade start points
	//			for (auto iter = m_vtObjectLoadData[i].m_umSBStartPointMap.begin(); iter != m_vtObjectLoadData[i].m_umSBStartPointMap.end(); iter++)
	//			{
	//				unsigned int wayPointID = (*iter).first;
	//				fout.write((char*)&wayPointID, sizeof(unsigned int));

	//				XMFLOAT3 startPos = (*iter).second;
	//				fout.write((char*)&startPos.x, sizeof(float));
	//				fout.write((char*)&startPos.y, sizeof(float));
	//				fout.write((char*)&startPos.z, sizeof(float));
	//			}

	//			//map of spinning balde end points
	//			for (auto iter = m_vtObjectLoadData[i].m_umSBEndPointMap.begin(); iter != m_vtObjectLoadData[i].m_umSBEndPointMap.end(); iter++)
	//			{
	//				unsigned int wayPointID = (*iter).first;
	//				fout.write((char*)&wayPointID, sizeof(unsigned int));

	//				XMFLOAT3 endPos = (*iter).second;
	//				fout.write((char*)&endPos.x, sizeof(float));
	//				fout.write((char*)&endPos.y, sizeof(float));
	//				fout.write((char*)&endPos.z, sizeof(float));
	//			}
	//		}
	//	}

	//	// Number of Meshes
	//	unsigned int numMeshes = _saveMesh.size();
	//	fout.write((char*)&numMeshes, sizeof(unsigned int));

	//	for (unsigned int mesh = 0; mesh < numMeshes; mesh++)
	//	{
	//		const char * name = _saveMesh[mesh]->GetName().c_str();
	//		unsigned int nameSize = _saveMesh[mesh]->GetName().length();
	//		unsigned int numVerts = _saveMesh[mesh]->GetVertices().size();
	//		unsigned int numIndices = _saveMesh[mesh]->GetIndices().size();

	//		// Size of name - unsigned int
	//		fout.write((char*)&nameSize, sizeof(unsigned int));

	//		// Name of mesh - string
	//		fout.write((char*)name, _saveMesh[mesh]->GetName().size());//the name should be the mesh prefix 

	//		// Number of vertices - unsigned int
	//		fout.write((char*)&numVerts, sizeof(unsigned int));

	//		// Number of indices - unsigned int
	//		fout.write((char*)&numIndices, sizeof(unsigned int));

	//		// TVertex information - vector<TVertex>
	//		fout.write((char*)_saveMesh[mesh]->GetVertices()._Myfirst, sizeof(TVertex) * numVerts);

	//		// Indices - vector<unsigned int>
	//		fout.write((char*)_saveMesh[mesh]->GetIndices()._Myfirst, sizeof(unsigned int) * numIndices);

	//		// Local Matrix - float4x4
	//		for (unsigned int i = 0; i < 4; i++)
	//		{
	//			for (unsigned int k = 0; k < 4; k++)
	//			{
	//				float temp = _saveMesh[mesh]->GetLocalMatrix().m[i][k];
	//				fout.write((char*)&temp, sizeof(float));
	//			}
	//		}
	//	}
	//}

	//fout.close();

}

void CLevelPrefab::BinaryLoadPrefab()
{
//	//loop through the load data vector to see if this object type has been loaded
//	for (unsigned int i = 0; i < m_vtObjectLoadData.size(); i++)
//	{
//		switch (m_vtObjectLoadData[i].m_eMeshType)
//		{
//		case CLevelPrefab::WALL_OBJECT:
//		{
//#pragma region Walls
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				CWallObject* tempWall = new CWallObject("Wall");
//				if (m_nPrefabType == ZONE_PREFAB)//green walls
//				{
//					//TODO: Hryshko: add green wall textures 
//
//					tempWall->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("Wal_"), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(),
//						nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Wall.dds"));
//					tempWall->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//					tempWall->GetRenderMesh()->GetSpecular() = 0.5f;
//				}
//				else//regular walls
//				{
//					tempWall->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("Wal_"), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(),
//						nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Wall.dds"));
//					tempWall->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//					tempWall->GetRenderMesh()->GetSpecular() = 0.5f;
//				}
//
//				tempWall->GetRenderMesh()->GetTransformMatrix() = m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1];
//				tempWall->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(tempWall);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::FLOOR_OBJECT:
//		{
//#pragma region Floors
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				CFloorObject* tempFloor = new CFloorObject("Floor");
//				tempFloor->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("FLR_"), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(),
//					nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Ground.dds"));
//
//				tempFloor->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//				tempFloor->GetRenderMesh()->GetSpecular() = 1.5f;
//				tempFloor->GetRenderMesh()->GetTransformMatrix() = m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1];
//				tempFloor->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(tempFloor);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::GRASS_FLOOR_OBJECT:
//		{
//#pragma region Grass Floors
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				CFloorObject* tempFloor = new CFloorObject("SafeHavenFloor");
//				tempFloor->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("GFL_"), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(),
//					nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Ground_Grass.dds"));
//				tempFloor->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//
//				tempFloor->GetRenderMesh()->GetSpecular() = 1.5f;
//				tempFloor->GetRenderMesh()->GetTransformMatrix() = m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1];
//				tempFloor->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(tempFloor);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::RENDER_ONLY_OBJECT:
//		{
//#pragma region Render Only Walls
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				CWallObject* tempWall = new CWallObject("Render Only Wall");
//				if (m_nPrefabType == ZONE_PREFAB)//green walls
//				{
//					//TODO: Hryshko: add green wall textures 
//
//					tempWall->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("ROW_"), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(),
//						nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Wall.dds"));
//					tempWall->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//					tempWall->GetRenderMesh()->GetSpecular() = 0.5f;
//				}
//				else
//				{
//					tempWall->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("ROW_"), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(),
//						nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Wall.dds"));
//					tempWall->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//					tempWall->GetRenderMesh()->GetSpecular() = 0.5f;
//				}
//
//				tempWall->GetRenderMesh()->GetTransformMatrix() = m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1];
//				tempWall->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(tempWall);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::DOOR_OBJECT:
//		{
//#pragma region Doors
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				CDoorObject* tempExitDoor = new CDoorObject("Door");
//				tempExitDoor->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("Dor_"), GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Door.dds"));
//				tempExitDoor->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(tempExitDoor);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::EXIT_DOOR_OBJECT:
//		{
//#pragma region Exit Doors
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				CExitDoorObject* tempExitDoor = new CExitDoorObject("ExitDoor");
//				tempExitDoor->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("EXT_"), GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Door.dds"));
//				tempExitDoor->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(tempExitDoor);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::SMALL_PIT_OBJECT:
//		{
//#pragma region Small Pits
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				CPitObject* tempPit = new CPitObject("Pit");
//				if (m_nPrefabType == ZONE_PREFAB)//grass texture
//				{
//					//TODO: Hryshko: add green wall textures 
//
//					tempPit->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("PIT_"), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(),
//						nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Ground_Grass.dds"));
//				}
//				else
//				{
//					tempPit->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("PIT_"), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(),
//						nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Ground.dds"));
//				}
//
//				tempPit->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//				tempPit->GetRenderMesh()->GetSpecular() = 1.5f;
//				tempPit->GetRenderMesh()->GetTransformMatrix() = m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1];
//				tempPit->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(tempPit);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::BIG_PIT_OBJECT:
//		{
//#pragma region Big Pits
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				CPitObject* tempBigPit = new CPitObject("BigPit");
//				if (m_nPrefabType == ZONE_PREFAB)//grass texture
//				{
//					//TODO: Hryshko: add green wall textures 
//
//					tempBigPit->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("BPT_"), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(),
//						nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Ground_Grass.dds"));
//				}
//				else
//				{
//					tempBigPit->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("BPT_"), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(),
//						nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Ground.dds"));
//				}
//				tempBigPit->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//				tempBigPit->GetRenderMesh()->GetSpecular() = 1.5f;
//				tempBigPit->GetRenderMesh()->GetTransformMatrix() = m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1];
//				tempBigPit->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(tempBigPit);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::PIT_WALL_OBJECT:
//		{
//#pragma region Pit Walls
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				CPitWallObject* tempPitWall = new CPitWallObject("PitWall");
//				tempPitWall->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("PWL_"), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Ground.dds"));
//				tempPitWall->GetRenderMesh()->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds");
//				tempPitWall->GetRenderMesh()->GetSpecular() = 1.5f;
//				tempPitWall->GetRenderMesh()->GetTransformMatrix() = m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1];
//				tempPitWall->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(tempPitWall);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::INVISIBLE_WALL_OBJECT:
//		{
//#pragma region Invisible Walls
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				CWallObject* tempWall = new CWallObject("Wall");
//				tempWall->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("INV_"), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS()));
//				tempWall->GetRenderMesh()->GetTransformMatrix() = m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1];
//				tempWall->GetRenderMesh()->GetRender() = false;
//				tempWall->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(tempWall);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::CRACKED_WALL_OBJECT:
//		{
//#pragma region Cracked Walls
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				CCrackedWall* tempCrackedWall = new CCrackedWall("Cracked Wall");
//				tempCrackedWall->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("CWL_"), GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr));
//				tempCrackedWall->GetRenderMesh()->GetSpecular() = 0.5f;
//				tempCrackedWall->GetRenderMesh()->GetTransformMatrix() = m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1];
//				tempCrackedWall->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(tempCrackedWall);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::ENEMY_CAVE_WALL_OBJECT:
//		{
//#pragma region Enemy Cave Walls
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				CWallObject* tempCavedWall = new CWallObject("Enemy Cave Wall");
//				tempCavedWall->SetRenderMesh(new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("ECW_"), GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr));
//				tempCavedWall->GetRenderMesh()->GetSpecular() = 0.5f;
//				tempCavedWall->GetRenderMesh()->GetTransformMatrix() = m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1];
//				tempCavedWall->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(tempCavedWall);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::TORCH_OBJECT:
//		{
//#pragma region Torches
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				XMFLOAT3 tiltDirection = { 0, 0, 0 };
//				std::string tempName = m_vtObjectLoadData[i].m_umMeshFullNameMap[k + 1];
//
//				//get x value
//				int secondNum = (int)tempName[5] - 48;
//				if (tempName[4] == 'n')
//				{
//					tiltDirection.x = -((float)secondNum);
//				}
//				else
//				{
//					tiltDirection.x = (float)secondNum;
//				}
//
//				//get y value
//				secondNum = (int)tempName[8] - 48;
//				if (tempName[7] == 'n')
//				{
//					tiltDirection.y = -((float)secondNum);
//				}
//				else
//				{
//					tiltDirection.y = (float)secondNum;
//				}
//
//				//get z value
//				secondNum = (int)tempName[11] - 48;
//				if (tempName[10] == 'n')
//				{
//					tiltDirection.z = -((float)secondNum);
//				}
//				else
//				{
//					tiltDirection.z = (float)secondNum;
//				}
//
//
//				int torchID = -1;
//				if (tiltDirection.x > tiltDirection.z && tiltDirection.z == 0.0f)//pos X
//				{
//					torchID = 3;
//				}
//				else if (tiltDirection.x < tiltDirection.z && tiltDirection.z == 0.0f)//neg x
//				{
//					torchID = 1;
//				}
//				else if (tiltDirection.z > tiltDirection.x && tiltDirection.x == 0.0f)//pos Z
//				{
//					torchID = 0;
//				}
//				else if (tiltDirection.z < tiltDirection.x && tiltDirection.x == 0.0f)//neg z
//				{
//					torchID = 2;
//				}
//
//				CSpawnerObject* tempTorch = new CSpawnerObject("Torch Spawner");
//				XMFLOAT3 tempPos = { m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._41, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._42, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._43 };
//				tempTorch->SetAttributes(tempPos, tiltDirection);
//				tempTorch->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_umWallTorchIDs[torchID] = tempTorch;
//				//m_cpObjects.push_back(tempTorch);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::BUSH_OBJECT:
//		{
//#pragma region Bushes
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				CSpawnerObject* temp = new CSpawnerObject("Bush Spawner");
//				XMFLOAT3 tempPos = { m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._41, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._42, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._43 };
//				temp->SetAttributes(tempPos, { 0, 0, 0 });
//				temp->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(temp);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::TREE_OBJECT:
//		{
//#pragma region Trees
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				CSpawnerObject* temp = new CSpawnerObject("Tree Spawner");
//				XMFLOAT3 tempPos = { m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._41, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._42, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._43 };
//				temp->SetAttributes(tempPos, { 0, 0, 0 });
//				temp->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(temp);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::SPIKE_TRAP_OBJECT:
//		{
//#pragma region Spike Traps
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				CSpawnerObject* temp = new CSpawnerObject("Spike Trap Spawner");
//				XMFLOAT3 tempPos = { m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._41, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._42, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._43 };
//				temp->SetAttributes(tempPos, 0.0f, 1.0f);
//				temp->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(temp);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::ALT_SPIKE_TRAP_OBJECT:
//		{
//#pragma region Alt Spike Traps
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				std::string tempName = m_vtObjectLoadData[i].m_umMeshFullNameMap[k + 1];
//
//				int firstNum = (int)tempName[4] - 48;
//				int secondNum = (int)tempName[5] - 48;
//
//				float startTime = (float)firstNum + ((float)secondNum / 10.0f);
//
//				firstNum = (int)tempName[7] - 48;
//				secondNum = (int)tempName[8] - 48;
//
//				float offsetTime = (float)firstNum + ((float)secondNum / 10.0f);
//
//				CSpawnerObject* temp = new CSpawnerObject("Alt Spike Trap Spawner");
//				XMFLOAT3 tempPos = { m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._41, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._42, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._43 };
//				temp->SetAttributes(tempPos, startTime, offsetTime);
//				temp->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(temp);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::SPINNING_BLADE_OBJECT:
//		{
//#pragma region Spinning Blade Traps
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				std::string tempName = m_vtObjectLoadData[i].m_umMeshFullNameMap[k + 1];
//				int firstNum = (int)tempName[4] - 48;
//				int secondNum = (int)tempName[5] - 48;
//
//				float rotationSpeed = (float)firstNum + ((float)secondNum / 10.0f);
//
//				int ID = (int)tempName[7] - 48;
//
//				if (ID == 0)//not moving
//				{
//					CSpawnerObject* tempTrap = new CSpawnerObject("Spinning Blade Spawner");
//					XMFLOAT3 tempPos = { m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._41, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._42, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._43 };
//					tempTrap->SetAttributes(tempPos, rotationSpeed, { 0, 0, 0 }, { 0, 0, 0 }, 0);
//					m_cpObjects.push_back(tempTrap);
//				}
//				else//moving
//				{
//					CSpawnerObject* tempTrap = new CSpawnerObject("Spinning Blade Spawner");
//					XMFLOAT3 tempPos = { m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._41, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._42, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._43 };
//					tempTrap->SetAttributes(tempPos, rotationSpeed, m_vtObjectLoadData[i].m_umSBStartPointMap[ID], m_vtObjectLoadData[i].m_umSBEndPointMap[ID], ID);
//					m_cpObjects.push_back(tempTrap);
//				}
//
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::FIRE_TRAP_OBJECT:
//		{
//#pragma region Fire Traps
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				XMFLOAT3 fireDirection = { 0, 0, 0 };
//				std::string tempName = m_vtObjectLoadData[i].m_umMeshFullNameMap[k + 1];
//
//				int secondNum = (int)tempName[5] - 48;
//				if (tempName[4] == 'n')
//				{
//					fireDirection.x = -((float)secondNum);
//				}
//				else
//				{
//					fireDirection.x = (float)secondNum;
//				}
//
//				secondNum = (int)tempName[8] - 48;
//				if (tempName[7] == 'n')
//				{
//					fireDirection.y = -((float)secondNum);
//				}
//				else
//				{
//					fireDirection.y = (float)secondNum;
//				}
//
//				secondNum = (int)tempName[11] - 48;
//				if (tempName[10] == 'n')
//				{
//					fireDirection.z = -((float)secondNum);
//				}
//				else
//				{
//					fireDirection.z = (float)secondNum;
//				}
//
//				CSpawnerObject* temp = new CSpawnerObject("Fire Trap Spawner");
//				XMFLOAT3 tempPos = { m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._41, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._42, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._43 };
//				temp->SetAttributes(tempPos, fireDirection);
//				temp->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(temp);
//			}
//
//#pragma endregion
//		}
//		break;
//		case CLevelPrefab::CHEST_OBJECT:
//		{
//#pragma region Chests
//
//			for (unsigned int k = 0; k < m_vtObjectLoadData[i].m_unMeshCount; k++)
//			{
//				XMFLOAT3 faceDirection = { 0, 0, 0 };
//				std::string tempName = m_vtObjectLoadData[i].m_umMeshFullNameMap[k + 1];
//
//				int secondNum = (int)tempName[5] - 48;
//				if (tempName[4] == 'n')
//				{
//					faceDirection.x = -((float)secondNum);
//				}
//				else
//				{
//					faceDirection.x = (float)secondNum;
//				}
//
//				secondNum = (int)tempName[8] - 48;
//				if (tempName[7] == 'n')
//				{
//					faceDirection.y = -((float)secondNum);
//				}
//				else
//				{
//					faceDirection.y = (float)secondNum;
//				}
//
//				secondNum = (int)tempName[11] - 48;
//				if (tempName[10] == 'n')
//				{
//					faceDirection.z = -((float)secondNum);
//				}
//				else
//				{
//					faceDirection.z = (float)secondNum;
//				}
//
//				CSpawnerObject* temp = new CSpawnerObject("Chest Spawner");
//				XMFLOAT3 tempPos = { m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._41, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._42, m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]._43 };
//				temp->SetAttributes(tempPos, faceDirection);
//				temp->SetWorldMatrix(m_vtObjectLoadData[i].m_umMeshMatrixMap[k + 1]);
//				m_cpObjects.push_back(temp);
//			}
//
//#pragma endregion
//		}
//		break;
//		default:
//			break;
//		}
//	}
}



/*	ACCESSORS	*/

std::vector<CMesh*>& CLevelPrefab::GetMeshes()
{
	return m_cpMeshes;
}

std::vector<IObject*>& CLevelPrefab::GetObjects()
{
	return m_cpObjects;
}

unsigned int CLevelPrefab::GetPrefabType()
{
	return m_nPrefabType;
}

XMFLOAT4X4 CLevelPrefab::GetWorldMatrix()
{
	return m_mWorlMatrix;
}

void CLevelPrefab::SetWorldMatrix(XMFLOAT4X4 worldMatrix)
{
	m_mWorlMatrix = worldMatrix;

	for (unsigned int i = 0; i < m_cpObjects.size(); i++)
	{
		XMMATRIX newMatrix;
		XMMATRIX tempPrefabWorld = XMLoadFloat4x4(&worldMatrix);
		XMMATRIX tempObjectWorld = XMLoadFloat4x4(&m_cpObjects[i]->GetWorldMatrix());
		if (m_cpObjects[i]->GetTag() == "Spike Trap Spawner")
		{
			tempObjectWorld.r[0].m128_f32[0] = 1.0f;
			tempObjectWorld.r[1].m128_f32[1] = 1.0f;
			tempObjectWorld.r[2].m128_f32[2] = 1.0f;

		}
		else if (m_cpObjects[i]->GetTag() == "Alt Spike Trap Spawner")
		{
			tempObjectWorld.r[0].m128_f32[0] = 1.0f;
			tempObjectWorld.r[1].m128_f32[1] = 1.0f;
			tempObjectWorld.r[2].m128_f32[2] = 1.0f;
		}
		else if (m_cpObjects[i]->GetTag() == "Fire Trap Spawner")
		{
			tempObjectWorld.r[0].m128_f32[0] = 1.0f;
			tempObjectWorld.r[1].m128_f32[1] = 1.0f;
			tempObjectWorld.r[2].m128_f32[2] = 1.0f;
		}
		
			newMatrix = tempObjectWorld * tempPrefabWorld;


		XMFLOAT4X4 newWorld;
		XMStoreFloat4x4(&newWorld, newMatrix);
		if (m_cpObjects[i]->GetTag() == "SpinningBlade")
		{
			//adjust waypoints
			CSpinningBlade* temp = ((CSpinningBlade*)(m_cpObjects[i]));
			XMFLOAT3& pos = temp->GetStartPoint();
			pos.x += worldMatrix._41;
			pos.y += worldMatrix._42;
			pos.z += worldMatrix._43;

			XMFLOAT3& pos2 = temp->GetEndPoint();
			pos2.x += worldMatrix._41;
			pos2.y += worldMatrix._42;
			pos2.z += worldMatrix._43;

			temp->GetTargetPoint() = pos2;

		}

		m_cpObjects[i]->SetWorldMatrix(newWorld);
	}
	for (auto iter = m_umWallTorchIDs.begin(); iter != m_umWallTorchIDs.end(); iter++)
	{
		
		XMMATRIX tempPrefabWorld = XMLoadFloat4x4(&worldMatrix);
		XMMATRIX tempObjectWorld = XMMatrixTranslation(iter->second->GetWorldMatrix()._41, iter->second->GetWorldMatrix()._42, iter->second->GetWorldMatrix()._43);
		XMMATRIX newMatrix = tempObjectWorld * tempPrefabWorld;

	
		XMFLOAT4X4 newWorld;
		XMStoreFloat4x4(&newWorld, newMatrix);
		XMFLOAT3 newPos = XMFLOAT3(newWorld._41, newWorld._42, newWorld._43);

		
		((CSpawnerObject*)iter->second)->SetAttributes(newPos, ((CSpawnerObject*)iter->second)->GetSpawnDirection());

		
		iter->second->SetWorldMatrix(newWorld);
	}
}

IObject* CLevelPrefab::GetTorch(unsigned int ID)
{
	return m_umWallTorchIDs[ID];
}

void CLevelPrefab::RemoveTorch(int index)
{
	delete m_umWallTorchIDs[index];
	m_umWallTorchIDs.erase(index);
}

std::vector<CLevelPrefab::TObjectMeshData>& CLevelPrefab::GetPrefabLoadData()
{
	return m_vtObjectLoadData;
}
