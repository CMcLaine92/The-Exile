/***********************************************
* Filename:  		Particle.cpp
* Date:      		08/25/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Individual Particle Behaviors
*					based on flyweight data from emitter
************************************************/
#include "Particle.h"
#include "Emitter.h"
#include "../Object Manager/IObject.h"
/*****************************************************************************
* Update()					Updates the Particle and despawns if lifetime is less than 0
*
* Ins:						None
*
* Outs:						None
*
* Returns:					None
*
* Mod. Date:				08/12/2015
* Mod. Initials:			MZ
***************************************************************************/
void CParticle::Update(void)
{
	if (m_bIsActive == false)
	{
		return;
	}
	//UpdateLifeTime
	m_f2Life[1] -= DELTA_TIME();
	if (m_f2Life[1] < 0.0f)
	{
		DespawnParticle();
		return;
	}
	float fVelocity = 0.0f;
	float f3ParentVelocity[3] = { 0.0f, 0.0f, 0.0f };
	IObject* pParent = m_cEmitter->GetParent();
	if (pParent != nullptr)
	{
		XMFLOAT3 f3WorldVelocity = pParent->GetWorldVelocity();
		f3ParentVelocity[0] = f3WorldVelocity.x;
		f3ParentVelocity[1] = f3WorldVelocity.y;
		f3ParentVelocity[2] = f3WorldVelocity.z;
	}
	//Update Velocity + Position
	for (unsigned int i = 0; i < 3; i++)
	{
		fVelocity = m_f3StartVelocity[i] + (m_f3EndVelocity[i] - m_f3StartVelocity[i]) * GetLifetimeRatio();
		fVelocity += f3ParentVelocity[i];
		m_f3CurrentVelocity[i] = fVelocity;
		m_f3Position[i] += m_f3CurrentVelocity[i] * DELTA_TIME();
	}
}

/*****************************************************************************
* CParticle(CEmitter*)		Creates a particle belonging to passed in emitter
*
* Ins:						CEmitter* cEmitter - The emitter that owns the particle
*
* Outs:						None
*
* Returns:					None
*
* Mod. Date:				08/12/2015
* Mod. Initials:			MZ
***************************************************************************/
CParticle::CParticle(CEmitter* cEmitter)
{
	m_bIsActive = false;
	m_cEmitter = cEmitter;
	m_f2Life[0] = 0.0f;
	m_f2Life[1] = 0.0f;
	m_f3Position[0] = 0.0f;
	m_f3Position[1] = -1000.0f;
	m_f3Position[2] = 0.0f;
}
CParticle::CParticle()
{
	m_bIsActive = false;
	m_f2Life[0] = 0.0f;
	m_f2Life[1] = 0.0f;
	m_f3Position[0] = 0.0f;
	m_f3Position[1] = -1000.0f;
	m_f3Position[2] = 0.0f;
}
/*****************************************************************************
* SpawnParticle()			Initializes Particle Values
*
* Ins:						None
*
* Outs:						None
*
* Returns:					None
*
* Mod. Date:				08/12/2015
* Mod. Initials:			MZ
***************************************************************************/
void CParticle::SpawnParticle()
{
	//Initialize Data
	m_bIsActive = true;
	if (m_cEmitter->GetShape() == Shape::Sphere)
	{
		XMFLOAT3 f3UnitVector;
		float fRand = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		fRand = (fRand*2.0f) - 1.0f;
		f3UnitVector.x = fRand;
		fRand = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		fRand = (fRand*2.0f) - 1.0f;
		f3UnitVector.y = fRand;
		fRand = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		fRand = (fRand*2.0f) - 1.0f;
		f3UnitVector.z = fRand;

		float fMagnitude = sqrtf(f3UnitVector.x * f3UnitVector.x + f3UnitVector.y * f3UnitVector.y + f3UnitVector.z * f3UnitVector.z);
		if (fMagnitude != 0.0f)
		{
			f3UnitVector.x /= fMagnitude;
			f3UnitVector.y /= fMagnitude;
			f3UnitVector.z /= fMagnitude;
		}
		m_f3Position[0] = f3UnitVector.x * m_cEmitter->GetScale(0) + m_cEmitter->GetPosition(0);
		m_f3Position[1] = f3UnitVector.y * m_cEmitter->GetScale(1) + m_cEmitter->GetPosition(1);
		m_f3Position[2] = f3UnitVector.z * m_cEmitter->GetScale(2) + m_cEmitter->GetPosition(2);

	}
	for (unsigned int i = 0; i < 3; i++)
	{
		if (m_cEmitter->GetShape() == Shape::Point)
		{
			m_f3Position[i] = m_cEmitter->GetPosition(i);
		}
		else if (m_cEmitter->GetShape() == Shape::Box)
		{
			//Value between 0-1
			float fRand = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			//Value between -.5 - +.5f
			fRand -= .5f;
			fRand *= m_cEmitter->GetScale(i);
			m_f3Position[i] = m_cEmitter->GetPosition(i) + fRand;
		}
		
		float fRandomRatio = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		float fMin = m_cEmitter->GetFlyweight().f3MinStartVelocity[i];
		float fMax = m_cEmitter->GetFlyweight().f3MaxStartVelocity[i];
		m_f3CurrentVelocity[i] = fMin + (fMax - fMin) * fRandomRatio;
		m_f3StartVelocity[i] = m_f3CurrentVelocity[i];
		if (m_cEmitter->GetFlyweight().bConstantVelocity == true)
		{
			m_f3EndVelocity[i] = m_f3StartVelocity[i];
		}
		else
		{
			fRandomRatio = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			fMin = m_cEmitter->GetFlyweight().f3MinEndVelocity[i];
			fMax = m_cEmitter->GetFlyweight().f3MaxEndVelocity[i];
			m_f3EndVelocity[i] = fMin + (fMax - fMin) * fRandomRatio;
		}
	}
	float fRandomRatio = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float fMin = m_cEmitter->GetFlyweight().f2Life[0];
	float fMax = m_cEmitter->GetFlyweight().f2Life[1];
	m_f2Life[0] = fMin + (fMax - fMin) * fRandomRatio;
	m_f2Life[1] = m_f2Life[0];
	fRandomRatio = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	fMin = m_cEmitter->GetFlyweight().f2Size[0];
	fMax = m_cEmitter->GetFlyweight().f2SizeMax[0];
	m_f2Size[0] = fMin + (fMax - fMin) * fRandomRatio;

	fRandomRatio = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	fMin = m_cEmitter->GetFlyweight().f2Size[1];
	fMax = m_cEmitter->GetFlyweight().f2SizeMax[1];
	m_f2Size[1] = fMin + (fMax - fMin) * fRandomRatio;
}

/*****************************************************************************
* DespawnParticle()			Deinitialize Particle Values
*
* Ins:						None
*
* Outs:						None
*
* Returns:					None
*
* Mod. Date:				08/12/2015
* Mod. Initials:			MZ
***************************************************************************/
void CParticle::DespawnParticle()
{
	//Dont Despawn if already inactive
	if (m_bIsActive == false)
	{
		return;
	}
	m_bIsActive = false;
	m_f3Position[1] = -1000.0f;
	m_cEmitter->GetNumActive()--;
}

/*****************************************************************************
* GetLifeTimeRatio()		Gets Ratio of time alive
*
* Ins:						None
*
* Outs:						None
*
* Returns:					float - ratio of time spent alive
*
* Mod. Date:				08/12/2015
* Mod. Initials:			MZ
***************************************************************************/
float CParticle::GetLifetimeRatio()
{
	return 1.0f - (m_f2Life[1] / m_f2Life[0]);
}