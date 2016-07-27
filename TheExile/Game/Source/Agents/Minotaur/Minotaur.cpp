/***********************************************
* Filename:  		Minotaur.cpp
* Date:      		08/16/2015
* Mod. Date: 		----------
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:   		Is in control of the Minotaur Agent behaviour
************************************************/

#include "Minotaur.h"
//#include "MinotaurIdle.h"
#include "MinotaurHostile.h"
#include "MinotaurPatrol.h"
//#include "MinotaurStomp.h"
//#include "MinotaurHurt.h"
//#include "MinotaurChase.h"
#include "MinotaurDead.h"

#include "MinotaurZone.h"
#include "../../Agents/AgentState.h"
#include "../../../../Engine/Core/Collision/Collision.h"
#include "../../../../Engine/Particles/Emitter.h"

#include "../../../../Engine/Particles/ParticleManager.h"
#include "../../../../Engine/Core/Collision/AABB.h"
#include "../../../../Engine/Core/Collision/Collider.h"
#include "../../../../Engine/Core/Collision/Capsule.h"
#include "../../../../Engine/AI/Node.h"
#include "../../../../Engine/Animation/Mesh.h"
#include "../../../../Engine/AI/PathFinding.h"
#include "../../../../Engine/Animation/Animation.h"
#include "../../../../Engine/Object Manager/IObject.h"
#include "../../../../Engine/Core/Input/InputManager.h"
#include "../../../../Engine/Asset Manager/AssetManager.h"
#include "../../../../Engine/Animation/AnimationMesh.h"
#include "../../../../Engine/Animation/AnimationManager.h"
#include "../../../../Engine/Object Manager/ObjectManager.h"
#include "../../../../Engine/Core/Messages/RemoveObjectMessage.h"
#include "../../../../Engine/Core/Messages/AddObjectMessage.h"
#include "../../../../Engine/AI/SteeringManager.h"
#include "../../Player/Player.h"
#include "../../Environment/Bridge.h"
#include "../Skeleton/Skeleton.h"
#include "../../../../Engine/AI/NavGraphManager.h"
#include "../../../../Engine/AI/CNavGraph.h"

#define XMF3DotProduct(a,b) (a.x*b.x + a.y*b.y + a.z*b.z)
CMinotaur::CMinotaur() : CBoid(20.0f, 500.0f, "Minotaur")
{

	m_cpObjectManager = nullptr;
	m_cpAnimationManager = nullptr;
}



