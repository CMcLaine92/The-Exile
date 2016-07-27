#include "AssetManager.h"

#include <algorithm>
#include <fbxsdk/fileio/fbxiosettings.h>
#include "../Renderer/DDSTextureLoader.h"
#include <fstream>
#include "../../Game/Source/Base/LevelPrefab.h"
#include "../Object Manager/ObjectManager.h"
#include "../Animation/Rig.h"

CAssetManager * CAssetManager::m_cpInstance = nullptr;


bool CAssetManager::LoadMeshFbx(const std::string& file_name, FullMesh& meshes)
{
	return 0;
}

bool CAssetManager::LoadAnimMesh(FbxNode* node, CMesh& mesh, std::vector< FbxNode* >& fbx_joints)
{
	// TODO
	FbxNodeAttribute* attribute = node->GetNodeAttribute();
	if (attribute != NULL)
	{
		if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			LoadMesh((FbxMesh*)attribute, mesh);
			LoadSkin((FbxMesh*)attribute, mesh, fbx_joints);

			mesh.ConvertVertices(true);
		}
		else if (attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			LoadSkin((FbxMesh*)attribute, mesh, fbx_joints);

		}
	}
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		LoadAnimMesh(node->GetChild(i), mesh, fbx_joints);
	}
	return true;
}
bool CAssetManager::LoadAnimMesh(const std::string& file_name)
{
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
		return false;
	}

	manager->SetIOSettings(io_settings);

	// Create importer
	FbxImporter* importer = FbxImporter::Create(manager, "");
	if (importer == 0)
	{
		return false;
	}

	// Initialize importer
	if (importer->Initialize(file_name.c_str(), -1, io_settings) == false)
	{
		std::string error;
		error += "FbxImporter::Initialize failed:  ";
		error += importer->GetStatus().GetErrorString();
		//MessageBox(NULL, (LPCWSTR)error.c_str(), "Error", MB_OK);
		return false;
	}

	// Create a scene
	FbxScene* scene = FbxScene::Create(manager, "myScene");
	if (scene == 0)
	{
		return false;
	}

	// Load the scene with contents from the file
	if (importer->Import(scene) == false)
	{
		return false;
	}

	// No longer need the importer
	importer->Destroy();

	// Our list of unique joints
	std::vector< FbxNode* > fbx_joints;

	// Traverse the scene
	CMesh mesh;
	FbxNode* root_node = scene->GetRootNode();
	if (LoadAnimMesh(root_node, mesh, fbx_joints) == false)
	{
		return false;
	}
	manager->Destroy();

	string prefabName = file_name;
	prefabName.erase(0, prefabName.find_last_of('/') + 1);
	prefabName.erase(prefabName.find_last_of('.'), prefabName.size());

	mesh.GetName() = prefabName;
	string path = "../Game/Assets/Art/3D/AnimModels/" + mesh.GetName() + ".animMesh";
	mesh.BinaryWrite(path.c_str());
}

bool CAssetManager::Load(const std::string& file_name, FullMesh& meshes, CAnimation& cAnimation)
{
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
		return false;
	}

	manager->SetIOSettings(io_settings);

	// Create importer
	FbxImporter* importer = FbxImporter::Create(manager, "");
	if (importer == 0)
	{
		return false;
	}

	// Initialize importer
	if (importer->Initialize(file_name.c_str(), -1, io_settings) == false)
	{
		std::string error;
		error += "FbxImporter::Initialize failed:  ";
		error += importer->GetStatus().GetErrorString();
		//MessageBox(NULL, (LPCWSTR)error.c_str(), "Error", MB_OK);
		return false;
	}

	// Create a scene
	FbxScene* scene = FbxScene::Create(manager, "myScene");
	if (scene == 0)
	{
		return false;
	}

	// Load the scene with contents from the file
	if (importer->Import(scene) == false)
	{
		return false;
	}

	// No longer need the importer
	importer->Destroy();

	// Our list of unique joints
	std::vector< FbxNode* > fbx_joints;

	// Traverse the scene
	FbxNode* root_node = scene->GetRootNode();
	if (ProcessNode(root_node, meshes, fbx_joints) == false)
	{
		return false;
	}

	// Get the number of Animation stacks
	int num_anim_stacks = scene->GetSrcObjectCount< FbxAnimStack >();

	FbxAnimStack* anim_stack;
	for (int i = 0; i < num_anim_stacks; ++i)
	{
		string name = file_name.substr(file_name.find_last_of('/') + 1);
		name.erase(name.length() - 4, name.length());
		// Get the current CAnimation stack
		anim_stack = scene->GetSrcObject< FbxAnimStack >(i);

		cAnimation.GetName() = name;

		// Get the number of CAnimation layers in the current Animation stack
		int num_anim_layers = anim_stack->GetMemberCount< FbxAnimLayer >();

		FbxAnimLayer* anim_layer;
		for (int j = 0; j < num_anim_layers; ++j)
		{
			anim_layer = anim_stack->GetMember< FbxAnimLayer >(j);

			LoadAnimation(anim_layer, cAnimation, meshes[0], fbx_joints);
		}
	}

	manager->Destroy();
	return true;
}
void CAssetManager::LoadAllPrefabs()
{
	vector<string> allPrefabFBX;
	vector<string> FbxToLoad;
	vector<string> allPrefabBin;


	FindFilesByPath("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/", ".fbx", allPrefabFBX);
	FindFilesByPath("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/", ".mesh", allPrefabBin);

	int currBins;
	int currFbx;

	for (currFbx = 0; currFbx < allPrefabFBX.size(); currFbx++)
	{
		for (currBins = 0; currBins < allPrefabBin.size(); currBins++)
		{
			string cmp1 = allPrefabBin[currBins];
			string cmp2 = allPrefabFBX[currFbx];
			cmp1.erase(cmp1.find_last_of('.'), cmp1.length());
			cmp2.erase(cmp2.find_last_of('.'), cmp2.length());


			if (cmp1 == cmp2)
				break;
		}
		if (currBins == allPrefabBin.size())
			FbxToLoad.push_back(allPrefabFBX[currFbx]);
	}

	for (size_t currPath = 0; currPath < FbxToLoad.size(); currPath++)
	{
		LoadPrefabFBX(FbxToLoad[currPath].c_str());
	}
	allPrefabBin.clear();

	FindFilesByPath("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/", ".mesh", allPrefabBin);
	for (size_t currPath = 0; currPath < allPrefabBin.size(); currPath++)
	{
		CMesh mesh;
		mesh.BinaryRead(allPrefabBin[currPath].c_str());

		XMStoreFloat4x4(&mesh.GetLocalMatrix(), XMMatrixIdentity());
		m_PrefabMeshes[mesh.GetName()] = mesh;
	}


}
void CAssetManager::LoadAnimMeshs()
{
	vector<string> allFbxPaths;
	vector<string> fbxPaths;

	vector<string> binPaths;

	FindFilesByPath("../Game/Assets/Art/3D/AnimModels/", ".fbx", allFbxPaths);
	FindFilesByPath("../Game/Assets/Art/3D/AnimModels/", ".animMesh", binPaths);
	int currFbx, currBins;
	for (currFbx = 0; currFbx < allFbxPaths.size(); currFbx++)
	{
		for (currBins = 0; currBins < binPaths.size(); currBins++)
		{
			string cmp1 = binPaths[currBins];
			string cmp2 = allFbxPaths[currFbx];
			cmp1.erase(cmp1.find_last_of('.'), cmp1.length());
			cmp2.erase(cmp2.find_last_of('.'), cmp2.length());


			if (cmp1 == cmp2)
				break;
		}
		if (currBins == binPaths.size())
			fbxPaths.push_back(allFbxPaths[currFbx]);
	}
	for (size_t currPath = 0; currPath < fbxPaths.size(); currPath++)
	{
		LoadAnimMesh(fbxPaths[currPath].c_str());
	}
	binPaths.clear();


	FindFilesByPath("../Game/Assets/Art/3D/AnimModels/", ".animMesh", binPaths);

}

