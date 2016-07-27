/***********************************************
* Filename:  		MinotaurChase.h
* Date:      		09/08/2015
* Mod. Date: 		----------
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:   		Derived state to control the agent behavior
*					when chasing the player.
***********************************************/

#ifndef _MINOTAUR_CHASE_H
#define _MINOTAUR_CHASE_H

#define MINO_MOVE_SPEED 700.0f
//#define PATH_TIMER 5.0f
#define STOMP_DISTANCE 500.0f
#define MIN_CHARGE 750.0f
#define MAX_CHARGE 1500.0f



#include "../AgentState.h"

class CMinotaur;

class CMinotaurChase : public IAgentState < CMinotaur >
{


	float m_fRepathTimer;
public:
	void Enter(CMinotaur* _agent);
	void Execute(CMinotaur* _agent);
	void Exit(CMinotaur* _agent);
	int UpdateState(CMinotaur* _agent);
	bool m_bRepath;

private:

	int m_nLastPlayerNode;
	void BuildPath(CMinotaur* _agent);

};



#endif