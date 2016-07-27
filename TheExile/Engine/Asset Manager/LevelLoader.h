/***********************************************
* Filename:  		LevelLoader.h
* Date:      		8/7/2015
* Mod. Date: 		9/2/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Loads the entire level 
************************************************/

#ifndef CLEVELLOADER_H
#define CLEVELLOADER_H

#include <fbxsdk.h>
#include "../Core/Collision/Collider.h"


class CMesh;
class CRenderMesh;
class CNavGraph;
class CObjectManager;
class IObject;

class CWallObject;
class CFloorObject;
class CPitObject;
class CPitWallObject;
class CDoorObject;
class CExitDoorObject;

class CCrackedWall;
class CBush;
class CTree;
class CTorch;
class CLightWeaponChest;
class CHeavyWeaponChest;
class CHealthUpgradeChest;

class CSpikeTrap;
class CSpinningBlade;
class CFireTrap;

class CSpawnerObject;
class CWaypointObject;



class CLevelLoader
{
private:

	//big pit bottom
	std::vector<CPitObject*> m_cpBigPitObjects;
	std::vector<CWaypointObject*> m_cpBridgeTeleporters;

	//safe haven doors(4 possible)
	std::vector<CDoorObject*> m_cpDoorObjects;

	//exit doors(up to 4 possible)
	std::vector<CMesh> m_cvExitDoorMeshes;
	CExitDoorObject* m_cpTheExitDoor;
	std::vector<CMesh> m_cvExitTeleporterMeshes;
	CWaypointObject* m_cpTheExitTeleporter;

	//skeleton spawners
	std::vector<CSpawnerObject*> m_cpSkeletonSpawnObjects;

	//orc spawners(inside of caves)
	std::vector<CSpawnerObject*> m_cpOrcSpawnObjects;

	//minotaur spawners 
	std::vector<CSpawnerObject*> m_cpMinotaurSpawnObjects;

	//minotaur waypoints 
	CNavGraph* m_cpMinotaurNavGraph;

	//cracked walls
	std::vector<XMFLOAT3> m_cpCrackedWallObjects;

	//cave spawn walls
	//std::vector<CWallObject*> m_cpCaveSpawnObjects;

	//light weapon chests
	//std::vector<CLightWeaponChest*> m_cpLightChestObjects;

	//heavy weapon chests
	//std::vector<CHeavyWeaponChest*> m_cpHeavyChestObjects;

	//health upgrade chests
	//std::vector<CHealthUpgradeChest*> m_cpHealthChestObjects;

	//torches
	std::vector<CTorch*> m_cpTorchObjects;

	//pressure plate spike trap
	std::vector<IObject*> m_cpSpikeTrapObjects;

	//alternating spike trap
	std::vector<IObject*> m_cpAltSpikeTrapObjects;

	//spinning blade trap
	std::vector<IObject*> m_cpSpinningBladeObjects;

	//fire trap trap
	std::vector<CFireTrap*> m_cpFireTrapObjects;

	//cuttable trees
	//std::vector<CTree*> m_cpTreeObjects;

	//bushes
	std::vector<CBush*> m_cpBushObjects;

	//invisible walls
	std::vector<CWallObject*> m_cpInvisibleWallObjects;

	//kill floor
	std::vector<CFloorObject*> m_cpKillFloorObjects;

	//all environment objects
	std::vector<IObject*> m_cpEnvironmentObjects;

	//load in meshes
	std::vector<CMesh> m_cvMeshes;

	//the object manager
	CObjectManager* m_cpObjectManager;


	/// <summary>
	/// Processes a single fbx node and loads in appropiate data
	/// </summary>
	/// <param name = "node"> The node being processed. </param>
	/// <param name = "meshes"> The meshes being loaded. </param>
	bool ProcessLevelNode(FbxNode* node, vector<CMesh>& meshes);

	XMFLOAT3 GetAABBCentroid(std::vector<TVertex> verts);

public:

	/// <summary>
	/// The constructor
	/// </summary>
	/// <param> none</param>
	CLevelLoader(CObjectManager* manager);

	/// <summary>
	/// The destructor
	/// </summary>
	/// <param> none</param>
	~CLevelLoader();

	/// <summary>
	/// Loads the entire level through FBX.
	/// Returns true if successfully loaded.
	/// </summary>
	/// <param name = "file_name"> The filepath at which the level FBX is located. </param>
	bool LoadLevel(string file_name);



	/***************
	*  Accessors
	***************/

	/// <summary>
	/// Returns the vector of all environment obejcts
	/// </summary>
	/// <param> none</param>
	vector<IObject*> GetEnvironmentObjects();

	/// <summary>
	/// Returns all skeleton spawner objects loaded from the level
	/// </summary>
	/// <param> none</param>
	vector<CSpawnerObject*> GetSkeletonSpawnerObjects();

	/// <summary>
	/// Returns all orc spawner objects loaded from the level
	/// </summary>
	/// <param> none</param>
	vector<CSpawnerObject*> GetOrcSpawnerObjects();

	/// <summary>
	/// Returns all minotaur spawner objects loaded from the level
	/// </summary>
	/// <param> none</param>
	vector<CSpawnerObject*> GetMinotaurSpawnerObjects();

	/// <summary>
	/// Returns all minotaur waypoint objects loaded from the level
	/// </summary>
	/// <param> none</param>
	CNavGraph* GetMinotaurNavGraph();

	/// <summary>
	/// Returns all cracked wall objects loaded from the level
	/// </summary>
	/// <param> none</param>
	vector<XMFLOAT3> GetCrackedWallObjects();

	/// <summary>
	/// Returns all big pit objects loaded from the level
	/// </summary>
	/// <param> none</param>
	vector<CPitObject*> GetBigPitObjects();

	/// <summary>
	/// Returns all bridge teleporters
	/// </summary>
	/// <param> none</param>
	XMFLOAT3 GetBridgeTeleporterPosition(unsigned int ID);

	/// <summary>
	/// Returns the waypoint at which the exit door is located
	/// </summary>
	/// <param> none</param>
	XMFLOAT3 GetExitTeleportPosition() const;

	void ActivateExitDoor(float moveTime);

};

#endif
