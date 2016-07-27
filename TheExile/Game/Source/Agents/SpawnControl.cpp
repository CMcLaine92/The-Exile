/***********************************************
* Filename:  		SpawnControl.cpp
* Date:      		08/24/2015
* Mod. Date: 		----------
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:   		Controls when and where to spawn enemies
************************************************/
#include "SpawnControl.h"
#include "Minotaur/MinotaurController.h"
#include "Orc/OrcController.h"
#include "Skeleton/SkeletonController.h"
#include "../../../Engine/Asset Manager/LevelLoader.h"
#include "Minotaur/Minotaur.h"
#include "../../../Engine/Core/Messages/EnemySpawnMessage.h"
#include "../Base/Maze.h"
#include "../../../Engine/Object Manager/ObjectManager.h"
#include "../../../Engine/Animation/AnimationManager.h"
#include "../../../Engine/Object Manager/SpawnerObject.h"
#include "../Environment/DayNight.h"
#include "../../../Engine/Core/State Machine/States/GameplayState.h"
#include "Minotaur/MinotaurController.h"

CSpawnControl::CSpawnControl(CMaze* _loader, CObjectManager* _objManager, CAnimationManager* _animManager, CDayNight* _dayNight)
{

	// Set the player pointer
	m_ipPlayer = nullptr;
	m_cpLevel = nullptr;
	m_ipPlayer = _objManager->GetPlayer();
	m_cpDayNight = _dayNight;
	// set the LevelLoader for future use
	m_cpLevel = _loader;

	// Get the spawn locations in world space
	m_vSkeletonSpawnObjects = m_cpLevel->GetSkeletonSpawns();
	m_vNightSkeletonSpawnObjects = m_cpLevel->GetGZSkelSpawns();
	m_vMinotaurSpawnObjects = m_cpLevel->GetMinotaursSpawns();
	m_vOrcSpawnObjects = m_cpLevel->GetOrcSpawns();


	//// Mark all spots as available for spawning
	//for (size_t obj = 0; obj < m_vSkeletonSpawnObjects.size(); obj++)
	//	m_vSkelSpawnAvailable.push_back(true);
	//// Mark all spots as available for spawning
	//for (size_t obj = 0; obj < m_vMinotaurSpawnAvailable.size(); obj++)
	//	m_vMinotaurSpawnAvailable.push_back(true);
	//for (size_t obj = 0; obj < m_vOrcSpawnAvailable.size(); obj++)
	//	m_vOrcSpawnAvailable.push_back(true);


	m_cpOrcController = new COrcController(_objManager, _animManager, m_vOrcSpawnObjects, _dayNight);

	m_cpMinotaurController = new CMinotaurController(_objManager, _animManager, m_vMinotaurSpawnObjects, _dayNight);
	
	m_cpSkeletonController = new CSkeletonController(_objManager, _animManager, m_vSkeletonSpawnObjects, _dayNight);

}

CSpawnControl::~CSpawnControl()
{
	SAFE_DELETE(m_cpOrcController);
	SAFE_DELETE(m_cpMinotaurController);
	SAFE_DELETE(m_cpSkeletonController);
	
}


void CSpawnControl::Update()
{
	

	m_cpMinotaurController->Update();

	m_cpOrcController->Update();

	
}

void CSpawnControl::AddToRenderer()
{
	m_cpSkeletonController->AddToRenderer();

	m_cpOrcController->AddToRenderer();
	m_cpMinotaurController->AddToRenderer();

}
