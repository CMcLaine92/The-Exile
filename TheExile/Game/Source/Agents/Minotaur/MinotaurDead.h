/***********************************************
* Filename:  		MinotaurDead.h
* Date:      		09/10/2015
* Mod. Date: 		----------
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:   		Derived state to control the agent behavior
*					when dying.
***********************************************/

#ifndef _MINOTAUR_DEAD_H
#define _MINOTAUR_DEAD_H

#include "../AgentState.h"

class CMinotaur;

class CMinotaurDead : public IAgentState < CMinotaur >
{

	float m_fStateDuration;
	float m_fStateTime;
public:
	void Enter(CMinotaur* _agent);
	void Execute(CMinotaur* _agent);
	void Exit(CMinotaur* _agent);
	int UpdateState(CMinotaur* _agent);
private:


};



#endif