void CAssetManager::LoadPrefabFBX(const char* filePath)
{
	// Get an FBX manager
	FbxManager* manager = FbxManager::Create();
	if (manager == 0)
	{
		return;
	}

	// Create IO settings
	FbxIOSettings* io_settings = FbxIOSettings::Create(manager, IOSROOT);
	if (io_settings == 0)
	{
		return;
	}

	manager->SetIOSettings(io_settings);

	// Create importer
	FbxImporter* importer = FbxImporter::Create(manager, "");
	if (importer == 0)
	{
		return;
	}

	// Initialize importer
	if (importer->Initialize(filePath, -1, io_settings) == false)
	{
		return;
	}

	// Create a scene
	FbxScene* scene = FbxScene::Create(manager, "PrefabScene");
	if (scene == 0)
	{
		return;
	}

	// Load the scene with contents from the file
	if (importer->Import(scene) == false)
	{
		return;
	}

	// No longer need the importer
	importer->Destroy();

#pragma endregion

	//traverse the scene based on the prefab type
	FbxNode* root_node = scene->GetRootNode();
	CMesh theMesh;
	string prefabName = filePath;
	prefabName.erase(0, prefabName.find_last_of('/') + 1);
	prefabName.erase(prefabName.find_last_of('.'), prefabName.size());
	
	theMesh.GetName() = prefabName;
	//fbx loader;
	FBXLoadPrefab(root_node, theMesh);
	string path = "../Game/Assets/Art/3D/Models/Environment/Level Prefabs/" + theMesh.GetName() + ".mesh";
	theMesh.BinaryWrite(path.c_str());
	manager->Destroy();

}



void CAssetManager::FBXLoadPrefab(FbxNode* node, CMesh& mesh)
{
	FbxNodeAttribute* attribute = node->GetNodeAttribute();
	
	if (attribute != NULL)
	{
		if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			
			//get name prefix for this object from the fbx node
			std::string name = node->GetName();

			int nDelimiter = name.find_last_of('_');
			if (nDelimiter != string::npos)
			{
				name.erase(nDelimiter, name.length());

				if (name == "AABB" || name == "Plane")
				{
					CMesh tMesh;
					LoadMesh((FbxMesh*)attribute, tMesh);
					tMesh.ConvertVertices();

					if (name == "AABB")
					{
						CMesh::TAABB collider;
						tMesh.BuildAABB(collider);
						mesh.GetAABBs().push_back(collider);
					}
					else if (name == "Plane")
					{
						CMesh::TPlane plane;
						tMesh.BuildPlane(plane);
						mesh.GetPlanes().push_back(plane);
					}
					
				}
				else
				{
					mesh.GetName() = node->GetName();
					LoadMesh((FbxMesh*)attribute, mesh);
					mesh.ConvertVertices();
				}
			}
			else
			{
				mesh.GetName() = name.c_str();
				LoadMesh((FbxMesh*)attribute, mesh);
				mesh.ConvertVertices();
				
			}

			
		}
	}
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		FBXLoadPrefab(node->GetChild(i), mesh);
	}
}
bool CAssetManager::LoadAnim(const char* path)
{
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
		return false;
	}

	manager->SetIOSettings(io_settings);

	// Create importer
	FbxImporter* importer = FbxImporter::Create(manager, "");
	if (importer == 0)
	{
		return false;
	}

	// Initialize importer
	if (importer->Initialize(path, -1, io_settings) == false)
	{
		std::string error;
		error += "FbxImporter::Initialize failed:  ";
		error += importer->GetStatus().GetErrorString();
		//MessageBox(NULL, (LPCWSTR)error.c_str(), "Error", MB_OK);
		return false;
	}

	// Create a scene
	FbxScene* scene = FbxScene::Create(manager, "myScene");
	if (scene == 0)
	{
		return false;
	}

	// Load the scene with contents from the file
	if (importer->Import(scene) == false)
	{
		return false;
	}

	// No longer need the importer
	importer->Destroy();

	// Our list of unique joints
	std::vector< FbxNode* > fbx_joints;
	CMesh mesh;
	CAnimation anim;
	// Traverse the scene
	FbxNode* root_node = scene->GetRootNode();
	if (LoadAnimMesh(root_node, mesh, fbx_joints) == false)
	{
		return false;
	}

	// Get the number of Animation stacks
	int num_anim_stacks = scene->GetSrcObjectCount< FbxAnimStack >();

	FbxAnimStack* anim_stack;
	string file_name = path;
	for (int i = 0; i < num_anim_stacks; ++i)
	{
		string name = file_name.substr(file_name.find_last_of('/') + 1);
		name.erase(name.length() - 4, name.length());
		// Get the current CAnimation stack
		anim_stack = scene->GetSrcObject< FbxAnimStack >(i);

		anim.GetName() = name;

		// Get the number of CAnimation layers in the current Animation stack
		int num_anim_layers = anim_stack->GetMemberCount< FbxAnimLayer >();

		FbxAnimLayer* anim_layer;
		for (int j = 0; j < num_anim_layers; ++j)
		{
			anim_layer = anim_stack->GetMember< FbxAnimLayer >(j);

			LoadAnimation(anim_layer, anim, mesh, fbx_joints);
		}
	}

	manager->Destroy();
	return true;
}

