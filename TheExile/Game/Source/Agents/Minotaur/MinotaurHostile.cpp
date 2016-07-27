#include "MinotaurHostile.h"
#include "Minotaur.h"
#include "../../../../Animation/Animation.h"
#define CHARGE_RECAST_TIME 0.25f
#define CHARGE_SPEED 3000.0f

void CMinotaurHostile::Enter(CMinotaur* _agent)
{
	m_nCurrWaypointIndex = 0;
	m_nCurrWaypoint = _agent->GetWaypoint(m_nCurrWaypointIndex);

	_agent->SetMoveSpeed(MOVE_SPEED);
	_agent->GetCurrAnimation().SetScale(0.75f);
	vChargeVector = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_eNewState = FOLLOW;
	m_eCurrState = FOLLOW;
	EnterSubState(_agent);


}
void CMinotaurHostile::Exit(CMinotaur* _agent)
{
	_agent->GetCurrAnimation().SetScale(1.0f);

	m_fExitTimer = 0.0f;
}
bool CMinotaurHostile::NearlyEqual(float a, float b)
{
	return (a < b + 0.01f) && (a > b - 0.01f);
}
int CMinotaurHostile::UpdateState(CMinotaur* _agent)
{
	XMFLOAT3 curPos = *_agent->GetPosition();

	// Convert them for math
	XMVECTOR mathPos = XMLoadFloat3(&curPos);
	XMVECTOR mathTarget = XMLoadFloat3(_agent->GetPlayer()->GetPosition());

	// Find the vector between the two points
	XMVECTOR mathToVec = XMVectorSubtract(mathTarget, mathPos);
	XMVECTOR mathDistToTarget = XMVector3Length(mathToVec);
	
	/*if (m_bPlayerLost == true)
		return CMinotaur::ePatrolState;*/
	
	return CMinotaur::eHostileState;

}
void CMinotaurHostile::Execute(CMinotaur* _agent)
{
	AdjustRotation(_agent);
	CheckState(_agent);

	switch (m_eCurrState)
	{
	case FOLLOW:
		m_eNewState = Follow(_agent);
		break;
	case STOMP:
		m_eNewState = Stomp(_agent);
		break;
	case ENRAGED:
		m_eNewState = Enraged(_agent);
		break;
	case CHARGE:
		m_eNewState = Charge(_agent);
		break;
	case STUNNED:
		m_eNewState = Stunned(_agent);
		break;
	default:
		break;
	}

}



