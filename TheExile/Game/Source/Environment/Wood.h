/***********************************************
* Filename:  		Wood.h
* Date:      		9/29/2015
* Mod. Date: 		
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			On ground loot for Wood.
************************************************/

#ifndef WOOD_H
#define WOOD_H

#include "../../../Engine/Object Manager/IObject.h"

#define ACTIVE_TIME 25.0f

class CObjectManager;
class CParticleManager;
struct TPointLight;

class CWood : public IObject
{

	CParticleManager* m_cpParticleManager;
	unsigned int m_unParticle;

	IObject* m_cpParent;

	bool m_bActive;

	float m_fDespawnTime;

public:
	const bool IsActive() const { return m_bActive; }
	CWood(CObjectManager* _objManager, IObject* _parent);
	~CWood();
	virtual void SetWorldMatrix(const XMFLOAT4X4 world);
	void Update();
	void Activate();
	void DeActivate();

	void AddToRenderer();

};


#endif