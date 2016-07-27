/***********************************************
* Filename:  		ExitDoorObject.cpp
* Date:      		8/20/2015
* Mod. Date: 		8/20/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents the exit door
************************************************/

#include "ExitDoorObject.h"
#include "../Asset Manager/AssetManager.h"

CExitDoorObject::CExitDoorObject(XMFLOAT3 position, string szTag, bool verical) : IObject(szTag)
{
	m_bMoving = false;
	m_fMoveTimer = 0.0f;
	m_fTotalMoveTime = 0.0f;
	m_nMoveCounter = 0;
	m_bPlayAudio = false;

	m_bIsActive = true;

	m_mWorld =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		position.x, position.y, position.z, 1
	};

	CMesh* mesh = nullptr;
	mesh = (verical) ? CAssetManager::GetInstance()->GetPrefabMesh("VDoor") : CAssetManager::GetInstance()->GetPrefabMesh("HDoor");
	CMesh::TAABB aabb = mesh->GetAABBs()[0];
	aabb.center.x += position.x;
	aabb.center.z += position.z;
	AddCollider(new CCollider(true, new CAABB(aabb.center, aabb.extents)));
	
	SetRenderMesh(new CRenderMesh(mesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Door.dds"));
	GRAPHICS->AddRenderMesh(m_cpRenderMesh);	
	
	//set up audio
	m_cpExitDoorAudio = m_cpExitDoorAudio->Get();

}

CExitDoorObject::~CExitDoorObject()
{
}

void CExitDoorObject::Update()
{
	if (m_bMoving == true)
	{
		if (m_bPlayAudio == true)
		{
			m_cpExitDoorAudio->PostEvent(AK::EVENTS::DOORSLIDE, XMFLOAT3(GetPosition()->x, GetPosition()->y, GetPosition()->z));
			m_bPlayAudio = false;
		}

		//move the door
		m_f3Velocity.y = MOVE_DISTANCE / m_fTotalMoveTime;
		UpdateColliders(m_f3Velocity);

		m_fMoveTimer += DELTA_TIME();
		if (m_fMoveTimer >= m_fTotalMoveTime)
		{
			m_fMoveTimer = 0.0f;
			m_bMoving = false;
		}
	}

}

void CExitDoorObject::ActivateVerticleMovement(float moveTime)
{
	m_nMoveCounter++;
	if (m_nMoveCounter == 1)
	{
		m_fMoveTimer = 0.0f;
		m_fTotalMoveTime = moveTime;
		m_bPlayAudio = true;
		m_bMoving = true;
	}
}

void CExitDoorObject::SetMovementTime(float moveTime)
{
	//m_fTotalMoveTime = moveTime;
	//m_fMoveTimer = moveTime;
}

void CExitDoorObject::SetWorldMatrix(const XMFLOAT4X4 world)
{
	m_mWorld = world;
	UpdateColliders(GetWorldVelocity());
}
