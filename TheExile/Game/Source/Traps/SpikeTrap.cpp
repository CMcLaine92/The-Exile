/***********************************************
* Filename:  		SpikeTrap.cpp
* Date:      		08/24/2015
* Mod. Date: 		----------
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:   		Controles the behaviour of individual traps
************************************************/
#include "SpikeTrap.h"
#include "../../../Engine/Object Manager/PressurePlate.h"
#include "../../../Engine/Object Manager/IObject.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Animation/Mesh.h"
#include "../../../Engine/Object Manager/ObjectManager.h"
#include "../../../Engine/Core/Messages/AddObjectMessage.h"
#include "../../../Engine/Core/Messages/RemoveObjectMessage.h"
#include "../../../Engine/Core/Collision/AABB.h"
#include "../../../Engine/Audio/Source/AudioSystemWwise.h"
#include "../../../Engine/Audio/Source/Wwise_IDs.h"
#include "../../../Game/Source/Player/Player.h"
#include "../../../Game/Source/Agents/Skeleton/Skeleton.h"

#define ACTIVE_DELAY 0.4f
#define RESET_DELAY 1.0f
#define TRAP_SPEED 1200.0f
#define TRAP_RESET_SPEED -400.0f
#define TRAP_START_Y -190.0f
#define TRAP_DMG 18

