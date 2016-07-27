#include "Camera.h"
#include "../../../Engine/Core/MathWrapper.inl"
#include "../../../Engine/Renderer/Renderer.h"
#include "../../../Engine/Core/Input/InputManager.h"
#include "../../../Engine/Core/Collision/Collision.h"
#include "../../../Engine/Object Manager/QuadTree.h"
#include "../../../Engine/Object Manager/ObjectManager.h"
#include "../../../Engine/Core/State Machine/StateMachine.h"
#include "../../../Engine/Core/State Machine/States/GameplayState.h"

#define XMF3DotProduct(a,b) (a.x*b.x + a.y*b.y + a.z*b.z)
#define PLAYER_CENTER 90.0f

CCamera::CCamera(float _offSetX, float _offSetY, float _offSetZ, CObjectManager* objManager) : IObject("Camera")
{
	XMMATRIX mMat = XMMatrixIdentity();
	mMat = XMMatrixTranslation(_offSetX, _offSetY, _offSetZ);
	XMStoreFloat4x4(&m_mLocal, mMat);
	cObjectManager = objManager;

	x = _offSetX;
	y = _offSetY;
	z = _offSetZ;
}

void CCamera::Update()
{
	if (m_bActive == false)
		return;

	//if (!bCatch)
	//{
	//	unsigned int nStateType = CStateMachine::GetInstance()->GetCurrentState()->GetStateType();
	//	
	//	if (nStateType == eGAMEPLAY)
	//	{
	//		cObjectManager = ((CGameplayState*)CStateMachine::GetInstance()->GetCurrentState())->GetObjectManager();
	//		bCatch = true;
	//		//m_vDoors = cObjectManager->FindObjectsWithTag("Door", CObjectManager::Static);
	//	}
	//}

	// Timer Update //
	if (m_bShaking)
	{
		m_fDuration -= DELTA_TIME();

		if (m_fDuration <= 0)
			m_bShaking = false;
	}

	// Rotation //
	CInputManager* cInput = CInputManager::GetInstance();
	XMFLOAT4X4 f44ParentMatrix = m_ipParent->GetWorldMatrix();
	XMMATRIX m_mParent = XMLoadFloat4x4(&f44ParentMatrix);
	XMFLOAT4X4 mParentMatrix = m_ipParent->GetWorldMatrix();
	mParentMatrix.m[3][1] += PLAYER_CENTER;

	if (cInput->GetControllerBeingUsed() == true)
	{
		if (cInput->IsAxisDown("Right JS Pos Y"))
		{
			m_fJoystickY = cInput->GetRegisteredAxisValue("Right JS Pos Y");
			//m_fJoystickY += 1.0f;
		}
		if (cInput->IsAxisDown("Right JS Neg Y"))
		{
			m_fJoystickY = cInput->GetRegisteredAxisValue("Right JS Neg Y");
			//m_fJoystickY -= 1.0f;
		}

		if (m_fJoystickY && !m_bLocked)
		{
			if (!bInverse)
				m_fJoystickY = -m_fJoystickY;

			//m_fJoystickY *= SENSITIVITY;
			m_fJoystickY = (m_fJoystickY / AXIS_DEADZONE_LERP_Y_VALUE) * SENSITIVITY;

			if (m_fCameraAngle + m_fJoystickY < 30 &&
				m_fCameraAngle + m_fJoystickY > -30)
			{
				// Update Angle //
				m_fCameraAngle += m_fJoystickY;

				// Rotate On X-Axis //
				m_mLocal.m[3][1] -= y; // Rotate as if directly in line with it
				RotateGlobalX(m_mLocal, m_fJoystickY);
				m_mLocal.m[3][1] += y;
			}
		}

		XMStoreFloat4x4(&m_mWorld, XMLoadFloat4x4(&m_mLocal) * m_mParent);
	}
	else
	{
		float fDeltaY = cInput->GetMouseDirection().y;

		if (fDeltaY && !m_bLocked)
		{
			if (bInverse)
				fDeltaY = -fDeltaY;

			fDeltaY *= SENSITIVITY;

			if (m_fCameraAngle + fDeltaY < 30 &&
				m_fCameraAngle + fDeltaY > -30)
			{
				// Update Angle //
				m_fCameraAngle += fDeltaY;

				// Rotate On X-Axis //
				m_mLocal.m[3][1] -= y; // Rotate as if directly in line with it
				RotateGlobalX(m_mLocal, fDeltaY);
				m_mLocal.m[3][1] += y;
			}
		}

		XMStoreFloat4x4(&m_mWorld, XMLoadFloat4x4(&m_mLocal) * m_mParent);
	}



	if (cObjectManager)
	{
		TRay tRayStruct;
		/*float3 start = { m_mWorld.m[3][0], m_mWorld.m[3][1] - 1.0f, m_mWorld.m[3][2] };
		float3 end = { mParentMatrix.m[3][0], mParentMatrix.m[3][1], mParentMatrix.m[3][2] };
		GRAPHICS->DrawLine(start, end, { 1, 1, 1 });*/
		
		tRayStruct.f3Direction.x = m_mWorld.m[3][0] - mParentMatrix.m[3][0];
		tRayStruct.f3Direction.y = m_mWorld.m[3][1] - mParentMatrix.m[3][1];
		tRayStruct.f3Direction.z = m_mWorld.m[3][2] - mParentMatrix.m[3][2];
		tRayStruct.f3Start.x = mParentMatrix.m[3][0];
		tRayStruct.f3Start.y = mParentMatrix.m[3][1];
		tRayStruct.f3Start.z = mParentMatrix.m[3][2];
		tRayStruct.fDistance = sqrtf(XMF3DotProduct(tRayStruct.f3Direction, tRayStruct.f3Direction));
		XMStoreFloat3(&tRayStruct.f3Direction, XMVector3Normalize(XMLoadFloat3(&tRayStruct.f3Direction)));
		
		float fHitTime = 0.0f;
		float fXMin = min(m_mWorld.m[3][0], mParentMatrix.m[3][0]);
		float fXMax = max(m_mWorld.m[3][0], mParentMatrix.m[3][0]);
		float fZMin = min(m_mWorld.m[3][2], mParentMatrix.m[3][2]);
		float fZMax = max(m_mWorld.m[3][2], mParentMatrix.m[3][2]);
		std::vector<IObject*> cObjectList = cObjectManager->GetQuadTree()->GetNearbyObjects(fXMin, fZMin, fXMax, fZMax);
		
	/*	for (unsigned int i = 0; i < m_vDoors.size(); i++)
			cObjectList.push_back(m_vDoors[i]);
		*/
		if (LineSegmentIntersection(tRayStruct, cObjectList, fHitTime))
		{
			fHitTime -= 1.0f;
			float3 fNewDirection = { tRayStruct.f3Direction.x * fHitTime, tRayStruct.f3Direction.y * fHitTime, tRayStruct.f3Direction.z * fHitTime };
			float3 fPlayerPosition = { mParentMatrix.m[3][0], mParentMatrix.m[3][1], mParentMatrix.m[3][2] };
			m_mWorld.m[3][0] = fPlayerPosition.x + fNewDirection.x;
			m_mWorld.m[3][1] = fPlayerPosition.y + fNewDirection.y;
			m_mWorld.m[3][2] = fPlayerPosition.z + fNewDirection.z;
		}
	}

	if (m_bShaking && !m_bLocked)
		ApplyShake();

	GRAPHICS->GetViewMatrix() = m_mWorld;

}

void CCamera::Shake(float fDuration, float fIntensity)
{
	m_bShaking = true;
	m_fDuration = fDuration;
	m_fIntensity = fIntensity;
}

void CCamera::ApplyShake()
{
	float fShakeX = ((rand() % 20000) - 10000) / 10000.0f * m_fIntensity;
	float fShakeY = ((rand() % 20000) - 10000) / 10000.0f * m_fIntensity;
	float fShakeZ = ((rand() % 20000) - 10000) / 10000.0f * m_fIntensity;

	TranslateLocalX(m_mWorld, fShakeX);
	TranslateLocalY(m_mWorld, fShakeY);
	TranslateLocalZ(m_mWorld, fShakeZ);
}