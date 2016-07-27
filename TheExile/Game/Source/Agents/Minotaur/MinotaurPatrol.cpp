
#include "MinotaurPatrol.h"
#include "Minotaur.h"
#include "../../../../Animation/Animation.h"
#include "../../../../Engine/Core/Collision/Capsule.h"
#include "../../../../Engine/Core/Collision/Collider.h"
#include "../../../../Engine/Object Manager/ObjectManager.h"
#include "../../../../Engine/Object Manager/QuadTree.h"
#define XMF3DotProduct(a,b) (a.x*b.x + a.y*b.y + a.z*b.z)

void CMinotaurPatrol::Enter(CMinotaur* _agent)
{
	m_nCurrWaypointIndex = 0;
	m_nCurrWaypoint = _agent->GetWaypoint(m_nCurrWaypointIndex);

	_agent->SetMoveSpeed(MOVE_SPEED);
	_agent->GetCurrAnimation().SetScale(0.75f);

	m_eNewState = ENRAGE;
	m_eCurrState = ENRAGE;
	EnterSubState(_agent);





}

bool CMinotaurPatrol::NearlyEqual(float a, float b)
{
	return (a < b + 0.01f) && (a > b - 0.01f);
}
void CMinotaurPatrol::Execute(CMinotaur* _agent)
{
	AdjustRotation(_agent);

	CheckState(_agent);

	switch (m_eCurrState)
	{
	case MOVING:
		m_eNewState = Moving(_agent);
		break;
	case WAITING:
		m_eNewState = Waiting(_agent);
		break;
	case ENRAGE:
		m_eNewState = Roaring(_agent);
		break;
	default:
		break;
	}

}

void CMinotaurPatrol::BuildPath(CMinotaur* _agent)
{
	
	
	int nMinotaurNode = _agent->GetPathPlanner()->FindClosestNode(_agent->GetPosition());
	vector<XMFLOAT3> newPath;
	if (nMinotaurNode == m_nCurrWaypoint)
	{
		newPath.push_back(*(_agent->GetPosition()));
		newPath.push_back(*(_agent->GetPlayer()->GetPosition()));
	}
	else
	{
		newPath = _agent->GetPathPlanner()->GeneratePath(nMinotaurNode, m_nCurrWaypoint, false);

	}


	_agent->SetPath(newPath);
	_agent->SetNodeOnPath(newPath.size() - 1);
}

void CMinotaurPatrol::Exit(CMinotaur* _agent)
{
	_agent->GetCurrAnimation().SetScale(1.0f);


}
void CMinotaurPatrol::AdjustRotation(CMinotaur* _agent)
{
	if (_agent->GetPath().size() > 0)
	{
		XMFLOAT3 up = XMFLOAT3(0, 1, 0);
		XMVECTOR newX, newY, newZ, globalY;
		globalY = XMLoadFloat3(&up);

		int m_currNode = _agent->GetCurNodeOnPath();
		XMVECTOR targetLocation;
		
		targetLocation = XMLoadFloat3(&_agent->GetPath()[m_currNode]);
		

		XMVECTOR currentLocation = XMLoadFloat3(_agent->GetPosition());
		currentLocation.m128_f32[2] += 1.0f;
		newZ = targetLocation - currentLocation;
		newX = XMVector3Cross(globalY, newZ);
		newY = XMVector3Cross(newZ, newX);

		newX = XMVector3Normalize(newX);
		newY = XMVector3Normalize(newY);
		newZ = XMVector3Normalize(newZ);

		XMFLOAT3 xAxis, yAxis, zAxis;
		XMStoreFloat3(&xAxis, newX);
		XMStoreFloat3(&yAxis, newY);
		XMStoreFloat3(&zAxis, newZ);

		xAxis.y = 0.0f;
		yAxis.x = 0.0f;
		yAxis.z = 0.0f;
		zAxis.y = 0.0f;

		_agent->SetXAxis(xAxis);
		_agent->SetYAxis(yAxis);
		_agent->SetZAxis(zAxis);
	}

}
int CMinotaurPatrol::UpdateState(CMinotaur* _agent)
{
	XMFLOAT3 curPos = *_agent->GetPosition();

	// Convert them for math
	XMVECTOR mathPos = XMLoadFloat3(&curPos);
	XMVECTOR mathTarget = XMLoadFloat3(_agent->GetPlayer()->GetPosition());
	XMFLOAT3 start, end;
	XMStoreFloat3(&start, mathPos);
	XMStoreFloat3(&end, mathTarget);
	start.y = 75.0f;
	end.y = 75.0f;

	// Find the vector between the two points
	XMVECTOR mathToVec = XMVectorSubtract(mathTarget, mathPos);
	XMVECTOR mathDistToTarget = XMVector3Length(mathToVec);
	float fHitTime = 0;
	m_fRayCastTimer -= DELTA_TIME();
	if (m_fRayCastTimer < 0.0f)
	{
		m_fRayCastTimer = 1.0f;
		if (_agent->RayCastToPlayer(fHitTime) == false)
		{
			cout << "Can See Player\n";
			return CMinotaur::eHostileState;
		}
	}
	
	return CMinotaur::ePatrolState;
}


