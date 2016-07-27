/***********************************************
* Filename:  		MinotaurController.h
* Date:      		9/7/2015
* Mod. Date: 		9/7/2015
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:			Handles the activation and deactivation
*					of minotaurs as well as 
************************************************/

#ifndef _MINOTAUR_CONTROLLER_H
#define _MINOTAUR_CONTROLLER_H


class CMinotaur;
class CDayNight;
class CSpawnerObject;
class CObjectManager;
class CAnimationManager;

class CMinotaurController
{


public:

	CMinotaurController(CObjectManager* objectManager, CAnimationManager* animationManager, std::vector<XMFLOAT4X4> spawners, CDayNight* _dayNight = nullptr);
	void LoadConfigXML(CObjectManager* objectManager, CAnimationManager* animationManager);

	virtual ~CMinotaurController();
	
	void Update();
	void SetSpawnPoints(std::vector<XMFLOAT4X4>& spawners){ m_vSpawnPoints = spawners; }
	void SetDayNight(CDayNight* dayNight) { m_cpDayNight = dayNight; }

	void AddToRenderer();
	
private:

	std::vector<CMinotaur*> m_vMinotaurs;
	std::vector<XMFLOAT4X4> m_vSpawnPoints;
	CDayNight* m_cpDayNight;
	bool m_bNightTime;
	


};

#endif