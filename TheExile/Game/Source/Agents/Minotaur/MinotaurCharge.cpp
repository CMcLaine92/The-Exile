
#include "MinotaurCharge.h"
#include "Minotaur.h"
#include "../../Player/Player.h"
#define CHARGE_SPEED 15.0f
#define MAX_DISTANCE 1500.0f

void CMinotaurCharge::Enter(CMinotaur* _agent)
{
	m_fDistanceMoved = 0.0f;
	_agent->SetMoveSpeed(CHARGE_SPEED);
	_agent->SetWorldVelocity({ 0, 0, 0 });
	vector<XMFLOAT3>& vPath = _agent->GetPath(); // path here from group
	int nCurrentNode = _agent->GetCurNodeOnPath();
	if (nCurrentNode < 0) nCurrentNode = 0;
	else if (nCurrentNode > (int)vPath.size()) nCurrentNode = (int)vPath.size();
	//AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYMINOALERT, *_agent->GetPosition());

	_agent->GetCurrAnimation().SetScale(1.50f);

}
void CMinotaurCharge::Execute(CMinotaur* _agent)
{
	DBG_UNREFERENCED_PARAMETER(_agent);

}
void CMinotaurCharge::Exit(CMinotaur* _agent)
{
	_agent->GetCurrAnimation().SetScale(1.0f);


}

int CMinotaurCharge::UpdateState(CMinotaur* _agent)
{
	if (((CPlayer*)_agent->GetPlayer())->GetHealth() <= 0)
	{
		return CMinotaur::ePatrolState;
	}
	
	return 0;
}