CMinotaur::CMinotaur(const XMFLOAT3& spawnPosition, CObjectManager* _manager, CAnimationManager* _animManager) : CBoid(15.0f, 300.0f, "Minotaur")
{
	// Drop Shadow Creation //
	CMesh cDropShadow;

	cDropShadow.GetIndices().push_back(0);
	cDropShadow.GetIndices().push_back(1);
	cDropShadow.GetIndices().push_back(2);

	cDropShadow.GetIndices().push_back(0);
	cDropShadow.GetIndices().push_back(2);
	cDropShadow.GetIndices().push_back(3);

	cDropShadow.GetVertices().push_back
		({
			{ -400.0f, 3.0f, 400.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 0, 0 }
	});

	cDropShadow.GetVertices().push_back
		({
			{ 400.0f, 3.0f, 400.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 1, 0 }
	});

	cDropShadow.GetVertices().push_back
		({
			{ 400.0f, 3.0f, -400.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 1, 1 }
	});

	cDropShadow.GetVertices().push_back
		({
			{ -400.0f, 3.0f, -400.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 0, 1 }
	});

	m_cDropShadow = new CRenderMesh(&cDropShadow, GRAPHICS->GetStandardVS(), GRAPHICS->GetStandardPS());
	m_cDropShadow->SetTexture(L"../Game/Assets/Art/2D/Textures/Shadow.dds");
	m_cDropShadow->GetTransparent() = true;
	m_cDropShadow->GetRender() = false;
	GRAPHICS->AddRenderMesh(m_cDropShadow);

	XMStoreFloat4x4(&m_mWorld, XMMatrixTranslation(spawnPosition.x, spawnPosition.y, spawnPosition.z));
	m_tSpawnPosition = spawnPosition;
	m_pPathPlanner = new CPathFinding("MinotaurNavGraph", _manager, false);

	CNavGraph* cNavGraph = CNavGraphManager::GetReference().GetNavGraph("MinotaurNavGraph");

	int unNodes[2];
	unNodes[0] = rand() % cNavGraph->GetNodes().size();
	unNodes[1] = rand() % cNavGraph->GetNodes().size();
	if (unNodes[1] == unNodes[0])
	{
		unNodes[1]++;
	}

	m_vWaypoints.push_back(unNodes[0]);
	m_vWaypoints.push_back(unNodes[1]);



	//m_pSteering = new CSteeringManager(this);

	m_pAssetManager = CAssetManager::GetInstance();
	m_cpAnimationManager = _animManager;
	m_cpObjectManager = _manager;
	m_pPlayer = m_cpObjectManager->GetPlayer();
	CMesh* m_cMesh = m_pAssetManager->GetPrefabMesh("minotaur");
	m_cpRenderMesh = new CAnimationMesh(m_cMesh, GRAPHICS->GetSkinningVS(), GRAPHICS->GetStandardPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Minotaur.dds");
	m_cpRenderMesh->GetRender() = false;
	GRAPHICS->AddRenderMesh(m_cpRenderMesh);

	enum EMinotaurAnimation{ eIdleAnim, eRunAnim, eTellAnim, eChargeAnim, eStompAnim, eDeathAnim, eEnrageAnim, eStunnedAnim, eAnimCount };

	m_cpAnimations[eIdleAnim] = CAssetManager::GetInstance()->GetAnimation("MinotaurIdle");
	m_cpAnimations[eRunAnim] = CAssetManager::GetInstance()->GetAnimation("MinotaurRun");
	m_cpAnimations[eTellAnim] = CAssetManager::GetInstance()->GetAnimation("MinotaurTell");
	m_cpAnimations[eChargeAnim] = CAssetManager::GetInstance()->GetAnimation("MinotaurCharge");
	m_cpAnimations[eStompAnim] = CAssetManager::GetInstance()->GetAnimation("MinotaurStomp");
	m_cpAnimations[eDeathAnim] = CAssetManager::GetInstance()->GetAnimation("MinotaurDeath");
	m_cpAnimations[eEnrageAnim] = CAssetManager::GetInstance()->GetAnimation("MinotaurEnrage");
	m_cpAnimations[eStunnedAnim] = CAssetManager::GetInstance()->GetAnimation("MinotaurHurt");





	for (unsigned int animation = 0; animation < eAnimCount; animation++)
	{
		m_cpAnimations[animation].SetAnimMesh((CAnimationMesh*)m_cpRenderMesh);

	}
	//m_cpAnimationManager->AddAnimation(&m_cpAnimations[eRunAnim]);
	/*m_cpAnimationManager->AddAnimation(&m_cpAnimations[eWalkAnim]);
	m_cpAnimationManager->AddAnimation(&m_cpAnimations[eRunAnim]);
	m_cpAnimationManager->AddAnimation(&m_cpAnimations[eStompAnim]);*/




	CCapsule*  pCapsule = new CCapsule(XMFLOAT3(spawnPosition.x, 90.0f, spawnPosition.z), XMFLOAT3(spawnPosition.x, 250.0f, spawnPosition.z), 90.0f);
	CCollider* pCapCollider = new CCollider(true, pCapsule, true, false);
	CCollider* pBoxCollider = new CCollider(true, new CAABB(float3(spawnPosition.x, 250.0f, spawnPosition.z), float3(250.0f, 250.0f, 250.0f)), false, false);

	CCollider* pZoneCollider = new CCollider(true, new CAABB(float3(spawnPosition.x, 250.0f, spawnPosition.z), float3(2500.0f, 250.0f, 2500.0f)), true, false);

	


	m_pMinotaurZone = new CMinotaurZone();

	m_pMinotaurZone->AddCollider(pZoneCollider);
	pBoxCollider->GetBounds()->UpdatePosition(spawnPosition);
	AddCollider(pBoxCollider);
	AddCollider(pCapCollider);



	m_pParticleManager = new CParticleManager(_manager);
	m_unParticleEmitters[eBloodParticle] = m_pParticleManager->LoadEmitter("BloodDrop.xml");
	m_unParticleEmitters[eBloodString] = m_pParticleManager->LoadEmitter("BloodString.xml");
	m_unParticleEmitters[eBloodMist] = m_pParticleManager->LoadEmitter("BloodMist.xml");
	m_unParticleEmitters[eBloodMist2] = m_pParticleManager->LoadEmitter("BloodMist.xml");
	m_unParticleEmitters[eStompDustOut] = m_pParticleManager->LoadEmitter("MinotaurStompDust1.xml");
	m_unParticleEmitters[eStompDustUp] = m_pParticleManager->LoadEmitter("MinotaurStompDust2.xml");
	m_unParticleEmitters[eStepDust] = m_pParticleManager->LoadEmitter("MinoStepDust.xml");
	m_unParticleEmitters[eChargeParticle] = m_pParticleManager->LoadEmitter("MinoCharge.xml");
	m_pParticleManager->GetEmitter(m_unParticleEmitters[eChargeParticle])->GetParent() = this;
	m_pParticleManager->GetEmitter(m_unParticleEmitters[eChargeParticle])->Stop();

	m_pStates[ePatrolState] = new CMinotaurPatrol;
	m_pStates[eHostileState] = new CMinotaurHostile;
	m_pStates[eDeadState] = new CMinotaurDead;


	m_unCurrAnim = -1;
	m_nCurrState = ePatrolState;
	//Activate();

}
CMinotaur::~CMinotaur()
{
	SAFE_DELETE(m_pPathPlanner);
	SAFE_DELETE(m_cDropShadow);

	for (unsigned int state = 0; state < eNumStates; state++)
		SAFE_DELETE(m_pStates[state]);

	SAFE_DELETE(m_pParticleManager);

	SAFE_DELETE(m_pMinotaurZone);


	//GRAPHICS->RemoveRenderMesh(m_cpRenderMesh);
	m_vPath.clear();
}

void CMinotaur::Kill()
{
	ChangeState(eDeadState);
}

void CMinotaur::Update()
{
	if (m_bChargeParticlePlaying)
	{
		m_pParticleManager->GetEmitter(m_unParticleEmitters[eChargeParticle])->GetPosition(0) = GetPosition()->x;
		m_pParticleManager->GetEmitter(m_unParticleEmitters[eChargeParticle])->GetPosition(2) = GetPosition()->z;
	}
	m_pParticleManager->Update();
	int nNewState = 0;

	if (m_bIsGrounded)
	{
		if (m_fStompTimer > 0.0f)
		{
			m_fStompTimer -= DELTA_TIME();
			if (m_fStompTimer < 0.0f)
				m_fStompTimer = 0.0f;
		}
		if (m_fSlowTimer > 0.0f)
		{
			m_fSlowTimer -= DELTA_TIME();
			if (m_fSlowTimer < 0.0f)
				m_fSlowTimer = 0.0f;
		}
		if (m_nCurrState != eDeadState)
		{
			AdjustRotation();

		}

		//Run the current State
		m_pStates[m_nCurrState]->Execute(this);
	}

	m_f3Velocity.y += GRAVITY * DELTA_TIME();

	if (GetPosition()->y < -900.0f)
		DeActivate();

	if (m_mWorld.m[3][1] < 0.0f)
	{
		m_bIsGrounded = false;
	}

	// Update the rotation

	CCapsule* pCapsule = (CCapsule*)m_pvColliders[1]->GetBounds();

	XMFLOAT3 f3NewStart = pCapsule->GetStart();
	XMFLOAT3 f3NewEnd = pCapsule->GetEnd();

	f3NewStart.x = f3NewEnd.x = m_mWorld._41;
	f3NewStart.z = f3NewEnd.z = m_mWorld._43;

	pCapsule->SetStart(f3NewStart);
	pCapsule->SetEnd(f3NewEnd);

	m_pMinotaurZone->UpdateColliders(GetWorldVelocity());
	UpdateColliders(GetWorldVelocity());
	// Update states
	if (m_bIsGrounded)
	{
		nNewState = m_pStates[m_nCurrState]->UpdateState(this);

	}



	if (nNewState == -1)
	{
		DeActivate();
		return;
	}

	if (nNewState != m_nCurrState)
		ChangeState(nNewState);

#if _DEBUG
	/*if (m_vPath.size())
	{
	for (size_t i = 0; i < m_vPath.size() - 1; i
	{
	XMFLOAT3 thisPos = m_vPath[i];
	XMFLOAT3 nextPos = m_vPath[i + 1];
	GRAPHICS->DrawLine(thisPos, nextPos, { 100, 100, 0 });
	}
	}*/
	XMFLOAT3 start = ((CCapsule*)m_pvColliders[1]->GetBounds())->GetStart();
	XMFLOAT3 end = ((CCapsule*)m_pvColliders[1]->GetBounds())->GetEnd();
	//GRAPHICS->DrawLine(start, end, { 1, 0, 1 });
	DEBUG_UTIL::DrawCapsuleCollider(start, end, 150.0f);
#endif

	SnapPosition(m_cDropShadow->GetTransformMatrix(), m_mWorld);
	m_cDropShadow->GetTransformMatrix().m[3][1] += 10.0f;

	if (m_bIsActive == false)
		m_cDropShadow->GetTransformMatrix().m[3][1] = -20000.0f;
}
void CMinotaur::PlayStepParticle()
{
	XMFLOAT3 pos = *GetPosition();
	pos.z += 40.0f;
	if (m_bStepBool)
	{
		pos.x -= 75.0f;
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::MINOSTEP1, *GetPosition());

	}
	else
	{
		pos.x += 75.0f;
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::MINOSTEP2, *GetPosition());

	}

	PlayParticle(m_unParticleEmitters[eStepDust], pos);
	m_bStepBool = !m_bStepBool;


}
void CMinotaur::PlayChargeParticle()
{
	XMFLOAT3 pos = *GetPosition();
	pos.y += 480.0f;
	PlayParticle(m_unParticleEmitters[eChargeParticle], pos);
	m_bChargeParticlePlaying = true;
}
void CMinotaur::PlayStopParticle()
{

}
void CMinotaur::PlayHurtParticle()
{
	PlayParticle(m_unParticleEmitters[eBloodParticle], { 0.0f, 300.0f, 0.0f });
	PlayParticle(m_unParticleEmitters[eBloodString], { 0.0f, 300.0f, 0.0f });
	PlayParticle(m_unParticleEmitters[eBloodMist], { 0.0f, 300.0f, 0.0f });
	PlayParticle(m_unParticleEmitters[eBloodMist2], { 0.0f, 320.0f, 0.0f });
}
void CMinotaur::StopChargeParticle()
{
	m_pParticleManager->GetEmitter(m_unParticleEmitters[eChargeParticle])->Stop();
	m_bChargeParticlePlaying = false;
}
void CMinotaur::CollisionResponse(IObject* _cCollidedObject)
{
	//DBG_UNREFERENCED_PARAMETER(_cCollidedObject);
	/*if (_cCollidedObject->GetTag() == "Floor" || _cCollidedObject->GetTag() == "SafeHavenFloor")
	{
	if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
	{
	CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);

	}
	}*/

	if (_cCollidedObject->GetTag() == "Wall" || _cCollidedObject->GetTag() == "Door" || _cCollidedObject->GetTag() == "CrackedWall" || _cCollidedObject->GetTag() == "Enemy Cave Wall")
	{

		for (size_t i = 1; i < _cCollidedObject->GetColliders().size(); i++)
		{
			if (_cCollidedObject->GetColliders()[i]->GetCollided())
			{
				if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::AABB)
				{
					CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
					_cCollidedObject->GetColliders()[i]->SetCollided(false);
				}
				else if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::Plane)
				{
					if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
					{
						CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
						_cCollidedObject->GetColliders()[i]->SetCollided(false);
						m_bIsGrounded = true;
					}
				}
			}
		}
	}
	else if (_cCollidedObject->GetTag() == "MinotaurSpawn")
	{
	
		for (size_t i = 1; i < _cCollidedObject->GetColliders().size(); i++)
		{
			if (_cCollidedObject->GetColliders()[i]->GetCollided())
			{
				if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::AABB)
				{
					CAABB* bounds = (CAABB*)(_cCollidedObject->GetColliders()[i]->GetBounds());

					CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);

					_cCollidedObject->GetColliders()[i]->SetCollided(false);
				}
				else if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::Plane)
				{
					if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
					{
						CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
						_cCollidedObject->GetColliders()[i]->SetCollided(false);
						m_bIsGrounded = true;
					}
				}
			}
		}
	}
	else if (_cCollidedObject->GetTag() == "SmallPit")
	{
		if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
		{
			CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[_cCollidedObject->GetColliders().size() - 1]);
			_cCollidedObject->GetColliders()[_cCollidedObject->GetColliders().size() - 1]->SetCollided(false);

			m_bIsGrounded = true;

		}
		else if (m_f3Velocity.y < 0.0f)
		{
			m_bIsGrounded = true;

			for (size_t i = 1; i < _cCollidedObject->GetColliders().size() - 1; i++)
			{
				if (_cCollidedObject->GetColliders()[i]->GetCollided())
				{
					CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
					_cCollidedObject->GetColliders()[i]->SetCollided(false);
				}
			}
		}
		//CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[GetColliders().size() - 1]);
	}
	else if (_cCollidedObject->GetTag() == "BigPit")
	{
		if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
		{
			CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[_cCollidedObject->GetColliders().size() - 1]);
			_cCollidedObject->GetColliders()[_cCollidedObject->GetColliders().size() - 1]->SetCollided(false);

			m_bIsGrounded = true;

		}
		else if (m_f3Velocity.y < 0.0f)
		{

			for (size_t i = 1; i < _cCollidedObject->GetColliders().size() - 1; i++)
			{
				if (_cCollidedObject->GetColliders()[i]->GetCollided())
				{
					CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
					_cCollidedObject->GetColliders()[i]->SetCollided(false);
				}
			}
		}
	}
	else if (_cCollidedObject->GetTag() == "CaveSpawn")
	{
		CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
		_cCollidedObject->GetColliders()[0]->SetCollided(false);

	}
	else if (_cCollidedObject->GetTag() == "Player")
	{
		CPlayer* player = reinterpret_cast<CPlayer*>(GetPlayer());
		player->TakeDamage(200);
		if (player->GetHealth() > 0)
		{
			XMFLOAT3 pos = *GetPosition();
			XMFLOAT3 playerPos = *player->GetPosition();
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::MINOGRUNT, pos);
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYERTRAMPLE, playerPos);
		}
		//_cCollidedObject->GetColliders()[1]->SetCollided(false);

	}
	else if (_cCollidedObject->GetTag() == "SafeHavenFloor")
	{
		ChangeState(eDeadState);
	}
	else if (_cCollidedObject->GetTag() == "Floor")
	{
		if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
		{
			CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
			m_bIsGrounded = true;
		}
	}

	else if (_cCollidedObject->GetTag() == "SpikeTrap" || _cCollidedObject->GetTag() == "SpinningBlade")
	{
		if (m_fSlowTimer >= 0.0f)
		{
			m_fSlowTimer = SLOW_TIME;
			SetMoveSpeed(GetMoveSpeed() * 0.5f);
			m_fCurrentHealth -= 10.0f;
		}
	}
	else if (_cCollidedObject->GetTag() == "TrapPrefab")
	{

		CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
		m_bIsGrounded = true;

		/*for (size_t i = 1; i < _cCollidedObject->GetColliders().size(); i++)
		{
		if (_cCollidedObject->GetColliders()[i]->GetCollided())
		{
		if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::AABB)
		{
		if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
		CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
		else
		CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);

		m_bIsGrounded = true;

		_cCollidedObject->GetColliders()[i]->SetCollided(false);
		}
		else if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::Plane)
		{
		if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
		{
		CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
		_cCollidedObject->GetColliders()[i]->SetCollided(false);
		m_bIsGrounded = true;
		}
		}
		}
		}*/
	}
	else if (_cCollidedObject->GetTag() == "ExitDoor")
	{

		CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
		_cCollidedObject->GetColliders()[0]->SetCollided(false);

	}
	else if (_cCollidedObject->GetTag() == "Bridge")
	{
		CBridge* curBridge = reinterpret_cast<CBridge*>(_cCollidedObject);


		if (curBridge->IsActive() && curBridge->IsIntact())
		{



			if (curBridge->IsActive() && curBridge->IsIntact())
			{
				if (_cCollidedObject->GetColliders()[1]->GetCollided())
				{
					CAABB* bounds = ((CAABB*)_cCollidedObject->GetColliders()[1]->GetBounds());
					if ((bounds->GetExtents().x == 250.0f &&
						bounds->GetExtents().y == 150.0f &&
						bounds->GetExtents().z == 50.0f) || (bounds->GetExtents().x == 50.0f &&
						bounds->GetExtents().y == 150.0f &&
						bounds->GetExtents().z == 250.0f))
					{
						CBridge* bridge = (CBridge*)_cCollidedObject;
						bridge->Destroy();
						m_bIsGrounded = false;
						ChangeState(EMinotaurStates::eDeadState);

					}
					_cCollidedObject->GetColliders()[1]->SetCollided(false);


				}
				for (size_t i = 2; i < 10; i++)
				{
					if (_cCollidedObject->GetColliders()[i]->GetCollided())
					{
						if (GetPosition()->y < 20.0f)
						{
							CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
							_cCollidedObject->GetColliders()[i]->SetCollided(false);
							m_bIsGrounded = true;
						}
						else
						{
							CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
							_cCollidedObject->GetColliders()[i]->SetCollided(false);
						}
					}
				}
				for (size_t i = 10; i < _cCollidedObject->GetColliders().size(); i++)
				{
					if (_cCollidedObject->GetColliders()[i]->GetCollided())
					{

						CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
						_cCollidedObject->GetColliders()[i]->SetCollided(false);
					}
				}

				//for (size_t i = 1; i < _cCollidedObject->GetColliders().size(); i++)
				//{
				//	if (_cCollidedObject->GetColliders()[i]->GetCollided())
				//	{
				//		//CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
				//		_cCollidedObject->GetColliders()[i]->SetCollided(false);
				//	}
				//}
				//if (_cCollidedObject->GetColliders()[0]->GetCollided())
				//{
				//	//CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
				//	_cCollidedObject->GetColliders()[0]->SetCollided(false);
				//	m_bIsGrounded = true;
				//}
			}

		}
	}

	else if (_cCollidedObject->GetTag() == "CrackedWall")
	{
		for (size_t i = 0; i < _cCollidedObject->GetColliders().size() - 4; i++)
		{
			if ((_cCollidedObject->GetColliders()[i])->GetCollided())
			{
				if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::AABB)
				{
					CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
					_cCollidedObject->GetColliders()[i]->SetCollided(false);
				}
				else if (_cCollidedObject->GetColliders()[i]->GetType() == Bounds::Plane)
				{
					CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
					_cCollidedObject->GetColliders()[i]->SetCollided(false);
					m_bIsGrounded = true;
				}
			}
		}
	}
	else if (_cCollidedObject->GetTag() == "Minotaur")
	{
		CapsuleToCapsule(this, m_pvColliders[1], _cCollidedObject->GetColliders()[1]);
	}
	else if (_cCollidedObject->GetTag() == "Skeleton")
	{
		reinterpret_cast<CSkeleton*>(_cCollidedObject)->TakeDamage(200);
	}
	else if (_cCollidedObject->GetTag() == "PitWall")
	{
		//		m_f3Velocity.y = 1000.0f;
	}
}
/*****************************************************************
* ChangeState()			Changes the current state, exits the old one and enter the new.
*
* Ins:					IAgentState<CMinotaur>*
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    CM
*****************************************************************/
void CMinotaur::ChangeState(int _newState)
{
	// Exit the current state
	m_pStates[m_nCurrState]->Exit(this);

	// Set the new state
	m_nCurrState = _newState;

	// Enter the new state.
	m_pStates[m_nCurrState]->Enter(this);

}
void CMinotaur::UpdateState()
{

}

