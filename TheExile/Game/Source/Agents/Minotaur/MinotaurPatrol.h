/***********************************************
* Filename:  		MinotaurPatrol.h
* Date:      		09/01/2015
* Mod. Date: 		----------
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:   		Derived state to control the agent behavior
*					when looking for player.
***********************************************/

#ifndef _MINOTAUR_PATROL_H
#define _MINOTAUR_PATROL_H
#define MOVE_SPEED 300.0f
#define PATH_TIMER 2.5f
#include "../AgentState.h"

class CMinotaur;


class CMinotaurPatrol : public IAgentState < CMinotaur >
{

	enum ESubState { MOVING, WAITING, ENRAGE };

	// Exit timer
	float m_fExitTimer;
	int m_nCurrWaypointIndex;
	int m_nCurrWaypoint;

	ESubState m_eCurrState;
	ESubState m_eNewState;
	
	float m_fRayCastTimer = 1.0f;


public:
	void Enter(CMinotaur* _agent);
	void Execute(CMinotaur* _agent);
	void Exit(CMinotaur* _agent);
	int UpdateState(CMinotaur* _agent);
private:

	ESubState Moving(CMinotaur* _agent);
	ESubState Waiting(CMinotaur* _agent);
	ESubState Roaring(CMinotaur* _agent);
	void CheckState(CMinotaur* _agent);

	void EnterSubState(CMinotaur* _agent);
	void ExitSubState(CMinotaur* _agent);
	void AdjustRotation(CMinotaur* _agent);



	//SubStates



	bool NearlyEqual(float a, float b);

	void BuildPath( CMinotaur* _agent);

};



#endif