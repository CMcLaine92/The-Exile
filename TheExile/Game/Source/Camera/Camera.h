
#ifndef CCAMERA_H
#define CCAMERA_H

#include "../../../Engine/Object Manager/IObject.h"

class CObjectManager;

class CCamera : public IObject
{
	bool bCatch = false;
	CObjectManager* cObjectManager;

	float x;
	float y;
	float z;

	bool m_bLocked = false;

	float m_fCameraAngle = 0;
	XMFLOAT4X4 m_mLocal;

	bool m_bShaking = false;
	float m_fDuration = 0.0f;
	float m_fIntensity = 0.0f;

	float m_fJoystickY = 0.0f;

	//Turned off During Cutscenes
	bool m_bActive = true;

	void ApplyShake();

	
public:

	CCamera(float _offSetX = 0.0f, float _offSetY = 0.0f, float _offSetZ = 0.0f, CObjectManager* objManager = nullptr);
	~CCamera() = default;

	void Update() override;
	void Shake(float fDuration = 0.2f, float fIntensity = 10.0f);

	bool& GetLocked() { return m_bLocked; }
	bool& GetActive() { return m_bActive; }
	bool& GetShaking(){ return m_bShaking; }
};

#endif