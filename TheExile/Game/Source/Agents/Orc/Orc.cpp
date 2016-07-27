
#include "Orc.h"
#include "../../../../Engine/Core/Collision/AABB.h"
#include "../../../../Engine/Core/Collision/Capsule.h"
#include "../../Environment/Bone.h"
#include "../../Environment/DayNight.h"
#include "../../../../Engine/AI/Node.h"
#include "../../../../Engine/Core/Messages/RemoveObjectMessage.h"
#include "../../../../Engine/Core/Messages/AddObjectMessage.h"
#include "../../Player/Attack.h"
#include "../../../../Engine/Particles/Emitter.h"
#include "../../../../Engine/Particles/ParticleManager.h"

#include "../../../../Engine/Asset Manager/AssetManager.h"
#include "../../../../Engine/Animation/AnimationManager.h"
#include "../../../../Engine/Animation/Animation.h"
#include "../../../../Engine/Animation/AnimationMesh.h"

#include "../../../../Engine/AI/PathFinding.h"
#include "../../Traps/FireTrap.h"
#include "../../Traps/SpikeTrap.h"
#include "../../Traps/SpinningBlade.h"
#include "../../Environment/Bridge.h"
#include "../../Environment/DayNight.h"
#include "../../Environment/Bone.h"
#include "../../Player/Player.h"
#include "../../../../Engine/Object Manager/OrcBomb.h"
#include "../../Environment/Gunpowder.h"
#define GRAVITY -1500.0f

#define ATTACK_RANGE 125
#define LEFT_HAND "LeftHand"
#define GRAVITY -1500.0f
#define DAY_DAMAGE 15
#define DAY_BLOCK_CHANCE 35
#define DAY_MAX_HEALTH 100
#define DAY_MOVE_SPEED 500.0f
#define NIGHT_DAMAGE 25
#define NIGHT_BLOCK_CHANCE 70
#define NIGHT_MAX_HEALTH 200
#define NIGHT_MOVE_SPEED 500.0f
#define MOVE_SPEED 350.0f
#define THROW_TIME 2.5f
#define SWING_TIME 3.0f

#define THROW_DISTANCE 2500.0f

