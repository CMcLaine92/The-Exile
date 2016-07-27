/***********************************************
* Filename:  		DoorObject.cpp
* Date:      		8/14/2015
* Mod. Date: 		9/10/2015
* Mod. Initials:	MZ
* Author:    		Nick Hryshko
* Purpose:			Represents a door object
************************************************/

#include "DoorObject.h"
#include "../Core/Collision/AABB.h"
#include "../Audio/Source/AudioSystemWwise.h"
#include "../Audio/Source/Wwise_IDs.h"
#include "../../Game/Source/Environment/DayNight.h"
#include "../Asset Manager/AssetManager.h"
#define DOOR_MOVE_DISTANCE 900.0f
enum EDirection{eForward, eBackward, eLeft, eRight};

static int m_nDoorsClosed;

void CDoorObject::CalculateDirection()
{
	m_nDoorsClosed = 0;

	if (m_pvColliders.size() > 0)
	{
		CAABB* pBounds = (CAABB*)m_pvColliders[0]->GetBounds();
		if (pBounds->GetCenter().x > 0 && pBounds->GetCenter().z > 0) // Forward Door
		{
			m_unDirection = eLeft;
		}
		else if (pBounds->GetCenter().x > 0 && pBounds->GetCenter().z < 0)//Right Door
		{
			m_unDirection = eForward;
		}
		else if (pBounds->GetCenter().x < 0 && pBounds->GetCenter().z > 0) // Left Door
		{
			m_unDirection = eBackward;
		}
		else // Back door
		{
			m_unDirection = eRight;
		}
		m_bHasMoveDirection = true;
	}

	CAABB* newBounds = new CAABB(*((CAABB*)m_pvColliders[0]->GetBounds()));

	if (m_unDirection == eLeft)
		newBounds->SetCenter({ newBounds->GetCenter().x, newBounds->GetCenter().y, newBounds->GetCenter().z - 150 });
	else if (m_unDirection == eRight)
		newBounds->SetCenter({ newBounds->GetCenter().x, newBounds->GetCenter().y, newBounds->GetCenter().z + 150 });
	else if (m_unDirection == eForward)
		newBounds->SetCenter({ newBounds->GetCenter().x - 150, newBounds->GetCenter().y, newBounds->GetCenter().z });
	else if (m_unDirection == eBackward)
		newBounds->SetCenter({ newBounds->GetCenter().x + 150, newBounds->GetCenter().y, newBounds->GetCenter().z });

	CCollider* newCollider = new CCollider(false, newBounds, true, false);
	AddCollider(newCollider);
}
void CDoorObject::Movement()
{
	switch (m_unDirection)
	{
	case eForward:
		m_f3Velocity.z = DOOR_MOVE_DISTANCE / DOOR_MOVE_TIME;
		break;
	case eBackward:
		m_f3Velocity.z = -DOOR_MOVE_DISTANCE / DOOR_MOVE_TIME;
		break;
	case eRight:
		m_f3Velocity.x = DOOR_MOVE_DISTANCE / DOOR_MOVE_TIME;
		break;
	case eLeft:
		m_f3Velocity.x = -DOOR_MOVE_DISTANCE / DOOR_MOVE_TIME;
		break;
	}
	if (m_fMoveTimer <= 0.0f)
	{

		m_f3Velocity.x = 0.0f;
		m_f3Velocity.z = 0.0f;
		switch (m_unDirection)
		{
		case eForward:
			m_unDirection = eBackward;
			break;
		case eBackward:
			m_unDirection = eForward;
			break;
		case eRight:
			m_unDirection = eLeft;
			break;
		case eLeft:
			m_unDirection = eRight;
			break;
		}
	}
}

void CDoorObject::SetMoveTimer(float fMoveTimer)
{
	m_fMoveTimer = fMoveTimer;
}

CDoorObject::CDoorObject(XMFLOAT3 position, string szTag, bool vertical) : IObject(szTag)
{
	m_mWorld =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		position.x, position.y, position.z, 1
	};
	CMesh* tempMesh = (vertical) ? CAssetManager::GetInstance()->GetPrefabMesh("VDoor") : CAssetManager::GetInstance()->GetPrefabMesh("HDoor");
	CMesh::TAABB aabb = tempMesh->GetAABBs()[0];
	CCollider* collider = new CCollider(false, new CAABB(aabb.center, aabb.extents));
	collider->UpdatePosition(position);
	AddCollider(collider);
	SetRenderMesh(new CRenderMesh(tempMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Door.dds"));
	GRAPHICS->AddRenderMesh(m_cpRenderMesh);
	AudioSystemWwise::Get()->RegisterEntity(this, "Door");

}

CDoorObject::~CDoorObject()
{
	AudioSystemWwise::Get()->UnRegisterEntity(this);
}

void CDoorObject::Update()
{
	if (m_bHasMoveDirection == false)
	{
		CalculateDirection();
	}
	if (m_fMoveTimer > 0.0f)
	{
		m_fMoveTimer -= DELTA_TIME();
		Movement();
		UpdateColliders(GetWorldVelocity());

		if (m_fMoveTimer <= 0.0f && m_cpDayNight->IsNight() && !bOpening)
		{
			if (m_nDoorsClosed++ == 0)
				AudioSystemWwise::Get()->PostEvent(AK::EVENTS::STARTNIGHT);
			else if (m_nDoorsClosed == 4)
				m_nDoorsClosed = 0;
		}
	}

	if (bOpening && m_fMoveTimer <= 0) // Door has opened
	{
		bOpening = false;
		bClosing = true;
		SetMoveTimer(5.0f);
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::DOORSLIDE);
	}
	if (bClosing && m_fMoveTimer) // Door has shut
		bClosing = false;

	#if _DEBUG
	DEBUG_UTIL::DrawAABBCollider(((CAABB*)m_pvColliders[0]->GetBounds())->Min(), ((CAABB*)m_pvColliders[0]->GetBounds())->Max());
		//DEBUG::DrawAABBCollider(((CAABB*)m_pvColliders[1]->GetBounds())->Min(), ((CAABB*)m_pvColliders[1]->GetBounds())->Max(), { 0, 1, 1 });
	#endif
}

void CDoorObject::CollisionResponse(IObject * _cCollidedObject)
{
	GetColliders()[0]->SetCollided(false);
	GetColliders()[1]->SetCollided(false);
}

void CDoorObject::TempOpen()
{
	if (m_fMoveTimer > 0.0f)
		return;

	if (!bOpening && !bClosing) // Start opening
	{
		bOpening = true;
		SetMoveTimer(5.0f);
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::DOORSLIDE);
	}
}