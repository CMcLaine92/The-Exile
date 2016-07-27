/***********************************************
* Filename:  		Gunpowder.h
* Date:      		9/29/2015
* Mod. Date: 		
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			On ground loot for Gunpowder.
************************************************/

#ifndef Gunpowder_H
#define Gunpowder_H

#include "../../../Engine/Object Manager/IObject.h"

class CObjectManager;
class CParticleManager;
struct TPointLight;

class CGunpowder : public IObject
{
	CParticleManager* m_cpParticleManager;
	unsigned int m_unParticle;

	IObject* m_cpParent;

	bool m_bActive;

	float m_fDespawnTime;

public:
	CGunpowder(CObjectManager* _objManager, IObject* _parent);
	~CGunpowder();

	void Update();
	void Activate();
	void DeActivate();

	void AddToRenderer();
};


#endif