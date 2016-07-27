#ifndef _BOID_H_
#define _BOID_H_

#include "../Object Manager/IObject.h"
#include "SteeringManager.h"

class CBoid : public IObject
{
public:
	CBoid(const char* tag = "Boid") : IObject(tag), m_fMass(0.0f), m_fMaxSpeed(0.0f) ,m_cSteering(CSteeringManager(this)){}
	CBoid(float _mass, float _maxSpeed, const char* tag = "Boid") : IObject(tag), m_fMass(_mass), m_fMaxSpeed(_maxSpeed),m_cSteering(CSteeringManager(this)){}

	virtual ~CBoid(){}
	float GetMass() const { return m_fMass; }
	float GetMaxVelocity() const { return m_fMaxSpeed; }

	void SetMass(float _mass)  { m_fMass = _mass; }
	void SetMaxVelocity(float velocity) { m_fMaxSpeed = velocity; }

	CSteeringManager& Steering() { return m_cSteering; }

	void AddToRenderer();


private:
	float m_fMass;
	float m_fMaxSpeed;
	CSteeringManager m_cSteering;


};


#endif