void CMinotaur::AdjustRotation()
{
	if (m_vPath.size() > 0)
	{
		XMFLOAT3 up = XMFLOAT3(0, 1, 0);
		XMVECTOR newX, newY, newZ, globalY;
		globalY = XMLoadFloat3(&up);

		XMVECTOR targetLocation;
		if (m_nCurrentNodeOnPath != -1)
			targetLocation = XMLoadFloat3(&m_vPath[m_nCurrentNodeOnPath]);
		else
			targetLocation = XMLoadFloat3(m_pPlayer->GetPosition());

		XMVECTOR currentLocation = XMLoadFloat3(GetPosition());

		newZ = targetLocation - currentLocation;
		newX = XMVector3Cross(globalY, newZ);
		newY = XMVector3Cross(newZ, newX);

		newX = XMVector3Normalize(newX);
		newY = XMVector3Normalize(newY);
		newZ = XMVector3Normalize(newZ);

		XMFLOAT3 xAxis, yAxis, zAxis;
		XMStoreFloat3(&xAxis, newX);
		XMStoreFloat3(&yAxis, newY);
		XMStoreFloat3(&zAxis, newZ);

		xAxis.y = 0.0f;
		yAxis.x = 0.0f;
		yAxis.z = 0.0f;
		zAxis.y = 0.0f;

		SetXAxis(xAxis);
		SetYAxis(yAxis);
		SetZAxis(zAxis);
	}

}
void CMinotaur::Activate()
{
	if (m_bIsActive == false)
	{
		SetPosition(m_tSpawnPosition);
		for (unsigned int i = 0; i < GetColliders().size(); i++)
		{
			if (GetColliders()[i]->GetBounds()->GetType() == Bounds::Capsule)
			{
				((CCapsule*)GetColliders()[i]->GetBounds())->SetStart(XMFLOAT3(GetWorldMatrix()._41, GetWorldMatrix()._42 + 150.0f, GetWorldMatrix()._43));
				((CCapsule*)GetColliders()[i]->GetBounds())->SetEnd(XMFLOAT3(GetWorldMatrix()._41, GetWorldMatrix()._42 + 135.0f, GetWorldMatrix()._43));
				((CCapsule*)GetColliders()[i]->GetBounds())->SetRadius(150.0f);
			}
			else if (GetColliders()[i]->GetBounds()->GetType() == Bounds::AABB)
			{
				((CAABB*)GetColliders()[i]->GetBounds())->SetCenter({ GetWorldMatrix()._41, GetWorldMatrix()._42 + 45, GetWorldMatrix()._43 });
			}

			GetColliders()[i]->UpdatePosition(GetWorldVelocity());
		}
		m_cpRenderMesh->GetRender() = true;
		m_cDropShadow->GetRender() = true;

		m_cpRenderMesh->SetTexture(L"../Game/Assets/Art/2D/Textures/Minotaur.dds");
		IObject::UpdatePosition();
		// Create a AddObj Message
		CAddObjectMessage* addObj = new CAddObjectMessage(this, CObjectManager::AI);
		CAddObjectMessage* addObj2 = new CAddObjectMessage(m_pMinotaurZone, CObjectManager::AI);

		//ChangeState(eIdleState);
		// Send the message
		ChangeState(ePatrolState);
		addObj->Send();
		addObj2->Send();
		m_fCurrentHealth = m_fMaxHealth;
		m_bInvincible = false;
		m_bIsActive = true;

		m_vDoors = m_cpObjectManager->FindObjectsWithTag("Door", CObjectManager::Static);
		m_pCamera = (CCamera*)m_cpObjectManager->GetCamera();

		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::MINOALERT, m_tSpawnPosition);
	}

}
void CMinotaur::DeActivate()
{
	if (m_bIsActive == true)
	{
		// Build a message
		CRemoveObjectMessage* removeObj = new CRemoveObjectMessage(this, CObjectManager::AI);

		// Send the message
		removeObj->Send();
		m_pStates[m_nCurrState]->Exit(this);
		m_bInvincible = false;
		m_bIsActive = false;
		m_cpRenderMesh->GetRender() = false;
		m_cDropShadow->GetRender() = false;

		for (size_t i = 0; i < eParticleCount; i++)
		{
			m_pParticleManager->GetEmitter(i)->Stop();

		}

		m_vPath.clear();
	}
}
void CMinotaur::PlayParticle(unsigned int _toPlay, XMFLOAT3 _relativePos)
{

	m_pParticleManager->GetEmitter(_toPlay)->Stop();
	m_pParticleManager->GetEmitter(_toPlay)->GetPosition(0) = _relativePos.x;
	m_pParticleManager->GetEmitter(_toPlay)->GetPosition(1) = _relativePos.y;
	m_pParticleManager->GetEmitter(_toPlay)->GetPosition(2) = _relativePos.z;
	m_pParticleManager->GetEmitter(_toPlay)->ePlay();
}
void CMinotaur::TakeDamage(int damageAmount)
{
	m_fCurrentHealth -= (float)damageAmount;
	PlayHurtParticle();
}

