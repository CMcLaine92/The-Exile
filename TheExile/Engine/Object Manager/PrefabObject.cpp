#include "PrefabObject.h"
#include "../Asset Manager/AssetManager.h"
#include "../Core/Collision/Plane.h"
#include "../Core/Collision/AABB.h"

#include "../../Game/Source/Environment/Torch.h"
#include "../../Game/Source/Environment/Bush.h"
#include "../../Game/Source/Environment/Tree.h"
#include "../../Game/Source/Environment/Torch.h"
#include "../../Game/Source/Traps/FireTrap.h"
#include "../../Game/Source/Traps/SpikeTrap.h"
#include "../../Game/Source/Traps/SpinningBlade.h"
#include "../../Engine/Object Manager/DoorObject.h"
#include "../../Engine/Object Manager/ExitDoorObject.h"
#include "../../Engine/Object Manager/TestObject.h"//this is the prop object
#include "../../Game/Source/Environment/CaveSpawn.h"
#include "../../Game/Source/Environment/HealthUpgradeChest.h"
#include "../../Game/Source/Environment/HeavyWeaponChest.h"
#include "../../Game/Source/Environment/LightWeaponChest.h"
#include "../../Game/Source/Environment/Bridge.h"
#include "../Core/Messages/AddObjectMessage.h"
#include "../Core/Messages/RemoveObjectMessage.h"
using namespace DEBUG_UTIL;

#include "../../Engine/Particles/tinyxml/tinyxml.h"
CPrefabObject::CPrefabObject() : IObject("Prefab")
{
}
//Only need to pass in ObjectManager if the prefab you are loading has torches and/or traps
CPrefabObject::CPrefabObject(const char* filePath, const char* tag, CObjectManager* objManager) : IObject(tag)
{
	LoadFromXML(filePath, objManager);
	
	CreateRenderMeshes();
	CreateSceneBounds();
	AddColliders();


}
void CPrefabObject::AddColliders()
{
	for (size_t currMesh = 0; currMesh < meshes.size(); currMesh++)
	{
		CMesh* mesh = meshes[currMesh];

		int startIndex = 0;
		if (mesh->IsPlane() )
		{
			int index = 0;

			for (index = 0; index < (int)mesh->GetAABBs().size(); index++)
			{
				CMesh::TAABB aabb = mesh->GetAABBs()[index];
				if (m_szTag == "SafeHaven" && (aabb.extents.x == 500.0f &&
					aabb.extents.y == 300.0f &&
					aabb.extents.z == 500.0f))
					AddCollider(new CCollider(false, new CAABB(aabb.center, aabb.extents), true, false));
				else
					AddCollider(new CCollider(false, new CAABB(aabb.center, aabb.extents)));
			}
			index = 0;
			if (m_pvColliders[0]->GetType() == Bounds::Plane)
				index = 1;
			for (index; index < mesh->GetPlanes().size(); index++)
			{
				CMesh::TPlane plane = mesh->GetPlanes()[index];
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
				if (m_szTag == "SafeHaven" && (aabb.extents.x == 500.0f &&
					aabb.extents.y == 300.0f &&
					aabb.extents.z == 500.0f))
					AddCollider(new CCollider(false, new CAABB(aabb.center, aabb.extents), true, false));
				else
					AddCollider(new CCollider(false, new CAABB(aabb.center, aabb.extents)));
			}
			for (index = 0; index < mesh->GetPlanes().size(); index++)
			{
				CMesh::TPlane plane = mesh->GetPlanes()[index];

				if (m_szTag == "SmallPit")
				{
					if (plane.center.y < 0.0f)
						AddCollider(new CCollider(false, new CPlane(plane.distance, plane.normal, plane.center, plane.extents)));
					else
						AddCollider(new CCollider(false, new CPlane(plane.distance, plane.normal, plane.center, plane.extents),true, false));

				}
				else
					AddCollider(new CCollider(false, new CPlane(plane.distance, plane.normal, plane.center, plane.extents)));


			}
		}
	}
}
void CPrefabObject::CreateRenderMeshes()
{
	if (meshes.size() > 0)
	{
		for (size_t i = 0; i < meshes.size(); i++)
		{
			CRenderMesh* pRenderMesh = new CRenderMesh;

			wstring diffuse, normal;
			if (diffuseTextures.size() > 0)
				 diffuse = wstring(diffuseTextures[i].begin(), diffuseTextures[i].end());

			if (normalMaps.size() > 0)
				 normal = wstring(normalMaps[i].begin(), normalMaps[i].end());

			if (normal.empty() == false)
			{
				pRenderMesh->Create(meshes[i], GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), NULL, NULL, NULL, diffuse.c_str());
				pRenderMesh->SetNormals(normal.c_str());
			}
			else
			{
				pRenderMesh->Create(meshes[i], GRAPHICS->GetVertexShader(), GRAPHICS->GetStandardPS(), NULL, NULL, NULL, diffuse.c_str());
			}

			m_vRenderMeshes.push_back(pRenderMesh);
			GRAPHICS->AddRenderMesh(pRenderMesh);
		}

	}
}
void CPrefabObject::CreateSceneBounds()
{
	if (meshes[0]->IsPlane())
	{
		CMesh::TPlane plane = meshes[0]->GetPlanes()[0];
		AddCollider(new CCollider(false, new CPlane(plane.distance, plane.normal, plane.center, plane.extents), true, false));
	}
	else
	{
		CMesh::TAABB aabb = meshes[0]->GetAABBs()[0];
		AddCollider(new CCollider(false, new CAABB(aabb.center, aabb.extents), true, false));
	}
}

