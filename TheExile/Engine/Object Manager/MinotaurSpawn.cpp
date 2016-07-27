#include "MinotaurSpawn.h"
#include "../Renderer/Renderer.h"
#include "../Particles/tinyxml/tinyxml.h"
#include "../Asset Manager/AssetManager.h"
#include "../../Game/Source/Environment/Torch.h"
#include "../../Game/Source/Environment/Bush.h"
#include "../../Game/Source/Environment/Tree.h"
#include "../../Game/Source/Environment/Torch.h"
#include "../../Game/Source/Traps/FireTrap.h"
#include "../../Game/Source/Traps/SpikeTrap.h"
#include "../../Game/Source/Traps/SpinningBlade.h"
#include "../../Engine/Object Manager/DoorObject.h"
#include "../../Engine/Object Manager/ExitDoorObject.h"
#include "../../Game/Source/Environment/CaveSpawn.h"
#include "../../Game/Source/Environment/HealthUpgradeChest.h"
#include "../../Game/Source/Environment/HeavyWeaponChest.h"
#include "../../Game/Source/Environment/LightWeaponChest.h"
#include "../../Game/Source/Environment/Bridge.h"
#include "../Core/Messages/AddObjectMessage.h"
#include "../Core/Messages/RemoveObjectMessage.h"
#include "../../Game/Source/Environment/DayNight.h"
#include "../Renderer/RenderMesh.h"
#include "../../Game/Source/Environment/ItemCrate.h"


CMinotaurSpawn::CMinotaurSpawn()
{
	isDay = true;
	m_pDayNight = nullptr;
	isMoving = false;
	m_eDirection = eDefault;
	startLeftPos = float3(0.0f, 0.0f, 0.0f);
	endLeftPos = float3(0.0f, 0.0f, 0.0f);

	startRightPos = float3(0.0f, 0.0f, 0.0f);
	endRightPos = float3(0.0f, 0.0f, 0.0f);




}
XMFLOAT3 CMinotaurSpawn::LerpPosition(XMFLOAT3 _start, XMFLOAT3 _end, float lamda)
{
	XMFLOAT3 result;

	XMVECTOR start = XMLoadFloat3(&_start);
	XMVECTOR end = XMLoadFloat3(&_end);

	XMStoreFloat3(&result, XMVectorLerp(start, end, lamda));
	return result;


}

CMinotaurSpawn::CMinotaurSpawn(const char* filePath, CObjectManager* objManager, CDayNight* dayNight)
{
	string name = filePath;
	string dir = name.substr(name.size() - 1);
	switch (*dir.c_str())
	{
	case 'N':
		m_eDirection = eNorth;
		break;
	case 'S':
		m_eDirection = eSouth;
		break;
	case 'E':
		m_eDirection = eEast;
		break;
	case 'W':
		m_eDirection = eWest;
		break;
	}
	m_pDayNight = dayNight;
	SetTag("MinotaurSpawn");
	LoadFromXML(filePath, objManager);
	this->CreateRenderMeshes();
	CreateSceneBounds();
	this->AddColliders();
	isDay = true;
	isMoving = false;
	LerpRatio = 0.0f;


}
CMinotaurSpawn::~CMinotaurSpawn()
{
	m_pDayNight = nullptr;
}
void CMinotaurSpawn::Update()
{
	//CAABB* leftCollider = ((CAABB*)(m_pvColliders[LEFT_COLLIDER_INDEX]->GetBounds()));
	//CAABB* rightCollider = ((CAABB*)(m_pvColliders[RIGHT_COLLIDER_INDEX]->GetBounds()));

	//DEBUG_UTIL::DrawAABBCollider(leftCollider->Min(), leftCollider->Max());
	//DEBUG_UTIL::DrawAABBCollider(rightCollider->Min(), rightCollider->Max(), XMFLOAT3(1.0f, 1.0f, 0.0f));

	if (m_pDayNight->IsNight())
	{
		if (isDay == true)
		{
			isDay = false;
			LerpRatio = 0.0f;
			isMoving = true;
			std::swap(startLeftPos, endLeftPos);
			std::swap(startRightPos, endRightPos);

		}

	}
	else
	{
		if (isDay == false)
		{
			LerpRatio = 0.0f;

			isDay = true;
			isMoving = true;
			std::swap(startLeftPos, endLeftPos);
			std::swap(startRightPos, endRightPos);
		}
	}


	if (isMoving == true)
	{
		LerpRatio += DELTA_TIME();
		if (LerpRatio > MAX_OPEN_TIME)
		{
			LerpRatio = MAX_OPEN_TIME;
			isMoving = false;
		}
		float R = (LerpRatio / MAX_OPEN_TIME);

		XMFLOAT3 finalLeftPos = LerpPosition(startLeftPos, endLeftPos, R);
		XMFLOAT3 finalRightPos = LerpPosition(startRightPos, endRightPos, R);

		m_vRenderMeshes[LEFT_MESH_INDEX]->SetPosition(finalLeftPos);
		m_vRenderMeshes[RIGHT_MESH_INDEX]->SetPosition(finalRightPos);

		CAABB* leftCollider = ((CAABB*)(m_pvColliders[LEFT_COLLIDER_INDEX]->GetBounds()));
		CAABB* rightCollider = ((CAABB*)(m_pvColliders[RIGHT_COLLIDER_INDEX]->GetBounds()));

		XMFLOAT3 leftCenter = leftCollider->GetCenter();
		leftCenter.x = finalLeftPos.x;
		leftCenter.z = finalLeftPos.z;

		XMFLOAT3 rightCenter = rightCollider->GetCenter();
		rightCenter.x = finalRightPos.x;
		rightCenter.z = finalRightPos.z;


		leftCollider->SetCenter(leftCenter);
		rightCollider->SetCenter(rightCenter);
	}

	
}