void CMinotaur::SwitchAnimation(EMinotaurAnimation _animation)
{
	if (m_unCurrAnim == _animation)
		return;
	if (m_unCurrAnim == -1)
	{
		m_unCurrAnim = _animation;
		m_cpAnimationManager->Switch(&m_cpAnimations[m_unCurrAnim], &m_cpAnimations[_animation]);
		return;
	}

	m_cpAnimationManager->Switch(&m_cpAnimations[m_unCurrAnim], &m_cpAnimations[_animation]);
	m_unCurrAnim = _animation;
}
bool CMinotaur::RayCastToPlayer(float& fHitTime)
{
	XMFLOAT3 start = *GetPosition();
	XMFLOAT3 startL = start;
	XMFLOAT3 startR = start;

	startL.x += GetVelocity().x * 45;
	startR.x -= GetVelocity().x * 45;



	XMFLOAT3 end = *GetPlayer()->GetPosition();
	start.y = 75.0f;
	end.y = 75.0f;
	TRay tRay;
	tRay.f3Start = startL;
	tRay.f3Direction.x = end.x - startL.x;
	tRay.f3Direction.y = end.y - startL.y;
	tRay.f3Direction.z = end.z - startL.z;
	float fDist = sqrt(XMF3DotProduct(tRay.f3Direction, tRay.f3Direction));
	tRay.f3Direction.x /= fDist;
	tRay.f3Direction.y /= fDist;
	tRay.f3Direction.z /= fDist;

	tRay.fDistance = fDist - 46.0f;

	float fMinX, fMaxX, fMinZ, fMaxZ;
	fMinX = min(start.x, end.x);
	fMaxX = max(start.x, end.x);
	fMinZ = min(start.z, end.z);
	fMaxZ = max(start.z, end.z);

	std::vector<IObject*> vColliders = m_cpObjectManager->GetQuadTree()->GetNearbyObjects(fMinX, fMinZ, fMaxX, fMaxZ);
	for (unsigned int i = 0; i < m_vDoors.size(); i++)
	{
		vColliders.push_back(m_vDoors[i]);
	}
	bool bOutput = true;
	float tempHitTime = 0.0f;
	if (LineSegmentIntersection(tRay, vColliders, fHitTime) == false)
	{
		CCapsule* pCapsule = (CCapsule*)m_pvColliders[1]->GetBounds();
		float fRadius = pCapsule->GetRadius();
		tRay.f3Start.x += fRadius;
		tRay.fDistance = sqrt(XMF3DotProduct(tRay.f3Direction, tRay.f3Direction)) - 46.0f;
		if (LineSegmentIntersection(tRay, vColliders, fHitTime) == false)
		{
			tRay.f3Start.x -= fRadius * 2;
			tRay.fDistance = sqrt(XMF3DotProduct(tRay.f3Direction, tRay.f3Direction)) - 46.0f;

			if (LineSegmentIntersection(tRay, vColliders, fHitTime) == false)
			{
				tRay.f3Start.z += fRadius;
				tRay.f3Start.x += fRadius;
				tRay.fDistance = sqrt(XMF3DotProduct(tRay.f3Direction, tRay.f3Direction)) - 46.0f;

				if (LineSegmentIntersection(tRay, vColliders, fHitTime) == false)
				{

					tRay.f3Start.z -= fRadius*2.0f;
					tRay.f3Start.x += fRadius;
					tRay.fDistance = sqrt(XMF3DotProduct(tRay.f3Direction, tRay.f3Direction)) - 46.0f;

					if (LineSegmentIntersection(tRay, vColliders, fHitTime) == false)
					{
						bOutput = false;
					}
				}
			}
		}
	}
	return bOutput;
}
bool CMinotaur::RayCastInfinite(float& fHitTime, XMFLOAT3& f3CollisionPoint)
{
	XMFLOAT3 start = *GetPosition();
	XMFLOAT3 end = *GetPlayer()->GetPosition();
	start.y = 75.0f;
	end.y = 75.0f;
	TRay tRay;
	tRay.f3Start = start;
	tRay.f3Direction.x = end.x - start.x;
	tRay.f3Direction.y = end.y - start.y;
	tRay.f3Direction.z = end.z - start.z;
	float fDist = sqrt(XMF3DotProduct(tRay.f3Direction, tRay.f3Direction));
	tRay.f3Direction.x /= fDist;
	tRay.f3Direction.y /= fDist;
	tRay.f3Direction.z /= fDist;

	tRay.fDistance = 25000.0f;

	float fMinX, fMaxX, fMinZ, fMaxZ;
	fMinX = min(start.x, end.x);
	fMaxX = max(start.x, end.x);
	fMinZ = min(start.z, end.z);
	fMaxZ = max(start.z, end.z);

	std::vector<IObject*> vColliders = m_cpObjectManager->GetList(CObjectManager::Static);
	for (unsigned int i = 0; i < m_vDoors.size(); i++)
	{
		vColliders.push_back(m_vDoors[i]);
	}
	bool bOutput = true;
	if (LineSegmentIntersection(tRay, vColliders, fHitTime) == false)
	{
		bOutput = false;
	}
	if (bOutput)
	{
		f3CollisionPoint.x = tRay.f3Start.x + tRay.f3Direction.x * fHitTime;
		f3CollisionPoint.y = tRay.f3Start.y + tRay.f3Direction.y * fHitTime;
		f3CollisionPoint.z = tRay.f3Start.z + tRay.f3Direction.z * fHitTime;
	}
	return bOutput;
}
bool CMinotaur::RayCastForward(float& fHitTime)
{
	XMFLOAT3 start = *GetPosition();
	start.y = 75.0f;
	TRay tRay;
	tRay.f3Start = start;
	XMFLOAT3 f3ZAxis = *GetZAxis();
	tRay.f3Direction.x = f3ZAxis.x;
	tRay.f3Direction.y = f3ZAxis.y;
	tRay.f3Direction.z = f3ZAxis.z;

	tRay.fDistance = 25000.0f;


	std::vector<IObject*> vColliders = m_cpObjectManager->GetList(CObjectManager::Static);
	for (unsigned int i = 0; i < m_vDoors.size(); i++)
	{
		vColliders.push_back(m_vDoors[i]);
	}
	bool bOutput = true;
	if (LineSegmentIntersection(tRay, vColliders, fHitTime) == false)
	{
		bOutput = false;
	}
	return bOutput;
}