CPrefabObject::~CPrefabObject()
{
	VECTOR_DELETE_ALL(m_vRenderMeshes);
	
	VECTOR_DELETE_ALL(m_vObjects);

	
	m_vObjects.clear();
	m_vRenderMeshes.clear();
}
void CPrefabObject::AddChildrenToObjectManager()
{
	for (size_t i = 0; i < m_vObjects.size(); i++)
	{
		if (m_vObjects[i]->GetColliders().size() > 0)
		{
			CAddObjectMessage* msg;
			if (m_vObjects[i]->GetColliders()[0]->GetDynamic() == false)
				msg = new CAddObjectMessage(m_vObjects[i], CObjectManager::eObjectType::Static);
			else
				msg = new CAddObjectMessage(m_vObjects[i], CObjectManager::eObjectType::Dynamic);

			msg->Send();


			
		}
	}
}
void CPrefabObject::AddToObjectManager()
{
	if (m_pvColliders.size() > 0)
	{
		CAddObjectMessage* msg = new CAddObjectMessage(this, CObjectManager::eObjectType::Static);
		msg->Send();
	}
	AddChildrenToObjectManager();
}


void CPrefabObject::SetWorldMatrix(const XMFLOAT4X4 world)
{
	m_mWorld = world;
	for (size_t i = 0; i < m_vRenderMeshes.size(); i++)
		m_vRenderMeshes[i]->GetPositionMatrix() = m_mWorld;

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

		if (m_vObjects[i]->GetTag() == "SpinningBlade")
		{
			//adjust waypoints
			CSpinningBlade* temp = ((CSpinningBlade*)(m_vObjects[i]));
			XMFLOAT3& pos = temp->GetStartPoint();
			pos.x += world._41;
			pos.y += world._42;
			pos.z += world._43;

			XMFLOAT3& pos2 = temp->GetEndPoint();
			pos2.x += world._41;
			pos2.y += world._42;
			pos2.z += world._43;

			temp->GetTargetPoint() = pos2;

			temp->CreateTrack();
		}

		m_vObjects[i]->SetWorldMatrix(newWorld);

		if (m_vObjects[i]->GetTag() != "FireTrap")
		{
			for (unsigned int collider = 0; collider < m_vObjects[i]->GetColliders().size(); collider++)
				m_vObjects[i]->GetColliders()[collider]->UpdatePosition(XMFLOAT3(world._41, world._42, world._43));
		}

	}
}
//Only need to pass in ObjectManager if the prefab you are loading has torches and/or traps

