/***********************************************
* Filename:  		Skeleton.cpp
* Date:      		08/16/2015
* Mod. Date: 		----------
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:   		Is in control of the Skeleton Agent behaviour
************************************************/
#include "Skeleton.h"
#include "../../Player/Player.h"

// States
//#include "SkeletonFollow.h"
//#include "SkeletonAttack.h"
//#include "SkeletonIdle.h"
//#include "SkeletonHurt.h"
//#include "SkeletonFlee.h"
//#include "SkeletonRetreat.h"
//#include "SkeletonBlock.h"

#include "../../Environment/Gunpowder.h"

#include "../../Traps/FireTrap.h"
#include "../../Traps/SpikeTrap.h"
#include "../../Traps/SpinningBlade.h"
#include "../../Environment/Bridge.h"
#include "../../Environment/DayNight.h"
#include "../../Environment/Bone.h"
#include "../../Agents/AgentState.h"
#include "../../../../Engine/Core/Collision/AABB.h"
#include "../../../../Engine/Core/Collision/Capsule.h"
#include "../../../../Engine/AI/Node.h"
#include "../../../../Engine/Animation/Mesh.h"
#include "../../../../Engine/AI/PathFinding.h"
#include "../../../../Engine/Animation/Animation.h"
#include "../../../../Engine/Animation/AnimationMesh.h"
#include "SkeletonController.h"
#include "../../../../Engine/Object Manager/IObject.h"
#include "../../../../Engine/Asset Manager/AssetManager.h"
#include "../../../../Engine/Animation/AnimationManager.h"
#include "../../../../Engine/Object Manager/ObjectManager.h"
#include "../../../../Engine/Core/Messages/RemoveObjectMessage.h"
#include "../../../../Engine/Core/Messages/AddObjectMessage.h"
#include "../../Player/Attack.h"
#include "SkeletonSword.h"
#include "../../../../Engine/Particles/Emitter.h"
#include "../../../../Engine/Particles/ParticleManager.h"
#include <iostream>



/*****************************************************************
* CSkeleton()			Skeleton Constructor that initializes the data members
*
* Ins:					CObjectManager*
*						CAnimationManager*
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NS
*****************************************************************/
CSkeleton::CSkeleton(CObjectManager* _manager, CAnimationManager* _animManager, CDayNight* _dayNight, CSkeletonController* skeletonController) : CBoid(10.0f, 500.0f, "Skeleton")
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

	// Initilize Managers
#if _DEBUG
	m_pPathPlanner = new CPathFinding("SkeletonNavGraph", _manager, false);

#else
	m_pPathPlanner = new CPathFinding("SkeletonNavGraph", _manager, false);

#endif

	m_pDayNight = _dayNight;
	m_fScale = 1.0f;
	
	m_pAssetManager = CAssetManager::GetInstance();
	m_bPoweringUp = false;
	//Set the Animation Manager
	m_cpAnimationManager = _animManager;
	m_cpAnimations = new CAnimation[eAnimCount];

	m_cpSkeletonController = skeletonController;
	// Set the object manager
	m_cpObjectManager = _manager;
	m_bBuffed = false;

	TPointLight tempLight;
	tempLight.m_fRadius = 0.0f;
	tempLight.m_fColor[0] = 1.0f;
	tempLight.m_fColor[1] = 0.0f;
	tempLight.m_fColor[2] = 0.0f;
	tempLight.m_fBrightness = 2.5f;
	tempLight.m_fPosition[0] = 0.0f;
	tempLight.m_fPosition[1] = 0.0f;
	tempLight.m_fPosition[2] = 0.0f;
	tempLight.m_fPosition[3] = 1.0f;
	AddLight(tempLight);

	m_bHit = false;

	// Load the asset
	/*CMesh* m_cShieldMesh = m_pAssetManager->GetMesh(CAssetManager::eMeshType::SkeletonShield)[0];
	m_cShieldRender[0] = new CRenderMesh(m_cShieldMesh, GRAPHICS->GetStandardVS(), GRAPHICS->GetStandardPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Shield2_Diffuse.dds");
	m_cShieldRender[1] = new CRenderMesh(m_cShieldMesh, GRAPHICS->GetStandardVS(), GRAPHICS->GetStandardPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Shield2_Diffuse.dds");*/

	CMesh* m_cMesh = CAssetManager::GetInstance()->GetPrefabMesh("RemnantMesh");
	// Create a render mesh for the object

	m_cpRenderMesh = new CAnimationMesh(m_cMesh, GRAPHICS->GetSkinningVS(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/RemnantDiffuse.dds");
	m_cpRenderMesh->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/RemnantNormal.dds");
	// Set the animation mesh
	m_cpSword = new CSkeletonSword;

	XMMATRIX mSkele = XMMatrixIdentity();
	XMStoreFloat4x4(&m_mSkeleLocal, mSkele);

	//	AttachToJoint(m_cpSword->S(), LEFT_HAND);
	m_cpRenderMesh->GetRender() = false;

	m_cpAnimations[eRunAnim] = CAssetManager::GetInstance()->GetAnimation("RemnantRun");
	m_cpAnimations[eRunAnim].SetLooping(true);

	

	m_cpAnimations[eBlockWalkAnim] = CAssetManager::GetInstance()->GetAnimation("RemnantRun");
	m_cpAnimations[eBlockWalkAnim].SetLooping(true);

	m_cpAnimations[eAttackAnim] = CAssetManager::GetInstance()->GetAnimation("RemnantLightAttack1");
	m_cpAnimations[eAttackAnim].SetLooping(false);
	m_cpAnimations[eAttackAnim].SetScale(1.33f);

	m_cpAnimations[eDeathAnim] = CAssetManager::GetInstance()->GetAnimation("RemnantDeath1");
	m_cpAnimations[eDeathAnim].SetLooping(false);

	m_cpAnimations[eHurtAnim] = CAssetManager::GetInstance()->GetAnimation("RemnantHurt");
	m_cpAnimations[eHurtAnim].SetLooping(false);
	m_cpAnimations[eHurtAnim].SetScale(1.0f);

	m_cpAnimations[eHurt2Anim] = CAssetManager::GetInstance()->GetAnimation("RemnantHurt");
	m_cpAnimations[eHurt2Anim].SetLooping(false);

	m_cpAnimations[eSpawnAnim] = CAssetManager::GetInstance()->GetAnimation("RemnantSpawn1");
	m_cpAnimations[eSpawnAnim].SetLooping(false);
	m_cpAnimations[eSpawnAnim].SetScale(0.0f);

	m_pParticleManager = new CParticleManager(_manager);
	m_unParticleEmitters[eBoneParticle] = m_pParticleManager->LoadEmitter("BoneFragment.xml");
	m_unParticleEmitters[eDustParticle] = m_pParticleManager->LoadEmitter("BoneDust.xml");
	m_unParticleEmitters[eFadeAway] = m_pParticleManager->LoadEmitter("EnemyFadeAway.xml");

	for (unsigned int animation = 0; animation < eAnimCount; animation++)
	{
		m_cpAnimations[animation].SetAnimMesh((CAnimationMesh*)m_cpRenderMesh);
		m_cpAnimations[animation].IntializeAnimation();
	}

	// Add the starting animation
	m_cpAnimationManager->AddAnimation(&m_cpAnimations[eSpawnAnim]);

	// Set the current animaton
	m_unCurrAnim = eSpawnAnim;

	// Add collision box
	AddCollider(new CCollider(true, new CAABB(XMFLOAT3(m_mWorld._41, 55.0f, m_mWorld._43), XMFLOAT3(90.0f, 90.f, 90.f)), false));
	CCapsule* theCapsule = new CCapsule({ m_mWorld._41, 45.0f, m_mWorld._43 }, { m_mWorld._41, 240.0f, m_mWorld._43 }, 45.0f);
	AddCollider(new CCollider(true, theCapsule));

	((CAABB*)m_pvColliders[0]->GetBounds())->SetCenter({ m_mWorld._41, m_mWorld._42, m_mWorld._43 });


	// Create the attack instance
	AttachToJoint(m_cpSword->GetRenderMesh(), "RightHand");
	GRAPHICS->AddRenderMesh(m_cpSword->GetRenderMesh());
	m_cpAttack = new CAttack(this, CAssetManager::GetInstance()->GetPrefabMesh("OrcDagger"), &m_cpSword->GetRenderMesh()->GetPositionMatrix(), 20, "SkeletonAttack");
	m_cpAttack->SetParent(this);

	// Add the object to start attacking.
	CAddObjectMessage* addObj = new CAddObjectMessage(m_cpAttack, CObjectManager::Dynamic);

	// Send the message
	addObj->Send();

	SetWorldVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));

	
	GRAPHICS->AddRenderMesh(m_cpRenderMesh);
	GRAPHICS->AddLight(&m_vtPointLights[0]);


	m_ipCurrentState = SPAWN;
	EnterSubState();

	m_cpTheLoot = new CBone(m_cpObjectManager, this);
	m_cpThePowder = new CGunpowder(m_cpObjectManager, this);
}

