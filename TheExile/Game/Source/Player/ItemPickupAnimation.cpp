#include "ItemPickupAnimation.h"

#define MAX_TIME 2.0f
#define ROTATION_ANGLE 180.0f

CItemPickupAnimation::CItemPickupAnimation(CMesh* _mesh, wstring _texture, IObject * _player, IObject * _camera)
{
	m_cpRenderMesh = new CRenderMesh(_mesh, GRAPHICS->GetStandardVS(), GRAPHICS->GetStandardPS(), nullptr, nullptr, nullptr, _texture.c_str());
	m_cpPlayer = _player;
	m_cpCamera = _camera;
	m_fCurrentTime = MAX_TIME / 2.0f;

	m_cpRenderMesh->SetEdgeGlow(true);
	m_cpRenderMesh->GetPositionMatrix() = _player->GetWorldMatrix();

	GRAPHICS->AddRenderMesh(m_cpRenderMesh);
}


CItemPickupAnimation::~CItemPickupAnimation()
{
	SAFE_DELETE(m_cpRenderMesh);
}

bool CItemPickupAnimation::Update()
{
	assert("Pickup animation being updated after it's RenderMesh has been deleted!" && m_cpRenderMesh);

	XMFLOAT3 cameraPos = *m_cpCamera->GetPosition();
	XMFLOAT3 playerPos = *m_cpPlayer->GetPosition();
	XMFLOAT4X4 mat = m_cpRenderMesh->GetPositionMatrix();

	m_fCurrentAngle = ROTATION_ANGLE * DELTA_TIME();
	m_fCurrentTime -= DELTA_TIME();

	// Set Position To World Center // Global rotation becomes same as local
	mat.m[3][0] = 0;
	mat.m[3][1] = 0;
	mat.m[3][2] = 0;
	mat.m[3][3] = 0;

	// Rotate Globally On Y-Axis //
	float fAngleY = RADIANS(m_fCurrentAngle);
	XMMATRIX d3dRotateY = XMMatrixRotationY(fAngleY);
	XMStoreFloat4x4(&mat, XMMatrixMultiply(XMLoadFloat4x4(&mat), d3dRotateY));

	XMVECTOR newPos = { 0.0f, 0.0f, 0.0f };
	XMVECTOR playerVec = XMLoadFloat3(&playerPos);
	XMVECTOR cameraVec = XMLoadFloat3(&cameraPos);

	playerVec = playerVec + (XMVector2Normalize(cameraVec - playerVec) * 15.0f);
	playerVec.m128_f32[1] += 90.0f;

	cameraVec = cameraVec + (XMVector2Normalize(playerVec - cameraVec) * 180.0f);
	cameraVec.m128_f32[1] -= 35.0f;

	if (!m_bGoingForward)
	{
		newPos = (playerVec + (((MAX_TIME / 2.0f) - m_fCurrentTime) / (MAX_TIME / 2.0f)) * (cameraVec - playerVec));
	}
	else
	{
		newPos = (cameraVec + (((MAX_TIME / 2.0f) - m_fCurrentTime) / (MAX_TIME / 2.0f)) * (playerVec - cameraVec));
	}

	mat.m[3][0] = newPos.m128_f32[0];
	mat.m[3][1] = newPos.m128_f32[1];
	mat.m[3][2] = newPos.m128_f32[2];
	mat.m[3][3] = 1;

	m_cpRenderMesh->GetPositionMatrix() = mat;

	if (m_fCurrentTime < 0.0f && !m_bGoingForward)
	{
		m_fCurrentTime = MAX_TIME / 2.0f;
		m_bGoingForward = true;
	}
	else if (m_fCurrentTime < 0.0f && m_bGoingForward)
	{
		End();
		return false;
	}
	
	return true;
}

void CItemPickupAnimation::End()
{
	GRAPHICS->RemoveRenderMesh(m_cpRenderMesh);
	m_bAlive = false;
}
