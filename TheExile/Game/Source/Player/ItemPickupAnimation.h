#pragma once
#include "../../../Engine/Core/stdafx.h"
#include "../../../Engine/Renderer/RenderMesh.h"
#include "../../../Engine/Animation/Mesh.h"
#include "../../../Engine/Object Manager/IObject.h"
class CItemPickupAnimation
{
	CRenderMesh * m_cpRenderMesh = nullptr;
	IObject * m_cpPlayer = nullptr;
	IObject * m_cpCamera = nullptr;

	bool m_bGoingForward = false;

	float m_fCurrentAngle = 0.0f;
	float m_fCurrentTime = 0.0f;

	void End();

public:
	bool m_bAlive = true;

	CItemPickupAnimation(CMesh* _mesh, wstring _texture, IObject * _player, IObject * _camera);
	~CItemPickupAnimation();

	bool Update();
};

