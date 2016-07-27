/***********************************************
* Filename:  		OrcFollow.h
* Date:      		09/01/2015
* Mod. Date: 		----------
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:   		Derived state to control the agent behavior
*					when player is found but not close enough to 
*					attack.
************************************************/

#ifndef _ORC_FOLLOW_H
#define _ORC_FOLLOW_H

#include "../AgentState.h"

class COrc;

class COrcFollow : public IAgentState < COrc >
{




public:
	void Enter(COrc* _agent);
	void Execute(COrc* _agent);
	void Exit(COrc* _agent);
	int UpdateState(COrc* _agent);

};



#endif