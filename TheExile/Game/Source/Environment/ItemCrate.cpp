#include "ItemCrate.h"
#include "../../../Game/Source/Environment/DayNight.h"
#include "../../../Engine/Core/Collision/AABB.h"

CItemCrate::CItemCrate(EDirection _direction, XMFLOAT3 _pos, CDayNight* dayNight, string _szTag) : IChest(eItemChest, _direction, _pos, _szTag)
{
	m_pDayNight = dayNight;
	m_bActive = true;
	m_fInactiveTimer = 0.0f;
}

CItemCrate::~CItemCrate()
{
	m_pDayNight = nullptr;
}

void CItemCrate::Update()
{
	

	if (m_fInactiveTimer > 0.0f)
		m_fInactiveTimer -= DELTA_TIME();

	if (m_bActive == false)
	{
		if (m_pDayNight->IsDay() && m_fInactiveTimer < 0.0f)
			SetActive(true);
	}
	
}


void CItemCrate::SetActive(bool active)
{ 
	if (active == false)
		m_fInactiveTimer = 60.0f;

	m_bActive = active; 
	m_cpRenderMesh->GetRender() = active;
	GetColliders()[0]->SetActive(active);
}

