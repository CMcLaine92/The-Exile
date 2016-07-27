/***********************************************
* Filename:  		OrcBombThrow.h
* Date:      		09/01/2015
* Mod. Date: 		----------
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:   		Derived state to control the agent behavior
*					when able to throw a bomb.
***********************************************/

#ifndef _ORC_BOMBTHROW_H
#define _ORC_BOMBTHROW_H

#include "../AgentState.h"

class COrc;

class COrcBombThrow : public IAgentState < COrc >
{




public:
	void Enter(COrc* _agent);
	void Execute(COrc* _agent);
	void Exit(COrc* _agent);
	int UpdateState(COrc* _agent);

};



#endif