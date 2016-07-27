/***********************************************
* Filename:  		ExitKey.h
* Date:      		12/4/2015
* Mod. Date:		12/4/2015
* Mod. Initials:	NH
* Author:    		NIck Hryshko
* Purpose:			This is the key that is needed in order to win the game
************************************************/

#ifndef CEXITKEY_H
#define CEXITKEY_H

#include "../../../Engine/Object Manager/IObject.h"

#define SPAWN_OFFSET_Y 325.0f
#define LIGHT_OFFSET_Y 7.5f
#define BOUNCE_TIME 3.0f
#define PARTICLE_OFFSET_Y 75.0f

class CObjectManager;
class CParticleManager;
struct TPointLight;

class CExitKey : public IObject
{
private:

	bool m_bActive;
	bool m_bBounceUp;
	float m_fBounceTime;
	XMFLOAT3 m_f3SpawnPoint;

	CParticleManager* m_cpParticleManager;
	unsigned int m_nParticleID;

	void BounceEffect();

public:

	CExitKey(XMFLOAT3 position, CObjectManager* objectManager);
	~CExitKey();

	void Update();

	void Activate();
	void DeAactive();

	void AddToRenderer();

};


#endif
