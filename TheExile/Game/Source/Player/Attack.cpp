#include "Attack.h"
#include "../../../Engine/Renderer/Renderer.h"
#include "../../../Engine/Core/Collision/AABB.h"
#include "../../../Engine/Core/Messages/DestroyBushMessage.h"
#include "../Environment/Tree.h"
#include "../../../Engine/Animation/Mesh.h"

CAttack::CAttack(IObject * _cpParent, CMesh * _wepMesh, XMFLOAT4X4 * renderMatrix, int _nDamage, std::string _szTag) : IObject(_szTag)
{
	m_ipParent = _cpParent;
	m_nDamage = _nDamage;
	m_bIsActive = true;
	m_cRenderMatrix = renderMatrix;

	vector<TVertex>& verts = _wepMesh->GetVertices();
	float highestY = -FLT_MAX;
	float lowestY = FLT_MAX;
	float tipIndex = 0;
	float baseIndex = 0;
	for (unsigned int vert = 0; vert < verts.size(); vert++)
	{
		if (verts[vert].m_fPosition[1] > highestY)
		{
			highestY = verts[vert].m_fPosition[1];
			tipIndex = vert;
		}
		else if (verts[vert].m_fPosition[1] < lowestY)
		{
			lowestY = verts[vert].m_fPosition[1];
			baseIndex = vert;
		}
	}

	m_cSwordTip = &verts[tipIndex];
	m_cSwordBase = &verts[baseIndex];

	XMFLOAT3 tipPos = { m_cSwordTip->m_fPosition[0], m_cSwordTip->m_fPosition[1], m_cSwordTip->m_fPosition[2] };
	XMFLOAT3 basePos = { m_cSwordBase->m_fPosition[0], m_cSwordBase->m_fPosition[1], m_cSwordBase->m_fPosition[2] };

	XMVECTOR tipVec = XMLoadFloat3(&tipPos);
	XMVECTOR baseVec = XMLoadFloat3(&basePos);
	XMMATRIX renderMat = XMLoadFloat4x4(m_cRenderMatrix);

	tipVec = XMVector3Transform(tipVec, renderMat);
	baseVec = XMVector3Transform(baseVec, renderMat);

	XMStoreFloat3(&tipPos, tipVec);
	XMStoreFloat3(&basePos, baseVec);

	CCapsule* swordCapsule = new CCapsule(basePos, tipPos, 30.0f);
	CCollider* swordCollider = new CCollider(true, swordCapsule);
	AddCollider(swordCollider);

	m_unDoorCounter = 0;
	m_unWallCounter = 0;

	m_fWallAudioTimer = 0.0f;
	m_fDoorAudioTimer = 0.0f;

	m_bWallAudioON = false;
	m_bDoorAudioON = false;

	SetIsActive(false);
}

CAttack::~CAttack()
{

}

void CAttack::Update()
{

	XMFLOAT3 tipPos = { m_cSwordTip->m_fPosition[0], m_cSwordTip->m_fPosition[1], m_cSwordTip->m_fPosition[2] };
	XMFLOAT3 basePos = { m_cSwordBase->m_fPosition[0], m_cSwordBase->m_fPosition[1], m_cSwordBase->m_fPosition[2] };

	XMVECTOR tipVec = XMLoadFloat3(&tipPos);
	XMVECTOR baseVec = XMLoadFloat3(&basePos);
	XMMATRIX renderMat = XMLoadFloat4x4(m_cRenderMatrix);

	tipVec = XMVector3Transform(tipVec, renderMat);
	baseVec = XMVector3Transform(baseVec, renderMat);

	XMStoreFloat3(&tipPos, tipVec);
	XMStoreFloat3(&basePos, baseVec);

	((CCapsule*)GetColliders()[0]->GetBounds())->SetStart(basePos);
	((CCapsule*)GetColliders()[0]->GetBounds())->SetEnd(tipPos);

//GRAPHICS->DrawLine(	((CCapsule*)GetColliders()[0]->GetBounds())->GetStart(), ((CCapsule*)GetColliders()[0]->GetBounds())->GetEnd(), float3(0.0f, 1.0f, 0.0f));

	if (m_bIsActive)
	{
		m_fAttackTimeLeft -= DELTA_TIME();

		XMMATRIX m_mParent = XMLoadFloat4x4(&m_ipParent->GetWorldMatrix());
		XMMATRIX local = XMLoadFloat4x4(&m_mLocal);
		XMMATRIX velocity = XMMatrixTranslation(m_f3Velocity.x, m_f3Velocity.y, m_f3Velocity.z);
		XMMATRIX newLocal = XMMatrixMultiply(local, velocity);
		XMStoreFloat4x4(&m_mLocal, newLocal);
		XMStoreFloat4x4(&m_mWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_mLocal), m_mParent));

		if (m_bWallAudioON == true)
		{
			m_fWallAudioTimer += DELTA_TIME();
			m_unWallCounter++;

			if (m_unWallCounter <= 1)
			{
				AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYERHITWALL, *m_ipParent->GetPosition());
			}

			if (m_fWallAudioTimer >= 1.0f)
			{
				m_unWallCounter = 0;
				m_fWallAudioTimer = 0.0f;
				m_bWallAudioON = false;
			}
		}

		if (m_bDoorAudioON == true)
		{
			m_fDoorAudioTimer += DELTA_TIME();
			m_unDoorCounter++;

			if (m_unDoorCounter <= 1)
			{
				AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYERHITDOOR, *m_ipParent->GetPosition());
			}

			if (m_fDoorAudioTimer >= 1.0f)
			{
				m_unDoorCounter = 0;
				m_fDoorAudioTimer = 0.0f;
				m_bDoorAudioON = false;
			}
		}
	}

}

void CAttack::CollisionResponse(IObject * _cCollidedObject)
{
	if (m_ipParent->GetTag() != _cCollidedObject->GetTag() && m_ipParent->GetTag() == "Player")
	{
		if (_cCollidedObject->GetTag() == "Wall")
		{
			m_bWallAudioON = true;
		}
		else if ((_cCollidedObject->GetTag() == "Door" || _cCollidedObject->GetTag() == "ExitDoor"))
		{
			m_bDoorAudioON = true;
		}

		if (_cCollidedObject->GetTag() == "Bush")
		{
			((CBush*)_cCollidedObject)->DestroyBush();
		}
		else if (_cCollidedObject->GetTag() == "Tree")
		{
			if (!reinterpret_cast<CTree*>(_cCollidedObject)->GetFalling())
			{
				reinterpret_cast<CTree*>(_cCollidedObject)->SetFalling(true);
			}
		}
	}
}

void CAttack::Attack(int _nDamage)
{
	m_nDamage = _nDamage;
	SetIsActive(true);
}

void CAttack::SetIsActive(bool _isActive)
{
	m_pvColliders[0]->SetActive(_isActive);
	m_bIsActive = _isActive;
}