bool CPrefabObject::LoadFromXML(const char* prefabName, CObjectManager* objManager)
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
			//TiXmlElement *  = Prefab->FirstChild("Mesh");

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

				meshes.push_back(pAssetManager->GetPrefabMesh(name));
				Mesh = Mesh->NextSiblingElement();
			}
		}

		ContentList = root->FirstChildElement("Torches");
		if (ContentList && objManager)
		{
			TiXmlElement * Torch = ContentList->FirstChildElement("Torch");
			while (Torch != nullptr)
			{
				double data[6] = { 0.0 };
				Torch->Attribute("transX", &data[0]);
				Torch->Attribute("transY", &data[1]);
				Torch->Attribute("transZ", &data[2]);
				Torch->Attribute("rotX", &data[3]);
				Torch->Attribute("rotY", &data[4]);
				Torch->Attribute("rotZ", &data[5]);

				m_vObjects.push_back(new CTorch(objManager,
					XMFLOAT3(float(data[0]), float(data[1]), float(data[2])),
					XMFLOAT3(float(data[3]), float(data[4]), float(data[5]))));

				Torch = Torch->NextSiblingElement();
			}
		}

		ContentList = root->FirstChildElement("Traps");
		if (ContentList && objManager)
		{
			TiXmlElement * Trap = ContentList->FirstChildElement("Trap");
			while (Trap != nullptr)
			{

				int _type = 0;
				double data[8] = { 0.0 };
				Trap->Attribute("type", &_type);
				Trap->Attribute("transX", &data[0]);
				Trap->Attribute("transY", &data[1]);
				Trap->Attribute("transZ", &data[2]);
				XMFLOAT3 position = XMFLOAT3(float(data[0]), float(data[1]), float(data[2]));

				switch (_type)
				{
				case ePressureSpike:
				{
					m_vObjects.push_back(new CSpikeTrap(position, true, 0.0f, 1.0f));
				}
				break;
				case eTimedSpike:
				{
					Trap->Attribute("startTime", &data[3]);
					Trap->Attribute("offset", &data[4]);
					m_vObjects.push_back(new CSpikeTrap(position, false, float(data[3]), float(data[4])));
				}
				break;
				case eSpinningBlade:
				{
					Trap->Attribute("rotSpeed", &data[3]);
					m_vObjects.push_back(new CSpinningBlade(position, float(data[3])));
				}
				break;
				case eMovingSpinBlade:
				{
					Trap->Attribute("rotSpeed", &data[4]);
					Trap->Attribute("wayPointX", &data[5]);
					Trap->Attribute("wayPointY", &data[6]);
					Trap->Attribute("wayPointZ", &data[7]);
					XMFLOAT3 wayPoint = XMFLOAT3(float(data[5]), float(data[6]), float(data[7]));
					m_vObjects.push_back(new CSpinningBlade(position, float(data[4]), position, wayPoint));
				}
				break;
				
				case eFireTrap:
				{
					Trap->Attribute("dirX", &data[3]);
					Trap->Attribute("dirY", &data[4]);
					Trap->Attribute("dirZ", &data[5]);
					XMFLOAT3 direction = XMFLOAT3(float(data[3]), float(data[4]), float(data[5]));
					m_vObjects.push_back(new CFireTrap(objManager, position, direction));
				}
				break;
				default:
					break;
				}



				Trap = Trap->NextSiblingElement();
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
				
				switch (_type)
				{
				case eBridge:
				{
					CMesh::TPlane& tPlane = meshes[0]->GetPlanes()[0];
					if (tPlane.extents.z > tPlane.extents.x)
						m_vObjects.push_back(new CBridge({ 0.0f, 0.0f, 0.0f }, true));
					else
						m_vObjects.push_back(new CBridge);
				}
				break;
				case eDoor:
				{
					int vertical = 0;
					Object->Attribute("vertical", &vertical);

					m_vObjects.push_back(new CDoorObject(position, "Door",bool(vertical)));
					
				}
				break;
				case eExitDoor:
				{
					int vertical = 0;
					Object->Attribute("vertical", &vertical);

					m_vObjects.push_back(new CExitDoorObject(position, "ExitDoor", bool(vertical)));

				}
				break;
				case eTree:
				{
					m_vObjects.push_back(new CTree(objManager, position));
				}
				break;
				case eBush:
				{
					m_vObjects.push_back(new CBush(objManager, position));

				}
				break;
				case eOrcSpawn:
				{
					m_vObjects.push_back(new CCaveSpawn(position));

				}
				case eHealthChest:
				{
					int orientation = 0;
					Object->Attribute("dir", &orientation);
					m_vObjects.push_back(new CHealthUpgradeChest((IChest::EDirection)orientation, position));
				}
				break;
				case eLightWeaponChest:
				{
					int orientation = 0;
					Object->Attribute("dir", &orientation);
					m_vObjects.push_back(new CLightWeaponChest((IChest::EDirection)orientation, position));
				}

				break;
				case eHeavyWeaponChest:
				{
					int orientation = 0;
					Object->Attribute("dir", &orientation);
					m_vObjects.push_back(new CHeavyWeaponChest((IChest::EDirection)orientation, position));
				}
				case eSHProp:
				{
					int id = 0;
					Object->Attribute("ID", &id);
					m_vObjects.push_back(new CTestObject("Prop", position, id));
				}
				break;
				default:
					break;
				}

				Object = Object->NextSiblingElement();
			}
		}

	}
	return true;
}

