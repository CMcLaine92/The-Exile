#pragma once
#include <string>
#include "../../../Engine/Animation/Mesh.h"
#include "../../../Engine/Renderer/RenderMesh.h"
#include "../Player/Attack.h"

class CWeapon
{
protected:
	enum EWeaponType {eDagger, eSword, eAcidSword, eHatchet, eDoubleAxe, eFlamingAxe};
	int m_nDamage = 0;
	float m_fAttackTime = 0.0f;
	std::string m_sName;
	CAttack * m_cAttack = nullptr;

	//CMesh m_cWeaponMesh;
	CRenderMesh * m_cWeaponRender;

public:
	CWeapon(CMesh* _mesh, const wchar_t* texture, const wchar_t* noramlPath = nullptr, int _damage = 0, float _attackTime = 0.0f, std::string _name = "Weapon");
	virtual ~CWeapon();

	virtual void Activate();
	virtual void Deactivate();

	//Mutators
	void SetDamage(int _damage)  { m_nDamage = _damage; }
	void SetString(const std::string& _name)  { m_sName = _name; }
	void SetAttackTime(float time) { m_fAttackTime = time; }
	void SetAttack(CAttack * _attack) { m_cAttack = _attack; }

	//Accessors
	const int GetDamage() const { return m_nDamage; }
	const std::string& GetString() const { return m_sName; }
	CRenderMesh * GetRenderMesh() { return m_cWeaponRender; }
	const float GetAttackTime() const { return m_fAttackTime; }
	CAttack * GetAttack() { return m_cAttack; }

};

