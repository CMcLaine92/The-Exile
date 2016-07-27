#include "CaveSpawn.h"
#include "../../../Engine/Asset Manager/AssetManager.h"


CCaveSpawn::CCaveSpawn(XMFLOAT3 position) : IObject("CaveSpawn")
{
	CMesh* mesh = nullptr;
	mesh = CAssetManager::GetInstance()->GetPrefabMesh("OrcSpawn");
	CMesh::TAABB aabb = mesh->GetAABBs()[0];
	AddCollider(new CCollider(false, new CAABB(aabb.center, aabb.extents), true, true));
	SetRenderMesh(new CRenderMesh(mesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/EnemyCaveGate.dds"));
	GRAPHICS->AddRenderMesh(m_cpRenderMesh);
}