/*****************************************************************
* TakeDamage()			Decrements skeleton health by passed in amount.
*
* Ins:					int
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    09/02/2015
* Mod. Initials:	    CM
*****************************************************************/
void CSkeleton::TakeDamage(int damageAmount)
{
	m_fCurrentHealth -= (float)damageAmount;
	m_bHit = true;

	m_bInvincible = true;
	m_fInvincibilityTimer = 1.5f;
	//ChangeState(CSkeleton::eHurtState);
}

void CSkeleton::PlayHitParticles()
{
	PlayParticle(m_unParticleEmitters[eBoneParticle], XMFLOAT3(0.0f, 150.0f, 25.0f));
	PlayParticle(m_unParticleEmitters[eDustParticle], XMFLOAT3(0.0f, 150.0f, 25.0f));

}

void CSkeleton::PlayParticle(unsigned int _toPlay, XMFLOAT3 _relativePos)
{
	m_pParticleManager->GetEmitter(_toPlay)->Stop();
	m_pParticleManager->GetEmitter(_toPlay)->GetPosition(0) = GetPosition()->x + _relativePos.x;
	m_pParticleManager->GetEmitter(_toPlay)->GetPosition(1) = GetPosition()->y + _relativePos.y;
	m_pParticleManager->GetEmitter(_toPlay)->GetPosition(2) = GetPosition()->z + _relativePos.z;
	m_pParticleManager->GetEmitter(_toPlay)->ePlay();
}

/*****************************************************************
* ~CSkeleton()			Skeleton Destructor cleans up the memory
*
* Ins:					None
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NS
*****************************************************************/
CSkeleton::~CSkeleton()
{
	m_vPath.clear();
	m_cpSword->Deactivate();
	m_cpAnimationManager->RemoveAnimation(&m_cpAnimations[m_unCurrAnim]);
	m_cpAnimationManager = nullptr;

	for (size_t i = 0; i < m_vtPointLights.size(); i++)
		GRAPHICS->RemoveLight(&m_vtPointLights[i]);
	m_vtPointLights.clear();

	GRAPHICS->RemoveRenderMesh(m_cpRenderMesh);
	GRAPHICS->RemoveRenderMesh(m_cDropShadow);



	SAFE_DELETE(m_cpTheLoot);
	SAFE_DELETE(m_cpThePowder);
	SAFE_DELETE(m_cDropShadow);
	SAFE_DELETE(m_cpAttack);
	SAFE_DELETE(m_pPathPlanner);
	SAFE_DELETE(m_pParticleManager);
	SAFE_DELETE(m_cpSword);
	delete[] m_cpAnimations;

	
	
}

/*****************************************************************
* Update()				Updates the skeleton behaviour.
*
* Ins:					None
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NS
*****************************************************************/
const CAnimation* CSkeleton::GetAnimation(ESkeletonAnimation anim) const
{
	return &m_cpAnimations[anim];
}

/*****************************************************************
* Update()				Updates the skeleton behaviour.
*
* Ins:					None
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NS
*****************************************************************/
const CAnimation* CSkeleton::GetCurrAnimation() const
{
	return &m_cpAnimations[m_unCurrAnim];
}
/*****************************************************************
* Update()				Updates the skeleton behaviour.
*
* Ins:					None
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NS
*****************************************************************/
CAnimation* CSkeleton::GetCurrAnimation()
{
	return &m_cpAnimations[m_unCurrAnim];
}

void CSkeleton::UpdateDayStatus()
{
	if (m_pDayNight->IsDay())
	{
		if (m_bBuffed)
		{
			MakeDaySkeleton();
			m_bBuffed = false;
		}
	}
	else
	{
		if (!m_bBuffed)
		{
			MakeNightSkeleton();
			m_bBuffed = true;
		}
	}
}