void CAssetManager::LoadTextures()
{
	vector<string> texturePaths;
	FindFilesByPath("../Game/Assets/Art/2D/Textures/", ".dds", texturePaths);
	FindFilesByPath("../Game/Assets/Art/2D/HUD/", ".dds", texturePaths);
	FindFilesByPath("../Game/Assets/Art/2D/Inventory/", ".dds", texturePaths);
	FindFilesByPath("../Game/Assets/Art/2D/Menu/", ".dds", texturePaths);
	FindFilesByPath("../Game/Assets/Art/2D/Normal Maps/", ".dds", texturePaths);
	//FindFilesByPath("../Game/Assets/Art/2D/FX/", ".dds", texturePaths);

	for (size_t currTex = 0; currTex < texturePaths.size(); currTex++)
	{
		wstring wPath(texturePaths[currTex].begin(), texturePaths[currTex].end());

		if (GRAPHICS->GetTextureList().find(wPath) == GRAPHICS->GetTextureList().end())
		{
			HRESULT hr = CreateDDSTextureFromFile(GRAPHICS->GetDevice(), wPath.c_str(), NULL, &GRAPHICS->GetTextureList()[wPath]);
			if (hr != S_OK)
			{
				string msg = "Failed to Load Texture: ";
				msg += texturePaths[currTex].c_str();
				msg += "!\n";
				OUTPUT(msg);
				continue;
			}
		}
		else
			continue;
		

	}



}
void CAssetManager::FindFilesByPath(std::string folderPath, std::string fileExtension, std::vector<std::string>& filePaths)
{
	std::string search_path = folderPath + "*" + fileExtension;


	WIN32_FIND_DATAA fd;
	HANDLE hFind = ::FindFirstFileA(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				std::string path = folderPath + fd.cFileName;
				filePaths.push_back(path);
			}
		} while (::FindNextFileA(hFind, &fd));
		::FindClose(hFind);
	}
}
bool CAssetManager::ProcessNode(FbxNode* node, FullMesh& meshes, std::vector< FbxNode* >& fbx_joints)
{
	// TODO
	FbxNodeAttribute* attribute = node->GetNodeAttribute();
	if (attribute != NULL)
	{
		if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			CMesh  tempMesh;
			tempMesh.GetName() = node->GetName();
			LoadMesh((FbxMesh*)attribute, tempMesh);
			LoadSkin((FbxMesh*)attribute, tempMesh, fbx_joints);
			meshes.push_back(tempMesh);
		}
	}
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		ProcessNode(node->GetChild(i), meshes, fbx_joints);
	}
	return true;
}

bool CAssetManager::LoadMesh(FbxMesh* fbx_mesh, CMesh& mesh)
{
	int polygon_count;
	int polygon;
	int polygon_vertex_count;
	int polygon_vertex;
	int uv_element_count;
	int uv_element;
	int normal_element_count;
	int normal_element;
	int vertex_id;
	string name;



	polygon_count = fbx_mesh->GetPolygonCount();
	FbxVector4* control_points = fbx_mesh->GetControlPoints();

	int control_point_count = fbx_mesh->GetControlPointsCount();

	vertex_id = 0;

	for (polygon = 0; polygon < polygon_count; ++polygon)
	{
		polygon_vertex_count = fbx_mesh->GetPolygonSize(polygon);
		for (polygon_vertex = 0; polygon_vertex < polygon_vertex_count; ++polygon_vertex)
		{
			CMesh::UniqueVertex unique_vertex;

			int control_point_index = fbx_mesh->GetPolygonVertex(polygon, polygon_vertex);

			unique_vertex.control_point_index = control_point_index;
			unique_vertex.position.x = (float)control_points[control_point_index].mData[0];
			unique_vertex.position.y = (float)control_points[control_point_index].mData[1];
			unique_vertex.position.z = (float)control_points[control_point_index].mData[2];

			// Get texture coords
			uv_element_count = fbx_mesh->GetElementUVCount();
			for (uv_element = 0; uv_element < uv_element_count; ++uv_element)
			{
				FbxGeometryElementUV* geometry_element_uv = fbx_mesh->GetElementUV(uv_element);

				FbxLayerElement::EMappingMode mapping_mode = geometry_element_uv->GetMappingMode();
				FbxLayerElement::EReferenceMode reference_mode = geometry_element_uv->GetReferenceMode();

				int direct_index = -1;

				// eByControlPoint:  One control point has one uv
				// eByPolygonVertex:  One control point can have multiple uv's
				if (mapping_mode == FbxGeometryElement::eByControlPoint)
				{
					if (reference_mode == FbxGeometryElement::eDirect)
					{
						direct_index = control_point_index;
					}
					else if (reference_mode == FbxGeometryElement::eIndexToDirect)
					{
						direct_index = geometry_element_uv->GetIndexArray().GetAt(control_point_index);
					}
				}
				else if (mapping_mode == FbxGeometryElement::eByPolygonVertex)
				{
					if (reference_mode == FbxGeometryElement::eDirect ||
						reference_mode == FbxGeometryElement::eIndexToDirect)
					{
						direct_index = fbx_mesh->GetTextureUVIndex(polygon, polygon_vertex);
					}
				}

				if (direct_index != -1)
				{
					FbxVector2 uv = geometry_element_uv->GetDirectArray().GetAt(direct_index);

					unique_vertex.tex_coord.u = (float)uv.mData[0];
					unique_vertex.tex_coord.v = 1 - (float)uv.mData[1];
				}
			}

			// Get normals
			//normal_element_count = fbx_mesh->GetElementNormalCount();
			//for (normal_element = 0; normal_element < normal_element_count; ++normal_element)
			//{
			//	FbxGeometryElementNormal* geometry_element_normal = fbx_mesh->GetElementNormal(normal_element);
			//
			//	FbxLayerElement::EMappingMode mapping_mode = geometry_element_normal->GetMappingMode();
			//	FbxLayerElement::EReferenceMode reference_mode = geometry_element_normal->GetReferenceMode();
			//
			//	int direct_index = -1;
			//
			//	if (mapping_mode == FbxGeometryElement::eByPolygonVertex)
			//	{
			//		if (reference_mode == FbxGeometryElement::eDirect)
			//		{
			//			direct_index = vertex_id;
			//		}
			//		else if (reference_mode == FbxGeometryElement::eIndexToDirect)
			//		{
			//			direct_index = geometry_element_normal->GetIndexArray().GetAt(vertex_id);
			//		}
			//	}
			//
			//	if (direct_index != -1)
			//	{
			//		FbxVector4 norm = geometry_element_normal->GetDirectArray().GetAt(direct_index);
			//
			//		unique_vertex.normal.x = (float)norm.mData[0];
			//		unique_vertex.normal.y = (float)norm.mData[1];
			//		unique_vertex.normal.z = (float)norm.mData[2];
			//	}
			//}

#pragma region Loading Normals

			// Get normals
			normal_element_count = fbx_mesh->GetElementNormalCount();
			for (normal_element = 0; normal_element < normal_element_count; ++normal_element)
			{
				FbxGeometryElementNormal* geometry_element_normal = fbx_mesh->GetElementNormal(normal_element);

				FbxLayerElement::EMappingMode mapping_mode = geometry_element_normal->GetMappingMode();
				FbxLayerElement::EReferenceMode reference_mode = geometry_element_normal->GetReferenceMode();

				int direct_index = -1;

				if (mapping_mode == FbxGeometryElement::eByControlPoint)
				{
					if (reference_mode == FbxGeometryElement::eDirect)
					{
						FbxVector4 norm = geometry_element_normal->GetDirectArray().GetAt(control_point_index);

						unique_vertex.normal.x = (float)norm[0];
						unique_vertex.normal.y = (float)norm[1];
						unique_vertex.normal.z = (float)norm[2];
					}
					else if (reference_mode == FbxGeometryElement::eIndexToDirect)
					{
						int id = geometry_element_normal->GetIndexArray().GetAt(control_point_index);
						FbxVector4 norm = geometry_element_normal->GetDirectArray().GetAt(id);

						unique_vertex.normal.x = (float)norm[0];
						unique_vertex.normal.y = (float)norm[1];
						unique_vertex.normal.z = (float)norm[2];
					}
				}
				else if (mapping_mode == FbxGeometryElement::eByPolygonVertex)
				{
					if (reference_mode == FbxGeometryElement::eDirect)
					{
						FbxVector4 norm = geometry_element_normal->GetDirectArray().GetAt(vertex_id);

						unique_vertex.normal.x = (float)norm[0];
						unique_vertex.normal.y = (float)norm[1];
						unique_vertex.normal.z = (float)norm[2];
					}
					else if (reference_mode == FbxGeometryElement::eIndexToDirect)
					{
						int id = geometry_element_normal->GetIndexArray().GetAt(vertex_id);
						FbxVector4 norm = geometry_element_normal->GetDirectArray().GetAt(id);

						unique_vertex.normal.x = (float)norm[0];
						unique_vertex.normal.y = (float)norm[1];
						unique_vertex.normal.z = (float)norm[2];
					}
				}
			}

#pragma endregion

			std::vector< CMesh::UniqueVertex >& unique_vertices = mesh.GetUniqueVertices();

			size_t size = unique_vertices.size();
			size_t i;
			for (i = 0; i < size; ++i)
			{
				if (unique_vertex == unique_vertices[i])
				{
					break;
				}
			}

			if (i == size)
			{
				unique_vertices.push_back(unique_vertex);
			}

			mesh.GetIndices().push_back(i);

			++vertex_id;
		}
	}

	return true;
}

