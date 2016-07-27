/***********************************************
* Filename:  		Torch.h
* Date:      		9/2/2015
* Mod. Date: 		9/10/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a torch
************************************************/

#ifndef CTORCH_H
#define CTORCH_H

#include "../../../Engine/Object Manager/IObject.h"

#define ACTIVATE_DISTANCE 750
#define PARTICLE_Y_OFFSET 60.0f
#define PARTICLE_XZ_OFFSET 30.0f
#define TORCH_TILT_ANGLE 25.0f
#define TORCH_XZ_TRANSLATE 10.0f

class IObject;
class CParticleManager;
class CObjectManager;

class CTorch : public IObject
{
private:

	CObjectManager* m_cpObjectManager;

	CParticleManager * m_cpParticleManager;
	unsigned int m_vnFireParticle;

	bool m_bFlameON;
	float m_fXOffset;
	float m_fZOffset;

	AudioSystemWwise* m_cpTorchAudio;
	XMFLOAT3 tilt;
public:

	CTorch(CObjectManager* objectManager, XMFLOAT3 position, XMFLOAT3 tiltDirection);
	~CTorch();

	void Update();
	void SetWorldMatrix(const XMFLOAT4X4 world);
	void ActivateTorch();

	void AddToRenderer();

	bool& GetFlameOn() { return m_bFlameON; }
};

#endif