/*****************************************************************
* Update()				Updates the skeleton behaviour.
*
* Ins:					None
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NS
*****************************************************************/
void CSkeleton::Update()
{
	if (m_bIsActive)
	{
		//CCapsule* tempCapsule = (CCapsule*)GetColliders()[1]->GetBounds();
		//DEBUG_UTIL::DrawCapsuleCollider(tempCapsule->GetStart(), tempCapsule->GetEnd(), tempCapsule->GetRadius());

		if (m_eCurrState != SPAWN)
		{
			float3 target = float3(0.0f, 0.0f, 0.0f);
			m_f3Velocity = float3(0.0f, m_f3Velocity.y, 0.0f);
			m_pParticleManager->Update();

			if (m_bInvincible)
			{
				m_fInvincibilityTimer -= DELTA_TIME();
				if (m_fInvincibilityTimer < 0.0f)
				{
					m_bInvincible = false;
				}
			}

			UpdateBlockSide();
			UpdateDayStatus();
		}

		if (m_bPoweringUp)
		{
			if (m_pDayNight->IsDay())
			{
				m_fScale -= DELTA_TIME() * 0.5f;
				if (m_fScale < 1.0f)
				{
					m_fScale = 1.0f;
					m_bPoweringUp = false;
				}
			}
			else
			{
				m_fScale += DELTA_TIME() * 0.5f;

				if (m_fScale > 1.25f)
				{
					m_fScale = 1.25f;
					m_bPoweringUp = false;
				}
			}
		}
		if (GetPosition()->y < -900.0f)
		{
			m_bHit = true;
			m_fCurrentHealth = 0;
			m_bIsGrounded = true;
		}
		// Exeute the current action
		if (m_bIsGrounded)
		{
			//AdjustRotation();
			CheckState();
			switch (m_eCurrState)
			{
			case SPAWN:
				m_eNewState = Spawn();
				break;
			case MANUVERING:
				m_eNewState = Manuver();
				break;
			case MOVING_IN:
				m_eNewState = Move(true);
				break;
			case BACKING_OUT:
				m_eNewState = Move(false);
				break;
			case SWINGING:
				m_eNewState = Swinging();
				break;
				/*case BLOCKING:
				m_eNewState = Blocking();
				break;*/
			case RETREAT:
				m_eNewState = Retreating();
				break;
			case DEATH:
				m_eNewState = Die();
				break;
			case HURT:
				m_eNewState = Hurt();
				break;
			case FOLLOW:
				m_eNewState = Follow();
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
		if (m_eCurrState != SPAWN)
		{
			//m_bIsGrounded = false;

			// Update the rotation

			// velocity.x *= DELTA_TIME();
			// velocity.y *= DELTA_TIME();
			// velocity.z *= DELTA_TIME();

			XMFLOAT3 start = ((CCapsule*)m_pvColliders[1]->GetBounds())->GetStart();
			XMFLOAT3 end = ((CCapsule*)m_pvColliders[1]->GetBounds())->GetEnd();

			m_f3Velocity.y += GRAVITY * DELTA_TIME();

			if (m_mWorld.m[3][1] < 0.0f)
			{
				m_bIsGrounded = false;
			}
			CCapsule* pCapsule = (CCapsule*)m_pvColliders[1]->GetBounds();

			XMFLOAT3 f3NewStart = pCapsule->GetStart();
			XMFLOAT3 f3NewEnd = pCapsule->GetEnd();

			f3NewStart.x = f3NewEnd.x = m_mWorld._41;
			f3NewStart.z = f3NewEnd.z = m_mWorld._43;

			pCapsule->SetStart(f3NewStart);
			pCapsule->SetEnd(f3NewEnd);



			for (unsigned int collider = 0; collider < m_pvColliders.size(); collider++)
			{
				m_pvColliders[collider]->UpdatePosition({ GetWorldVelocity().x * DELTA_TIME(), GetWorldVelocity().y * DELTA_TIME(), GetWorldVelocity().z * DELTA_TIME() });
			}


#pragma region DebugStuff

			/*if (m_vPath.size())
			{
			for (size_t i = 0; i < m_vPath.size() - 1; i++)
			{
			XMFLOAT3 thisPos = m_vPath[i];
			XMFLOAT3 nextPos = m_vPath[i + 1];
			GRAPHICS->DrawLine(thisPos, nextPos, { 100, 100, 0 });
			}
			}
			*/
			//for (size_t i = 0; i < m_pPathPlanner->GetGraph()->GetNodes().size(); i++)
			//{
			//	//	XMFLOAT3 start = m_vPath[i]->GetPosition();
			//	//	XMFLOAT3 end = m_vPath[i + 1]->GetPosition();
			//	//
			//	//	GRAPHICS->DrawLine(start, end, XMFLOAT3(100, 100, 0));
			//
			//	for (size_t k = 0; k < m_pPathPlanner->GetGraph()->GetNodeAtIndex(i)->GetEdges().size(); k++)
			//	{
			//		CEdge* edge = m_pPathPlanner->GetGraph()->GetNodeAtIndex(i)->GetEdges()[k];
			//		XMFLOAT3 edgeEnd = m_pPathPlanner->GetGraph()->GetNodeAtIndex(edge->GetAdjNode())->GetPosition();
			//	
			//		GRAPHICS->DrawLine(m_pPathPlanner->GetGraph()->GetNodeAtIndex(i)->GetPosition(), edgeEnd, XMFLOAT3(0, 200, 0));
			//	
			//	}
			//}
#pragma endregion

		}

	}

	SnapPosition(m_cDropShadow->GetTransformMatrix(), m_mWorld);
	m_cDropShadow->GetTransformMatrix().m[3][1] += 10.0f;

	if (m_mWorld.m[3][1] < -10.0f || !m_bIsActive)
		m_cDropShadow->GetTransformMatrix().m[3][1] = -20000.0f;
}

void CSkeleton::MakeNightSkeleton()
{
	m_fCurrentHealth = 300.0f;
	m_cpSword->SetDamage(20);
	m_vtPointLights[0].m_fColor[0] = 1.0f;
	m_vtPointLights[0].m_fColor[1] = 0.0f;
	m_vtPointLights[0].m_fColor[2] = 0.0f;
	m_vtPointLights[0].m_fBrightness = 2.0f;
	m_bPoweringUp = true;

	//m_unBlockChance = NIGHT_BLOCK_CHANCE;


}

void CSkeleton::MakeDaySkeleton()
{
	m_fCurrentHealth = 100.0f;
	//m_unBlockChance = DAY_BLOCK_CHANCE;

	m_cpSword->SetDamage(10);
	m_vtPointLights[0].m_fColor[0] = 1.0f;
	m_vtPointLights[0].m_fColor[1] = 1.0f;
	m_vtPointLights[0].m_fColor[2] = 0.0f;
	m_vtPointLights[0].m_fBrightness = 1.0f;
	m_bPoweringUp = true;


}

/*****************************************************************
* UpdateState()			Updates the current state of the skeleton.
*
* Ins:					None
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NS
*****************************************************************/

/*****************************************************************
* ChangeState()			Changes the current state, exits the old one and enter the new.
*
* Ins:					IAgentState<CSkeleton>*
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NS
*****************************************************************/

/*****************************************************************
* AdjustRotation()		Rotates the skeleton.
*
* Ins:					IAgentState<CSkeleton>*
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NS
*****************************************************************/
void CSkeleton::AdjustRotation()
{
	XMFLOAT3 up = XMFLOAT3(0, 1, 0);
	XMVECTOR newX, newY, newZ, globalY;
	globalY = XMLoadFloat3(&up);

	XMVECTOR targetLocation;
	if (m_nCurrentNodeOnPath != -1 && !m_vPath.empty())
		targetLocation = XMLoadFloat3(&m_vPath[m_nCurrentNodeOnPath]);
	else
		targetLocation = XMLoadFloat3(m_pPlayer->GetPosition());

	if (/*m_ipCurrentState == eBlockState ||*/ m_ipCurrentState != FOLLOW)
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
/*****************************************************************
* CollisionResponse()	Responds to collision
*
* Ins:					IObject*
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NS
*****************************************************************/
void CSkeleton::CollisionResponse(IObject * _cCollidedObject)
{
	
		if (_cCollidedObject->GetTag() == "PlayerAttack" && !m_bInvincible && m_fCurrentHealth > 0.0f)
		{
			m_bInvincible = true;
			m_fInvincibilityTimer = 0.35f;
			m_fCurrentHealth -= ((CAttack*)_cCollidedObject)->GetDamage();
			m_eNewState = HURT;
			m_eCurrState = HURT;
			EnterSubState();
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
	
		else if (_cCollidedObject->GetTag() == "Wall" || _cCollidedObject->GetTag() == "Door" || _cCollidedObject->GetTag() == "CrackedWall" || _cCollidedObject->GetTag() == "SafeHaven" || _cCollidedObject->GetTag() == "MinotaurSpawn")
		{
			if (m_eCurrState == ESkeletonStates::BACKING_OUT || m_eCurrState == ESkeletonStates::MANUVERING)
			{
				m_eNewState = ESkeletonStates::MOVING_IN;
			}

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
				m_eNewState = RETREAT;
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
		else if (_cCollidedObject->GetTag() == "Skeleton")
		{
			//Need capsule to capulse to make slide off eachother
			CapsuleToCapsule(this, m_pvColliders[1], _cCollidedObject->GetColliders()[1]);
		}
		else if (_cCollidedObject->GetTag() == "Floor" || _cCollidedObject->GetTag() == "SafeHavenFloor")
		{
			if (m_mWorld.m[3][1] > -20.0f && m_f3Velocity.y < 0.0f)
			{
				CapsuleToFloor(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
				m_bIsGrounded = true;
			}
		}

		else if (_cCollidedObject->GetTag() == "SpikeTrap")
		{

			if (m_fCurrentHealth > 0.0f)
			{
				CSpikeTrap* curTrap = reinterpret_cast<CSpikeTrap*>(_cCollidedObject);

				if (curTrap->GetActive() || curTrap->GetReset())
				{
					SetWorldVelocity({ 0.0f, 0.0f, 0.0f });
				}

				if (curTrap->GetActive() && !m_bInvincible)
				{
					TakeDamage(curTrap->GetTrapDamage());
					SetWorldVelocity({ 0, 1000.0f, 0 });
					//ChangeState(ESkeletonStates::eIdleState);
				}
				else
				{
					CapsuleToWall(this, m_pvColliders[1], _cCollidedObject->GetColliders()[0]);
				}
			}


		}
		else if (_cCollidedObject->GetTag() == "FireTrap")
		{
			if (m_fCurrentHealth > 0.0f)
			{
				CFireTrap* curTrap = reinterpret_cast<CFireTrap*>(_cCollidedObject);

				if (curTrap->GetActive() && !m_bInvincible)
				{
					TakeDamage(curTrap->GetTrapDamage());
				}
			}
		}
		else if (_cCollidedObject->GetTag() == "SpinningBlade")
		{
			if (m_fCurrentHealth > 0.0f && !m_bInvincible)
			{
				CSpinningBlade* curTrap = reinterpret_cast<CSpinningBlade*>(_cCollidedObject);

				XMVECTOR mathTarget = XMVector3Normalize(XMLoadFloat3(GetPosition()) - XMLoadFloat3(curTrap->GetPosition()));

				mathTarget *= 1000.0f;

				XMFLOAT3 toTarget; XMStoreFloat3(&toTarget, mathTarget);

				SetWorldVelocity(toTarget);

				TakeDamage(curTrap->GetTrapDamage());
			}
		}
		else if (_cCollidedObject->GetTag() == "PressurePlate")
		{
			_cCollidedObject->CollisionResponse(this);
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
		else if (_cCollidedObject->GetTag() == "Minotaur")
		{
			TakeDamage(200.0f);
		}
		else if (_cCollidedObject->GetTag() == "MinotaurZone")
		{
			if (m_pDayNight->IsNight())
			{
				if (m_eNewState != DEATH && m_eCurrState != DEATH)
				{
					m_eNewState = RETREAT;
				}
			}
		}
	
}
/*****************************************************************
* Activate()			Activates the skeleton
*
* Ins:					None
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NS
*****************************************************************/
void CSkeleton::Activate(XMFLOAT3 spawnPosition)
{
	m_cpRenderMesh->GetRender() = true;
	m_eCurrState = SPAWN;
	m_eNewState = SPAWN;
	XMFLOAT3 start = spawnPosition;
	XMFLOAT3 end = spawnPosition;
	start.y = 55.0f;
	end.y = 240.0f;

	spawnPosition.y = 0.0f;

	// Set the skeletons position
	SetPosition(spawnPosition);
	SetSpawnPoint(spawnPosition);
	m_cpRenderMesh->GetPositionMatrix() = m_mWorld;
	reinterpret_cast<CAABB*>(GetColliders()[0]->GetBounds())->SetCenter(spawnPosition);

	reinterpret_cast<CCapsule*>(GetColliders()[1]->GetBounds())->SetStart(start);
	reinterpret_cast<CCapsule*>(GetColliders()[1]->GetBounds())->SetEnd(end);

	GetColliders()[1]->SetActive(true);
	GetColliders()[0]->SetActive(false);

	

	// Draw the Object
	m_cpSword->Activate();
	m_vtPointLights[0].m_fRadius = 350.0f;
	m_vtPointLights[0].m_fPosition[0] = spawnPosition.x;
	m_vtPointLights[0].m_fPosition[1] = 150.0f;
	m_vtPointLights[0].m_fPosition[2] = spawnPosition.z;

	if (m_pDayNight->IsDay())
	{
		//m_unBlockChance = DAY_BLOCK_CHANCE;
		m_fCurrentHealth = DAY_MAX_HEALTH;
		m_cpSword->SetDamage(DAY_DAMAGE);
		m_vtPointLights[0].m_fColor[0] = 1.0f;
		m_vtPointLights[0].m_fColor[1] = 1.0f;
		m_vtPointLights[0].m_fColor[2] = 0.0f;
		m_fScale = 1.0f;
	}
	else
	{
		//m_unBlockChance = NIGHT_BLOCK_CHANCE;
		m_fCurrentHealth = NIGHT_MAX_HEALTH;
		m_cpSword->SetDamage(NIGHT_DAMAGE);
		m_vtPointLights[0].m_fColor[0] = 1.0f;
		m_vtPointLights[0].m_fColor[1] = 0.0f;
		m_vtPointLights[0].m_fColor[2] = 0.0f;
		m_fScale = 1.25f;
	}


	// Mark as not Invinsable
	m_bInvincible = false;
	m_fInvincibilityTimer = 0.0f;

	// Mark as active
	m_cDropShadow->GetRender() = true;
	m_bIsActive = true;
	

	m_eCurrState = SPAWN;
	m_eNewState = SPAWN;
	m_bIsGrounded = true;

	// Create a AddObj Message
	CAddObjectMessage* addObj = new CAddObjectMessage(this, CObjectManager::AI);
	// Send the message
	addObj->Send();
//	m_fHeight = -180.0f;

	/*XMFLOAT4X4 * skeleMat = &GetWorldMatrix();
	skeleMat->_42 = m_fHeight;
	SetWorldMatrix(*skeleMat);*/

	// Set the temp state
	EnterSubState();

}
/*****************************************************************
* DeActivate()			DeActives the skeleton without delting
*
* Ins:					None
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NS
*****************************************************************/
void CSkeleton::DeActivate()
{
	

	m_vtPointLights[0].m_fRadius = 0;

	for (int i = 0; i < eParticleCount; i++)
	{
		m_pParticleManager->GetEmitter(i)->Stop();
	}
	//m_cShieldRender[0]->GetRender() = false;
	//m_cShieldRender[1]->GetRender() = false;

	int chance = rand() % 2;
	if (chance == 0)
		m_cpTheLoot->Activate();
	else
		m_cpThePowder->Activate();

	//m_cpObjectManager->RemoveObject(this, CObjectManager::Dynamic);

	// Exit the current State
	//m_vStates[m_ipCurrentState]->Exit(this);
	ExitSubState();

	m_bHit = false;
	// Mark as not active
	m_bIsActive = false;

	// Mark as not Invinsable
	m_bInvincible = false;
	m_cpSword->Deactivate();
	m_cpRenderMesh->GetRender() = false;

	m_cDropShadow->GetRender() = false;
	
	m_cpSkeletonController->KillSkeleton();

	SwitchAnimation(eBlockWalkAnim);
	// Remove the mesh
	//GRAPHICS->RemoveRenderMesh(m_cpRenderMesh);


	m_vPath.clear();


	// Build a message
	CRemoveObjectMessage* removeObj = new CRemoveObjectMessage(this, CObjectManager::AI);

	// Send the message
	removeObj->Send();
}
/*****************************************************************
* SwitchAnimation()		Wtiches the current animation
*
* Ins:					None
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NS
*****************************************************************/
void CSkeleton::SwitchAnimation(ESkeletonAnimation _animation)
{
	if (m_unCurrAnim == (unsigned int)_animation)
		return;
	m_cpAnimationManager->Switch(&m_cpAnimations[m_unCurrAnim], &m_cpAnimations[_animation]);
	m_unCurrAnim = _animation;
}
CSkeleton::ESkeletonStates  CSkeleton::Move(bool _forward)
{
	XMVECTOR vPlayer = XMLoadFloat3(GetPlayer()->GetPosition());
	XMVECTOR vSkeleton = XMLoadFloat3(GetPosition());
	XMVECTOR vVelocity;
	XMFLOAT3 fVelocity3(0.0f, m_f3Velocity.y, 0.0f);

	m_fExitTimer -= DELTA_TIME();
	if (_forward)
		vVelocity = vPlayer - vSkeleton;
	else
		vVelocity = vSkeleton - vPlayer;

	XMVECTOR vDistance = XMVector3Length(vVelocity);
	float fDistance = vDistance.m128_f32[0];

	vVelocity = XMVector3Normalize(vVelocity);
	vVelocity *= 350.0f;
	XMStoreFloat3(&fVelocity3, vVelocity);
	fVelocity3.y = m_f3Velocity.y;



	if (m_eCurrState == BACKING_OUT)
	{

		if (fDistance > 500.0f)
			return MANUVERING;
	}
	if (m_eCurrState == MOVING_IN)
	{
		CPlayer* player = reinterpret_cast<CPlayer*>(GetPlayer());



		/*if (player->IsAttacking() && fDistance < 200.0f && fDistance > 125.0f)
		{
		unsigned int blockChance = (rand() % 100) + 1;
		if (blockChance < GetBlockChance())
		m_eNewState = BLOCKING;
		}
		*/
		if (fDistance <= 125.0f)
		{
			if (m_bIsGrounded)
				SetWorldVelocity({ 0.0f, m_f3Velocity.y, 0.0f });
			return SWINGING;
		}



	}
	if (fDistance > 3000.0f)
	{
		m_eNewState = RETREAT;

	}
	if (fDistance > 1250.0f)
	{
		return FOLLOW;
	}


	//if (m_fExitTimer < 0.0f)
	//return MANUVERING;

	Steering().Seek(*GetPlayer()->GetPosition());
	if (m_bIsGrounded)
	{
		Steering().Update(false, m_fScale);
		SetWorldVelocity(fVelocity3);
	}
	else
		SetWorldVelocity({ knockBackVelocity.x, m_f3Velocity.y, knockBackVelocity.z });

	if (_forward)
		return MOVING_IN;

	return BACKING_OUT;

}
CSkeleton::ESkeletonStates  CSkeleton::Swinging()
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
		m_fExitTimer = 0.0f;
		return BACKING_OUT;
	}

	return SWINGING;
}
CSkeleton::ESkeletonStates CSkeleton::Blocking()
{
	XMFLOAT3 fVelocity3(0.0f, m_f3Velocity.y, 0.0f);
	float distToPlayer = DistanceToPlayer();

	m_fExitTimer -= DELTA_TIME();

	if (m_fExitTimer < 0.0f)
	{
		m_fExitTimer = 0.0f;

		if (distToPlayer < 125.0f)
		{
			int chance = rand() % 2;
			if (chance == 0)
				return SWINGING;
			else
				return BACKING_OUT;
		}
		return BACKING_OUT;
	}
	//XMFLOAT4X4 world = GetWorldMatrix();

	//XMFLOAT4X4 local = GetShieldMatrix(m_nShieldIndex);
	//XMMATRIX shieldWorld = XMLoadFloat4x4(&world);
	//XMMATRIX shieldLocal = XMLoadFloat4x4(&local);
	//shieldWorld = shieldLocal * shieldWorld;
	//shieldWorld *= XMMatrixTranslation(0.0f, 90.0f, 0.0f);
	//XMStoreFloat4x4(&GetShieldRenderMesh(m_nShieldIndex)->GetPositionMatrix(), shieldWorld);
	return BACKING_OUT;

}




CSkeleton::ESkeletonStates CSkeleton::Spawn()
{
	SetWorldVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
	if (m_fExitTimer > 0)
	{
		m_fExitTimer -= DELTA_TIME();


		return SPAWN;
	}
	else
	{
		
		return FOLLOW;
	}
}
CSkeleton::ESkeletonStates CSkeleton::Die()
{
	SetWorldVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_fExitTimer -= DELTA_TIME();
	if (m_fExitTimer < 0.0f)
	{
		DeActivate();
		return SPAWN;
	}
	return DEATH;
}
CSkeleton::ESkeletonStates CSkeleton::Retreating()
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
		return DEATH;

	// If no path was generated, then return
	if (vPath.size() <= 0)
	{
		nCurrentNode = -1;
		return DEATH;
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
	//mathVelocity += GetParentGroup()->CalculateSeparation(this);
	//mathVelocity += GetParentGroup()->CalculateAlignment();
	mathVelocity *= MOVE_SPEED;

	XMFLOAT3 realVelocity; XMStoreFloat3(&realVelocity, mathVelocity);
	realVelocity.y = m_f3Velocity.y;

	// Get the distance to target - ANY XYZ HOLDS THE LENGTH
	XMVECTOR mathDistToTarget = XMVector3Length(mathToVec);
	XMFLOAT3 distToTarget; XMStoreFloat3(&distToTarget, mathDistToTarget);

	float nNextNodeDistance = 125.0f;

	Steering().Seek(targetPos);
	// If i reached the node, move on the next one
	if (distToTarget.x < nNextNodeDistance && nCurrentNode >= 0)
		nCurrentNode--;

	if (m_bIsGrounded)
	{
		Steering().Update(false, m_fScale);
		SetWorldVelocity(realVelocity);
	}
	else
		SetWorldVelocity({ knockBackVelocity.x, m_f3Velocity.y, knockBackVelocity.z });

	SetNodeOnPath(nCurrentNode);

	return RETREAT;
}
CSkeleton::ESkeletonStates CSkeleton::Manuver()
{
	XMFLOAT3 tVector(0.0f, m_f3Velocity.y, 0.0f);
	// Update the timer
	ResetManuverVector(m_nChance, tVector);
	m_fExitTimer -= DELTA_TIME();
	if (m_fExitTimer < 0.0f)
	{
		m_nManuverCount++;
		m_fExitTimer = 1.5f;
		if (m_nManuverCount == m_nManuverCount)
			return MOVING_IN;
	}
	Steering().Seek(*GetPlayer()->GetPosition());
	if (m_bIsGrounded)
	{
		Steering().Update(false, m_fScale);

		SetWorldVelocity(tVector);
	}
	else
		SetWorldVelocity({ knockBackVelocity.x, m_f3Velocity.y, knockBackVelocity.z });

	return MANUVERING;

}
CSkeleton::ESkeletonStates CSkeleton::Hurt()
{
	m_fExitTimer -= DELTA_TIME();
	if (m_fExitTimer < 0.0f)
	{
		m_bHit = false;
		if (GetHealth() > 0)
			return MOVING_IN;
		else
			return DEATH;

	}
	return HURT;
}
CSkeleton::ESkeletonStates CSkeleton::Follow()
{

	if (NearlyEqual(GetCurrAnimation()->GetCurrTime(), 0.05f))
	{
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYERFOOTSTEP, *GetPosition());
	}
	if (NearlyEqual(GetCurrAnimation()->GetCurrTime(), 0.5f))
	{
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYERFOOTSTEP, *GetPosition());
	}

	if (m_bIsGrounded)
		SetWorldVelocity({ 0, m_f3Velocity.y, 0 });
	else
		SetWorldVelocity({ knockBackVelocity.x, m_f3Velocity.y, knockBackVelocity.z });

	// Get the current path
	vector<XMFLOAT3>& vPath = GetPath(); // path here from group

	// Get the current velocity
	//XMVECTOR currentVelocity; XMLoadFloat3(&m_f3vel);

	if (DistanceToPlayer() < 500.0f)
	{
		return MOVING_IN;
	}

	// Node that I'm on along the path
	int nCurrentNode = GetCurNodeOnPath();


	if (nCurrentNode < 0 || vPath.empty())
	{

		BuildPathToPlayer();
		vPath = GetPath();

		if (vPath.size() < 2)
		{
			return FOLLOW;
		}
	}

	nCurrentNode = GetCurNodeOnPath();

	// Get the current positions
	XMFLOAT3 curPos = *GetPosition();
	XMFLOAT3 targetPos = vPath[nCurrentNode];

	// Convert them for math
	XMVECTOR mathPos = XMLoadFloat3(&curPos);
	XMVECTOR mathTarget = XMLoadFloat3(&targetPos);

	// Find the vector between the two points
	XMVECTOR mathToVec = XMVectorSubtract(mathTarget, mathPos);

	//XMFLOAT3 toVec; XMStoreFloat3(&toVec, mathToVec);

	// Normalize the toVector to get the direction
	XMVECTOR mathVelocity = XMVector3Normalize(mathToVec);



	// Add the separation factor
	//	mathVelocity += GetParentGroup()->CalculateSeparation(this);

	mathVelocity *= MOVE_SPEED;

	XMFLOAT3 realVelocity; XMStoreFloat3(&realVelocity, mathVelocity);
	realVelocity.y = m_f3Velocity.y;

	// Get the distance to target - ANY XYZ HOLDS THE LENGTH
	XMVECTOR mathDistToTarget = XMVector3Length(mathToVec);
	XMFLOAT3 distToTarget; XMStoreFloat3(&distToTarget, mathDistToTarget);

	// Offset the current node

	float nNextNodeDistance = 300.0f;



	// If i reached the node, move on the next one
	if (distToTarget.x < nNextNodeDistance && nCurrentNode >= 0)
	{
		nCurrentNode--;

	}
	Steering().Seek(targetPos);

	if (m_bIsGrounded)
	{
		Steering().Update(false, m_fScale);

		SetWorldVelocity(realVelocity);
	}
	SetNodeOnPath(nCurrentNode);
	return FOLLOW;
}
int CSkeleton::CheckBlockSide()
{
	/*if (m_nLeftBlockChance == m_nRightBlockChance)
		return rand() % 2;

		return (m_nLeftBlockChance < m_nRightBlockChance) ? BLOCK_RIGHT : BLOCK_LEFT;*/
	return 0;
}
void CSkeleton::EnterSubState()
{
	switch (m_eCurrState)
	{
	case FOLLOW:
		BuildPathToPlayer();
		SwitchAnimation(CSkeleton::eBlockWalkAnim);
		GetCurrAnimation()->SetScale(1.0f);
		break;
	case MANUVERING:
	{
		/*GetShieldRenderMesh(0)->GetRender() = false;
		GetShieldRenderMesh(1)->GetRender() = false;*/
		m_nMaxManuver = (rand() % 3) + 1;
		m_nManuverCount = 0;
		m_nChance = rand() % 2;

		SwitchAnimation(CSkeleton::eBlockWalkAnim);
		GetCurrAnimation()->SetScale(0.8f);
		m_fExitTimer = 2.0f;

	}
	break;
	case CSkeleton::MOVING_IN:
		SwitchAnimation(CSkeleton::eBlockWalkAnim);
		GetCurrAnimation()->SetScale(1.0f);
		m_fExitTimer = 2.0f;
		/*GetShieldRenderMesh(0)->GetRender() = false;
		GetShieldRenderMesh(1)->GetRender() = false;*/
		break;
	case CSkeleton::RETREAT:
		SwitchAnimation(CSkeleton::eBlockWalkAnim);
		GetCurrAnimation()->SetScale(1.0f);
		BuildPathHome();
		/*GetShieldRenderMesh(0)->GetRender() = false;
		GetShieldRenderMesh(1)->GetRender() = false;*/
		break;
	case CSkeleton::BACKING_OUT:
		SwitchAnimation(CSkeleton::eBlockWalkAnim);
		GetCurrAnimation()->SetScale(-1.0f);
		m_fExitTimer = 2.0f;
		/*GetShieldRenderMesh(0)->GetRender() = false;
		GetShieldRenderMesh(1)->GetRender() = false;*/
		break;
	case CSkeleton::SWINGING:
	{
		SwitchAnimation(CSkeleton::eAttackAnim);

		const XMFLOAT3& skelPosition = *GetPosition();
		GetAttack()->Attack(GetWeapon()->GetDamage());
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::SKELETONATTACK, XMFLOAT3(skelPosition.x, skelPosition.y, skelPosition.z));
		// Set the timer
		m_fExitTimer = GetCurrAnimation()->GetDuration();
		/*GetShieldRenderMesh(0)->GetRender() = false;
		GetShieldRenderMesh(1)->GetRender() = false;*/
	}
	break;
	case CSkeleton::SPAWN:
	{
		SwitchAnimation(CSkeleton::eSpawnAnim);
		m_cpAnimations[eSpawnAnim].IntializeAnimation();
		m_cpAnimations[eSpawnAnim].SetScale(1.0f);
		m_fExitTimer = m_cpAnimations[eSpawnAnim].GetDuration() - 0.25f;
		/*GetShieldRenderMesh(0)->GetRender() = false;
		GetShieldRenderMesh(1)->GetRender() = false;*/
	}
	break;
	/*case CSkeleton::BLOCKING:
	{
	SwitchAnimation(CSkeleton::eBlockAnim);
	int nBlockSide = (int)CheckBlockSide();


	GetShieldRenderMesh(nBlockSide)->GetRender() = true;
	m_nShieldIndex = nBlockSide;

	m_fExitTimer = GetCurrAnimation()->GetDuration();

	}
	break;*/
	case CSkeleton::HURT:
	{
		const XMFLOAT3& skelPosition = *GetPosition();

		if (GetHealth() > 0)
		{
			int rndHitAnim = rand() % 2;

			switch (rndHitAnim)
			{
			case 0:
				SwitchAnimation(CSkeleton::eHurtAnim);
				break;
			case 1:
				SwitchAnimation(CSkeleton::eHurt2Anim);
				break;
			}
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::SKELETONATTACK, XMFLOAT3(skelPosition.x, skelPosition.y, skelPosition.z));
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::SKELETONHURT, XMFLOAT3(skelPosition.x, skelPosition.y, skelPosition.z));
			m_fExitTimer = GetCurrAnimation()->GetDuration();
		}
		else
		{
			PlayParticle(eFadeAway, XMFLOAT3{ 0.0f, -50.0f, 0.0f });

			SwitchAnimation(CSkeleton::eDeathAnim);
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::SKELETONHURT, XMFLOAT3(skelPosition.x, skelPosition.y, skelPosition.z));
			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::SKELETONDEATH, XMFLOAT3(skelPosition.x, skelPosition.y, skelPosition.z));
			m_fExitTimer = GetCurrAnimation()->GetDuration();
		}
		PlayHitParticles();
		XMFLOAT3 tVelocity(0.0f, 0.0f, 0.0f);
		SetWorldVelocity(tVelocity);
	}

	break;
	case DEATH:
	{
		m_cpAttack->SetIsActive(false);
		break;
	}
	default:
		break;
	}
}


