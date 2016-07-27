
#include "MinotaurStomp.h"
#include "Minotaur.h"
#include "../../../../Animation/Animation.h"
#include "../../Player/Player.h"

void CMinotaurStomp::Enter(CMinotaur* _agent)
{
	DBG_UNREFERENCED_PARAMETER(_agent);
	m_fStateDuration = _agent->GetAnimation(CMinotaur::eStompAnim).GetDuration();
	m_fStompStart = m_fStateDuration * 0.5f;
	_agent->SetMoveSpeed(0.0f);
	_agent->SwitchAnimation(CMinotaur::eStompAnim);
	//AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYMINOALERT, *_agent->GetPosition());
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::MINOALERT, *_agent->GetPosition());
	//AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYMINOALERT, *_agent->GetPosition());

	m_bPlayerDead = false;
}
void CMinotaurStomp::Execute(CMinotaur* _agent)
{
	m_fStompStart -= DELTA_TIME();
	m_fStateDuration -= DELTA_TIME();

	if (m_bPlayerDead == false && m_fStompStart <= 0.0f)
	{
		CPlayer* pPlayer = (CPlayer*)_agent->GetPlayer();


		XMFLOAT3 curPos = *_agent->GetPosition();

		// Convert them for math
		XMVECTOR mathPos = XMLoadFloat3(&curPos);
		XMVECTOR mathTarget = XMLoadFloat3(pPlayer->GetPosition());

		// Find the vector between the two points
		XMVECTOR mathToVec = XMVectorSubtract(mathTarget, mathPos);
		XMVECTOR mathDistToTarget = XMVector3Length(mathToVec);
		if (mathDistToTarget.m128_f32[0] <= STOMP_AOE)
		{
			pPlayer->TakeDamage(100);
			m_bPlayerDead = true;
		}
	}
	
		

}
void CMinotaurStomp::Exit(CMinotaur* _agent)
{
	//AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYMINOSTOMP, *_agent->GetPosition());
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::MINOSTOMP, *_agent->GetPosition());
	//AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYMINOSTOMP, *_agent->GetPosition());

	_agent->PlayStopParticle();
	_agent->SetStompTimer(3.0f);

}

int CMinotaurStomp::UpdateState(CMinotaur* _agent)
{
	DBG_UNREFERENCED_PARAMETER(_agent);
	
//	if (m_fStateDuration < 0.0f && m_bPlayerDead)
//		return CMinotaur::eIdleState;
//
//	/*if (m_fStateDuration < 0.0f)
//		return CMinotaur::eChaseState;
//*/
//	return CMinotaur::eStompState;
	return 0;


}