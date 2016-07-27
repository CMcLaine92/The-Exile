/***********************************************
* Filename:  		OrcHurt.h
* Date:      		09/01/2015
* Mod. Date: 		----------
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:   		Derived state to control the agent behavior
*					when damage is taken
************************************************/

#ifndef _ORC_HURT_H
#define _ORC_HURT_H

#include "../AgentState.h"

class COrc;

class COrcHurt : public IAgentState < COrc >
{




public:
	void Enter(COrc* _agent);
	void Execute(COrc* _agent);
	void Exit(COrc* _agent);
	int UpdateState(COrc* _agent);

};



#endif