bool CMinotaurSpawn::LoadFromXML(const char* prefabName, CObjectManager* objManager)
{
	TiXmlDocument doc;

	string path = "../Game/Assets/Scripts/Prefabs/";
	path += prefabName;
	path += ".xml";


	if (doc.LoadFile(path.c_str(), TiXmlEncoding::TIXML_ENCODING_UTF8))
	{
		CAssetManager* pAssetManager = CAssetManager::GetInstance();
		TiXmlElement * root = doc.RootElement();


		TiXmlElement * ContentList = root->FirstChildElement("Meshes");
		if (ContentList)
		{
			TiXmlElement * Mesh = ContentList->FirstChildElement("Mesh");

			while (Mesh != nullptr)
			{
				double pos[4] = { 0.0 };
				XMFLOAT3 position;
				const char* name = Mesh->Attribute("name");
				const char* diffuse = Mesh->Attribute("diffuse");
				const char* normal = Mesh->Attribute("normalMap");

				Mesh->Attribute("transX", &pos[0]);
				Mesh->Attribute("transY", &pos[1]);
				Mesh->Attribute("transZ", &pos[2]);

				if (diffuse)
					diffuseTextures.push_back(diffuse);
				if (normal)
					normalMaps.push_back(normal);
				CMesh* mesh = pAssetManager->GetPrefabMesh(name);
				if (pos[3] == 90.0f)
				{
					for (size_t i = 0; i < mesh->GetPlanes().size(); i++)
						std::swap(mesh->GetPlanes()[i].extents.x, mesh->GetPlanes()[i].extents.z);

					for (size_t i = 0; i < mesh->GetAABBs().size(); i++)
						std::swap(mesh->GetAABBs()[i].extents.x, mesh->GetAABBs()[i].extents.z);
				}
				XMFLOAT4X4 local = {};
				XMStoreFloat4x4(&local, XMMatrixTranslation(pos[0], pos[1], pos[2]));
				meshLocalMatricies.push_back(local);
				meshes.push_back(mesh);
				Mesh = Mesh->NextSiblingElement();
			}
		}

		ContentList = root->FirstChildElement("Objects");
		if (ContentList && objManager)
		{

			TiXmlElement * Object = ContentList->FirstChildElement("Object");
			while (Object != nullptr)
			{
				int _type = 0;
				double data[4];
				XMFLOAT3 position;

				Object->Attribute("type", &_type);

				Object->Attribute("transX", &data[0]);
				Object->Attribute("transY", &data[1]);
				Object->Attribute("transZ", &data[2]);
				position = XMFLOAT3(float(data[0]), float(data[1]), float(data[2]));


				int orientation = 0;
				Object->Attribute("dir", &orientation);
				m_vObjects.push_back(new CItemCrate((IChest::EDirection)orientation, position, m_pDayNight));


				Object = Object->NextSiblingElement();
			}
		}
	}
	return true;
}

