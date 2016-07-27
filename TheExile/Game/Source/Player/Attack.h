#pragma once
#include "..\..\..\Engine\Object Manager\IObject.h"
#include "../../../Engine/Core/Collision/Capsule.h"

class CAttack :
	public IObject
{
public:
	enum EAttackType {eLightAttack, eHeavyAttack};

private:
	EAttackType eType;
	XMFLOAT4X4 m_mLocal;
	std::vector<TVertex> tCubeVertex;
	XMFLOAT4X4 * m_cRenderMatrix;

	int m_nDamage;
	float m_fAttackSpeed;
	float m_fAttackTime;
	float m_fAttackTimeLeft = 0.0f;
	bool m_bIsActive;

	float m_fAttackDelayTimer = 0.0f;
	TVertex * m_cSwordTip;
	TVertex * m_cSwordBase;

	unsigned int m_unWallCounter;
	float m_fWallAudioTimer;
	bool m_bWallAudioON;

	unsigned int m_unDoorCounter;
	float m_fDoorAudioTimer;
	bool m_bDoorAudioON;

	bool m_bAboutToAttack = false;

public:
	CAttack(IObject * _cpParent, CMesh * _wepMesh, XMFLOAT4X4 * renderMatrix, int _nDamage = 0, std::string _szTag = "Attack");
	~CAttack();

	void Update() override;
	void CollisionResponse(IObject * _cCollidedObject) override;
	int GetDamage() { return m_nDamage; }
	void SetDamage(int _damage){ m_nDamage = _damage; }
	float GetAttackTimeLeft() { return m_fAttackTimeLeft; }
	float GetAttackTime() { return m_fAttackTime; }


	void SetIsActive(bool _active);
	bool GetIsActive() { return m_bIsActive; }
	void Attack(int _nDamage = 0);
};