CMinotaurPatrol::ESubState CMinotaurPatrol::Moving(CMinotaur* _agent)
{
	//Reset Velocity
	_agent->SetWorldVelocity({ 0, 0, 0 });
	if (NearlyEqual(_agent->GetCurrAnimation().GetCurrTime(), 0.05f) || NearlyEqual(_agent->GetCurrAnimation().GetCurrTime(), 0.5f))
	{
		//AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYMINORUN, *_agent->GetPosition());
		//AudioSystemWwise::Get()->PostEvent(AK::EVENTS::PLAYMINORUN, *_agent->GetPosition());
		_agent->PlayStepParticle();
	}



	// Node that I'm on along the path
	int nCurrentNode = _agent->GetCurNodeOnPath();

	//Check if path needs to be rebuilt
	

	// Get the current path
	vector<XMFLOAT3>& vPath = _agent->GetPath();


	// Get the current velocity
	//XMVECTOR currentVelocity; XMLoadFloat3(&m_f3vel);

	// Node that I'm on along the path
	nCurrentNode = _agent->GetCurNodeOnPath();

	// If no path was generated, then return
	if (vPath.size() <= 0)
		return ENRAGE;

	// Get the current positions
	XMFLOAT3 curPos = *_agent->GetPosition();
	XMFLOAT3 targetPos = vPath[nCurrentNode];

	// Convert them for math
	XMVECTOR mathPos = XMLoadFloat3(&curPos);
	XMVECTOR mathTarget = XMLoadFloat3(&targetPos);

	// Find the vector between the two points
	XMVECTOR mathToVec = XMVectorSubtract(mathTarget, mathPos);
	XMVECTOR mathDistToTarget = XMVector3Length(mathToVec);


	//XMFLOAT3 toVec; XMStoreFloat3(&toVec, mathToVec);

	// Normalize the toVector to get the direction
	XMVECTOR mathVelocity = XMVector3Normalize(mathToVec);

	mathVelocity *= _agent->GetMoveSpeed();


	XMFLOAT3 realVelocity; XMStoreFloat3(&realVelocity, mathVelocity);
	//realVelocity.y = 0.0f;

	// Get the distance to target - ANY XYZ HOLDS THE LENGTH
	XMFLOAT3 distToTarget; XMStoreFloat3(&distToTarget, mathDistToTarget);

	// Offset the current node
	int nOffsetNode = 1;
	int nNextNodeDistance = 250;



	// If i reached the node, move on the next one
	if (distToTarget.x < nNextNodeDistance && nCurrentNode >= 0)
	{
		nCurrentNode -= nOffsetNode;
		if (nCurrentNode < 0)
		{
			nCurrentNode = 0;
			m_nCurrWaypointIndex++;
			if (m_nCurrWaypointIndex > 2)
				m_nCurrWaypointIndex = 0;
			m_nCurrWaypoint = _agent->GetWaypoint(m_nCurrWaypointIndex);
			//BuildPath(_agent);
			return WAITING;

		}
	}

	_agent->SetWorldVelocity(realVelocity);
	_agent->SetNodeOnPath(nCurrentNode);
	return MOVING;
}
CMinotaurPatrol::ESubState CMinotaurPatrol::Waiting(CMinotaur* _agent)
{
	_agent->SetWorldVelocity({ 0, 0, 0 });
	m_fExitTimer -= DELTA_TIME();
	if (m_fExitTimer < 0.0f)
	{
		int chance = rand() % 2;
		if (chance == 0)
			return ENRAGE;
		return MOVING;
	}
	return WAITING;
}
CMinotaurPatrol::ESubState CMinotaurPatrol::Roaring(CMinotaur* _agent)
{
	XMVECTOR vToPlayer = XMLoadFloat3(_agent->GetPlayer()->GetPosition());
	XMVECTOR vSkeleton = XMLoadFloat3(_agent->GetPosition());
	vToPlayer = vToPlayer - vSkeleton;
	float dist = XMVector3Length(vToPlayer).m128_f32[0];

	_agent->SetWorldVelocity({ 0, 0, 0 });
	m_fExitTimer -= DELTA_TIME();
	if (m_fExitTimer < 0.0f)
	{
		if (dist < 1000.0f)
			return  ENRAGE;
		return MOVING;
	}
		
	
	return ENRAGE;

}
void CMinotaurPatrol::CheckState(CMinotaur* _agent)
{
	/*XMVECTOR vToPlayer = XMLoadFloat3(_agent->GetPlayer()->GetPosition());
	XMVECTOR vSkeleton = XMLoadFloat3(_agent->GetPosition());
	vToPlayer = vToPlayer - vSkeleton;
	float dist = XMVector3Length(vToPlayer).m128_f32[0];*/

	

	if (m_eNewState != m_eCurrState)
	{
		ExitSubState(_agent);
		m_eCurrState = m_eNewState;
		EnterSubState(_agent);
	}
}

void CMinotaurPatrol::EnterSubState(CMinotaur* _agent)
{
	switch (m_eCurrState)
	{
	case MOVING:
	{
		_agent->SwitchAnimation(CMinotaur::eRunAnim);
		_agent->GetCurrAnimation().SetScale(0.75f);
		BuildPath(_agent);
		
	}
	break;
	case CMinotaurPatrol::WAITING:
		_agent->SwitchAnimation(CMinotaur::eIdleAnim);
		m_fExitTimer = 3.0f;
		
		break;
	case CMinotaurPatrol::ENRAGE:
	{
		const XMFLOAT3 pos = *_agent->GetPosition();
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::MINOALERT, pos);
		_agent->SwitchAnimation(CMinotaur::eEnrageAnim);
		// Set the timer
		m_fExitTimer = _agent->GetCurrAnimation().GetDuration();
		
	}
	break;
	default:
		break;
	}
}
void CMinotaurPatrol::ExitSubState(CMinotaur* _agent)
{
	switch (m_eCurrState)
	{
	case MOVING:
		m_fExitTimer = 0.0f;
	break;
	case CMinotaurPatrol::WAITING:
		m_fExitTimer = 0.0f;
		break;
	case CMinotaurPatrol::ENRAGE:
		m_fExitTimer = 0.0f;
	break;
	default:
		break;
	}
}


