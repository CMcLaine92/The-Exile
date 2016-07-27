/***********************************************
* Filename:  		CrackedWall.h
* Date:      		9/2/2015
* Mod. Date: 		9/2/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a cracked wall
************************************************/

#ifndef CCRACKEDWALL_H
#define CCRACKEDWALL_H

#include "../../../Engine/Object Manager/PrefabObject.h"

class CRenderMesh;
class CHUDMesh;
class CObjectManager;
class CParticleManager;

class CCrackedWall : public CPrefabObject
{

	CParticleManager* m_cpParticleManager;
	vector<unsigned int> m_vunParticleID;
	int m_unCurrParticleID;
	vector<CRenderMesh*> m_cpIndicatorBomb;
	CHUDMesh* m_cpTextFrame;
	CHUDMesh* m_cpControllerPopUp;

	bool m_bActive;
	bool m_bInitialized;
	float m_fFuseTimer;

	void BuildTextFrame();
	void SetBombPosition();
	void SetupParticle();
public:

	CCrackedWall(string szTag, CObjectManager* objManager);
	~CCrackedWall();

	void Update();
	void Initialize();

	void DrawAvailable();
	void DrawUnAvailable();


	void Activate(bool _audio, int curID);
	bool IsActive() const {return m_bActive;}

	void SetActive(bool _state) { m_bActive = _state;}

	void AddToRenderer();

};

#endif