bool CAssetManager::LoadSkin(FbxMesh* fbx_mesh, CMesh& mesh, std::vector< FbxNode* >& fbx_joints)
{
	// TODO
	for (int deformerIndex = 0; deformerIndex < fbx_mesh->GetDeformerCount(); deformerIndex++)
	{
		FbxDeformer* tempDef = fbx_mesh->GetDeformer(deformerIndex);
		if (tempDef->GetDeformerType() == FbxDeformer::eSkin)
		{
			FbxSkin* tempSkin = (FbxSkin*)tempDef;
			for (int clusterIndex = 0; clusterIndex < tempSkin->GetClusterCount(); clusterIndex++)
			{
				unsigned int joint_Index = -1;
				FbxCluster*  tempCluster = tempSkin->GetCluster(clusterIndex);
				LoadSkeletonNode(tempCluster->GetLink(), mesh, fbx_joints, joint_Index);
				FbxAMatrix transform, linkTransform;
				tempCluster->GetTransformMatrix(transform);
				tempCluster->GetTransformLinkMatrix(linkTransform);
				FbxAMatrix worldBindPoseMat = transform * linkTransform;
				XMFLOAT4X4 bindPose;
				for (unsigned int i = 0; i < 4; i++)
				{
					for (unsigned int j = 0; j < 4; j++)
					{
						//Check Later to Make sure matrix is set properly
						bindPose.m[i][j] = (float)worldBindPoseMat.Get(i, j);
					}
				}
				//Redundant check?
				if (mesh.GetJoints().size() > joint_Index)
				{
					mesh.GetJoints()[joint_Index].bind_pose_transform = bindPose;
					int* ControlPointIndexArray = tempCluster->GetControlPointIndices();
					double* ControlPointWeightArray = tempCluster->GetControlPointWeights();
					mesh.GetInfluences().resize(mesh.GetUniqueVertices().size());
					for (unsigned int vertIndex = 0; vertIndex < mesh.GetUniqueVertices().size(); vertIndex++)
					{
						for (int controlPoints = 0; controlPoints < tempCluster->GetControlPointIndicesCount(); controlPoints++)
						{
							if (mesh.GetUniqueVertices()[vertIndex].control_point_index == ControlPointIndexArray[controlPoints])
							{
								CMesh::JointInfluence tempInfluence;
								tempInfluence.joint_index = joint_Index;
								tempInfluence.weight = (float)ControlPointWeightArray[controlPoints];
								mesh.GetInfluences()[vertIndex].push_back(tempInfluence);
							}
						}
					}
				}
			}
		}
	}
	return true;
}

bool CAssetManager::LoadSkeletonNode(FbxNode* node, CMesh& mesh, std::vector< FbxNode* >& fbx_joints, unsigned int& index)
{
	// TODO
	bool unique = true;
	FbxNodeAttribute* attribute = node->GetNodeAttribute();
	if (attribute == nullptr)
		return false;
	if (attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		for (unsigned int jointIndex = 0; jointIndex < fbx_joints.size(); jointIndex++)
		{
			if (node == fbx_joints[jointIndex])
			{
				index = jointIndex;
				unique = false;
				break;
			}
		}
		if (unique)
		{
			const char* name = node->GetName();
			index = fbx_joints.size();
			fbx_joints.push_back(node);
			Joint tempJoint;
			tempJoint.name = name;

			string newName;
			bool found = false;
			int count = 0;
			for (unsigned int letter = 0; letter < tempJoint.name.size(); letter++)
			{
				if (found)
				{
					newName[count] = tempJoint.name.c_str()[letter];
					count++;
				}

				if (tempJoint.name[letter] == ':')
				{
					found = true;
					newName.resize(tempJoint.name.size() + 1 - letter);
				}

			}

			if (found)
				tempJoint.name = newName;

			mesh.GetJoints().push_back(tempJoint);
			for (int childIndex = 0; childIndex < node->GetChildCount(); childIndex++)
			{
				unsigned int childID;
				LoadSkeletonNode(node->GetChild(childIndex), mesh, fbx_joints, childID);
				mesh.GetJoints()[index].children.push_back(childID);
			}
			if (node->GetParent() != NULL)
			{
				unsigned int parentID;
				LoadSkeletonNode(node->GetParent(), mesh, fbx_joints, parentID);
				mesh.GetJoints()[index].parent = parentID;
			}
		}

	}
	else
		return false;
	return true;
}

