/***********************************************
* Filename:  		MinotaurController.h
* Date:      		9/7/2015
* Mod. Date: 		9/7/2015
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:			Handles the activation and deactivation
*					of minotaurs as well as
************************************************/

#ifndef _ORC_CONTROLLER_H
#define _ORC_CONTROLLER_H

#include "../../../../Engine/Object Manager/IObject.h"

class COrc;
class CDayNight;
class CSpawnerObject;
class CObjectManager;
class CAnimationManager;

class COrcController : public IObject
{


public:

	COrcController(CObjectManager* _objectManager, CAnimationManager* animManager, std::vector<XMFLOAT4X4>& spawners, CDayNight* _dayNight = nullptr);

	virtual ~COrcController();

	void Update();
	void SetSpawnPoints(std::vector<XMFLOAT4X4>& spawnPoints){ m_vSpawnPoints = spawnPoints; }
	void SetDayNight(CDayNight* dayNight) { m_cpDayNight = dayNight; }
	void AddToRenderer();

private:

	std::vector<COrc*> m_vOrcs;
	std::vector<XMFLOAT4X4> m_vSpawnPoints;
	CDayNight* m_cpDayNight;
	bool m_bNightTime;



};

#endif