COrc::COrc(XMFLOAT3 posistion, COrcBomb* bomb, CObjectManager* _manager, CAnimationManager* _animManager, CDayNight* _dayNight) : CBoid(5.0f, 500.0f, "Orc")
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
			{ -100.0f, 3.0f, 100.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 0, 0 }
	});

	cDropShadow.GetVertices().push_back
		({
			{ 100.0f, 3.0f, 100.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 1, 0 }
	});

	cDropShadow.GetVertices().push_back
		({
			{ 100.0f, 3.0f, -100.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 1, 1 }
	});

	cDropShadow.GetVertices().push_back
		({
			{ -100.0f, 3.0f, -100.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 0, 1 }
	});

	m_cDropShadow = new CRenderMesh(&cDropShadow, GRAPHICS->GetStandardVS(), GRAPHICS->GetStandardPS());
	m_cDropShadow->SetTexture(L"../Game/Assets/Art/2D/Textures/Shadow.dds");
	m_cDropShadow->GetTransparent() = true;
	m_cDropShadow->GetRender() = false;

	GRAPHICS->AddRenderMesh(m_cDropShadow);

	CAssetManager* m_pAssetManager = CAssetManager::GetInstance();
	m_pBomb = bomb;
	SetPosition(posistion);
	m_tSpawnPosition = posistion;
	m_pPathPlanner = new CPathFinding("SkeletonNavGraph", _manager, false);
	m_cpAnimations = new CAnimation[eAnimCount];
	SetMaxVelocity(500.0f);
	m_pPlayer = _manager->GetPlayer();
	m_pDayNight = _dayNight;
	m_cpAnimationManager = _animManager;

	/*m_pBomb = new COrcBomb(this, _manager);*/
	m_bBuffed = false;
	TPointLight tempLight;
	tempLight.m_fRadius = 200.0f;
	tempLight.m_fColor[0] = 1.0f;
	tempLight.m_fColor[1] = 0.0f;
	tempLight.m_fColor[2] = 0.0f;
	tempLight.m_fBrightness = 2.5f;
	tempLight.m_fPosition[0] = 0.0f;
	tempLight.m_fPosition[1] = 0.0f;
	tempLight.m_fPosition[2] = 0.0f;
	tempLight.m_fPosition[3] = 1.0f;
	AddLight(tempLight);

	theLoot = new CGunpowder(_manager, this);


	m_bHit = false;

	CMesh* m_cMesh = m_pAssetManager->GetPrefabMesh("Vampire");
	// Create a render mesh for the object

	m_cpRenderMesh = new CAnimationMesh(m_cMesh, GRAPHICS->GetSkinningVS(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/GoblinDiffuse.dds");
	m_cpRenderMesh->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/GoblinNormal.dds");
	//	AttachToJoint(m_cpSword->S(), LEFT_HAND);
	m_cpRenderMesh->GetRender() = true;

	m_cpAnimations[eIdle1Anim] = CAssetManager::GetInstance()->GetAnimation("GoblinRun");
	m_cpAnimations[eIdle1Anim].SetLooping(true);

	m_cpAnimations[eIdle2Anim] = CAssetManager::GetInstance()->GetAnimation("GoblinRun");
	m_cpAnimations[eIdle2Anim].SetLooping(true);

	m_cpAnimations[eRunAnim] = CAssetManager::GetInstance()->GetAnimation("GoblinRun");
	m_cpAnimations[eRunAnim].SetLooping(true);

	m_cpAnimations[eMeleeAnim] = CAssetManager::GetInstance()->GetAnimation("GoblinMelee");
	m_cpAnimations[eMeleeAnim].SetLooping(false);
	m_cpAnimations[eMeleeAnim].SetScale(2.0f);


	m_cpAnimations[eDeathAnim] = CAssetManager::GetInstance()->GetAnimation("GoblinDeath1");
	m_cpAnimations[eDeathAnim].SetLooping(false);

	m_cpAnimations[eHurtAnim] = CAssetManager::GetInstance()->GetAnimation("GoblinHurt1");
	m_cpAnimations[eHurtAnim].SetLooping(false);

	m_cpAnimations[eHurt2Anim] = CAssetManager::GetInstance()->GetAnimation("GoblinHurt2");
	m_cpAnimations[eHurt2Anim].SetLooping(false);

	m_cpAnimations[ePreThrowAnim] = CAssetManager::GetInstance()->GetAnimation("GoblinThrow");
	m_cpAnimations[ePreThrowAnim].SetLooping(false);

	m_cpAnimations[eThrowBomb] = CAssetManager::GetInstance()->GetAnimation("GoblinThrow");
	m_cpAnimations[eThrowBomb].SetLooping(false);

	m_pParticleManager = new CParticleManager(_manager);

	m_unParticleEmitters[eBloodParticle] = m_pParticleManager->LoadEmitter("BloodDrop.xml");
	m_unParticleEmitters[eBloodString] = m_pParticleManager->LoadEmitter("BloodString.xml");
	m_unParticleEmitters[eBloodMist] = m_pParticleManager->LoadEmitter("BloodMist.xml");
	m_unParticleEmitters[eBloodMist2] = m_pParticleManager->LoadEmitter("BloodMist.xml");
	m_unParticleEmitters[eFadeAway] = m_pParticleManager->LoadEmitter("EnemyFadeAway.xml");
	m_unParticleEmitters[eCaveSmoke1] = m_pParticleManager->LoadEmitter("EnemyCaveSmokeParticle.xml");
	m_unParticleEmitters[eCaveSmoke2] = m_pParticleManager->LoadEmitter("EnemyCaveSmokeParticle2.xml");
	m_unParticleEmitters[eCaveSmoke3] = m_pParticleManager->LoadEmitter("EnemyCaveSmokeParticle.xml");


	for (unsigned int animation = 0; animation < eAnimCount; animation++)
		m_cpAnimations[animation].SetAnimMesh((CAnimationMesh*)m_cpRenderMesh);

	// Add the starting animation
	m_cpAnimationManager->AddAnimation(&m_cpAnimations[eRunAnim]);

	// Set the current animaton
	m_unCurrAnim = eRunAnim;

	AddCollider(new CCollider(true, new CAABB(float3(m_mWorld._41,90.0f, m_mWorld._43), float3(90.0f, 90.0f, 90.0f)), false));
	CCapsule* theCapsule = new CCapsule({ m_mWorld._41, 55.0f, m_mWorld._43 }, { m_mWorld._41, 240.0f, m_mWorld._43 }, 45.0f);
	AddCollider(new CCollider(true, theCapsule));

	m_cWepMesh = new CRenderMesh(CAssetManager::GetInstance()->GetPrefabMesh("OrcDagger"), GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/OrcDaggerDiffuse.dds");
	AttachToJoint(m_cWepMesh, "RightHand");
	GRAPHICS->AddRenderMesh(m_cWepMesh);

	m_cpAttack = new CAttack(this, CAssetManager::GetInstance()->GetPrefabMesh("OrcDagger"), &m_cWepMesh->GetPositionMatrix(), 20, "OrcMelee");
	m_cpAttack->SetParent(this);

	CAddObjectMessage* addObj = new CAddObjectMessage(m_cpAttack, CObjectManager::Dynamic);
	CAddObjectMessage* addObj1 = new CAddObjectMessage(this, CObjectManager::AI);

	m_fInvincibilityTimer = 0.0f;

	// Send the message
	addObj->Send();
	addObj1->Send();
	GRAPHICS->AddRenderMesh(m_cpRenderMesh);
	GRAPHICS->AddLight(&m_vtPointLights[0]);
	m_bIsActive = true;

	m_eCurrState = eSpawnState;
	m_eNewState = eSpawnState;
	m_bIsGrounded = true;

	// Set the temp state
	EnterSubState();
	//m_cpTheLoot = new CBone(_manager, this);
}
COrc::~COrc()
{
	GRAPHICS->RemoveLight(&m_vtPointLights[0]);
	GRAPHICS->RemoveRenderMesh(m_cpRenderMesh);
	GRAPHICS->RemoveRenderMesh(m_cDropShadow);

	SAFE_DELETE(m_cDropShadow);
	SAFE_DELETE(m_pPathPlanner);
	SAFE_DELETE(m_pParticleManager);
	SAFE_DELETE(theLoot);
	SAFE_DELETE(m_cpAttack);
	SAFE_DELETE(m_cWepMesh);
	SAFE_DELETE(m_pBomb);

	

	delete[] m_cpAnimations;
	m_cpAnimations = nullptr;
	m_vPath.clear();
	m_vtPointLights.clear();
}
void COrc::Update()
{
	float3 target = float3(0.0f, 0.0f, 0.0f);
	m_f3Velocity = XMFLOAT3(0.0f, m_f3Velocity.y, 0.0f);
	m_pParticleManager->Update();

	if (m_fThrowTimer > 0.0f)
	{
		m_fThrowTimer -= DELTA_TIME();
		if (m_fThrowTimer <= 0.0f)
			m_pBomb->GetRenderMesh()->GetRender() = true;
	}

	if (m_fInvincibilityTimer > 0.0f)
	{
		m_fInvincibilityTimer -= DELTA_TIME();
		if (m_fInvincibilityTimer < 0.0f)
			m_fInvincibilityTimer = 0.0f;
	}
	if (m_fSwingTimer > 0.0f)
		m_fSwingTimer -= DELTA_TIME();

	UpdateDayStatus();
	if (GetPosition()->y < -800.0f)
	{
		m_bHit = true;
		m_nCurrHealth = 0;
		m_bIsGrounded = true;
		m_eNewState = eHurtState;
	}
	// Exeute the current action
	if (m_bIsGrounded)
	{
		//AdjustRotation();
		CheckState();
		switch (m_eCurrState)
		{
		case eIdleState:
			m_eNewState = Idle();
			break;
		case eReposition:
			m_eNewState = Reposition();
			break;
		case eMeleeState:
			m_eNewState = Melee();
			break;
		case eMoveIn:
			m_eNewState = MoveIn();
			break;
		case eThrowBombState:
			m_eNewState = ThrowBomb();
			break;
		case eFollowState:
			m_eNewState = Follow();
			break;
		case eRetreatState:
			m_eNewState = Retreat();
			break;
		case eHurtState:
			m_eNewState = Hurt();
			break;
		case eSpawnState:
			m_eNewState = Spawn();
			break;
		case eDeathState:
			m_eNewState = Dead();
			break;
		case eNumStates:
			DeActivate();
			m_eNewState = eSpawnState;
			break;
		default:
			break;
		}
		m_vtPointLights[0].m_fPosition[0] = GetPosition()->x;
		m_vtPointLights[0].m_fPosition[1] = 150.0f;
		m_vtPointLights[0].m_fPosition[2] = GetPosition()->z;
	}
	else
	{
		SetWorldVelocity({ knockBackVelocity.x, m_f3Velocity.y, knockBackVelocity.z });
		m_vtPointLights[0].m_fPosition[0] += GetWorldVelocity().x * DELTA_TIME();
		m_vtPointLights[0].m_fPosition[1] += GetWorldVelocity().y * DELTA_TIME();
		m_vtPointLights[0].m_fPosition[2] += GetWorldVelocity().z * DELTA_TIME();
	}

	XMFLOAT3 start = ((CCapsule*)m_pvColliders[1]->GetBounds())->GetStart();
	XMFLOAT3 end = ((CCapsule*)m_pvColliders[1]->GetBounds())->GetEnd();



	m_f3Velocity.y += GRAVITY * DELTA_TIME();

	if (m_mWorld.m[3][1] < 0.0f)
	{
		m_bIsGrounded = false;
	}
	/*CCapsule* pCapsule = (CCapsule*)m_pvColliders[1]->GetBounds();

	XMFLOAT3 f3NewStart = pCapsule->GetStart();
	XMFLOAT3 f3NewEnd = pCapsule->GetEnd();

	f3NewStart.x = f3NewEnd.x = m_mWorld._41;
	f3NewStart.z = f3NewEnd.z = m_mWorld._43;

	pCapsule->SetStart(f3NewStart);
	pCapsule->SetEnd(f3NewEnd);
*/


	for (unsigned int collider = 0; collider < m_pvColliders.size(); collider++)
	{
		m_pvColliders[collider]->UpdatePosition({ GetWorldVelocity().x * DELTA_TIME(), GetWorldVelocity().y * DELTA_TIME(), GetWorldVelocity().z * DELTA_TIME() });
	}

	
	//DEBUG::DrawCapsuleCollider(f3NewStart, f3NewEnd, pCapsule->GetRadius());

#if _DEBUG



#endif

	SnapPosition(m_cDropShadow->GetTransformMatrix(), m_mWorld);
	m_cDropShadow->GetTransformMatrix().m[3][1] += 10.0f;

	if (m_mWorld.m[3][1] < -10.0f || !m_bIsActive)
		m_cDropShadow->GetTransformMatrix().m[3][1] = -20000.0f;

}
void COrc::UpdatePosition()
{
	IObject::UpdatePosition();

	XMFLOAT4X4 joint = ((CAnimationMesh*)m_cpRenderMesh)->GetJointMatrix("Hips");

	XMMATRIX mat1 = XMLoadFloat4x4(&m_mWorld);
	XMMATRIX mat2 = XMLoadFloat4x4(&joint);

	XMStoreFloat4x4(&joint, XMMatrixMultiply(mat2, mat1));
	float3 pos = float3(joint._41, joint._42, joint._43);


	m_pvColliders[0]->GetBounds()->SetCenter(pos);
	m_pvColliders[1]->GetBounds()->SetCenter(pos);

	CAABB* aabb = (CAABB*)(m_pvColliders[0]->GetBounds());
	CCapsule* capsule = (CCapsule*)(m_pvColliders[1]->GetBounds());

	//DEBUG_UTIL::DrawAABBCollider(aabb->Min(), aabb->Max());
	DEBUG_UTIL::DrawCapsuleCollider(capsule->GetStart(), capsule->GetEnd(), capsule->GetRadius());


}

void COrc::CollisionResponse(IObject* _cCollidedObject)
{

	if (_cCollidedObject->GetTag() == "PlayerAttack" && m_fInvincibilityTimer <= 0.0f && m_eCurrState != eSpawnState)
	{
		if (m_nCurrHealth > 0 && m_bHit == false)
		{
			m_nCurrHealth -= ((CAttack*)_cCollidedObject)->GetDamage();
			m_fInvincibilityTimer = 1.5f;
			m_bHit = true;
			m_eNewState = eHurtState;
		}
	}
	else if (_cCollidedObject->GetTag() == "MinotaurZone")
	{
		if (m_pDayNight->IsNight())
		{
			if (m_eNewState != eDeathState && m_eCurrState != eDeathState)
			{
				m_eNewState = eRetreatState;
			}
		}
	}
	else if (_cCollidedObject->GetTag() == "SpinningBlade")
	{
		CSpinningBlade* curTrap = reinterpret_cast<CSpinningBlade*>(_cCollidedObject);

		XMVECTOR mathPlayer = XMVector3Normalize(XMLoadFloat3(GetPosition()) - XMLoadFloat3(curTrap->GetPosition()));


		mathPlayer *= 500.0f;
		
		XMFLOAT3 toPlayer; XMStoreFloat3(&toPlayer, mathPlayer);

		SetWorldVelocity(toPlayer);

		TakeDamage(curTrap->GetTrapDamage());
		m_eNewState = eHurtState;

	}
	else if (_cCollidedObject->GetTag() == "Wall" || _cCollidedObject->GetTag() == "Door" || _cCollidedObject->GetTag() == "CrackedWall" ||
		_cCollidedObject->GetTag() == "SafeHaven" || _cCollidedObject->GetTag() == "Enemy Cave Wall" || _cCollidedObject->GetTag() == "MinotaurSpawn")
	{
		if (m_eCurrState == eReposition)
			m_eNewState = eMoveIn;

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
		if (_cCollidedObject->GetTag() == "SafeHaven")
		{
			m_eNewState = eRetreatState;

		}
	}
	
	else if (_cCollidedObject->GetTag() == "Floor" || _cCollidedObject->GetTag() == "SafeHavenFloor")
	{
		if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
		{
			CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[1]);
			m_bIsGrounded = true;
		}
	}
	else if (_cCollidedObject->GetTag() == "TrapPrefab")
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
	else if (_cCollidedObject->GetTag() == "PitWall")
	{
		if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
		{
			CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
			m_bIsGrounded = true;
		}
		else
		{
			CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
		}
	}
	else if (_cCollidedObject->GetTag() == "SmallPit")
	{
		if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
		{
			for (size_t i = 1; i < _cCollidedObject->GetColliders().size(); i++)
			{
				CCollider* tested = _cCollidedObject->GetColliders()[i];
				if (tested->GetCollided())
				{
					if (tested->GetType() == Bounds::Plane)
					{
						if (((CPlane*)(tested->GetBounds()))->GetCenter().y < 0.0f)
						{
							CapsuleToFloor(this, m_pvColliders[1], tested);
							tested->SetCollided(false);
							m_bIsGrounded = true;
						}
					}
					else
					{
						CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
						_cCollidedObject->GetColliders()[i]->SetCollided(false);
						m_bIsGrounded = true;
					}
				}
			}
		}
		else if (m_f3Velocity.y < 0.0f)
		{
			for (size_t i = 1; i < _cCollidedObject->GetColliders().size() - 2; i++)
			{
				if (_cCollidedObject->GetColliders()[i]->GetCollided())
				{
					CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[i]);
					_cCollidedObject->GetColliders()[i]->SetCollided(false);
				}

			}
		}
	}
	else if (_cCollidedObject->GetTag() == "BigPit")
	{
		if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
		{
			for (size_t i = 1; i < _cCollidedObject->GetColliders().size(); i++)
			{
				CCollider* tested = _cCollidedObject->GetColliders()[i];
				if (tested->GetCollided())
				{
					CapsuleToFloor(this, m_pvColliders[1], tested);
					tested->SetCollided(false);
					m_bIsGrounded = true;
				}
			}
		}
		else if (m_f3Velocity.y < 0.0f)
		{
			for (size_t i = 1; i < _cCollidedObject->GetColliders().size() - 1; i++)
			{
				CCollider* collider = _cCollidedObject->GetColliders()[i];
				if (collider->GetCollided())
				{
					CapsuleToWall(this, m_pvColliders[1], collider);
					collider->SetCollided(false);
				}
			}
		}
	}
	else if (_cCollidedObject->GetTag() == "SpikeTrap")
	{

		if (m_nCurrHealth > 0.0f)
		{
			CSpikeTrap* curTrap = reinterpret_cast<CSpikeTrap*>(_cCollidedObject);

			if (curTrap->GetActive() || curTrap->GetReset())
			{
				SetWorldVelocity({ 0.0f, 0.0f, 0.0f });
			}

			if (curTrap->GetActive() && m_fInvincibilityTimer <= 0.0f)
			{
				TakeDamage(curTrap->GetTrapDamage());
				m_eNewState = eHurtState;

				SetWorldVelocity({ 0, 1000.0f, 0 });
				//ChangeState(ESkeletonStates::eIdleState);
			}
			else
			{
				CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
			}
		}


	}
	else if (_cCollidedObject->GetTag() == "Skeleton")
	{
		//Need capsule to capulse to make slide off eachother
		CapsuleToCapsule(this, m_pvColliders[1], _cCollidedObject->GetColliders()[1]);
	}
	else if (_cCollidedObject->GetTag() == "CrackedWall")
	{
		// For the last 4 colliders (Bomb colliders)


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
	else if (_cCollidedObject->GetTag() == "FireTrap")
	{
		if (m_nCurrHealth > 0.0f)
		{
			CFireTrap* curTrap = reinterpret_cast<CFireTrap*>(_cCollidedObject);

			if (curTrap->GetActive() && m_fInvincibilityTimer <= 0.0f)
			{
				TakeDamage(curTrap->GetTrapDamage());
				m_eNewState = eHurtState;

			}
		}
	}
	else if (_cCollidedObject->GetTag() == "SpinningBlade")
	{
		if (m_nCurrHealth > 0.0f && m_fInvincibilityTimer <= 0.0f)
		{
			CSpinningBlade* curTrap = reinterpret_cast<CSpinningBlade*>(_cCollidedObject);

			XMVECTOR mathTarget = XMVector3Normalize(XMLoadFloat3(GetPosition()) - XMLoadFloat3(curTrap->GetPosition()));

			mathTarget *= 1000.0f;

			XMFLOAT3 toTarget; XMStoreFloat3(&toTarget, mathTarget);

			SetWorldVelocity(toTarget);

			TakeDamage(curTrap->GetTrapDamage());
		}
	}

	else if (_cCollidedObject->GetTag() == "Bridge")
	{
		CBridge* curBridge = reinterpret_cast<CBridge*>(_cCollidedObject);
		curBridge->SetCollided(true);

		if (curBridge->IsActive() && curBridge->IsIntact())
		{
			if (_cCollidedObject->GetColliders()[1]->GetCollided())
			{
				if (_cCollidedObject->GetColliders()[1]->GetCollided())
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
		}
	}
	else if (_cCollidedObject->GetTag() == "MinotaurZone")
	{
		if (m_pDayNight->IsNight())
		{
			if (m_eNewState != eDeathState && m_eCurrState != eDeathState)
			{
				m_eNewState = eRetreatState;
			}
		}
	}
	else if (_cCollidedObject->GetTag() == "Minotaur")
	{
		TakeDamage(200.0f);
	}
	else if (_cCollidedObject->GetTag() == "PressurePlate")
	{
		_cCollidedObject->CollisionResponse(this);
	}
	else if (_cCollidedObject->GetTag() == "Tree")
	{
		CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
	}
}
void COrc::SwitchAnimation(EOrcAnimation _animation)
{
	if (m_unCurrAnim == (unsigned int)_animation)
		return;
	m_cpAnimationManager->Switch(&m_cpAnimations[m_unCurrAnim], &m_cpAnimations[_animation]);
	m_unCurrAnim = _animation;
}
void COrc::ChangeState(int _newState)
{
	m_eNewState = (EOrcStates)_newState;
}
void COrc::UpdateState()
{

}
void COrc::AdjustRotation()
{

}
void COrc::Activate()
{
	// Draw the Object
	m_cpRenderMesh->GetRender() = true;
	m_cWepMesh->GetRender() = true;

	m_vtPointLights[0].m_fRadius = 350.0f;
	m_vtPointLights[0].m_fPosition[0] = GetPosition()->x;
	m_vtPointLights[0].m_fPosition[1] = 150.0f;
	m_vtPointLights[0].m_fPosition[2] = GetPosition()->z;

	m_fThrowTimer = 0.0f;
	if (m_pDayNight->IsDay())
	{
		m_nCurrHealth = DAY_MAX_HEALTH;
		m_vtPointLights[0].m_fColor[0] = 1.0f;
		m_vtPointLights[0].m_fColor[1] = 1.0f;
		m_vtPointLights[0].m_fColor[2] = 0.0f;

	}
	else
	{
		m_nCurrHealth = NIGHT_MAX_HEALTH;
		m_vtPointLights[0].m_fColor[0] = 1.0f;
		m_vtPointLights[0].m_fColor[1] = 0.0f;
		m_vtPointLights[0].m_fColor[2] = 0.0f;

	}
	m_bIsGrounded = true;

	// Mark as not Invinsable
	m_fInvincibilityTimer = 0.0f;

	// Mark as active
	m_bIsActive = true;
	m_cDropShadow->GetRender() = true;

	

	m_pBomb->Activate();

	// Set the temp state
}
void COrc::DeActivate()
{
	// Build a message
	//CRemoveObjectMessage* removeObj = new CRemoveObjectMessage(this, CObjectManager::AI);

	//// Send the message
	//removeObj->Send();
	m_vtPointLights[0].m_fRadius = 0;

	for (int i = 0; i < eParticleCount; i++)
	{
		m_pParticleManager->GetEmitter(i)->Stop();
	}

	m_fSpawnTimer = 10.0f;
	//m_cpObjectManager->RemoveObject(this, CObjectManager::Dynamic);

	// Exit the current State
	//m_vStates[m_ipCurrentState]->Exit(this);
	ExitSubState();
	m_eNewState = eSpawnState;

	m_bHit = false;
	// Mark as not active
	m_bIsActive = false;
	m_pBomb->DeActivate();
	m_cDropShadow->GetRender() = false;
	m_cWepMesh->GetRender() = false;
	
	// Mark as not Invinsable
	m_fInvincibilityTimer = 0.0f;
	m_cpRenderMesh->GetRender() = false;

	SwitchAnimation(eIdle1Anim);
	// Remove the mesh
	//GRAPHICS->RemoveRenderMesh(m_cpRenderMesh);

	m_vPath.clear();
}
void COrc::TakeDamage(int damageAmount)
{
	m_nCurrHealth -= damageAmount;
	m_bHit = true;
	m_fInvincibilityTimer = 1.5f;
	m_eNewState = eHurtState;


}
void COrc::PlayHitParticles()
{
	PlayParticle(m_unParticleEmitters[eBloodParticle], { 0.0f, 110.0f, 0.0f });
	PlayParticle(m_unParticleEmitters[eBloodString], { 0.0f, 110.0f, 0.0f });
	PlayParticle(m_unParticleEmitters[eBloodMist], { 0.0f, 110.0f, 0.0f });
	PlayParticle(m_unParticleEmitters[eBloodMist2], { 0.0f, 120.0f, 0.0f });

}
void COrc::UpdateDayStatus()
{
	if (m_pDayNight->IsDay())
	{
		if (m_bBuffed)
		{
			MakeDayOrc();
			m_bBuffed = false;
		}
	}
	else
	{
		if (!m_bBuffed)
		{
			MakeNightOrc();
			m_bBuffed = true;
		}
	}
}
void COrc::MakeDayOrc()
{
	m_nCurrHealth = DAY_MAX_HEALTH;

	m_cpAttack->SetDamage(20);
	m_vtPointLights[0].m_fColor[0] = 1.0f;
	m_vtPointLights[0].m_fColor[1] = 1.0f;
	m_vtPointLights[0].m_fColor[2] = 0.0f;
	m_vtPointLights[0].m_fBrightness = 1.0f;
}
void COrc::MakeNightOrc()
{
	m_cpAttack->SetDamage(30);
	m_vtPointLights[0].m_fColor[0] = 1.0f;
	m_vtPointLights[0].m_fColor[1] = 0.0f;
	m_vtPointLights[0].m_fColor[2] = 0.0f;
	m_vtPointLights[0].m_fBrightness = 1.0f;
}
void COrc::PlayParticle(unsigned int _toPlay, XMFLOAT3 _relativePos)
{
	m_pParticleManager->GetEmitter(_toPlay)->Stop();
	m_pParticleManager->GetEmitter(_toPlay)->GetPosition(0) = GetPosition()->x + _relativePos.x;
	m_pParticleManager->GetEmitter(_toPlay)->GetPosition(1) = GetPosition()->y + _relativePos.y;
	m_pParticleManager->GetEmitter(_toPlay)->GetPosition(2) = GetPosition()->z + _relativePos.z;
	m_pParticleManager->GetEmitter(_toPlay)->ePlay();
}
void COrc::Knockback(XMFLOAT3 _direction, float _force)
{
	_direction.x *= _force;
	_direction.y = 700.0f;
	_direction.z *= _force;

	knockBackVelocity = _direction;

	m_f3Velocity = _direction;
}
COrc::EOrcStates COrc::Idle()
{
	// Get the current positions
	XMFLOAT3 curPos = *(this->GetPosition());
	XMFLOAT3 targetPos = *GetPlayer()->GetPosition();

	// Convert them for math
	XMVECTOR mathPos = XMLoadFloat3(&curPos);
	XMVECTOR mathTarget = XMLoadFloat3(&targetPos);

	// Find the vector between the two points
	XMVECTOR mathToVec = XMVectorSubtract(mathTarget, mathPos);



	// Get the distance to target - ANY XYZ HOLDS THE LENGTH
	XMVECTOR mathDistToTarget = XMVector3LengthSq(mathToVec);
	XMFLOAT3 distToTarget; XMStoreFloat3(&distToTarget, mathDistToTarget);

	if (distToTarget.x < 2500.0f * 2500.0f)
	{
		return eFollowState;
	}

	return eIdleState;
}
COrc::EOrcStates COrc::Spawn()
{
	if (m_bIsActive)
		return EOrcStates::eIdleState;

	if (m_fSpawnTimer > 0.0f)
	{
		m_fSpawnTimer -= DELTA_TIME();
		if (m_fSpawnTimer < 0.0f)
		{
			Activate();
			m_pBomb->Activate();
			return eIdleState;
		}
	}
	return eSpawnState;


}
COrc::EOrcStates COrc::Reposition()
{
	// Get the current positions
	XMFLOAT3 curPos = *(this->GetPosition());
	XMFLOAT3 targetPos = *GetPlayer()->GetPosition();

	// Convert them for math
	XMVECTOR mathPos = XMLoadFloat3(&curPos);
	XMVECTOR mathTarget = XMLoadFloat3(&targetPos);

	// Find the vector between the two points
	XMVECTOR mathToVec = XMVectorSubtract(mathTarget, mathPos);

	// Normalize the toVector to get the direction
	XMVECTOR mathVelocity = XMVector3Normalize(mathToVec);
	mathVelocity *= MOVE_SPEED * 0.75f;

	XMFLOAT3 realVelocity; XMStoreFloat3(&realVelocity, mathVelocity);
	realVelocity.y = m_f3Velocity.y;

	// Get the distance to target - ANY XYZ HOLDS THE LENGTH
	XMVECTOR mathDistToTarget = XMVector3Length(mathToVec);
	XMFLOAT3 distToTarget; XMStoreFloat3(&distToTarget, mathDistToTarget);
	Steering().Seek(*GetPlayer()->GetPosition());
	Steering().Update(true);

	if (m_fThrowTimer <= 0.0f)
	{
		if (distToTarget.x > 1500.0f)
			return COrc::eThrowBombState;
	}
	else if (m_fSwingTimer <= 0.0f)
	{
		if (distToTarget.x < 250.0f)
			return eMeleeState;

		return eMoveIn;
	}
	else if (distToTarget.x > 1000.0f)
		return COrc::eMoveIn;

	return EOrcStates::eReposition;

}
COrc::EOrcStates COrc::MoveIn()
{
	// Get the current positions
	XMFLOAT3 curPos = *(this->GetPosition());
	XMFLOAT3 targetPos = *GetPlayer()->GetPosition();

	// Convert them for math
	XMVECTOR mathPos = XMLoadFloat3(&curPos);
	XMVECTOR mathTarget = XMLoadFloat3(&targetPos);

	// Find the vector between the two points
	XMVECTOR mathToVec = XMVectorSubtract(mathTarget, mathPos);

	// Normalize the toVector to get the direction
	XMVECTOR mathVelocity = XMVector3Normalize(mathToVec);
	mathVelocity *= MOVE_SPEED;

	XMFLOAT3 realVelocity; XMStoreFloat3(&realVelocity, mathVelocity);
	realVelocity.y = m_f3Velocity.y;

	// Get the distance to target - ANY XYZ HOLDS THE LENGTH
	XMVECTOR mathDistToTarget = XMVector3Length(mathToVec);
	XMFLOAT3 distToTarget; XMStoreFloat3(&distToTarget, mathDistToTarget);
	Steering().Seek(*GetPlayer()->GetPosition());
	Steering().Update();

	if (distToTarget.x < 300.0f && m_fSwingTimer <= 0.0f)
		return COrc::eMeleeState;
	else if (distToTarget.x > 1500.0f)
		return COrc::eFollowState;

	return EOrcStates::eMoveIn;

}

COrc::EOrcStates COrc::Follow()
{
	if (NearlyEqual(GetCurrAnimation()->GetCurrTime(), 0.05f))
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYERFOOTSTEP, *GetPosition());
	if (NearlyEqual(GetCurrAnimation()->GetCurrTime(), 0.5f))
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYERFOOTSTEP, *GetPosition());

	if (m_bIsGrounded)
		SetWorldVelocity({ 0, m_f3Velocity.y, 0 });
	else
		SetWorldVelocity({ knockBackVelocity.x, m_f3Velocity.y, knockBackVelocity.z });

	// Get the current path
	vector<XMFLOAT3>& vPath = GetPath(); // path here from group

	/*if (DistanceToPlayer() < THROW_DISTANCE)
		return EOrcStates::eThrowBombState;*/

	// Node that I'm on along the path
	int nCurrentNode = GetCurNodeOnPath();

	if (nCurrentNode < 0 || vPath.empty())
	{
		BuildPathToPlayer();
		vPath = GetPath();

		if (vPath.size() < 2)
			return eFollowState;
	}

	nCurrentNode = GetCurNodeOnPath();

	float distanceToPlayer = DistanceToPlayer();
	if (distanceToPlayer < 1000.0f && m_fThrowTimer <= 0.0f)
	{
		return eThrowBombState;
	}

	// Get the current positions
	XMFLOAT3 curPos = *(this->GetPosition());
	XMFLOAT3 targetPos = vPath[nCurrentNode];

	// Convert them for math
	XMVECTOR mathPos = XMLoadFloat3(&curPos);
	XMVECTOR mathTarget = XMLoadFloat3(&targetPos);

	// Find the vector between the two points
	XMVECTOR mathToVec = XMVectorSubtract(mathTarget, mathPos);

	// Normalize the toVector to get the direction
	XMVECTOR mathVelocity = XMVector3Normalize(mathToVec);
	mathVelocity *= MOVE_SPEED;

	XMFLOAT3 realVelocity; XMStoreFloat3(&realVelocity, mathVelocity);
	realVelocity.y = m_f3Velocity.y;

	// Get the distance to target - ANY XYZ HOLDS THE LENGTH
	XMVECTOR mathDistToTarget = XMVector3Length(mathToVec);
	XMFLOAT3 distToTarget; XMStoreFloat3(&distToTarget, mathDistToTarget);

	// Offset the current node
	float nNextNodeDistance = 125.0f;

	// If i reached the node, move on the next one
	if (distToTarget.x < nNextNodeDistance && nCurrentNode >= 0)
		nCurrentNode--;

	Steering().Seek(targetPos);

	if (m_bIsGrounded)
	{
		Steering().Update();
		//SetWorldVelocity(realVelocity);
	}
	SetNodeOnPath(nCurrentNode);
	return eFollowState;
}
COrc::EOrcStates COrc::Retreat()
{
	if (NearlyEqual(GetCurrAnimation()->GetCurrTime(), 0.05f))
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYERFOOTSTEP, *GetPosition());
	if (NearlyEqual(GetCurrAnimation()->GetCurrTime(), 0.5f))
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYERFOOTSTEP, *GetPosition());

	if (m_bIsGrounded)
		SetWorldVelocity({ 0, m_f3Velocity.y, 0 });
	else
		SetWorldVelocity({ knockBackVelocity.x, m_f3Velocity.y, knockBackVelocity.z });

	// Get the current path
	vector<XMFLOAT3>& vPath = GetPath(); // path here from group

	// Node that I'm on along the path
	int nCurrentNode = GetCurNodeOnPath();

	// Clamp CurNode to 0 and max
	if (nCurrentNode < 0)
		return eIdleState;

	// If no path was generated, then return
	if (vPath.size() <= 0)
	{
		nCurrentNode = -1;
		return eRetreatState;
	}


	// Get the current positions
	XMFLOAT3 curPos = *GetPosition();
	XMFLOAT3 targetPos = vPath[nCurrentNode];

	// Convert them for math
	XMVECTOR mathPos = XMLoadFloat3(&curPos);
	XMVECTOR mathTarget = XMLoadFloat3(&targetPos);

	// Find the vector between the two points
	XMVECTOR mathToVec = XMVectorSubtract(mathTarget, mathPos);

	// Normalize the toVector to get the direction
	XMVECTOR mathVelocity = XMVector3Normalize(mathToVec);

	// Add the separation factor

	mathVelocity *= MOVE_SPEED;

	XMFLOAT3 realVelocity; XMStoreFloat3(&realVelocity, mathVelocity);
	realVelocity.y = m_f3Velocity.y;

	// Get the distance to target - ANY XYZ HOLDS THE LENGTH
	XMVECTOR mathDistToTarget = XMVector3Length(mathToVec);
	XMFLOAT3 distToTarget; XMStoreFloat3(&distToTarget, mathDistToTarget);

	float nNextNodeDistance = 115.0f;

	Steering().Seek(targetPos);
	// If i reached the node, move on the next one
	if (distToTarget.x < nNextNodeDistance && nCurrentNode >= 0)
		nCurrentNode--;

	if (m_bIsGrounded)
	{
		Steering().Update();
		SetWorldVelocity(realVelocity);
	}
	else
		SetWorldVelocity({ knockBackVelocity.x, m_f3Velocity.y, knockBackVelocity.z });

	SetNodeOnPath(nCurrentNode);

	return EOrcStates::eRetreatState;
}
//States
COrc::EOrcStates COrc::Hurt()
{
	m_fExitTimer -= DELTA_TIME();
	if (m_fExitTimer < 0.0f)
	{
		m_bHit = false;
		if (GetHealth() > 0)
			return eReposition;
		else
			return eDeathState;
	}
	return EOrcStates::eHurtState;
}
COrc::EOrcStates COrc::Melee()
{
	XMFLOAT3 fVelocity3(0.0f, m_f3Velocity.y, 0.0f);

	if (m_bIsGrounded)
		SetWorldVelocity(fVelocity3);
	else
		SetWorldVelocity({ knockBackVelocity.x, m_f3Velocity.y, knockBackVelocity.z });

	// Update the timer
	m_fExitTimer -= DELTA_TIME();
	if (m_fExitTimer < 0.0f)
	{
		m_fSwingTimer = SWING_TIME;
		m_fExitTimer = 0.0f; 
		return eReposition;
	}

	return EOrcStates::eMeleeState;

}
COrc::EOrcStates COrc::ThrowBomb()
{
	m_fExitTimer -= DELTA_TIME();
	if (m_fExitTimer < 0.0f)
	{
		if (DistanceToPlayer() > 1500.0f)
			return eFollowState;
		else
			return eMoveIn;
	}
	
	return EOrcStates::eThrowBombState;


}

