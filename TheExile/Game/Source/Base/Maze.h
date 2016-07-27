#ifndef _MAZE_H_
#define _MAZE_H_

//#include "Tile.h"
#include "../../../Engine/Renderer/RenderMesh.h"
class CTileMap;
class CObjectManager;
class CSpawnerObject;
class CDayNight;
class CExitKey;
enum EGreenRoomType { UNASSIGNED = -1, Type1 = 0, Type2, Type3, Type4, Type5, Type6, Type7, NumTypes };

enum ePrefabType { NWALL, GWALL, CWALL, ECWALL, FLOOR, GFLOOR, HCHEST_U, HCHEST_R, 
	HCHEST_D, HCHEST_L,	WEAPCHEST_U, WEAPCHEST_R, WEAPCHEST_D, WEAPCHEST_L, SHLDCHEST_U, SHLDCHEST_R, 
	SHLDCHEST_D, SHLDCHEST_L,	STORCH_D, STORCH_R, STORCH_U, STORCH_L, DTORCH_V, DTORCH_H, 
	SAFE_HAVEN, EXIT_U, EXIT_D, EXIT_R, EXIT_L, SPIT_V, SPIT_H, BPIT_V, 
	BPIT_H, T_ThreeXOne_V, T_ThreeXOne_H, T_ThreeXThree_V, T_ThreeXThree_H, T_ThreeXFive_V,	T_ThreeXFive_H, SKEL_SPAWN, 
	MIN_SPAWN, BUSH_SPAWN, TREE_SPAWNER, PATH_NODE, CAVE_NODE, CWALL_NODE, PIT_NODE, MIN_PATROL_IGNORE, 
	MIN_SPAWN_N, MIN_SPAWN_E, MIN_SPAWN_S, MIN_SPAWN_W, SKELETON_IGNORE, GZ_SKEL_SPAWN, EXIT_KEY,
};
class CExitDoorObject;
class CPrefabObject;
//struct TFloorData
//{
//	XMFLOAT4X4 m_tMatrix;
//	CTile::ETileType m_eType;
//
//};
//struct TTrapData
//{
//	XMFLOAT4X4 m_tMatrix;
//	ETrapType m_eType;
//	int varient;
//
//};
//struct TGreenRoomData
//{
//	XMFLOAT4X4 m_tMatrix;
//	EGreenRoomType m_eType;
//};
//
//struct TWallData
//{
//	XMFLOAT4X4 m_tMatrix;
//	vector<int> torches;
//	CTile::ETileType m_eType;
//};
//
class CNode;
struct PrefabData
{
	XMFLOAT3 position;
	ePrefabType type; 
};

class CMaze
{

public:

	CMaze();
	~CMaze();


	void ClearMaze();
	void GenerateMaze();
	/*bool LoadGenParams(const char* filePath);*/
	bool LoadMaze(CObjectManager* objectManager, CDayNight* dayNight);
	
	
	vector<XMFLOAT4X4>& GetOrcSpawns() { return m_vOrcSpawns; }
	vector<XMFLOAT4X4>& GetMinotaursSpawns() { return m_vMinSpawns; }
	vector<XMFLOAT4X4>& GetSkeletonSpawns() { return m_vSkelSpawns; }
	vector<XMFLOAT4X4>& GetGZSkelSpawns() { return m_vGZSkelSpawns; }
	vector<CExitKey*>& GetExitKeys() { return m_vpExitKeys; }

private: //Methods

	bool LoadLayer(const char* filePath);
	bool LoadPathingData(const char* filePath);

	void ProcessLayer(CObjectManager* objectManager, CDayNight* dayNight);

	void BuildNavData();

	vector<CNode*> CrackedWallNodes;
	vector<CNode*> PitNodes;
	vector<CNode*> BridgeWallNodes;

	CExitDoorObject* exitDoor;
	vector<XMFLOAT4X4> m_vSkelSpawns;
	vector<XMFLOAT4X4> m_vGZSkelSpawns;

	vector<XMFLOAT4X4> m_vOrcSpawns;
	vector<XMFLOAT4X4> m_vMinSpawns;

	vector<CExitKey*> m_vpExitKeys;
	////Generation Methods






		
		

private: // Members
	 
	vector<PrefabData> levelData;
	vector<PrefabData> navData;
	
	vector<CPrefabObject*> m_pvLevelPrefabs;
	







	


};

#endif
