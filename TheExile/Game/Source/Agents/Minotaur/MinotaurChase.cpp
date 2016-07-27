
#include "MinotaurChase.h"
#include "Minotaur.h"
#include "../../Player/Player.h"


void CMinotaurChase::Enter(CMinotaur* _agent)
{
	m_fRepathTimer = -1;
	_agent->SwitchAnimation(CMinotaur::eRunAnim);
	_agent->GetCurrAnimation().SetScale(1.25f);

	BuildPath(_agent);
	m_bRepath = false;
	//AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYMINOALERT, *_agent->GetPosition());
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::MINOALERT, *_agent->GetPosition());
	//AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYMINOALERT, *_agent->GetPosition());

}
void CMinotaurChase::Execute(CMinotaur* _agent)
{
	//m_fRepathTimer -= DELTA_TIME();
	_agent->SetWorldVelocity({ 0, 0, 0 });

	int nCurrentNode = _agent->GetCurNodeOnPath();

	if (m_bRepath)
	{
		
		BuildPath(_agent);
		m_bRepath = false;
	}


	// Get the current path
	vector<XMFLOAT3>& vPath = _agent->GetPath(); // path here from group

	// Get the current velocity
	//XMVECTOR currentVelocity; XMLoadFloat3(&m_f3vel);

	// Node that I'm on along the path
	nCurrentNode = _agent->GetCurNodeOnPath();

	// Clamp CurNode to 0 and max
	if (nCurrentNode < 0) nCurrentNode = 0;
	else if (nCurrentNode > (int)vPath.size()) nCurrentNode = (int)vPath.size();

	// If no path was generated, then return
	if (vPath.size() <= 0)
		return;

	// Get the current positions
	XMFLOAT3 curPos = *_agent->GetPosition();
	XMFLOAT3 targetPos = vPath[nCurrentNode];

	// Convert them for math
	XMVECTOR mathPos = XMLoadFloat3(&curPos);
	XMVECTOR mathTarget = XMLoadFloat3(&targetPos);

	// Find the vector between the two points
	XMVECTOR mathToVec = XMVectorSubtract(mathTarget, mathPos);

	//XMFLOAT3 toVec; XMStoreFloat3(&toVec, mathToVec);

	// Normalize the toVector to get the direction
	XMVECTOR mathVelocity = XMVector3Normalize(mathToVec);

	

	XMFLOAT3 realVelocity; XMStoreFloat3(&realVelocity, mathVelocity);
	//realVelocity.y = 0.0f;

	// Get the distance to target - ANY XYZ HOLDS THE LENGTH
	XMVECTOR mathDistToTarget = XMVector3Length(mathToVec);
	XMFLOAT3 distToTarget; XMStoreFloat3(&distToTarget, mathDistToTarget);

	// Offset the current node
	int nNextNodeDistance = 300;


	// If i reached the node, move on the next one
	if (distToTarget.x < nNextNodeDistance && nCurrentNode >= 0)
	{
		nCurrentNode--;
		m_bRepath = true;
	}

	_agent->SetWorldVelocity(realVelocity);
	_agent->SetNodeOnPath(nCurrentNode);

}
void CMinotaurChase::Exit(CMinotaur* _agent)
{
	_agent->GetCurrAnimation().SetScale(1.0f);

}


void CMinotaurChase::BuildPath(CMinotaur* _agent)
{

	IObject* pPlayer = _agent->GetPlayer();
	int nPlayerNode = _agent->GetPathPlanner()->FindClosestNode(pPlayer->GetPosition());
	m_nLastPlayerNode = nPlayerNode;
	int nMinotaurNode = _agent->GetPathPlanner()->FindClosestNode(_agent->GetPosition());

	vector<XMFLOAT3> newPath = _agent->GetPathPlanner()->GeneratePath(nMinotaurNode, nPlayerNode, false);


	newPath.insert(newPath.begin(), *pPlayer->GetPosition());
	

	if (newPath.size() == 2)
		_agent->SetNodeOnPath(newPath.size() - 2);
	else
		_agent->SetNodeOnPath(newPath.size() - 1);

	_agent->SetPath(newPath);
}

int CMinotaurChase::UpdateState(CMinotaur* _agent)
{

	//XMFLOAT3 curPos = *_agent->GetPosition();

	//// Convert them for math
	//XMVECTOR mathPos = XMLoadFloat3(&curPos);
	//XMVECTOR mathTarget = XMLoadFloat3(_agent->GetPlayer()->GetPosition());

	//// Find the vector between the two points
	//XMVECTOR mathToVec = XMVectorSubtract(mathTarget, mathPos);
	//XMVECTOR mathDistToTarget = XMVector3Length(mathToVec);

	//if (((CPlayer*)_agent->GetPlayer())->GetHealth() <= 0)
	//{
	//	return CMinotaur::ePatrolState;
	//}

	//if (mathDistToTarget.m128_f32[0] <= 2500.0f)
	//	m_bRepath = true;
	//

	//if (mathDistToTarget.m128_f32[0] <= STOMP_DISTANCE && _agent->GetStompTimer() <= 0.0f)
	//	return CMinotaur::eStompState;

	////if (mathDistToTarget.m128_f32[0] <= MAX_CHARGE &&mathDistToTarget.m128_f32[0] >= MIN_CHARGE)
	////	return CMinotaur::eChargeState;

	return 1;

}