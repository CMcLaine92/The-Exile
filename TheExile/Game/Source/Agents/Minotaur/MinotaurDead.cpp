#include "Minotaur.h"
#include "MinotaurDead.h"



void CMinotaurDead::Enter(CMinotaur* _agent)
{
	_agent->SetWorldVelocity({ 0.0f, 0.0f, 0.0f });
	_agent->SwitchAnimation(CMinotaur::eDeathAnim);
	m_fStateDuration = _agent->GetCurrAnimation().GetDuration();
	m_fStateTime = 0.0f;
	//AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYMINOALERT, *_agent->GetPosition());
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::MINOALERT, *_agent->GetPosition());
	//AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYMINOALERT, *_agent->GetPosition());

}
void CMinotaurDead::Execute(CMinotaur* _agent)
{
	DBG_UNREFERENCED_PARAMETER(_agent);
	m_fStateTime += DELTA_TIME();
}
void CMinotaurDead::Exit(CMinotaur* _agent)
{
	DBG_UNREFERENCED_PARAMETER(_agent);

}
int CMinotaurDead::UpdateState(CMinotaur* _agent)
{
	DBG_UNREFERENCED_PARAMETER(_agent);

	if (m_fStateTime >= m_fStateDuration)
	{
		return -1;
	}
	return CMinotaur::eDeadState;
}