void CMinotaur::AddToRenderer()
{
	IObject::AddToRenderer();

	//particles
	m_unParticleEmitters[eBloodParticle] = m_pParticleManager->LoadEmitter("BloodDrop.xml");
	m_unParticleEmitters[eBloodString] = m_pParticleManager->LoadEmitter("BloodString.xml");
	m_unParticleEmitters[eBloodMist] = m_pParticleManager->LoadEmitter("BloodMist.xml");
	m_unParticleEmitters[eBloodMist2] = m_pParticleManager->LoadEmitter("BloodMist.xml");
	m_unParticleEmitters[eStompDustOut] = m_pParticleManager->LoadEmitter("MinotaurStompDust1.xml");
	m_unParticleEmitters[eStompDustUp] = m_pParticleManager->LoadEmitter("MinotaurStompDust2.xml");
	m_unParticleEmitters[eStepDust] = m_pParticleManager->LoadEmitter("MinoStepDust.xml");
	m_unParticleEmitters[eChargeParticle] = m_pParticleManager->LoadEmitter("MinoCharge.xml");
	m_pParticleManager->GetEmitter(m_unParticleEmitters[eChargeParticle])->GetParent() = this;
	m_pParticleManager->GetEmitter(m_unParticleEmitters[eChargeParticle])->Stop();
}
