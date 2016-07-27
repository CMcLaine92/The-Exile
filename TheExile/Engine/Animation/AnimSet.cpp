#include "AnimSet.h"

CAnimSet::CAnimSet()
{

}
CAnimSet::~CAnimSet()
{

}
const CAnimation* CAnimSet::GetAnimation(int index)
{
	return &m_cAnimations[index];
}
const CAnimation* CAnimSet::GetDefaultAnim()
{
	return &m_cAnimations[0];
}

void CAnimSet::AddRig(CRig* _rig)
{
	m_pRig = _rig;
}


bool CAnimSet::LoadFromXML(const char* filePath)
{
	return true;
}