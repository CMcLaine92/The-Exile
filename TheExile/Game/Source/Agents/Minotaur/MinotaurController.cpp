
#include "MinotaurController.h"
#include "../../../../Engine/Particles/tinyxml/tinyxml.h"
#include "../../../../Engine/Object Manager/ObjectManager.h"
#include "../../../../Animation/AnimationManager.h"
#include "../../Agents/Minotaur/Minotaur.h"
#include "../../../../Engine/Object Manager/SpawnerObject.h"
#include "../../Environment/DayNight.h"

CMinotaurController::CMinotaurController(CObjectManager* objectManager, CAnimationManager* animationManager, std::vector<XMFLOAT4X4> spawners, CDayNight* _dayNight)
{
	m_vSpawnPoints = spawners;
	m_cpDayNight = _dayNight;
	m_bNightTime = false;

	for (size_t i = 0; i < m_vSpawnPoints.size(); i++)
	{
		m_vMinotaurs.push_back(new CMinotaur(XMFLOAT3(spawners[i]._41, spawners[i]._42, spawners[i]._43), objectManager, animationManager));
	}
}
CMinotaurController::~CMinotaurController()
{
	VECTOR_DELETE_ALL(m_vMinotaurs);
	m_vMinotaurs.clear();
}

void CMinotaurController::Update()
{
	
	if (m_cpDayNight->IsNight() && m_bNightTime == false)
	{
		m_bNightTime = m_cpDayNight->IsNight();
		for (size_t i = 0; i < m_vMinotaurs.size(); i++)
		{
			
			m_vMinotaurs[i]->Activate();
			
		}

	}
	else if (m_cpDayNight->IsDay() && m_bNightTime == true)
	{
		m_bNightTime = m_cpDayNight->IsNight();
		for (size_t i = 0; i < m_vMinotaurs.size(); i++)
		{
			m_vMinotaurs[i]->Kill();
		}
	}
	
	
	
}

void CMinotaurController::LoadConfigXML(CObjectManager* objectManager, CAnimationManager* animationManager)
{
	TiXmlDocument doc;
	
	
	if (doc.LoadFile("../Game/Assets/Scripts/AI/MinotaurConfig.xml", TiXmlEncoding::TIXML_ENCODING_UTF8))
	{
		TiXmlElement * root = doc.RootElement();
		TiXmlElement * minotaurElmt = root->FirstChildElement("Minotaur");

		while (minotaurElmt != nullptr)
		{
			int spawnIndex;
			int waypoints[3] = { 0 };
			minotaurElmt->Attribute("spawnPoint", &spawnIndex);
			minotaurElmt->Attribute("wpIndex1", &waypoints[0]);
			minotaurElmt->Attribute("wpIndex2", &waypoints[1]);
			minotaurElmt->Attribute("wpIndex3", &waypoints[2]);

			XMFLOAT3 position = XMFLOAT3(m_vSpawnPoints[spawnIndex]._41, m_vSpawnPoints[spawnIndex]._42, m_vSpawnPoints[spawnIndex]._43);
			CMinotaur* pMinotaur = new CMinotaur(position, objectManager, animationManager);
		
			pMinotaur->SetPosition(position);
			pMinotaur->AddWaypoint(waypoints[0]);
			pMinotaur->AddWaypoint(waypoints[1]);
			pMinotaur->AddWaypoint(waypoints[2]);

			minotaurElmt = minotaurElmt->NextSiblingElement("Minotaur");

			m_vMinotaurs.push_back(pMinotaur);

		}
		
	}
}
void CMinotaurController::AddToRenderer()
{
	for (unsigned int i = 0; i < m_vMinotaurs.size(); i++)
	{
		m_vMinotaurs[i]->AddToRenderer();
	}
}
