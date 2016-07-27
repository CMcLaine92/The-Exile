/***********************************************
* Filename:  		Bush.h
* Date:      		9/2/2015
* Mod. Date: 		9/12/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a bush
************************************************/

#ifndef CBUSH_H
#define CBUSH_H

#include "../../../Engine/Object Manager/IObject.h"

class CObjectManager;
class CParticleManager;
class CHerb;
class CDayNight;

class CBush : public IObject
{
private:

	bool m_bActive;
	bool m_bDestroyedDay;//true if it was destroyed during the day time | false not destroyed
	bool m_bDestroyedNight;//true if it was destroyed during the night time | false not destroyed

	CObjectManager* m_cpObjectManager;

	CParticleManager * m_cpParticleManager;
	unsigned int m_vnBushParticle;

	AudioSystemWwise* m_cpBushAudio;

	XMFLOAT3 m_f3SpawnPos;

	CHerb* m_cpHerb;

	CDayNight* m_cpDayNight;

public:

	CBush(CObjectManager* objectManager, XMFLOAT3 position);

	~CBush();

	void Update();

	void DestroyBush();

	virtual void SetWorldMatrix(const XMFLOAT4X4 newWorld);

	void RespawnBush();

	void SetDayNight(CDayNight* dayNight);

	void AddToRenderer();

};

#endif