CMinotaurHostile::ESubState CMinotaurHostile::Stunned(CMinotaur* _agent)
{
	m_fExitTimer -= DELTA_TIME();
	_agent->SetWorldVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));

	if (m_fExitTimer < 0.0f)
	{
		return ENRAGED;
	}
	return STUNNED;
}
CMinotaurHostile::ESubState CMinotaurHostile::Charge(CMinotaur* _agent)
{
	m_fExitTimer -= DELTA_TIME();
	_agent->GetCamera()->Shake(0.5f);

	if (m_fExitTimer < 0.0f && m_bTell == false)
	{
		_agent->SetWorldVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		m_bTell = true;
		return STUNNED;
	}
	
	if (m_bTell)
	{
		_agent->SetWorldVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));

		if (m_fExitTimer  < 0.0f)
		{
			m_bTell = false;
			_agent->SwitchAnimation(CMinotaur::eChargeAnim);
			_agent->GetCurrAnimation().SetScale(1.50f);
			m_fExitTimer = 1.5f;
		}
	}
	else
	{
		if (NearlyEqual(_agent->GetCurrAnimation().GetCurrTime(), 0.05f) || NearlyEqual(_agent->GetCurrAnimation().GetCurrTime(), 0.5f))
			_agent->PlayStepParticle();
		
		m_fChargeRecast -= DELTA_TIME();

		if (m_fChargeRecast < 0.0f)
		{
			m_fChargeRecast = CHARGE_RECAST_TIME;
			float fHitTime = 0.0f;

			XMFLOAT3 f3HitLocation;
			if (_agent->RayCastInfinite(fHitTime,f3HitLocation))
			{
				chargeDest = f3HitLocation;
				cout << "HitLocation is new Destination \n";
				m_fChargeRecast = 10.0f;
			}
			else
			{
				chargeDest = *_agent->GetPlayer()->GetPosition();
			}
			
		}
		XMVECTOR vToPlayer = XMLoadFloat3(&chargeDest);
		XMVECTOR vSkeleton = XMLoadFloat3(_agent->GetPosition());
		vToPlayer = vToPlayer - vSkeleton;
		vToPlayer.m128_f32[1] = 0.0f;
		float dist = XMVector3Length(vToPlayer).m128_f32[0];
		vToPlayer = XMVector3Normalize(vToPlayer);
		vToPlayer *= CHARGE_SPEED;
		XMStoreFloat3(&vChargeVector, vToPlayer);
		float fHitTime = 0.0f;
		if (_agent->RayCastForward(fHitTime))
		{
			if (fHitTime < 225.0f)
			{
				m_bTell = true;
				return STUNNED;
			}
		}
		
		_agent->SetWorldVelocity(vChargeVector);
	}



	
	

	return CHARGE;
}
CMinotaurHostile::ESubState CMinotaurHostile::Enraged(CMinotaur* _agent)
{
	_agent->SetWorldVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));

	m_fExitTimer -= DELTA_TIME();
	if (m_fExitTimer < 0.0f)
	{
		XMVECTOR vToPlayer = XMLoadFloat3(_agent->GetPlayer()->GetPosition());
		XMVECTOR vSkeleton = XMLoadFloat3(_agent->GetPosition());
		vToPlayer = vToPlayer - vSkeleton;
		float dist = XMVector3Length(vToPlayer).m128_f32[0];

		float fHitTime = 0.0f;
		if (_agent->RayCastToPlayer(fHitTime) == false)
		{
			return CHARGE;
		}
		else
		{
			BuildPath(_agent);
			return FOLLOW;
		}
		if (m_ePrevState == STUNNED)
		{
			BuildPath(_agent);

			return FOLLOW;
		}
		//else if (m_ePrevState == FOLLOW)
		//{
		//	return CHARGE;
		//}
		
		
		
	}
	return ENRAGED;
}
CMinotaurHostile::ESubState CMinotaurHostile::Follow(CMinotaur* _agent)
{
	//Reset Velocity
	_agent->SetWorldVelocity({ 0, 0, 0 });
	if (NearlyEqual(_agent->GetCurrAnimation().GetCurrTime(), 0.05f) || NearlyEqual(_agent->GetCurrAnimation().GetCurrTime(), 0.5f))
	{
		
		_agent->PlayStepParticle();
	}



	XMVECTOR vToPlayer = XMLoadFloat3(_agent->GetPlayer()->GetPosition());
	XMVECTOR vSkeleton = XMLoadFloat3(_agent->GetPosition());
	vToPlayer = vToPlayer - vSkeleton;
	float dist = XMVector3Length(vToPlayer).m128_f32[0];
	
	//if (dist < 3000.0f)
	float fHitTime =0;
	if (_agent->RayCastToPlayer(fHitTime) == false)
	{
		return ENRAGED;
	}
	// Node that I'm on along the path
	int nCurrentNode = _agent->GetCurNodeOnPath();
	//BuildPath(_agent);

	//Check if path needs to be rebuilt
	if (nCurrentNode < 0)
	{
		
	//	m_nCurrWaypoint = _agent->GetWaypoint(m_nCurrWaypointIndex);
		BuildPath(_agent);
		return FOLLOW;

	}

	// Get the current path
	vector<XMFLOAT3>& vPath = _agent->GetPath();

	/*if (vPath.size() > 10)
	{
		m_bPlayerLost = true;
		return FOLLOW;
	}*/

	// Get the current velocity
	//XMVECTOR currentVelocity; XMLoadFloat3(&m_f3vel);

	// Node that I'm on along the path
	nCurrentNode = _agent->GetCurNodeOnPath();

	// If no path was generated, then return
	if (vPath.size() <= 0)
		return ENRAGED;

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
	}

	_agent->SetWorldVelocity(realVelocity);
	_agent->SetNodeOnPath(nCurrentNode);
	return FOLLOW;
}
CMinotaurHostile::ESubState CMinotaurHostile::Stomp(CMinotaur* _agent)
{
	_agent->SetWorldVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));

	(XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_fExitTimer -= DELTA_TIME();
	if (m_fExitTimer < 0.0f)
	{
		return FOLLOW;
	}
	return STOMP;
}

void  CMinotaurHostile::CheckState(CMinotaur* _agent)
{
	XMVECTOR vToPlayer = XMLoadFloat3(_agent->GetPlayer()->GetPosition());
	XMVECTOR vSkeleton = XMLoadFloat3(_agent->GetPosition());
	vToPlayer = vToPlayer - vSkeleton;
	float dist = XMVector3Length(vToPlayer).m128_f32[0];
	
	

	if (m_eNewState != m_eCurrState)
	{
		ExitSubState(_agent);
		m_ePrevState = m_eCurrState;
		m_eCurrState = m_eNewState;
		EnterSubState(_agent);
	}
}

