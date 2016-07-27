
#include "MinotaurIdle.h"

#include "Minotaur.h"

void CMinotaurIdle::Enter(CMinotaur* _agent)
{
	m_fWaitTimer = WAIT_TIME;
	_agent->SwitchAnimation(CMinotaur::eIdleAnim);
	_agent->GetRenderMesh()->GetRender() = true;
	_agent->GetCurrAnimation().SetScale(0.5f);
	//AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYMINOSNIFF, *_agent->GetPosition());
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::MINOSNIFF, *_agent->GetPosition());
//	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYMINOSNIFF, *_agent->GetPosition());


}
void CMinotaurIdle::Execute(CMinotaur* _agent)
{
	DBG_UNREFERENCED_PARAMETER(_agent);
	m_fWaitTimer -= DELTA_TIME();

}
void CMinotaurIdle::Exit(CMinotaur* _agent)
{
	DBG_UNREFERENCED_PARAMETER(_agent);
	m_fWaitTimer = 0;
	_agent->GetCurrAnimation().SetScale(1.0f);

}

int CMinotaurIdle::UpdateState(CMinotaur* _agent)
{
	XMFLOAT3 curPos = *_agent->GetPosition();

	// Convert them for math
	XMVECTOR mathPos = XMLoadFloat3(&curPos);
	XMVECTOR mathTarget = XMLoadFloat3(_agent->GetPlayer()->GetPosition());

	// Find the vector between the two points
	XMVECTOR mathToVec = XMVectorSubtract(mathTarget, mathPos);
	XMVECTOR mathDistToTarget = XMVector3Length(mathToVec);
	/*if (mathDistToTarget.m128_f32[0] <= 1000.0f)
		return CMinotaur::eChaseState;

	if (m_fWaitTimer < 0)*/
		return CMinotaur::ePatrolState;

	//return CMinotaur::eIdleState;
}