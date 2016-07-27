/***********************************************
* Filename:  		MinotaurStomp.h
* Date:      		09/01/2015
* Mod. Date: 		----------
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:   		Derived state to control the agent behavior
*					when close enough to stomp.
***********************************************/

#ifndef _MINOTAUR_STOMP_H
#define _MINOTAUR_STOMP_H
#define STOMP_AOE 300.0f

#include "../AgentState.h"

class CMinotaur;

class CMinotaurStomp : public IAgentState < CMinotaur >
{

	float m_fStateDuration;
	float m_fStompStart;
	bool m_bPlayerDead;


public:
	void Enter(CMinotaur* _agent);
	void Execute(CMinotaur* _agent);
	void Exit(CMinotaur* _agent);
	int UpdateState(CMinotaur* _agent);

};



#endif