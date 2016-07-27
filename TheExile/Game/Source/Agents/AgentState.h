/***********************************************
* Filename:  		AgentState.h
* Date:      		08/18/2015
* Mod. Date: 		----------
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:   		Interface class for the derived states
************************************************/
#ifndef _AGENT_STATE_H
#define _AGENT_STATE_H

#include "../../../Engine/Object Manager/IObject.h"

template <class Agent>
class IAgentState
{
public:
	 virtual void Enter(Agent* _agent) = 0;
	 virtual void Execute(Agent* _agent) = 0;
	 virtual void Exit(Agent* _agent) = 0;
	 virtual int UpdateState(Agent* _agent) = 0;


	IAgentState() = default;
	virtual ~IAgentState() {};

};





#endif