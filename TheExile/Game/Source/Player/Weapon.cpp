#include "Weapon.h"
#include "../../../Engine/Asset Manager/AssetManager.h"

CWeapon::CWeapon(CMesh* _mesh, const wchar_t* texturePath, const wchar_t* normalPath, int _damage, float _attackTime, std::string _name)
{
	m_nDamage = _damage;
	m_fAttackTime = _attackTime;
	m_sName = _name;
	

	m_cWeaponRender = new CRenderMesh(_mesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), nullptr, nullptr, nullptr, texturePath);
	if (normalPath)
		m_cWeaponRender->SetNormals(normalPath);
	m_cWeaponRender->GetRender() = false;
	GRAPHICS->AddRenderMesh(m_cWeaponRender);

}


CWeapon::~CWeapon()
{
	GRAPHICS->RemoveRenderMesh(m_cWeaponRender);

	delete m_cWeaponRender;
}

void CWeapon::Activate()
{
	m_cWeaponRender->GetRender() = true;

}

void CWeapon::Deactivate()
{
	m_cWeaponRender->GetRender() = false;

}