CSpikeTrap::CSpikeTrap(XMFLOAT3 _location, bool _pressurePlate, float _startTime, float _interval) : IObject("SpikeTrap")
{
	AudioSystemWwise::Get()->RegisterEntity(this, "SpikeTrap");

	// Assign the variables
	m_bPressurePlateActivated = _pressurePlate;
	m_fStartTime = _startTime;
	m_fCurrentTime = _startTime;
	m_fTimeInterval = _interval;
	m_nTrapDamage = TRAP_DMG;

	m_fPresetTimer = _interval - SPIKE_PRESET_TIME;
	m_bPresetOffset = false;
	m_fPresetActiveTimer = 0.0f;

	// Set it's new passed in position
	XMFLOAT3 newLocation = _location;
	newLocation.y = TRAP_START_Y;
	SetPosition(newLocation);

	// Add to the obj manager
	//CAddObjectMessage* addObj = new CAddObjectMessage(this, CObjectManager::Static);
	//if (!addObj->Send())
	//	return;

	// Get and Set the loaded in asset + collider + Draw

	CMesh* mesh = CAssetManager::GetInstance()->GetPrefabMesh("SpikeTrap");
	/*for (unsigned int i = 0; i < mesh.size(); i++)
	{
	m_vMesh.push_back(*mesh[i]);
	}*/

	SetRenderMesh(new CRenderMesh(mesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetStandardPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Failbot_Metal.dds"));

	AddCollider(new CCollider(false, Bounds::AABB, mesh->GetVertices(), true, false));
	m_cpRenderMesh->GetSpecular() = 1.5f;
	GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	// Set the colliders center
	for (size_t i = 0; i < m_pvColliders.size(); i++)
		((CAABB*)m_pvColliders[i]->GetBounds())->SetCenter({ m_mWorld._41, m_mWorld._42, m_mWorld._43 });

	// Assign timer variables
	m_fResetDelayTimer = RESET_DELAY;
	m_fActiveDelayTimer = ACTIVE_DELAY;

	m_bHasPressurePlate = _pressurePlate;
	// Create the new Pressure plate, and mark this as it's parent
	/*m_cpThePressurePlate = new CPressurePlate(_location, _pressurePlate);
	m_cpThePressurePlate->SetParent(this);*/
}

CSpikeTrap::~CSpikeTrap()
{
	AudioSystemWwise::Get()->UnRegisterListener(this);
	delete m_cpThePressurePlate;
	
}

void CSpikeTrap::Update()
{
	// If not Pressure Plate Controlled
	if (!m_bPressurePlateActivated && !m_bReset)
	{
		m_fCurrentTime -= DELTA_TIME();
		m_fPresetTimer -= DELTA_TIME();

		if (m_fPresetTimer < 0.0f)
		{
			m_fPresetTimer = m_fTimeInterval - SPIKE_PRESET_TIME;
			m_bPresetOffset = true;
		}

		if (m_fCurrentTime < 0.0f)
		{
			MakeActive();
			m_fPresetTimer = m_fTimeInterval - SPIKE_PRESET_TIME;
			m_fCurrentTime = m_fTimeInterval;
			return;
		}
	}

	if (m_bPresetOffset == true)
	{
		SetWorldVelocity(XMFLOAT3{ 0, SPIKE_PRESET_SPEED, 0 });

		m_fPresetActiveTimer += DELTA_TIME();
		if (m_fPresetActiveTimer > SPIKE_PRESET_OFFSET_TIME)
		{
			m_bPresetOffset = false;
			m_fPresetActiveTimer = 0.0f;
			SetWorldVelocity(XMFLOAT3{ 0, 0, 0 });
		}
	}


	// If it has been activated
	if (m_bActive)
	{
		m_fActiveDelayTimer -= DELTA_TIME();

		// Wait for the delay to run out
		if (m_fActiveDelayTimer <= 0.0f || !m_bPressurePlateActivated)
		{
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::SPIKETRAP, this);

			// Set the velocity to straight up
			XMFLOAT3 curVel = XMFLOAT3{ 0, TRAP_SPEED, 0 };
			SetWorldVelocity(curVel);

			// If damage has not been dealt, deal damage
			if (!m_bDamageDealtToPlayer)
				m_bDealDamageToPlayer = true;
			if (!m_bDamageDealtToSkeleton)
				m_bDealDamageToSkeleton = true;

			// Reset the variables
			if (GetPosition()->y >= -30.0f)
			{
				SetPosition({ GetPosition()->x, -30.0f, GetPosition()->z });
				SetWorldVelocity({ 0, 0, 0 });
				m_fActiveDelayTimer = ACTIVE_DELAY;
				m_fResetDelayTimer = RESET_DELAY;
				m_bActive = false;
				m_bReset = true;
			}
		}
	}
	else if (m_bReset)
	{
		m_fResetDelayTimer -= DELTA_TIME();
		if (m_fResetDelayTimer < 0.0f)
		{
			// Set the velocity back down
			XMFLOAT3 curVel = XMFLOAT3{ 0, TRAP_RESET_SPEED, 0 };
			SetWorldVelocity(curVel);

			// Reset the variables
			if (GetPosition()->y <= TRAP_START_Y)
			{
				SetPosition({ GetPosition()->x, TRAP_START_Y, GetPosition()->z });
				SetWorldVelocity({ 0, 0, 0 });
				m_fActiveDelayTimer = ACTIVE_DELAY;
				m_fResetDelayTimer = RESET_DELAY;
				m_bDamageDealtToPlayer = false;
				m_bDamageDealtToSkeleton = false;
				m_bActive = false;
				m_bReset = false;
			}
		}
	}

	// Update the colliders center (Might be temporary fix)
	for (size_t i = 0; i < m_pvColliders.size(); i++)
		((CAABB*)m_pvColliders[i]->GetBounds())->SetCenter({ m_mWorld._41, m_mWorld._42, m_mWorld._43 });

}

void CSpikeTrap::CollisionResponse(IObject * _cCollidedObject)
{
	DBG_UNREFERENCED_PARAMETER(_cCollidedObject);

	//// If colliding with player, deal damage if conditions are met.
	//if (_cCollidedObject->GetTag() == "Player")
	//{
	//	CPlayer* thePlayer = reinterpret_cast<CPlayer*>(_cCollidedObject);
	//	if ((m_bActive || m_bReset) && m_bDealDamageToPlayer)
	//	{
	//		if (m_bReset && thePlayer->GetVelocity().y == 0.0f)
	//		{
	//			// Don't take damage, Player handles collision for this case.
	//		}
	//		else
	//		{
	//			thePlayer->TakeDamage(TRAP_DMG);
	//		}

	//		thePlayer->SetWorldVelocity({ 0.0f, 0.0f, 0.0f });
	//		m_bDealDamageToPlayer = false;
	//		m_bDamageDealtToPlayer = true;
	//	}
	//	else if (m_bActive || m_bReset)
	//	{
	//		thePlayer->SetWorldVelocity({ 0.0f, 0.0f, 0.0f });
	//	}
	//}
	//else if (_cCollidedObject->GetTag() == "Skeleton")
	//{
	//	CSkeleton* theSkeleton = reinterpret_cast<CSkeleton*>(_cCollidedObject);

	//	theSkeleton->TakeDamage(200);
	//	m_bDealDamageToSkeleton = false;
	//	m_bDamageDealtToSkeleton = true;

	//	if (m_bActive || m_bReset)
	//	{
	//		theSkeleton->SetWorldVelocity({ 0.0f, 0.0f, 0.0f });
	//	}
	//}
}

void CSpikeTrap::SetWorldMatrix(const DirectX::XMFLOAT4X4 matrix)
{
	m_mWorld = matrix;

	//pressure plate
	m_cpThePressurePlate = new CPressurePlate(*GetPosition(), m_bHasPressurePlate);
	m_cpThePressurePlate->SetParent(this);
	m_cpThePressurePlate->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Textures/Spike_PressurePlate.dds");
}