void CMinotaurHostile::AdjustRotation(CMinotaur* _agent)
{
	if (m_eCurrState == STUNNED || m_eNewState == STUNNED)
	{
		return;
	}
	if (_agent->GetPath().size() > 0)
	{
		XMFLOAT3 up = XMFLOAT3(0, 1, 0);
		XMVECTOR newX, newY, newZ, globalY;
		globalY = XMLoadFloat3(&up);

		int m_currNode = _agent->GetCurNodeOnPath();
		XMVECTOR targetLocation;
		if (m_eCurrState == FOLLOW && m_currNode >= 0)
		{
			targetLocation = XMLoadFloat3(&_agent->GetPath()[m_currNode]);

		}
		else if (m_eCurrState == CHARGE)
		{
			targetLocation = XMLoadFloat3(&chargeDest);
		}
		else
			targetLocation = XMLoadFloat3(_agent->GetPlayer()->GetPosition());



		XMVECTOR currentLocation = XMLoadFloat3(_agent->GetPosition());

		newZ = targetLocation - currentLocation;
		newZ.m128_f32[2] += .001f;
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
void  CMinotaurHostile::EnterSubState(CMinotaur* _agent)
{
	const XMFLOAT3 pos = *_agent->GetPosition();
	

	switch (m_eCurrState)
	{
	case FOLLOW:
		cout << "Follow\n";
		_agent->SwitchAnimation(CMinotaur::eRunAnim);
		_agent->GetCurrAnimation().SetScale(1.0f);
		BuildPath(_agent);
		break;
	case STOMP:
		_agent->SwitchAnimation(CMinotaur::eStompAnim);
		_agent->GetCurrAnimation().SetScale(0.75f);
		m_fExitTimer = _agent->GetCurrAnimation().GetDuration();
		cout << "Stomp\n";
		break;
	case ENRAGED:
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::MINOALERT, pos);
		_agent->SwitchAnimation(CMinotaur::eEnrageAnim);
		cout << "Enraged\n";
		// Set the timer
		m_fExitTimer = _agent->GetCurrAnimation().GetDuration();
		break;
		
	case CHARGE:
		m_bTell = true;
		
		chargeDest = *_agent->GetPlayer()->GetPosition();
		_agent->PlayChargeParticle();
		cout << "Charge\n";
		_agent->SwitchAnimation(CMinotaur::eTellAnim);
		m_fExitTimer = _agent->GetCurrAnimation().GetDuration();

		m_fChargeRecast = 0.0f;
		_agent->GetCamera()->Shake(5.0f);
		break;
	case STUNNED:
		_agent->SwitchAnimation(CMinotaur::eStunnedAnim);
		m_fExitTimer = 3.0f;
		cout << "Stunned\n";
		_agent->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Textures/StunTexture.dds");
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::MINOWALLHIT, pos);
		break;
	default:
		break;
	}

}
void  CMinotaurHostile::ExitSubState(CMinotaur* _agent)
{
	switch (m_eCurrState)
	{
	case FOLLOW:
		_agent->GetCurrAnimation().SetScale(1.0f);
		m_fExitTimer = 0.0f;

		break;
	case STOMP:
		m_nCurrWaypoint = -1;

		_agent->GetCurrAnimation().SetScale(1.0f);
		m_fExitTimer = 0.0f;
		break;
	case ENRAGED:
		// Set the timer
		m_nCurrWaypoint = -1;

		m_fExitTimer = 0.0f;
		break;
	case CHARGE:
		m_nCurrWaypoint = -1;
		m_fExitTimer = 0.0f;
		_agent->GetCamera()->GetShaking() = false;
		_agent->StopChargeParticle();
		break;
	case STUNNED:
		m_fExitTimer = 0.0f;
		_agent->GetRenderMesh()->SetTexture(L"../Game/Assets/Art/2D/Textures/Minotaur.dds");
		break;
	default:
		break;
	}

}

void CMinotaurHostile::BuildPath(CMinotaur* _agent)
{
	
	IObject* pPlayer = _agent->GetPlayer();
	XMFLOAT3 playerPos = *pPlayer->GetPosition();
	

	int nStart = _agent->GetPathPlanner()->FindClosestNode(_agent->GetPosition());
	int nGoal = _agent->GetPathPlanner()->FindClosestNode(pPlayer->GetPosition());

	vector<XMFLOAT3> newPath = _agent->GetPathPlanner()->GeneratePath(nStart, nGoal, false);

	
	if (newPath.empty())
		newPath.push_back(*_agent->GetPosition());

	newPath.insert(newPath.begin(), playerPos);

	/*if (newPath.size() > 2)
	{
		XMVECTOR toPlayer = XMLoadFloat3(_agent->GetPlayer()->GetPosition()) - XMLoadFloat3(_agent->GetPosition());
		XMVECTOR toWapoint = XMLoadFloat3(&newPath[newPath.size() - 1]) - XMLoadFloat3(&newPath[newPath.size() - 2]);

		if (XMVector3Dot(toPlayer, toWapoint).m128_f32[0] < 0.0f)
		{
			newPath.erase(newPath.end() - 2);
		}
		
	}
*/


	_agent->SetNodeOnPath(newPath.size() - 1);

	_agent->SetPath(newPath);
}
