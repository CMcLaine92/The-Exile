#include "MinotaurZone.h"
#include "../../../../Engine/Core/Collision/AABB.h"
CMinotaurZone::CMinotaurZone() : IObject("MinotaurZone")
{

}
void CMinotaurZone::Update()
{
#if _DEBUG
	CAABB* pAABB = static_cast<CAABB*> (m_pvColliders[0]->GetBounds());
	DEBUG_UTIL::DrawAABBCollider(pAABB->Min(), pAABB->Max());
#endif
}