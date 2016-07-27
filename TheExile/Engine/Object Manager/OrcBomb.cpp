
#include "OrcBomb.h"
#include "../Particles/ParticleManager.h"
#include "../Particles/Particle.h"
#include "../Particles/Emitter.h"
#include "../Asset Manager/AssetManager.h"
#include "../Core/Messages/AddObjectMessage.h"
#include "../Core/Messages/RemoveObjectMessage.h"
#include "../../Game/Source/Agents/Orc/Orc.h"
#include "../../Game/Source/Environment/Bridge.h"
#include "../Animation/AnimationMesh.h"
#include "../../Game/Source/Player/Player.h"
#include "../../Game/Source/Agents/Skeleton/Skeleton.h"

COrcBomb::COrcBomb(COrc* parent, CObjectManager* objManager) : IObject("OrcBomb")
{
	m_pOwner = parent;
	m_cpParticleManager = new CParticleManager(objManager);
	m_cpParticleManager->LoadEmitter("WallExplosion.xml");
	m_cpParticleManager->GetEmitter(0)->Stop();



	CMesh* mesh = CAssetManager::GetInstance()->GetPrefabMesh("WallBomb");
	m_cpRenderMesh = new CRenderMesh(&mesh[0], GRAPHICS->GetVertexShader(), GRAPHICS->GetStandardPS(),
		nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Failbot_WallBomb.dds");
	m_cpRenderMesh->GetRender() = false;

	explosionExtents = float3(300.0f, 500.0f, 300.0f);

	CMesh::TAABB aabb;
	mesh[0].BuildAABB(aabb);
	bombExtents = aabb.extents;
	AddCollider(new CCollider(true, new CAABB(aabb.center, aabb.extents), true, false));
	((CAABB*)m_pvColliders[0]->GetBounds())->SetCenter({ m_mWorld._41, m_mWorld._42, m_mWorld._43 });
	GRAPHICS->AddRenderMesh(m_cpRenderMesh);
	XMFLOAT3 grav = { 0.0f, -1500.0f, 0.0f };
	gravityMag = XMVector3Length(XMLoadFloat3(&grav)).m128_f32[0];
	m_eCurrState = eInactive;
	traveled = false;
	m_fFuseTimer = 0.0f;

	CAddObjectMessage* msg = new CAddObjectMessage(this, CObjectManager::eObjectType::Dynamic);
	msg->Send();
	
}
COrcBomb::~COrcBomb()
{
	SAFE_DELETE(m_cpParticleManager);
	//delete m_cpParticleMananager;
	//m_pOwner = nullptr;
}


void COrcBomb::ChangeState(EBombState newState)
{
	if (newState != m_eCurrState)
	{
		m_eCurrState = newState;
		switch (m_eCurrState)
		{
		case eTraveling:
		{
			m_fFuseTimer = FUSE_TIMER;

			m_cpRenderMesh->GetRender() = true;
			XMFLOAT3 dir;
			bombTarget = *m_pOwner->GetPlayer()->GetPosition();
			XMVECTOR playerDir = XMLoadFloat3(m_pOwner->GetPlayer()->GetZAxis());
			playerDir = XMVector3Normalize(playerDir);
			XMStoreFloat3(&dir, playerDir);
			m_pvColliders[0]->SetActive(true);

			bombTarget.x += dir.x * 75.0f;
			bombTarget.y += dir.y * 75.0f;
			bombTarget.z += dir.z * 75.0f;

			bombPosition = *GetPosition();
			traveled = true;

			SetStartingVelocity(*GetPosition(), bombTarget);
		}

		break;
		case eHeld:
		{
			
			m_cpRenderMesh->GetRender() = true;
			m_fExitTime = .75f;
		}
			break;
		case eExploding:
		{
			float3 pos = *GetPosition();
			m_cpParticleManager->GetEmitter(0)->GetPosition(0) = pos.x;
			m_cpParticleManager->GetEmitter(0)->GetPosition(1) = pos.y;
			m_cpParticleManager->GetEmitter(0)->GetPosition(2) = pos.z;
			m_cpParticleManager->GetEmitter(0)->ePlay();

			AudioSystemWwise::Get()->PostEvent(AK::EVENTS::WALLEXPLOSION, pos);
			m_fExitTime = 2.0f;
			m_f3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
			m_cpRenderMesh->GetRender() = false;
			CAABB* aabb = (CAABB*)(m_pvColliders[0]->GetBounds());
			aabb->SetExtents(explosionExtents);
		}
			break;
		
		default:
			break;
		}
	}
}
void COrcBomb::BombHeld()
{

	XMMATRIX mat1 = XMLoadFloat4x4(&m_pOwner->GetWorldMatrix());
	XMMATRIX mat2 = XMLoadFloat4x4(&((CAnimationMesh*)(m_pOwner->GetRenderMesh()))->GetJointMatrix("LeftHand"));

	XMStoreFloat4x4(&m_mWorld , XMMatrixMultiply(mat2, mat1));
	
	//mat = XMLoadFloat4x4((CAnimationMesh*)(m_pOwner->GetRenderMesh()))->GetJointMatrix("RightHand") *
	if (m_eCurrState == eHeld)
	{
		m_fExitTime -= DELTA_TIME();
		if (m_fExitTime < 0.0f)
			ChangeState(eTraveling);
	}

	
}
void COrcBomb::BombExplode()
{
	
	
	m_cpParticleManager->Update();
	m_fExitTime -= DELTA_TIME();
	if (m_fExitTime < 0.0f)
		DeActivate();
	if (m_fExitTime < 1.85f && m_pvColliders[0]->GetActive() == true)
	{
		m_pvColliders[0]->SetActive(false);
	}

}
void COrcBomb::BombTravel()
{
	m_fFuseTimer -= DELTA_TIME();
	if (m_fFuseTimer < 0.0f)
		ChangeState(eExploding);

	worldVelocity.y -= 1500.0f * DELTA_TIME();
	SetWorldVelocity(worldVelocity);
	//XMFLOAT3 deltaVelocity;
	//XMStoreFloat3(&deltaVelocity, XMLoadFloat3(&m_f3Velocity));

	//m_pvColliders[0]->UpdatePosition(deltaVelocity);

}
void COrcBomb::Update()
{


	switch (m_eCurrState)
	{
	case eTraveling:
		BombTravel();
		break;
	case eInactive:
	case eHeld:
		BombHeld();
		break;
	case eExploding:
		BombExplode();
		break;
	default:
		break;
	}
	

	GetColliders()[0]->GetBounds()->SetCenter(float3(m_mWorld._41, m_mWorld._42, m_mWorld._43));;
	//DEBUG_UTIL::DrawAABBCollider(aabb->Min(), aabb->Max());
	/*for (unsigned int collider = 0; collider < m_pvColliders.size(); collider++)
	{
		m_pvColliders[collider]->UpdatePosition({ GetWorldVelocity().x * DELTA_TIME(), GetWorldVelocity().y * DELTA_TIME(), GetWorldVelocity().z * DELTA_TIME() });
	}*/
}
void COrcBomb::Activate()
{
	
	
	m_cpRenderMesh->GetRender() = true;
	XMMATRIX mat1 = XMLoadFloat4x4(&m_pOwner->GetWorldMatrix());
	XMMATRIX mat2 = XMLoadFloat4x4(&((CAnimationMesh*)(m_pOwner->GetRenderMesh()))->GetJointMatrix("LeftHand"));

	XMStoreFloat4x4(&m_mWorld, XMMatrixMultiply(mat2, mat1));

	GetColliders()[0]->GetBounds()->SetCenter(float3(m_mWorld._41, m_mWorld._42, m_mWorld._43));
	
}
void COrcBomb::DeActivate()
{

	m_cpRenderMesh->GetRender() = false;
	m_cpParticleManager->GetEmitter(0)->Stop(); 
	CAABB* aabb = (CAABB*)(m_pvColliders[0]->GetBounds());
	aabb->SetExtents(bombExtents);
	m_eCurrState = eInactive;

}
void COrcBomb::CollisionResponse(IObject * _cCollidedObject)
{
	if (m_eCurrState == eExploding)
	{
		if (_cCollidedObject->GetTag() == "Player")
		{

			((CPlayer*)_cCollidedObject)->TakeDamage(40);

		}
		else if (_cCollidedObject->GetTag() == "Skeleton")
		{
			((CSkeleton*)_cCollidedObject)->TakeDamage(40);

		}
		else if (_cCollidedObject->GetTag() == "Orc")
		{
			if (_cCollidedObject != m_pOwner)
			{
				((COrc*)_cCollidedObject)->TakeDamage(40);

			}

		}

	}
	else if (m_eCurrState == eTraveling)
	{
		if (_cCollidedObject->GetTag() == "Player")
		{
			ChangeState(eExploding);

		}
		else if (_cCollidedObject->GetTag() == "Floor" || _cCollidedObject->GetTag() == "SafeHavenFloor")
		{
			if ( m_f3Velocity.y < 0.0f)
			{
				ChangeState(eExploding);
			}
		}
		else if (_cCollidedObject->GetTag() == "Wall" || _cCollidedObject->GetTag() == "Enemy Cave Wall")
		{

			ChangeState(eExploding);
			
		}
		else if (_cCollidedObject->GetTag() == "CrackedWall")
		{
			// For the last 4 colliders (Bomb colliders)
			for (size_t i = 0; i < _cCollidedObject->GetColliders().size() - 4; i++)
			{
				if ((_cCollidedObject->GetColliders()[i])->GetCollided())
				{
					ChangeState(eExploding);
					break;
				}
			}
		}

		if (_cCollidedObject->GetTag() == "Door")
		{
			if (_cCollidedObject->GetColliders()[0]->GetCollided())
				ChangeState(eExploding);
		}


		if (_cCollidedObject->GetTag() == "PitWall")
		{
			ChangeState(eExploding);

		}
		else if (_cCollidedObject->GetTag() == "Pit" || _cCollidedObject->GetTag() == "BigPit")
		{
			ChangeState(eExploding);

		}
		else if (_cCollidedObject->GetTag() == "ExitDoor")
		{
			ChangeState(eExploding);

		}
		else if (_cCollidedObject->GetTag() == "SpikeTrap")
		{
			ChangeState(eExploding);

		}
		else if (_cCollidedObject->GetTag() == "SpinningBlade")
		{
			ChangeState(eExploding);

		}

		else if (_cCollidedObject->GetTag() == "Bridge")
		{
			CBridge* curBridge = reinterpret_cast<CBridge*>(_cCollidedObject);

			if (curBridge->IsActive())
				ChangeState(eExploding);
		}



		else if (_cCollidedObject->GetTag() == "Tree")
		{
			ChangeState(eExploding);

		}

	}
	





}

void COrcBomb::SetStartingVelocity(XMFLOAT3 start, XMFLOAT3 targetPos)
{
	/*float time = .7f;
	XMFLOAT3 gravity = XMFLOAT3(0.0f, -1500.0f, 0.0f);
	XMVECTOR totarget = XMLoadFloat3(&targetPos) - XMLoadFloat3(&start);
	XMVECTOR toTargetxz = totarget;
	toTargetxz.m128_f32[1] = 0.0f;


	float y = totarget.m128_f32[1];
	float xz = XMVector3Length(toTargetxz).m128_f32[0];

	float yvel = y / time + .5f * gravityMag * time;

	float zvel = xz / time;

	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Normalize(toTargetxz));
	result.x *= zvel;
	result.y = yvel;
	result.z *= zvel;
*/
	start.y = 0;
	targetPos.y = 0;

	XMFLOAT3 vImpulse = XMFLOAT3(0.0f,1150.0f, 0.0f);
	XMVECTOR toTarget = XMLoadFloat3(&targetPos) - XMLoadFloat3(&start);
	XMVECTOR impulse = XMLoadFloat3(&vImpulse);

	toTarget = XMVector3Normalize(toTarget);
	toTarget *= 500.0f;
	toTarget += impulse;

	XMStoreFloat3(&worldVelocity, toTarget);
	SetWorldVelocity(worldVelocity);
	//return result;
}