void CMinotaurSpawn::AddColliders()
{
	for (size_t currMesh = 0; currMesh < meshes.size(); currMesh++)
	{
		CMesh* mesh = meshes[currMesh];
		XMFLOAT3 posOffset = XMFLOAT3(meshLocalMatricies[currMesh]._41, meshLocalMatricies[currMesh]._42, meshLocalMatricies[currMesh]._43);

		int startIndex = 0;
		if (mesh->IsPlane())
		{
			int index = 0;

			for (index = 0; index < (int)mesh->GetAABBs().size(); index++)
			{
				CMesh::TAABB aabb = mesh->GetAABBs()[index];
				aabb.center.x += posOffset.x;
				aabb.center.y += posOffset.y;
				aabb.center.z += posOffset.z;

				AddCollider(new CCollider(false, new CAABB(aabb.center, aabb.extents)));
			}
			index = 0;
			if (m_pvColliders[0]->GetType() == Bounds::Plane)
				index = 1;
			for (index; index < mesh->GetPlanes().size(); index++)
			{
				CMesh::TPlane plane = mesh->GetPlanes()[index];
				plane.center.x += posOffset.x;
				plane.center.y += posOffset.y;
				plane.center.z += posOffset.z;
				AddCollider(new CCollider(false, new CPlane(plane.distance, plane.normal, plane.center, plane.extents)));
			}
		}
		else
		{
			int index = 0;
			if (m_pvColliders[0]->GetType() == Bounds::AABB)
				index = 1;

			for (index; index < mesh->GetAABBs().size(); index++)
			{
				CMesh::TAABB aabb = mesh->GetAABBs()[index];
				aabb.center.x += posOffset.x;
				aabb.center.y += posOffset.y;
				aabb.center.z += posOffset.z;
				AddCollider(new CCollider(false, new CAABB(aabb.center, aabb.extents)));
			}
			for (index = 0; index < mesh->GetPlanes().size(); index++)
			{
				CMesh::TPlane plane = mesh->GetPlanes()[index];
				plane.center.x += posOffset.x;
				plane.center.y += posOffset.y;
				plane.center.z += posOffset.z;
				AddCollider(new CCollider(false, new CPlane(plane.distance, plane.normal, plane.center, plane.extents)));
			}
		}
	}
}
void CMinotaurSpawn::FindDoorStartAndEnd(int DoorMeshIndex, XMFLOAT3 position)
{
	if (DoorMeshIndex == 4 || DoorMeshIndex == 5)
	{
		if (m_eDirection == eNorth || m_eDirection == eSouth)
		{
			if (DoorMeshIndex == 4)
			{

				endLeftPos = XMFLOAT3(position.x, position.y, position.z);
				startLeftPos = XMFLOAT3(position.x - 200.0f, position.y, position.z);
			}
			else
			{
				endRightPos = XMFLOAT3(position.x, position.y, position.z);
				startRightPos = XMFLOAT3(position.x + 200.0f, position.y, position.z);
			}
		}
		else
		{
			if (DoorMeshIndex == 4)
			{
				endLeftPos = XMFLOAT3(position.x, position.y, position.z);
				startLeftPos = XMFLOAT3(position.x, position.y, position.z - 200.0f);
			}
			else
			{
				endRightPos = XMFLOAT3(position.x, position.y, position.z);
				startRightPos = XMFLOAT3(position.x, position.y, position.z + 200.0f);
			}

		}
	}
}

void CMinotaurSpawn::SetWorldMatrix(const XMFLOAT4X4 world)
{
	m_mWorld = world;
	for (size_t i = 0; i < meshes.size(); i++)
	{
		XMMATRIX newMatrix;
		XMMATRIX tempPrefabWorld = XMLoadFloat4x4(&m_mWorld);
		XMMATRIX tempObjectWorld = XMLoadFloat4x4(&meshLocalMatricies[i]);
		newMatrix = tempObjectWorld * tempPrefabWorld;
		XMFLOAT4X4 newWorld;
		XMStoreFloat4x4(&newWorld, newMatrix);
		m_vRenderMeshes[i]->GetPositionMatrix() = newWorld;
		FindDoorStartAndEnd(i, XMFLOAT3(newWorld._41, newWorld._42, newWorld._43));



	}



	for (unsigned int collider = 0; collider < m_pvColliders.size(); collider++)
		m_pvColliders[collider]->UpdatePosition(XMFLOAT3(m_mWorld._41, m_mWorld._42, m_mWorld._43));

	for (size_t i = 0; i < m_vObjects.size(); i++)
	{

		XMMATRIX newMatrix;
		XMMATRIX tempPrefabWorld = XMLoadFloat4x4(&m_mWorld);
		XMMATRIX tempObjectWorld = XMLoadFloat4x4(&m_vObjects[i]->GetWorldMatrix());
		newMatrix = tempObjectWorld * tempPrefabWorld;
		XMFLOAT4X4 newWorld;
		XMStoreFloat4x4(&newWorld, newMatrix);


		m_vObjects[i]->SetWorldMatrix(newWorld);


		for (unsigned int collider = 0; collider < m_vObjects[i]->GetColliders().size(); collider++)
			m_vObjects[i]->GetColliders()[collider]->UpdatePosition(XMFLOAT3(world._41, world._42, world._43));

	}


}