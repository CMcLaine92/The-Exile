/***********************************************
* Filename:  		MinotaurCharge.h
* Date:      		09/01/2015
* Mod. Date: 		----------
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:   		Derived state to control the agent behavior
*					when idle.
***********************************************/

#ifndef _MINOTAUR_HURT_H
#define _MINOTAUR_HURT_H

#include "../AgentState.h"

class CMinotaur;

class CMinotaurHurt : public IAgentState < CMinotaur >
{




public:
	void Enter(CMinotaur* _agent);
	void Execute(CMinotaur* _agent);
	void Exit(CMinotaur* _agent);
	int UpdateState(CMinotaur* _agent);

};



#endif