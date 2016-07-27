/***********************************************
* Filename:  		MinotaurPatrol.h
* Date:      		09/01/2015
* Mod. Date: 		----------
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:   		Derived state to control the agent behavior
*					when looking for player.
***********************************************/

#ifndef _MINOTAUR_HOSTILE_H
#define _MINOTAUR_HOSTILE_H
#define MOVE_SPEED 300.0f
#define PATH_TIMER 2.5f
#include "../AgentState.h"

class CMinotaur;


class CMinotaurHostile : public IAgentState < CMinotaur >
{

	enum ESubState { FOLLOW, STOMP, ENRAGED, CHARGE, STUNNED, };

	// Exit timer
	XMFLOAT3 vChargeVector;
	float m_fExitTimer;
	float m_fChargeDistance;
	float m_fChargeRecast;
	int m_nCurrWaypointIndex;
	int m_nCurrWaypoint;
	bool m_bTell;
	bool m_bLockedVelocity = false;

	ESubState m_eCurrState;
	ESubState m_eNewState;
	ESubState m_ePrevState;


	

public:
	void Enter(CMinotaur* _agent);
	void Execute(CMinotaur* _agent);
	void Exit(CMinotaur* _agent);
	int UpdateState(CMinotaur* _agent);

private:
	ESubState Stunned(CMinotaur* _agent);
	ESubState Charge(CMinotaur* _agent);
	ESubState Enraged(CMinotaur* _agent);
	ESubState Follow(CMinotaur* _agent);
	ESubState Stomp(CMinotaur* _agent);

	void CheckState(CMinotaur* _agent);
	void AdjustRotation(CMinotaur* _agent);

	void EnterSubState(CMinotaur* _agent);
	void ExitSubState(CMinotaur* _agent);
	void BuildPath(CMinotaur* _agent);

	XMFLOAT3 chargeDest;

	//SubStates





	bool NearlyEqual(float a, float b);
	bool m_bPlayerLost = true;
	
};



#endif 