COrc::EOrcStates COrc::Dead()
{
	m_fExitTimer -= DELTA_TIME();
	if (m_fExitTimer < 0.0f)
	{
		theLoot->Activate();
		DeActivate();
		return COrc::EOrcStates::eSpawnState;
	}
	return COrc::EOrcStates::eDeathState;
}

const CAnimation* COrc::GetAnimation(EOrcAnimation anim) const
{
	return &m_cpAnimations[anim];
}
const CAnimation* COrc::GetCurrAnimation() const
{
	return &m_cpAnimations[m_unCurrAnim];
}
CAnimation* COrc::GetCurrAnimation()
{
	return &m_cpAnimations[m_unCurrAnim];
}

void COrc::Blocked()
{
	if (m_pDayNight->IsDay())
	{
		if (m_bBuffed)
		{
			MakeDayOrc();
			m_bBuffed = false;
		}
	}
	else
	{
		if (!m_bBuffed)
		{
			MakeNightOrc();
			m_bBuffed = true;
		}
	}
}

bool  COrc::NearlyEqual(float a, float b)
{
	return (a < b + 0.01f) && (a > b - 0.01f);

}
float  COrc::DistanceToPlayer()
{
	XMVECTOR vToPlayer = XMLoadFloat3(GetPlayer()->GetPosition());
	XMVECTOR vSkeleton = XMLoadFloat3(GetPosition());
	vToPlayer = vToPlayer - vSkeleton;

	return XMVector3Length(vToPlayer).m128_f32[0];
}
void COrc::CheckState()
{
	/*if (m_eNewState == eNumStates )
	{
	ExitSubState();
	m_eCurrState = m_eNewState;
	return;
	}*/
	//float dist = DistanceToPlayer();
	//if (dist < 300.0f && m_eNewState != EOrcStates::eMeleeState  && m_eNewState != eRetreatState && m_fSwingTimer <= 0.0f)
	//{
	//	m_eNewState = EOrcStates::eMeleeState;
	//}

	///*if (dist < 2000.0f && m_fThrowTimer <= 0.0f && m_eNewState != eRetreatState)
	//{
	//	m_eNewState = EOrcStates::eThrowBombState;
	//}*/
	//
	//else if (dist < 2000.0f && m_eNewState != eRetreatState && m_eNewState != EOrcStates::eMeleeState)
	//{
	//	m_eNewState = EOrcStates::eMoveIn;
	//}

	//if (m_bHit && m_nCurrHealth > 0)
	//	m_eNewState = EOrcStates::eHurtState;
	///*else if (m_bHit && m_nCurrHealth <= 0)
	//	m_eNewState = EOrcStates::eDeathState;*/

	if (reinterpret_cast<CPlayer*>(GetPlayer())->GetHealth() <= 0.0f)
	{
		m_eNewState = EOrcStates::eRetreatState;
	}

	if (m_eNewState != m_eCurrState)
	{
		ExitSubState();
		m_eCurrState = m_eNewState;
		EnterSubState();
	}
}
void COrc::EnterSubState()
{
	int rnd = 0;
	switch (m_eCurrState)
	{
	case EOrcStates::eFollowState:
		SwitchAnimation(COrc::eRunAnim);

		BuildPathToPlayer();
		m_pParticleManager->GetEmitter(eCaveSmoke1)->Stop();
		m_pParticleManager->GetEmitter(eCaveSmoke2)->Stop();
		m_pParticleManager->GetEmitter(eCaveSmoke3)->Stop();

		break;
	case  EOrcStates::eMeleeState:
	{
		LookAt(*GetPlayer()->GetPosition());
		m_cpRenderMesh->GetPositionMatrix() = m_mWorld;
		const XMFLOAT3& skelPosition = *GetPosition();
		m_cpAttack->SetIsActive(true);
		m_cpAttack->Attack(m_cpAttack->GetDamage());
	

		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::SKELETONATTACK, XMFLOAT3(skelPosition.x, skelPosition.y, skelPosition.z));
		SwitchAnimation(COrc::eMeleeAnim);
		// Set the timer
		m_fExitTimer = GetCurrAnimation()->GetDuration() / 2;

		m_pParticleManager->GetEmitter(eCaveSmoke1)->Stop();
		m_pParticleManager->GetEmitter(eCaveSmoke2)->Stop();
		m_pParticleManager->GetEmitter(eCaveSmoke3)->Stop();

	}

	break;
	case  EOrcStates::eIdleState:
		rnd = rand() % 2;
		(rnd == 0) ? SwitchAnimation(COrc::eIdle1Anim) : SwitchAnimation(COrc::eIdle2Anim);
		GetCurrAnimation()->SetScale(1.0f);
		BuildPathHome();
		Activate();
		PlayParticle(eCaveSmoke1, XMFLOAT3{ 0.0f, -50.0f, 0.0f });
		PlayParticle(eCaveSmoke2, XMFLOAT3{ 0.0f, -75.0f, 0.0f });
		PlayParticle(eCaveSmoke3, XMFLOAT3{ 0.0f, -50.0f, 0.0f });

		break;
	case EOrcStates::eThrowBombState:
		LookAt(*GetPlayer()->GetPosition());
		m_cpRenderMesh->GetPositionMatrix() = m_mWorld;
		m_pBomb->ChangeState(eHeld);
		m_fThrowTimer = THROW_COOLDOWN;
		SetWorldVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SwitchAnimation(COrc::eThrowBomb);
		GetCurrAnimation()->SetScale(1.0f);
		m_pParticleManager->GetEmitter(eCaveSmoke1)->Stop();
		m_pParticleManager->GetEmitter(eCaveSmoke2)->Stop();
		m_pParticleManager->GetEmitter(eCaveSmoke3)->Stop();

		m_fExitTimer = GetCurrAnimation()->GetDuration();
		break;
	case EOrcStates::eRetreatState:
	{
		SwitchAnimation(COrc::eRunAnim);

		BuildPathHome();
		m_pParticleManager->GetEmitter(eCaveSmoke1)->Stop();
		m_pParticleManager->GetEmitter(eCaveSmoke2)->Stop();
		m_pParticleManager->GetEmitter(eCaveSmoke3)->Stop();

	}
	break;
	case eMoveIn:
		SwitchAnimation(COrc::eRunAnim);
		m_pParticleManager->GetEmitter(eCaveSmoke1)->Stop();
		m_pParticleManager->GetEmitter(eCaveSmoke2)->Stop();
		m_pParticleManager->GetEmitter(eCaveSmoke3)->Stop();

		break;
	case EOrcStates::eReposition:
	{
		SwitchAnimation(COrc::eRunAnim);

		m_pParticleManager->GetEmitter(eCaveSmoke1)->Stop();
		m_pParticleManager->GetEmitter(eCaveSmoke2)->Stop();
		m_pParticleManager->GetEmitter(eCaveSmoke3)->Stop();


		m_fExitTimer = GetCurrAnimation()->GetDuration();

	}
	break;
	case EOrcStates::eHurtState:
	{
		m_pParticleManager->GetEmitter(eCaveSmoke1)->Stop();
		m_pParticleManager->GetEmitter(eCaveSmoke2)->Stop();
		m_pParticleManager->GetEmitter(eCaveSmoke3)->Stop();

		const XMFLOAT3& skelPosition = *GetPosition();

		if (GetHealth() > 0)
		{
			int rndHitAnim = rand() % 2;

			switch (rndHitAnim)
			{
			case 0:
				SwitchAnimation(COrc::eHurtAnim);
				break;
			case 1:
				SwitchAnimation(COrc::eHurt2Anim);
				break;
			}
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::SKELETONATTACK, XMFLOAT3(skelPosition.x, skelPosition.y, skelPosition.z));
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::SKELETONHURT, XMFLOAT3(skelPosition.x, skelPosition.y, skelPosition.z));
			m_fExitTimer = GetCurrAnimation()->GetDuration();
		}
		else
		{
			PlayParticle(eFadeAway, XMFLOAT3{ 0.0f, -50.0f, 0.0f });

			SwitchAnimation(COrc::eDeathAnim);
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::SKELETONHURT, XMFLOAT3(skelPosition.x, skelPosition.y, skelPosition.z));
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::SKELETONDEATH, XMFLOAT3(skelPosition.x, skelPosition.y, skelPosition.z));
			m_fExitTimer = GetCurrAnimation()->GetDuration();
		}
		PlayHitParticles();
		XMFLOAT3 tVelocity(0.0f, 0.0f, 0.0f);
		SetWorldVelocity(tVelocity);
	}

	break;
	default:
		break;
	}
}
void COrc::ExitSubState()
{
	switch (m_eCurrState)
	{
	case eIdleState:

		break;
	case eReposition:
		break;
	case eMeleeState:
		m_fSwingTimer = SWING_TIME;
		m_cpAttack->SetIsActive(false);
		break;
	case eThrowBombState:
		m_pBomb->ChangeState(eTraveling);
		break;
	case eFollowState:
		m_nCurrentNodeOnPath = -1;

		break;
	case eRetreatState:
		m_nCurrentNodeOnPath = -1;
		break;
	case eHurtState:
		m_bHit = false;
		break;
	case eSpawnState:
		break;
	case eDeathState:
		SetWorldVelocity(float3(0.0f, 0.0f, 0.0f));
		SetPosition(m_tSpawnPosition);

		break;

	default:
		break;
	}
	m_fExitTimer = 0.0f;
}
void COrc::BuildPathHome()
{
	int nStartNode = GetPathPlanner()->FindClosestNode(GetPosition());
	int nGoalNode = GetPathPlanner()->FindClosestNode(&GetSpawnPoint());

	vector<XMFLOAT3> newPath = GetPathPlanner()->GeneratePath(nStartNode, nGoalNode, false);
	SetNodeOnPath(newPath.size() - 1);

	SetPath(newPath);
}
void COrc::BuildPathToPlayer()
{
	IObject* pPlayer = GetPlayer();
	XMFLOAT3 playerPos = *pPlayer->GetPosition();

	int nPlayerNode = GetPathPlanner()->FindClosestNode(pPlayer->GetPosition());
	int nSkeletonNode = GetPathPlanner()->FindClosestNode(GetPosition());

	vector<XMFLOAT3> newPath = GetPathPlanner()->GeneratePath(nSkeletonNode, nPlayerNode, false);

	if (newPath.empty())
		newPath.push_back(*GetPosition());

	newPath.insert(newPath.begin(), playerPos);

	SetNodeOnPath(newPath.size() - 1);

	SetPath(newPath);
}

void COrc::AddToRenderer()
{
	IObject::AddToRenderer();

	//particles
	m_unParticleEmitters[eBloodParticle] = m_pParticleManager->LoadEmitter("BloodDrop.xml");
	m_unParticleEmitters[eBloodString] = m_pParticleManager->LoadEmitter("BloodString.xml");
	m_unParticleEmitters[eBloodMist] = m_pParticleManager->LoadEmitter("BloodMist.xml");
	m_unParticleEmitters[eBloodMist2] = m_pParticleManager->LoadEmitter("BloodMist.xml");
}