bool CAssetManager::LoadAnimation(FbxAnimLayer* anim_layer, CAnimation& cAnimation, CMesh& cMesh, std::vector< FbxNode* >& fbx_joints)
{
	// TODO
	for (unsigned int fbxJointIndex = 0; fbxJointIndex < fbx_joints.size(); fbxJointIndex++)
	{
		std::vector<float> key_times;
		LoadCurve(fbx_joints[fbxJointIndex]->LclTranslation.GetCurve(anim_layer), key_times);
		LoadCurve(fbx_joints[fbxJointIndex]->LclRotation.GetCurve(anim_layer), key_times);
		LoadCurve(fbx_joints[fbxJointIndex]->LclScaling.GetCurve(anim_layer), key_times);

		tKey temp_key;
		tChannel temp_channel;
		for (unsigned int keyIndex = 1; keyIndex < key_times.size(); keyIndex++)
		{
			FbxTime tempTime;
			tempTime.SetSecondDouble((double)key_times[keyIndex]);
			FbxAMatrix fbxNodeMat = fbx_joints[fbxJointIndex]->EvaluateGlobalTransform(tempTime);
			XMFLOAT4X4 nodeMat;
			for (unsigned int i = 0; i < 4; i++)
			{
				for (unsigned int j = 0; j < 4; j++)
				{
					//Check Later to Make sure matrix is set properly
					nodeMat.m[i][j] = (float)fbxNodeMat.Get(i, j);
				}
			}
			temp_key.node = nodeMat;
			temp_key.time = key_times[keyIndex];
			temp_channel.keys.push_back(temp_key);
		}

		cAnimation.GetChannels().push_back(temp_channel);
	}
	int maxKeys = -1;
	int fullIndex = 0;
	for (size_t i = 0; i < cAnimation.GetChannels().size(); i++)
	{
		if (maxKeys < (int)cAnimation.GetChannels()[i].keys.size())
		{
			maxKeys = (int)cAnimation.GetChannels()[i].keys.size();
			fullIndex = i;
		}
	}

	XMFLOAT4X4 identity;
	XMStoreFloat4x4(&identity, XMMatrixIdentity());
	for (size_t currChan = 0; currChan < cAnimation.GetChannels().size(); currChan++)
	{
		if (cAnimation.GetChannels()[currChan].keys.size() < maxKeys)
		{
			for (int currKey = cAnimation.GetChannels()[currChan].keys.size(); currKey < maxKeys; currKey++)
			{
				tKey tempKey;
				tempKey.node = cMesh.GetJoints()[currChan].bind_pose_transform;
				//tempKey.node = identity;

				tempKey.time = cAnimation.GetChannels()[fullIndex].keys[currKey].time;
				cAnimation.GetChannels()[currChan].keys.push_back(tempKey);
			}
		}
	}
	cAnimation.m_vBindPose.resize(cMesh.GetJoints().size());

	for (size_t joint = 0; joint < cMesh.GetJoints().size(); joint++)
		cAnimation.m_vBindPose[joint] = cMesh.GetJoints()[joint];

	cMesh.GetJoints().clear();

	return true;
}

void CAssetManager::LoadCurve(FbxAnimCurve* curve, std::vector< float >& key_times)
{
	if (curve)
	{
		// TODO
		for (int keyIndex = 0; keyIndex < curve->KeyGetCount(); keyIndex++)
		{
			float keyTime = (float)curve->KeyGetTime(keyIndex).GetSecondDouble();
			int currKey;
			for (currKey = 0; currKey < (int)key_times.size(); currKey++)
			{
				if (keyTime == key_times[currKey])
					break;
			}
			if (currKey == (int)key_times.size())
				key_times.push_back((float)curve->KeyGetTime(keyIndex).GetSecondDouble());

		}
	}


}

bool CAssetManager::InitializeAssets()
{
	
	vector<string> fbxPaths;
	vector<string> meshPaths;

	vector<string> importPaths;

	vector<string> animPaths;

	FindFilesByPath("../Game/Assets/Art/3D/Animations/", ".fbx", fbxPaths);
	FindFilesByPath("../Game/Assets/Art/3D/Animations/", ".anim", animPaths);

	for (size_t currFBX = 0; currFBX < fbxPaths.size(); currFBX++)
	{
		size_t currBin = 0;
		for (; currBin < animPaths.size(); currBin++)
		{
			string animPath(animPaths[currBin].begin(), animPaths[currBin].end() - 4);
			string fbxPath(fbxPaths[currFBX].begin(), fbxPaths[currFBX].end() - 3);


			if (animPath == fbxPath)
				break;
		}
		
		if (currBin == animPaths.size())
			importPaths.push_back(fbxPaths[currFBX]);

	}

	animPaths.clear();
	for (size_t currPath = 0; currPath < importPaths.size(); currPath++)
	{
		FullMesh _mesh;
		CAnimation  _animation;

		string message = importPaths[currPath].c_str();
		if (Load(importPaths[currPath], _mesh, _animation))
		{
			message += " : Successfully Loaded";
			OUTPUT(message.c_str());
		}
		else
		{
			message += " : Failed to Load";
			OUTPUT(message.c_str());
			return false;
		}

		for (unsigned int subMesh = 0; subMesh < _mesh.size(); subMesh++)
		{
			_mesh[subMesh].ConvertVertices();
		}

		BinarySave(importPaths[currPath], _mesh, _animation);
	}

	FindFilesByPath("../Game/Assets/Art/3D/Animations/", ".anim", animPaths);
	FindFilesByPath("../Game/Assets/Art/3D/Animations/", ".mesh", meshPaths);

	for (size_t currMesh = 0; currMesh < meshPaths.size(); currMesh++)
	{
		CMesh newMesh;
		newMesh.BinaryRead(meshPaths[currMesh].c_str());
		if (m_PrefabMeshes.find(newMesh.GetName()) == m_PrefabMeshes.end())
			m_PrefabMeshes[newMesh.GetName()] = newMesh;

	}

	for (size_t currAnim = 0; currAnim < animPaths.size(); currAnim++)
	{
		CAnimation newAnim;
		newAnim.BinaryRead(animPaths[currAnim].c_str());
		
		if (m_AllAnimations.find(newAnim.GetName()) == m_AllAnimations.end())
			m_AllAnimations[newAnim.GetName()] = newAnim;

	}


	return true;
}

