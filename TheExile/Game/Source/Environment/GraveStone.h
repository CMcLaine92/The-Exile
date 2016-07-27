/***********************************************
* Filename:  		GraveStone.h
* Date:      		9/29/2015
* Mod. Date: 		
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			On ground loot for GraveStone.
************************************************/

#ifndef GraveStone_H
#define GraveStone_H

#include "../../../Engine/Object Manager/IObject.h"

class CObjectManager;
class CParticleManager;
struct TPointLight;

class CGraveStone : public IObject
{
	map<int, int> m_mDropedStock;

	CParticleManager* m_cpParticleManager;
	unsigned int m_unParticle;

	IObject* m_cpParent;

	bool m_bActive;

	float m_fDespawnTime;

	unsigned int m_unID;

public:
	CGraveStone(CObjectManager* _objManager, IObject* _parent);
	~CGraveStone();

	void Update();
	void Activate();
	void DeActivate();

	void SetDropedStock(map<int, int> _dropped) {m_mDropedStock = _dropped;}
	map<int,int> GetDropedStock() {return m_mDropedStock;}

	bool GetActive();
	unsigned int GetID();
	void SetID(unsigned int id);

	void AddToRenderer();

};


#endif