void CPrefabObject::Update()
{
	/*if (m_szTag == "Trap3x3" || m_szTag == "Trap5x3")
	{
		for (size_t i = 0; i < m_pvColliders.size(); i++)
		{
			if (m_pvColliders[i]->GetType() == Bounds::eBoundsType::Plane)
				DrawAABBCollider(((CPlane*)(m_pvColliders[i]->GetBounds()))->Min(), ((CPlane*)(m_pvColliders[i]->GetBounds()))->Max());
		}
	}*/
	
	
#if _DEBUG
	/*for (size_t i = 0; i < m_pvcolliders.size(); i++)
	{
		if (m_pvcolliders[i]->gettype() == bounds::eboundstype::aabb)
			drawaabbcollider(((caabb*)(m_pvcolliders[i]->getbounds()))->min(), ((caabb*)(m_pvcolliders[i]->getbounds()))->max());
	}
	for (size_t i = 0; i < m_vobjects.size(); i++)
	{
		for (size_t j = 0; j < m_vobjects[i]->getcolliders().size(); j++)
		{
			if (m_vobjects[i]->getcolliders()[j]->gettype() == bounds::eboundstype::aabb)
				drawaabbcollider(((caabb*)(m_vobjects[i]->getcolliders()[j]->getbounds()))->min(), ((caabb*)(m_vobjects[i]->getcolliders()[j]->getbounds()))->max());
		}
	}*/
	
#endif
	
}

IObject* CPrefabObject::GetExitDoorObject()
{
	for (unsigned int i = 0; i < m_vObjects.size(); i++)
	{
		if (m_vObjects[i]->GetTag() == "ExitDoor")
		{
			return m_vObjects[i];
		}
	}

	return nullptr;
}

void CPrefabObject::AddToRenderer()
{
	for (unsigned int i = 0; i < m_vRenderMeshes.size(); i++)
	{
		if (m_vRenderMeshes[i] != nullptr)
		{
			GRAPHICS->AddRenderMesh(m_vRenderMeshes[i]);
		}
	}

	for (unsigned int i = 0; i < m_vObjects.size(); i++)
	{
		m_vObjects[i]->AddToRenderer();
	}
}
