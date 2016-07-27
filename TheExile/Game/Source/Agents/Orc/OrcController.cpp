
#include "OrcController.h"
#include "../../../../Engine/Particles/tinyxml/tinyxml.h"
#include "../../../../Engine/Object Manager/ObjectManager.h"
#include "../../../../Animation/AnimationManager.h"
#include "../../Agents/Orc/Orc.h"
#include "../../../../Engine/Object Manager/SpawnerObject.h"
#include "../../Environment/DayNight.h"
#include "../../../../Object Manager/OrcBomb.h"

COrcController::COrcController(CObjectManager* _objectManager, CAnimationManager* animManager, std::vector<XMFLOAT4X4>& spawners, CDayNight* _dayNight) : IObject("OrcController")
{
	m_vSpawnPoints = spawners;
	m_cpDayNight = _dayNight;
	m_bNightTime = false;

	for (size_t i = 0; i < spawners.size(); i++)
	{
		XMFLOAT3 pos = XMFLOAT3(spawners[i]._41, spawners[i]._42, spawners[i]._43);
		COrcBomb* bomb = new COrcBomb(nullptr, _objectManager);
		COrc* orc = new COrc(pos, bomb, _objectManager, animManager, _dayNight);
		bomb->SetOwner(orc);
		m_vOrcs.push_back(orc);

	}


}
COrcController::~COrcController()
{
	for (size_t currOrc = 0; currOrc < m_vOrcs.size(); currOrc++)
	{
		delete m_vOrcs[currOrc];
		m_vOrcs[currOrc] = nullptr;
	}
	m_vOrcs.clear();
}

void COrcController::Update()
{

	if (m_cpDayNight->IsNight() == true && m_bNightTime == false)
	{

		m_bNightTime = m_cpDayNight->IsNight();
		for (size_t i = 0; i < m_vOrcs.size(); i++)
			m_vOrcs[i]->ChangeState(COrc::eRetreatState);

	}
	else if (m_cpDayNight->IsNight() == false && m_bNightTime == true)
	{
		m_bNightTime = m_cpDayNight->IsNight();
		for (size_t i = 0; i < m_vOrcs.size(); i++)
			m_vOrcs[i]->ChangeState(COrc::eIdleState);

	}
	


}

void COrcController::AddToRenderer()
{
	for (unsigned int i = 0; i < m_vOrcs.size(); i++)
	{
		m_vOrcs[i]->AddToRenderer();
	}
}
