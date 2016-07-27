/***********************************************
* Filename:  		OrcMelee.h
* Date:      		09/01/2015
* Mod. Date: 		----------
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:   		Derived state to control the agent behavior
*					when the player is in range and the Agent
*					attacks the player.
************************************************/

#ifndef _ORC_MELEE_H
#define _ORC_MELEE_H

#include "../AgentState.h"

class COrc;
class CAttack;

class COrcMelee : public IAgentState < COrc >
{


	// Exit timer
	float m_fExitTimer;

	// Attacking bool
	bool m_bAttacking = false;

public:
	void Enter(COrc* _agent);
	void Execute(COrc* _agent);
	void Exit(COrc* _agent);
	int UpdateState(COrc* _agent);

};



#endif