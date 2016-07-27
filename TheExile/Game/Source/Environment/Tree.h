/***********************************************
* Filename:  		CTree.h
* Date:      		09/28/2015
* Mod. Date:
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			A cuttable tree that is harvested for wood.
************************************************/
#ifndef CTREE_H
#define CTREE_H

#include "../../../Engine/Object Manager/IObject.h"

class CObjectManager;
class CParticleManager;
class CWood;
class CDayNight;

class CTree : public IObject
{
	CParticleManager* m_cpParticleManager;
	unsigned int m_unParticle;

	bool m_bActive;//for when the loot is active
	bool m_bFalling;

	float m_fFallAngle;
	float m_fFallSpeed;

	CWood* m_cpTheLoot;

	XMFLOAT3 m_f3SpawnPos;

	bool m_bDestroyedDay;
	bool m_bDestroyedNight;
	CDayNight* m_cpDayNight;


public:

	CTree(CObjectManager* _objManager, XMFLOAT3 _pos);
	~CTree();

	void Update();

	void DeActivate();

	virtual void SetWorldMatrix(const XMFLOAT4X4 world);

	void SetFalling(bool _falling) {m_bFalling = _falling;}

	bool GetFalling() { return m_bFalling;}

	void Respawn();

	void SetDayNight(CDayNight* dayNight);

	void AddToRenderer();


};

#endif