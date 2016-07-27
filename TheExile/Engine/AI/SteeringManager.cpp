#include "SteeringManager.h"
#include "Boid.h"

#define MAX_FORCE 2.5f

CSteeringManager::CSteeringManager(CBoid* _boid)
{
	m_pBoid = _boid;
	m_vSteering = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

CSteeringManager::CSteeringManager(const CSteeringManager& other)
{
	*this = other;
}
CSteeringManager& CSteeringManager::operator = (const CSteeringManager& other)
{
	if (this != &other)
	{
		m_vSteering = other.m_vSteering;
		m_pBoid = other.m_pBoid;
	}
	return *this;
}
void CSteeringManager::Seek(XMFLOAT3 target, float slowDist)
{
	XMVECTOR steering = XMLoadFloat3(&m_vSteering);
	XMVECTOR force = XMLoadFloat3(&DoSeek(target, slowDist));
	steering += force;
	XMStoreFloat3(&m_vSteering, steering);

}

void CSteeringManager::Flee(XMFLOAT3 target)
{
	XMVECTOR steering = XMLoadFloat3(&m_vSteering);
	XMVECTOR force = XMLoadFloat3(&DoFlee(target));
	steering += force;
	XMStoreFloat3(&m_vSteering, steering);
}

void CSteeringManager::Wander()
{
	XMVECTOR steering = XMLoadFloat3(&m_vSteering);
	XMVECTOR force = XMLoadFloat3(&DoWander());
	steering += force;
	XMStoreFloat3(&m_vSteering, steering);
}

void CSteeringManager::Update(bool reverse, float scale)
{
	XMFLOAT3 worldUp = XMFLOAT3(0, 1, 0);
	XMVECTOR newX, newY, newZ, globalY;
	globalY = XMLoadFloat3(&worldUp);

	Truncate(m_vSteering, MAX_FORCE);
	XMVECTOR steering = XMLoadFloat3(&m_vSteering);
	XMVECTOR velocity = XMLoadFloat3(m_pBoid->GetZAxis());
	steering *= (1 / m_pBoid->GetMass()); 

	velocity += steering;
	XMFLOAT3 newVelocity;
	XMStoreFloat3(&newVelocity, velocity);

	Truncate(newVelocity, m_pBoid->GetMaxVelocity());

	newZ = XMLoadFloat3(&newVelocity);
	newZ = XMVector3Normalize(newZ);

	newX = XMVector3Cross(globalY, newZ);
	newY = XMVector3Cross(newZ, newX);

	newX = XMVector3Normalize(newX);
	newY = XMVector3Normalize(newY);
	newZ = XMVector3Normalize(newZ);

	XMFLOAT3 xAxis, yAxis, zAxis;
	XMStoreFloat3(&xAxis, newX);
	XMStoreFloat3(&yAxis, newY);
	XMStoreFloat3(&zAxis, newZ);

	xAxis.y = 0.0f;
	yAxis.x = 0.0f;
	yAxis.z = 0.0f;
	zAxis.y = 0.0f;

	m_pBoid->SetXAxis(xAxis);
	m_pBoid->SetYAxis(yAxis);
	m_pBoid->SetZAxis(zAxis);
	m_pBoid->Scale(scale, scale, scale);
	if (reverse)
	{
		newVelocity.x = -newVelocity.x;
		newVelocity.x = -newVelocity.x;
		newVelocity.x = -newVelocity.x;
	}

	m_pBoid->SetWorldVelocity(newVelocity); 
	Reset();

	

}

void CSteeringManager::Truncate(XMFLOAT3& vector, float maxLength)
{
	XMVECTOR steering = XMLoadFloat3(&vector);
	/*float len = XMVector3Length(steering).m128_f32[0];
	if ( len > maxLength)
	{*/
		steering = XMVector3Normalize(steering);
		steering *= maxLength;
	//}
	XMStoreFloat3(&vector, steering);

}

void CSteeringManager::Reset()
{
	m_vSteering = XMFLOAT3(0.0f, 0.0f, 0.0f); 
}


XMFLOAT3 CSteeringManager::DoSeek(XMFLOAT3 target, float slowDist)
{
	XMFLOAT3 force;
	XMVECTOR vForce;
	XMVECTOR desired;
	XMVECTOR currVelocity;

	currVelocity = XMLoadFloat3(m_pBoid->GetZAxis());

	float dist;

	desired = XMLoadFloat3(&target) - XMLoadFloat3(m_pBoid->GetPosition());
	dist = XMVector3Length(desired).m128_f32[0];
	desired = XMVector3Normalize(desired);

	if (dist <= slowDist)
		desired *= m_pBoid->GetMaxVelocity() * (dist / slowDist);
	else
		desired *= m_pBoid->GetMaxVelocity();

	vForce = desired - currVelocity;

	XMStoreFloat3(&force, vForce);
	return force;
}
XMFLOAT3 CSteeringManager::DoFlee(XMFLOAT3 target)
{
	return XMFLOAT3(0.0f, 0.0f, 0.0f);
}
XMFLOAT3 CSteeringManager::DoWander()
{
	return XMFLOAT3(0.0f, 0.0f, 0.0f);

}

