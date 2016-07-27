/***********************************************
* Filename:  		Herb.h
* Date:      		9/13/2015
* Mod. Date: 		9/13/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a herb
************************************************/

#ifndef CHERB_H
#define CHERB_H

#include "../../../Engine/Object Manager/IObject.h"

#define ACTIVE_TIME 25.0f

class CObjectManager;
class CParticleManager;
struct TPointLight;

class CHerb : public IObject
{
private:

	bool m_bActive;
	float m_fActiveTime;//if not picked up
	XMFLOAT3 m_f3SpawnPoint;

	CObjectManager* m_cpObjectManager;

	CParticleManager* m_cpParticleManager;
	unsigned int m_nDroppedItemParticle;

public:

	CHerb(XMFLOAT3 position, CObjectManager* objectManager);

	~CHerb();

	void Update();
	void SetSpawnPosition(XMFLOAT3 pos);
	

	void Activate();
	void DeActivate();

	void AddToRenderer();

};

#endif
