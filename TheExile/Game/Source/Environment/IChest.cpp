#include "../../../Engine/Core/stdafx.h"
#include "IChest.h"
#include "../../../Engine/Core/Collision/AABB.h"
#include "../../../Engine/Core/Collision/Collider.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Core/Messages/AddObjectMessage.h"

IChest::IChest(EChestType _eType, EDirection _direction, XMFLOAT3 _pos, string _szTag) : IObject(_szTag)
{
	float angle = 0.0f;
	switch (_direction)
	{
	case IChest::eNorth:
		angle = 180.0f;
		break;
	case IChest::eEast:
		angle = 90.0f;
		break;
	case IChest::eWest:
		angle = 270.0f;
		break;
	}

	switch (_eType)
	{
	case eWeaponChest:
		m_cpRenderMesh = new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("Chest"), GRAPHICS->GetStandardVS(), GRAPHICS->GetStandardPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/ShieldChestDiffuse.dds");
		AddCollider(new CCollider(true, Bounds::AABB, CAssetManager::GetInstance()->GetPrefabMesh("Chest")->GetVertices(), true, false));
		break;
	case eHealthChest:
		m_cpRenderMesh = new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("HealthChest"), GRAPHICS->GetStandardVS(), GRAPHICS->GetStandardPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/HealthChestDiffuse.dds");
		AddCollider(new CCollider(true, Bounds::AABB, CAssetManager::GetInstance()->GetPrefabMesh("HealthChest")->GetVertices(), true, false));
		break;
	case eShieldChest:
	case eItemChest:
		m_cpRenderMesh = new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("Loot_Bottom"), GRAPHICS->GetStandardVS(), GRAPHICS->GetStandardPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/WeaponChestDiffuse.dds");
		AddCollider(new CCollider(true, Bounds::AABB, CAssetManager::GetInstance()->GetPrefabMesh("Loot_Bottom")->GetVertices(), true, false));
		break;
	}

	// Store Position //
	float fMatrixPosX = m_mWorld.m[3][0];
	float fMatrixPosY = m_mWorld.m[3][1];
	float fMatrixPosZ = m_mWorld.m[3][2];
	float fMatrixPosW = m_mWorld.m[3][3];

	// Set Position To World Center // Global rotation becomes same as local
	m_mWorld.m[3][0] = 0;
	m_mWorld.m[3][1] = 0;
	m_mWorld.m[3][2] = 0;
	m_mWorld.m[3][3] = 0;

	// Rotate Globally On Y-Axis //
	float fAngleY = RADIANS(angle);
	XMMATRIX d3dRotateY = XMMatrixRotationY(fAngleY);
	XMStoreFloat4x4(&m_mWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_mWorld), d3dRotateY));

	// Restore Position //
	m_mWorld.m[3][0] = fMatrixPosX;
	m_mWorld.m[3][1] = fMatrixPosY;
	m_mWorld.m[3][2] = fMatrixPosZ;
	m_mWorld.m[3][3] = fMatrixPosW;

	//CAddObjectMessage * msg = new CAddObjectMessage(this, CObjectManager::Dynamic);
	//msg->Send();

	SetPosition(_pos);
	((CAABB*)GetColliders()[0]->GetBounds())->SetCenter(*GetPosition());

	GRAPHICS->AddRenderMesh(m_cpRenderMesh);
}


IChest::~IChest()
{
}

void IChest::Update()
{
#if _DEBUG
	DEBUG_UTIL::DrawAABBCollider(((CAABB*)GetColliders()[0]->GetBounds())->Max(), ((CAABB*)GetColliders()[0]->GetBounds())->Min());
#endif
}