bool CAssetManager::BinaryExists(std::string& _filePath)
{
	ConvertFilePath(_filePath, BINARY);
	struct stat lStat;

	if (stat(_filePath.c_str(), &lStat) != -1)
	{
		return true;
	}

	return false;


}

void CAssetManager::BinaryLoadMesh(std::string _filePath, FullMesh& _outMesh, CAnimation& _outAnim)
{
	ifstream fin;
	fin.open(_filePath.c_str(), std::ios_base::binary);

	if (fin.is_open())
	{
		//FullMesh fullMesh;

		//1: Number of Meshes - unsigned int
		unsigned int numMeshes;
		fin.read((char*)&numMeshes, sizeof(unsigned int));
		_outMesh.resize(numMeshes);

		for (unsigned int mesh = 0; mesh < numMeshes; mesh++)
		{
			CMesh  currentMesh;

			//2 A: Size of name - unsigned int
			unsigned int meshNameSize = 0;
			fin.read((char*)&meshNameSize, sizeof(unsigned int));

			//2 B: Name of mesh - string
			char* meshName = new char[meshNameSize + 1];

			fin.read(meshName, meshNameSize);
			meshName[meshNameSize] = '\0';
			currentMesh.GetName() = meshName;
			delete[] meshName;

			//3: Number of vertices - unsigned int
			unsigned int numVertices = 0;
			unsigned int numAnimVertices = 0;
			unsigned int numIndices = 0;
			unsigned int numJoints = 0;
			unsigned int numTextures = 0;

			fin.read((char*)&numVertices, sizeof(unsigned int));
			fin.read((char*)&numAnimVertices, sizeof(unsigned int));

			//4: Number of indices - unsigned int
			fin.read((char*)&numIndices, sizeof(unsigned int));

			//5: Number of Joints - unsigned int
			fin.read((char*)&numJoints, sizeof(unsigned int));

			//6: Number of Textures - unsigned int
			fin.read((char*)&numTextures, sizeof(unsigned int));


			currentMesh.GetVertices().resize(numVertices);
			currentMesh.GetAnimVertices().resize(numAnimVertices);
			currentMesh.GetIndices().resize(numIndices);
			currentMesh.GetJoints().resize(numJoints);

			//7: TVertex information - vector<TVertex>
			vector<TVertex> tVerts;
			for (unsigned int vert = 0; vert < numVertices; vert++)
			{
				TVertex vertex;
				fin.read((char*)&vertex, sizeof(TVertex));
				currentMesh.GetVertices()[vert] = vertex;
			}
			//currentMesh.GetVertices() = tVerts;

			//8: TAnimVertex information - vector<TAnimVertex>
			//vector<TAnimVertex> tAnimVerts;
			for (unsigned int vert = 0; vert < numAnimVertices; vert++)
			{
				TAnimVertex vertex;
				fin.read((char*)&vertex, sizeof(TAnimVertex));
				currentMesh.GetAnimVertices()[vert] = vertex;
			}
			//currentMesh.GetAnimVertices() = tAnimVerts;

			//9: Indices - vector<unsigned int>
			vector<unsigned int> indices;
			for (unsigned int index = 0; index < numIndices; index++)
			{
				unsigned int ind;
				fin.read((char*)&ind, sizeof(unsigned int));
				currentMesh.GetIndices()[index] = ind;
			}
			//currentMesh.GetIndices() = indices;

			//10: Joints
			vector<Joint> joints;
			for (unsigned int joint = 0; joint < numJoints; joint++)
			{
				Joint curJoint;

				//10 A: Save out size of name - unsigned int
				unsigned int nameSize;
				fin.read((char*)&nameSize, sizeof(unsigned int));

				//10 B: Save out joint name - string
				char chName[32];
				fin.read((char*)chName, nameSize);
				chName[nameSize] = '\0';
				curJoint.name = chName;

				//10 C: Save out number of children - unsigned int
				unsigned int numChildren;
				fin.read((char*)&numChildren, sizeof(unsigned int));

				//10 D: Save out joint children indices - vector<unsigned int>
				vector<unsigned int> children;
				for (unsigned int child = 0; child < numChildren; child++)
				{
					unsigned int ind;
					fin.read((char*)&ind, sizeof(unsigned int));
					children.push_back(ind);
				}
				curJoint.children = children;

				//10 E: Save out joint parent - unsigned int
				unsigned int parent;
				fin.read((char*)&parent, sizeof(unsigned int));
				curJoint.parent = parent;

				//10 F: Save out bind pose - XMFLOAT4X4
				float bindPose[16];
				fin.read((char*)&bindPose, sizeof(float) * 16);
				XMFLOAT4X4 matrix = (XMFLOAT4X4)bindPose;

				curJoint.bind_pose_transform = matrix;

				currentMesh.GetJoints()[joint] = curJoint;
			}

			//currentMesh.GetJoints() = joints;

			

			_outMesh[mesh] = currentMesh;
		}

		////////////////////////////////////////////////////
		////////////WRITING OUT ANIMATIONS//////////////////
		////////////////////////////////////////////////////


		//12 A: Number of Channels - unsigned int
		unsigned int numChannels;
		fin.read((char*)&numChannels, sizeof(unsigned int));
		_outAnim.GetChannels().resize(numChannels);
		for (unsigned int channel = 0; channel < numChannels; channel++)
		{
			tChannel newChannel;
			//vector<tKey> allKeys;
			//12 B: Number of keys - unsigned int
			unsigned int numKeys;
			fin.read((char*)&numKeys, sizeof(unsigned int));
			newChannel.keys.resize(numKeys);
			for (unsigned int key = 0; key < numKeys; key++)
			{
				tKey newKey;
				float node[16];
				float time;

				//12 C: Key matrix - XMFLOAT4X4
				fin.read((char*)&node, sizeof(float) * 16);

				//12 D: Key time - float
				fin.read((char*)&time, sizeof(float));

				newKey.node = (XMFLOAT4X4)node;
				newKey.time = time;
				newChannel.keys[key] = newKey;
			}
			//newChannel.keys = allKeys;
			_outAnim.GetChannels()[channel] = newChannel;
		}
		


		//13 A: Animation Name Size
		unsigned int chNameSize;
		fin.read((char*)&chNameSize, sizeof(unsigned int));

		//13 B: Name of animation - string
		char* name = new char[chNameSize + 1];
		fin.read((char*)name, chNameSize);
		name[chNameSize] = '\0';
		_outAnim.GetName() = name;
		delete[] name;

	}
	fin.close();

}

