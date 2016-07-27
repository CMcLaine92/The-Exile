#include "Animator.h"
#include "AnimationMesh.h"
#include "AnimSet.h"

CAnimator::CAnimator()
{

}
CAnimator::~CAnimator()
{

}

void CAnimator::SetAnimSet(string animKey)
{

}
void CAnimator::TransitionTo(EBlendType blendType, int AnimKey, float tranisitionTime)
{
	unsigned int nNextInterp;

	if (m_cInterpolators[m_nCurrInterp].GetAnimation() == 0)
	{
		m_fCurrBlendTime = 0.0f;
		m_fTotalBlendTime = 0.0f;
		m_cInterpolators[m_nCurrInterp].SetAnimation(m_pAnimSet->GetAnimation(AnimKey));
	}
		
	else
	{
		nNextInterp = m_nCurrInterp ^ 0x1;
		m_fTotalBlendTime = tranisitionTime;
		m_fCurrBlendTime = 0.0f;

		m_cInterpolators[nNextInterp].SetAnimation(m_pAnimSet->GetAnimation(AnimKey));
	}
}
void CAnimator::SetAnimMesh(CAnimationMesh* _animMesh)
{
	m_cpMesh = _animMesh;
}
void CAnimator::Update(float dt)
{
	unsigned int nNext = (m_nCurrInterp == 1) ? 0 : 1;

	if (m_fTotalBlendTime > 0.0f)
	{
		m_fCurrBlendTime += dt;
		if (m_fCurrBlendTime >= m_fTotalBlendTime)
		{
			m_nCurrInterp = nNext;
			nNext = (m_nCurrInterp == 1) ? 0 : 1;
			m_fTotalBlendTime = 0.0f;
			m_fCurrBlendTime = 0.0f;
		}
	}

	m_cInterpolators[m_nCurrInterp].Update(dt);
	m_cpMesh->GetPose().resize(m_cInterpolators[m_nCurrInterp].GetPose().size());

	if (m_fTotalBlendTime > 0.0f)
	{
		m_cInterpolators[nNext].Update(dt);

		for (size_t currNode = 0; currNode < m_cInterpolators[nNext].GetPose().size(); currNode++)
			m_cpMesh->GetPose()[currNode] = CInterpolator::Interpolate(m_cInterpolators[m_nCurrInterp].GetPose()[currNode], m_cInterpolators[nNext].GetPose()[currNode], m_fCurrBlendTime / m_fTotalBlendTime);
	}
	else if (m_fTotalBlendTime <= 0.0f)
	{
		for (size_t currNode = 0; currNode < m_cInterpolators[m_nCurrInterp].GetPose().size(); currNode++)
			m_cpMesh->GetPose()[currNode] = m_cInterpolators[m_nCurrInterp].GetPose()[currNode];
	}
}
bool CAnimator::InTransition(void)
{
	return m_fCurrBlendTime > 0.0f;
}
