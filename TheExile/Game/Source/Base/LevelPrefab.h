/***********************************************
* Filename:  		LevelPrefab.h
* Date:      		10/2/2015
* Mod. Date: 		10/2/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a level prefab object
************************************************/

#ifndef CLEVELPREFAB_H
#define CLEVELPREFAB_H

#include "../../../Engine/Object Manager/IObject.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include <iostream>

#include <fbxsdk.h>

#include <unordered_map>

class CObjectManager;
class CSpawnerObject;

class CLevelPrefab
{
public:

	enum eObjectMeshType { WALL_OBJECT, FLOOR_OBJECT, GRASS_FLOOR_OBJECT, RENDER_ONLY_OBJECT, DOOR_OBJECT, EXIT_DOOR_OBJECT, SMALL_PIT_OBJECT, BIG_PIT_OBJECT, PIT_WALL_OBJECT, INVISIBLE_WALL_OBJECT, CRACKED_WALL_OBJECT, 
		ENEMY_CAVE_WALL_OBJECT, TORCH_OBJECT, BUSH_OBJECT, TREE_OBJECT, SPIKE_TRAP_OBJECT, ALT_SPIKE_TRAP_OBJECT, SPINNING_BLADE_OBJECT, FIRE_TRAP_OBJECT, CHEST_OBJECT};

	struct TObjectMeshData
	{
		eObjectMeshType m_eMeshType;
		unsigned int m_unMeshCount;
		std::unordered_map<unsigned int, string> m_umMeshFullNameMap;//int = countID, string = full mesh name
		std::unordered_map<unsigned int, XMFLOAT4X4> m_umMeshMatrixMap;//int = countID, xmfloat4x4 = local matrix

		//spinning balde data
		std::unordered_map<unsigned int, XMFLOAT3> m_umSBStartPointMap;//WP ID, start position
		std::unordered_map<unsigned int, XMFLOAT3> m_umSBEndPointMap;//WP ID, end position
	};

	enum ePrefabType { WALL_PREFAB, FLOOR_PREFAB, PIT_PREFAB, ZONE_PREFAB, GREEN_ZONE_PREFAB, TRAP_SECTION_PREFAB, NONE };

private:

	CObjectManager* m_cpObjectManager;

	std::vector<CMesh*> m_cpMeshes;
	std::vector<IObject*> m_cpObjects;
	ePrefabType m_nPrefabType;
	XMFLOAT4X4 m_mWorlMatrix;
	std::string m_szFBXpath;

	//binary load data
	std::vector<TObjectMeshData> m_vtObjectLoadData;//there will be 1 for every object type in this prefab. (the size will == the number of mesh types in this prefab)

	//for spinning blade traps. spinning blade ID will match its starting points ID
	std::unordered_map<int, IObject*> m_umSpinningBladeMap;//SB ID, spawnerObejct
	std::unordered_map<int, float> m_umSBRotationSpeedMap;//SB ID, rotationSpeeds
	std::unordered_map<int, int> m_umSBMovePointsIDs;//WP ID, adjID
	std::unordered_map<int, XMFLOAT3> m_umSBMovePointsMap;//WP ID, position

	//wall torches
	std::unordered_map<int, IObject*> m_umWallTorchIDs;//torch ID, torch object. 0 = pos Z, 1 = neg X, 2 = neg Z, 3 = pos X

	//prefab loading functions
	bool FBXLoadPrefab(FbxNode* node, vector<CMesh*> meshes);

	//helper functions
	XMFLOAT4X4 GetMeshLocalMatrix(FbxNode* node);
	XMFLOAT3 GetAABBCentroid(std::vector<TVertex> verts);

	//binary load function
	void PrefabBinaryMeshLoad(string _filePath, std::vector<CMesh*>& _outMesh);
	void PrefabBinaryMeshSave(std::string _filePath, std::vector<CMesh*> _saveMesh);
	void BinaryLoadPrefab();

public:
	
	CLevelPrefab(string file_name, ePrefabType type, CObjectManager* objectManger);

	~CLevelPrefab();

	void Update();

	void SwitchPrefabTexture(const wchar_t* texture_path);
	CLevelPrefab* CopyPrefab();


	/*	ACCESSORS	*/
	std::vector<CMesh*>& GetMeshes();
	std::vector<IObject*>& GetObjects();
	unsigned int GetPrefabType();
	XMFLOAT4X4 GetWorldMatrix();
	IObject* GetTorch(unsigned int ID);
	unordered_map<int, IObject*>& GetTorches() { return m_umWallTorchIDs; }
	void RemoveTorch(int index);
	std::vector<TObjectMeshData>& GetPrefabLoadData();

	/*	MUTATORS	*/
	void SetWorldMatrix(XMFLOAT4X4 worldMatrix);

};

#endif
