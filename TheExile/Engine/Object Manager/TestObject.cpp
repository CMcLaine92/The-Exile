
#include "TestObject.h"
#include "../../Engine/Asset Manager/AssetManager.h"
#include "../../Engine/Core/Collision/Collision.h"
#include "../../Engine/Core/Collision/Collider.h"
#include "../../Engine/Core/Collision/Capsule.h"
#include "../../Engine/Core/Collision/AABB.h"

//this is the prop class
CTestObject::CTestObject(string _szTag, XMFLOAT3 position, unsigned int propID) : IObject(_szTag)
{
	//set position
	m_mWorld =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		position.x, position.y, position.z, 1
	};

	switch (propID)
	{
	case CTestObject::eTree:
	{
		//set rendermesh
		CMesh* tempMesh = CAssetManager::GetInstance()->GetPrefabMesh("Tree_Prop");
		SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Tree_Prop_Diffuse.dds"));
		m_cpRenderMesh->GetTransparent() = true;
		m_cpRenderMesh->GetTransparencyOverride() = 1.0f;

		//set up collider
		AddCollider(new CCollider(true, new CAABB(float3(m_mWorld._41, 90.0f, m_mWorld._43), float3(90.0f, 90.0f, 90.0f)), false, false));

		CCapsule* treeCapsule = new CCapsule(position, XMFLOAT3(position.x, position.y + 500.0f, position.z), 275.0f);
		AddCollider(new CCollider(false, treeCapsule));
	}
	break;
	//case CTestObject::eBoulder:
	//{
	//	CMesh* tempMesh = CAssetManager::GetInstance()->GetPrefabMesh("Boulder_Prop");
	//	SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Boulder_Prop_Diffuse.dds"));
	//
	//	//set up collider
	//	CCapsule* boulderCapsule = new CCapsule(position, XMFLOAT3(position.x, position.y + 300.0f, position.z), 150.0f);
	//	AddCollider(new CCollider(false, boulderCapsule));
	//}
	//break;
	//case CTestObject::eTent:
	//{
	//	CMesh* tempMesh = CAssetManager::GetInstance()->GetPrefabMesh("Tent_Prop");
	//	SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Tent_Prop_Diffuse.dds"));
	//
	//	//set up collider
	//	AddCollider(new CCollider(false, new CAABB(position, XMFLOAT3(300.0f, 500.0f, 300.0f)), false, false));
	//}
	//	break;
	default:
		break;
	}

	GRAPHICS->AddRenderMesh(m_cpRenderMesh);

}

CTestObject::~CTestObject()
{

}

void CTestObject::Update()
{
	
}
