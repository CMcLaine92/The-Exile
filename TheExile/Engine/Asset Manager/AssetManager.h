#pragma once
#include "../Animation/Mesh.h"
#include "../Animation/Animation.h"
#include "../Core/Collision/AABB.h"
#include "../Object Manager/SpawnerObject.h"
#include <fbxsdk.h>
#include <iostream>
#include <sys\stat.h>



typedef std::vector< std::pair< FbxNode*, FbxNodeAttribute* >> JointList;

class CLevelPrefab;
class CObjectManager;

class CAssetManager
{
	typedef vector<CMesh> FullMesh;

	//map for all prefabs
	std::unordered_map<std::string, CLevelPrefab*> m_PrefabsMap;
	std::unordered_map<std::string, CMesh> m_PrefabMeshes;//string is the mesh prefix
	std::unordered_map<std::string, CAnimation> m_AllAnimations;

	enum eFileExtension {FBX, BINARY};

	vector<vector<CMesh>> m_vMeshes;
	unordered_map<string, CMesh> m_vcMeshes;
	vector<CAnimation> m_vAnimations;
	vector<string> exportedMeshes;


	static CAssetManager * m_cpInstance;

	CAssetManager() {};
	~CAssetManager() {};

	/*void BuildAABB(CMesh::TAABB&, CMesh&);
	void BuildPlane(CMesh::TPlane&, CMesh&);*/

	bool LoadMeshFbx(const std::string& file_name, FullMesh& meshes);
	bool LoadAnimFbx(const std::string& file_name, CAnimation& animation);

	void LoadAnimMeshs();

public:

	
	//enum eMeshType { TestMesh,
	//	 PlayerMesh, PlayerSword, PlayerHatchet, PlayerSpear, PlayerAttack1, PlayerAttack2, PlayerIdle, PlayerHit, PlayerDeath, PlayerJump, BonePile,
	//				SkeletonSword, SkeletonShield, SkeletonRun,SkeletonAttack, SkeletonDeath, SkeletonHurt1, SkeletonHurt2, SkeletonBlock, SkeletonBlockWalk,
	//				MinotaurIdle, MinotaurRun, MinotaurCharge, MinotaurTell, MinotaurEnrage, MinotaurStomp, MinotaurDead, MinotaurStunned, OrcMelee, OrcPreThrow, OrcThrow,
	//				OrcHurt1, OrcHurt2, OrcDeath, OrcIdle1, OrcIdle2, OrcWalk, NavigationMesh, SpikeTrap, SpinningTrap, /*PressurePlate,*/ FireTrap, Torch, Bush, 
	//				 Herb, Wood, Bone, Gunpowder, GraveStone, Bed, Bridge, UnbuiltBridge, Workbench, HealthChest, WeaponChest, ShieldChest, WallBomb, MeshCount };
	//enum eAnimationType{PlayerRunAnim, PlayerAttack1Anim, PlayerAttack2Anim};

	bool InitializeAssets();
	bool Load(const std::string& file_name, std::vector< CMesh >& meshes, CAnimation& cAnimation);
	bool ProcessNode(FbxNode* node, std::vector< CMesh >& meshes, std::vector< FbxNode* >& fbx_joints);
	bool LoadMesh(FbxMesh* fbx_mesh, CMesh& mesh);
	bool LoadSkin(FbxMesh* fbx_mesh, CMesh& mesh, std::vector< FbxNode* >& fbx_joints);
	bool LoadSkeletonNode(FbxNode* node, CMesh& mesh, std::vector< FbxNode* >& fbx_joints, unsigned int& index);
	bool LoadAnimation(FbxAnimLayer* anim_layer, CAnimation& CAnimation, CMesh& cMesh, std::vector< FbxNode* >& fbx_joints);
	void LoadCurve(FbxAnimCurve* curve, std::vector< float >& key_times);
	bool LoadAnimMesh(FbxNode* node, CMesh& mesh, std::vector< FbxNode* >& fbx_joints);
	bool LoadAnim(FbxAnimLayer* anim_layer, CAnimation& CAnimation, CMesh& cMesh, std::vector< FbxNode* >& fbx_joints);
	bool LoadAnim(const char* path);

	void LoadModel(FbxMesh* fbx_mesh, CMesh& mesh);


	const CAnimation& GetAnimation(string name);

	typedef vector<CMesh> FullMesh;

	bool BinaryExists(string& _filePath);
	void BinaryLoadMesh(string _filePath, FullMesh& _outMesh, CAnimation& _outAnim);
	void ConvertFilePath(string& _filePath, eFileExtension _ext);
	void FindFilesByPath(std::string folderPath, std::string fileExtension, std::vector<std::string>& filePaths);
	void BinarySave(std::string _filePath, FullMesh& _saveMesh, CAnimation & _saveAnim);
	bool LoadAnimMesh(const std::string& file_name);


	vector<CMesh> playerMesh;
	CAnimation * playerAnimation;

	CLevelPrefab* GetPrefab(std::string prefabName);
	void LoadPrefabs(CObjectManager* objectManager);

	CMesh* GetPrefabMesh(std::string objectPrefix);
	std::unordered_map<std::string, CMesh>& GetPrefabMeshMap();
	void AddPrefabMesh(string prefix, CMesh& mesh);

	static CAssetManager * GetInstance();
	static void DeleteInstance();
	bool Shutdown();
	bool Initialize();

	void LoadTextures();

	void LoadAllPrefabs();
	void LoadPrefabFBX(const char* filePath);
	void FBXLoadPrefab(FbxNode* node, CMesh& prefab);


};