void CAssetManager::ConvertFilePath(std::string& _filePath, eFileExtension _ext)
{
	char chDrive[_MAX_PATH];
	char chDir[_MAX_DIR];
	char chName[_MAX_FNAME];
	char chExt[_MAX_EXT];

	_splitpath_s(_filePath.c_str(), chDrive, chDir, chName, chExt);

	_filePath = chDrive;
	_filePath += chDir;
	_filePath += chName;

	if (_ext == BINARY)
		_filePath += ".bin";
	else
		_filePath += ".fbx";
}

void CAssetManager::BinarySave(std::string _filePath, FullMesh& _saveMesh, CAnimation & _saveAnim)
{
	int endIndex = _filePath.find_last_of('/') + 1;
	string filePath(_filePath.begin(), _filePath.begin() + endIndex);

	unsigned int meshIndex = 0;
	unsigned int totalMeshes = 0;

	
	

	unsigned int numMeshes = _saveMesh.size();
	for (size_t currMesh = 0; currMesh < numMeshes; currMesh++)
	{
		totalMeshes = (unsigned int)exportedMeshes.size();

		for (meshIndex = 0; meshIndex < totalMeshes; meshIndex++)
		{
			if (exportedMeshes[meshIndex] == _saveMesh[currMesh].GetName())
				break;
		}
		if (meshIndex == totalMeshes)
		{
			exportedMeshes.push_back(_saveMesh[currMesh].GetName());
			string meshPath = filePath + _saveMesh[currMesh].GetName() + ".mesh";
			_saveMesh[currMesh].BinaryWrite(meshPath.c_str());
		}
		

	}
	
	filePath += _saveAnim.GetName() + ".anim";
	_saveAnim.BinaryWrite(filePath.c_str());

	//ofstream fout;
	//fout.open(meshPath.c_str(), std::ios_base::binary);

	//if (fout.is_open())
	//{
	//	unsigned int numMeshes = _saveMesh.size();

	//	//1: Number of Meshes
	//	fout.write((char*)&numMeshes, sizeof(unsigned int));

	//	

	//		const char * name = _saveMesh[mesh].GetName().c_str();
	//		unsigned int numVerts = _saveMesh[mesh].GetVertices().size();
	//		unsigned int numAnimVerts = _saveMesh[mesh].GetAnimVertices().size();

	//		unsigned int numIndices = _saveMesh[mesh].GetIndices().size();
	//		unsigned int numJoints = _saveMesh[mesh].GetJoints().size();
	//		unsigned int numTextures = _saveMesh[mesh].GetTextureNames().size();
	//		unsigned int numInfluences = _saveMesh[mesh].GetInfluences().size();
	//		unsigned int nameSize = _saveMesh[mesh].GetName().size();

	//		//2 A: Size of name - unsigned int
	//		fout.write((char*)&nameSize, sizeof(unsigned int));

	//		//2 B: Name of mesh - string
	//		fout.write((char*)name, _saveMesh[mesh].GetName().size());

	//		//3: Number of vertices - unsigned int
	//		fout.write((char*)&numVerts, sizeof(unsigned int));
	//		fout.write((char*)&numAnimVerts, sizeof(unsigned int));


	//		//4: Number of indices - unsigned int
	//		fout.write((char*)&numIndices, sizeof(unsigned int));

	//		//5: Number of Joints - unsigned int
	//		fout.write((char*)&numJoints, sizeof(unsigned int));

	//		//6: Number of Textures - unsigned int
	//		fout.write((char*)&numTextures, sizeof(unsigned int));

	//		//7: TVertex information - vector<TVertex>
	//		fout.write((char*)_saveMesh[mesh].GetVertices()._Myfirst, sizeof(TVertex)* _saveMesh[mesh].GetVertices().size());

	//		//8: TAnimVertex information - vector<TAnimVertex>
	//		fout.write((char*)_saveMesh[mesh].GetAnimVertices()._Myfirst, sizeof(TAnimVertex)* _saveMesh[mesh].GetAnimVertices().size());

	//		//9: Indices - vector<unsigned int>
	//		fout.write((char*)_saveMesh[mesh].GetIndices()._Myfirst, sizeof(unsigned int)* _saveMesh[mesh].GetIndices().size());

	//		//10: Joints
	//		for (unsigned int joint = 0; joint < numJoints; joint++)
	//		{
	//			const char * jointName = _saveMesh[mesh].GetJoints()[joint].name.c_str();
	//			unsigned int numChildren = _saveMesh[mesh].GetJoints()[joint].children.size();
	//			unsigned int parent = _saveMesh[mesh].GetJoints()[joint].parent;
	//			XMFLOAT4X4 bindPose = _saveMesh[mesh].GetJoints()[joint].bind_pose_transform;
	//			unsigned int jointNameSize = _saveMesh[mesh].GetJoints()[joint].name.size();

	//			//10 A: Save out size of name - unsigned int
	//			fout.write((char*)&jointNameSize, sizeof(unsigned int));

	//			//10 B: Save out joint name - string
	//			fout.write((char*)jointName, _saveMesh[mesh].GetJoints()[joint].name.size());

	//			//10 C: Save out number of children - unsigned int
	//			fout.write((char*)&numChildren, sizeof(unsigned int));

	//			//10 D: Save out joint children indices - vector<unsigned int>
	//			fout.write((char*)&_saveMesh[mesh].GetJoints()[joint].children._Myfirst, sizeof(unsigned int)*_saveMesh[mesh].GetJoints()[joint].children.size());

	//			//10 E: Save out joint parent - unsigned int
	//			fout.write((char*)&_saveMesh[mesh].GetJoints()[joint].parent, sizeof(unsigned int));

	//			//10 F: Save out bind pose - float[16]
	//			fout.write((char*)&_saveMesh[mesh].GetJoints()[joint].bind_pose_transform, sizeof(float) * 16);
	//		

	//		//11: Influences

	//		
	//	}

	//	////////////////////////////////////////////////////
	//	////////////WRITING OUT ANIMATIONS//////////////////
	//	////////////////////////////////////////////////////

	//	unsigned int numChannels = _saveAnim.GetChannels().size();

	//	//12 A: Number of Channels - unsigned int
	//	fout.write((char*)&numChannels, sizeof(unsigned int));

	//	for (unsigned int channel = 0; channel < numChannels; channel++)
	//	{
	//		unsigned int numKeys = _saveAnim.GetChannels()[channel].keys.size();

	//		//12 B: Number of keys - unsigned int
	//		fout.write((char*)&numKeys, sizeof(unsigned int));
	//		for (unsigned int key = 0; key < numKeys; key++)
	//		{
	//			XMFLOAT4X4 node = _saveAnim.GetChannels()[channel].keys[key].node;
	//			float time = _saveAnim.GetChannels()[channel].keys[key].time;

	//			//12 C: Key matrix - XMFLOAT4X4
	//			fout.write((char*)&node, sizeof(XMFLOAT4X4));

	//			//12 D: Key time - float
	//			fout.write((char*)&time, sizeof(float));
	//		}
	//	}

	//	unsigned int chNameSize = _saveAnim.GetName().size();
	//	const char * name = _saveAnim.GetName().c_str();

	//	//13 A: Animation Name Size
	//	fout.write((char*)&chNameSize, sizeof(unsigned int));

	//	//13 B: Name of animation - string
	//	fout.write((char*)name, _saveAnim.GetName().size());

	//}
	//fout.close();
}