void CSkeleton::ExitSubState()
{

	switch (m_eCurrState)
	{
	case CSkeleton::MANUVERING:
		break;
	case CSkeleton::MOVING_IN:
		break;
	case CSkeleton::FOLLOW:
		m_nCurrentNodeOnPath = -1;
		break;
	case CSkeleton::BACKING_OUT:
		GetCurrAnimation()->SetScale(1.0f);
		break;
	case CSkeleton::SWINGING:
		break;
		/*case CSkeleton::BLOCKING:
			GetShieldRenderMesh(0)->GetRender() = false;
			GetShieldRenderMesh(1)->GetRender() = false;
			break;*/
	case HURT:
		m_bHit = false;
		break;
	default:
		break;
	}
	m_fExitTimer = 0.0f;

}
bool  CSkeleton::NearlyEqual(float a, float b)
{
	return (a < b + 0.01f) && (a > b - 0.01f);
}
float  CSkeleton::DistanceToPlayer()
{
	XMVECTOR vToPlayer = XMLoadFloat3(GetPlayer()->GetPosition());
	XMVECTOR vSkeleton = XMLoadFloat3(GetPosition());
	vToPlayer = vToPlayer - vSkeleton;

	return XMVector3Length(vToPlayer).m128_f32[0];
}
void CSkeleton::CheckState()
{
	/*if (m_eNewState != BLOCKING && DistanceToPlayer() < 125.0f)
	{
	if (reinterpret_cast<CPlayer*>(GetPlayer())->IsAttacking())
	{
	int chance = (rand() % 100) + 1;
	if (chance <= (int)GetBlockChance())
	m_eNewState = BLOCKING;
	}
	}*/

	if (m_bHit && m_fCurrentHealth > 0)
		m_eNewState = HURT;

	else if (m_bHit && m_fCurrentHealth <= 0)
		m_eNewState = HURT;

	else if (reinterpret_cast<CPlayer*>(GetPlayer())->GetHealth() <= 0.0f)
		m_eNewState = RETREAT;

	if (m_eNewState != m_eCurrState)
	{
		ExitSubState();
		m_eCurrState = m_eNewState;
		EnterSubState();
	}
}
void CSkeleton::AddToBlockWeight(int side)
{
	/*if (side == BLOCK_LEFT)
	{
	m_nLeftBlockChance += 5;
	m_nRightBlockChance -= 5;

	}
	else
	{
	m_nLeftBlockChance -= 5;
	m_nRightBlockChance += 5;
	}*/
}
void CSkeleton::ResetManuverVector(int chance, XMFLOAT3& mVector)
{
	XMVECTOR zAxis = XMLoadFloat3(GetZAxis());
	XMVECTOR yAxis = XMLoadFloat3(GetYAxis());
	zAxis = XMVector3Normalize(zAxis);
	yAxis = XMVector3Normalize(yAxis);


	XMVECTOR translateVector;

	if (chance == 0)
		translateVector = XMVector3Cross(zAxis, yAxis);
	else
		translateVector = XMVector3Cross(yAxis, zAxis);
	translateVector = XMVector3Normalize(translateVector);
	translateVector *= 150.0f;
	XMStoreFloat3(&mVector, translateVector);
}
void CSkeleton::BuildPathToPlayer()
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
void CSkeleton::BuildPathHome()
{

	int nStartNode = GetPathPlanner()->FindClosestNode(GetPosition());
	int nGoalNode = GetPathPlanner()->FindClosestNode(&GetSpawnPoint());

	vector<XMFLOAT3> newPath = GetPathPlanner()->GeneratePath(nStartNode, nGoalNode, false);
	SetNodeOnPath(newPath.size() - 1);

	SetPath(newPath);
}
void CSkeleton::UpdateBlockSide()
{
	/*if (m_nLeftBlockChance != m_nRightBlockChance)
	{
	m_fBlockScaleTimer -= DELTA_TIME();



	if (m_fBlockScaleTimer < 0.0f)
	{
	if (abs(m_nLeftBlockChance - m_nRightBlockChance) < 2)
	{
	m_nLeftBlockChance = 50;
	m_nRightBlockChance = 50;
	}
	else
	{
	if (m_nLeftBlockChance < m_nRightBlockChance)
	{
	m_nLeftBlockChance += 2;
	m_nRightBlockChance -= 2;
	}
	else
	{
	m_nLeftBlockChance -= 2;
	m_nRightBlockChance += 2;
	}
	}
	m_fBlockScaleTimer = 1.0f;
	}

	if (m_nLeftBlockChance > 75)
	m_nLeftBlockChance = 75;
	else if (m_nLeftBlockChance > 75)
	m_nLeftBlockChance = 75;
	if (m_nRightBlockChance > 75)
	m_nRightBlockChance = 75;
	else if (m_nRightBlockChance < 25)
	m_nRightBlockChance = 25;
	}*/
}

void CSkeleton::Blocked()
{
	//Getting blocked, stun logic
	//m_bHit = true;
}

void CSkeleton::Knockback(XMFLOAT3 _direction, float _force)
{
	_direction.x *= _force;
	_direction.y = 700.0f;
	_direction.z *= _force;

	knockBackVelocity = _direction;

	m_f3Velocity = _direction;
}

void CSkeleton::UpdatePosition()
{
	IObject::UpdatePosition();
	
}

void CSkeleton::AddToRenderer()
{
	IObject::AddToRenderer();

	//particles
	m_unParticleEmitters[eBoneParticle] = m_pParticleManager->LoadEmitter("BoneFragment.xml");
	m_unParticleEmitters[eDustParticle] = m_pParticleManager->LoadEmitter("BoneDust.xml");
}
