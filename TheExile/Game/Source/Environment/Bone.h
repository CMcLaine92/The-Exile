/***********************************************
* Filename:  		Bone.h
* Date:      		9/29/2015
* Mod. Date: 		
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			On ground loot for Bone.
************************************************/

#ifndef Bone_H
#define Bone_H

#include "../../../Engine/Object Manager/IObject.h"

class CObjectManager;
class CParticleManager;
struct TPointLight;

class CBone : public IObject
{

	CParticleManager* m_cpParticleManager;
	unsigned int m_unParticle;

	IObject* m_cpParent;

	bool m_bActive;

	float m_fDespawnTime;

public:
	CBone(CObjectManager* _objManager, IObject* _parent);
	~CBone();

	void Update();
	void Activate();
	void DeActivate();

	bool IsActive() {return m_bActive;}

	void AddToRenderer();
};


#endif