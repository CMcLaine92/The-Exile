#ifndef _STEERING_MANAGER_H_
#define _STEERING_MANAGER_H_

class CBoid;

class CSteeringManager
{

public:
	CSteeringManager(CBoid* _boid = nullptr);
	//CSteeringManager();
	CSteeringManager(const CSteeringManager& other);
	CSteeringManager& operator = (const CSteeringManager& other);



	void Seek(XMFLOAT3 target, float slowDist = 50.0f);
	void Flee(XMFLOAT3 target);
	void Wander();

	void Update(bool reverse = false, float scale = 1.0f);
	void Reset();

	void SetBoid(CBoid* _boid);
private:

	XMFLOAT3 DoSeek(XMFLOAT3 target, float slowDist = 50.0f);
	XMFLOAT3 DoFlee(XMFLOAT3 target);
	XMFLOAT3 DoWander();
	void Truncate(XMFLOAT3& vector, float maxLength);






private:

	float3 m_vSteering;
	CBoid* m_pBoid;


};



#endif