/***********************************************
* Filename:  		MinotaurIdle.h
* Date:      		09/01/2015
* Mod. Date: 		----------
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:   		Derived state to control the agent behavior
*					when idle.
***********************************************/

#ifndef _MINOTAUR_IDLE_H
#define _MINOTAUR_IDLE_H

#include "../AgentState.h"

#define WAIT_TIME 4.0f

class CMinotaur;

class CMinotaurIdle : public IAgentState < CMinotaur >
{


	float m_fWaitTimer;

public:
	void Enter(CMinotaur* _agent);
	void Execute(CMinotaur* _agent);
	void Exit(CMinotaur* _agent);
	int UpdateState(CMinotaur* _agent);

};



#endif