CAssetManager * CAssetManager::GetInstance()
{
	if (m_cpInstance == nullptr)
	{
		m_cpInstance = new CAssetManager;
		m_cpInstance->Initialize();
	}

	return m_cpInstance;

}

const CAnimation& CAssetManager::GetAnimation(string name) 
{ 
	if (m_AllAnimations.find(name) != m_AllAnimations.end())
	{
		return m_AllAnimations[name];
	}
	return CAnimation();
}
void CAssetManager::DeleteInstance()
{
	if (m_cpInstance == nullptr)
	{
		return;
	}

	if (m_cpInstance->Shutdown() == false)
	{
		//send error message
	}

	delete m_cpInstance;
	m_cpInstance = nullptr;
}

bool CAssetManager::Shutdown()
{
	m_vAnimations.clear();
	

	for (unsigned int i = 0; i < m_vMeshes.size(); i++)
		m_vMeshes[i].clear();
	m_vMeshes.clear();


	m_PrefabMeshes.clear();

	/*for (unordered_map<string, CMesh*>::iterator i = m_PrefabMeshes.begin(); i != m_PrefabMeshes.end(); i++)
	{
		delete (*i).second;
		(*i).second = nullptr;
	}
*/
	return true;
}

bool CAssetManager::Initialize()
{
	LoadTextures();

	LoadAllPrefabs();
	if (InitializeAssets())
		return true;
	else
	{
		assert(false && "CAssetManager::Initalize() - InitalizeAssets() failed");
		return false;
	}
}

CLevelPrefab* CAssetManager::GetPrefab(std::string prefabName)
{
	return m_PrefabsMap[prefabName];
}

void CAssetManager::LoadPrefabs(CObjectManager* objectManager)
{
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Wall Prefab 1", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/WallPrefab_1.fbx", CLevelPrefab::WALL_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Wall Prefab 2", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/WallPrefab_2.fbx", CLevelPrefab::WALL_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Wall Prefab 3", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/WallPrefab_3.fbx", CLevelPrefab::WALL_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Wall Prefab 4", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/WallPrefab_4.fbx", CLevelPrefab::WALL_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Wall Prefab 5", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/WallPrefab_5.fbx", CLevelPrefab::WALL_PREFAB, objectManager)));

	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Green Zone Room 1", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/GreenZonePrefab_1.fbx", CLevelPrefab::ZONE_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Green Zone Room 2", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/GreenZonePrefab_2.fbx", CLevelPrefab::ZONE_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Green Zone Room 3", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/GreenZonePrefab_3.fbx", CLevelPrefab::ZONE_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Green Zone Room 4", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/GreenZonePrefab_4.fbx", CLevelPrefab::ZONE_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Green Zone Room 5", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/GreenZonePrefab_5.fbx", CLevelPrefab::ZONE_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Green Zone Room 6", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/GreenZonePrefab_6.fbx", CLevelPrefab::ZONE_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Green Zone Room 7", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/GreenZonePrefab_7.fbx", CLevelPrefab::ZONE_PREFAB, objectManager)));

	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Floor Prefab", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/FloorPrefab.fbx", CLevelPrefab::FLOOR_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Small Pit Prefab", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/SmallPit.fbx", CLevelPrefab::PIT_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Big Pit Prefab", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/BigPit.fbx", CLevelPrefab::PIT_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Cracked Wall Prefab", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/CrackedWall.fbx", CLevelPrefab::WALL_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Enemy Cave Wall Prefab", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/EnemyCaveWall.fbx", CLevelPrefab::WALL_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Exit Zone Prefab", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/ExitZonePrefab.fbx", CLevelPrefab::ZONE_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("Safe Haven Prefab", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/SafeHavenPrefab.fbx", CLevelPrefab::ZONE_PREFAB, objectManager)));

	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x1 Trap Section 1", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x1_1.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x1 Trap Section 2", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x1_2.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x1 Trap Section 3", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x1_3.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x1 Trap Section 4", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x1_4.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x1 Trap Section 5", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x1_5.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x1 Trap Section 6", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x1_6.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x1 Trap Section 7", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x1_7.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x1 Trap Section 8", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x1_8.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));

	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x3 Trap Section 1", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x3_1.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x3 Trap Section 2", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x3_2.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x3 Trap Section 3", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x3_3.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x3 Trap Section 4", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x3_4.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x3 Trap Section 5", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x3_5.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x3 Trap Section 6", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x3_6.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));

	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x5 Trap Section 1", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x5_1.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x5 Trap Section 2", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x5_2.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x5 Trap Section 3", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x5_3.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x5 Trap Section 4", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x5_4.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));
	m_PrefabsMap.insert(pair<string, CLevelPrefab*>("3x5 Trap Section 5", new CLevelPrefab("../Game/Assets/Art/3D/Models/Environment/Level Prefabs/TrapSection_1x5_5.fbx", CLevelPrefab::TRAP_SECTION_PREFAB, objectManager)));

}

CMesh* CAssetManager::GetPrefabMesh(std::string objectPrefix)
{ 
	if (m_PrefabMeshes.find(objectPrefix) != m_PrefabMeshes.end())
		return &m_PrefabMeshes[objectPrefix];

	return nullptr;
}

std::unordered_map<std::string, CMesh>& CAssetManager::GetPrefabMeshMap()
{
	return m_PrefabMeshes;
}

void CAssetManager::AddPrefabMesh(string prefix, CMesh& mesh)
{
	m_PrefabMeshes.insert(pair<string, CMesh>(prefix, mesh));
}
