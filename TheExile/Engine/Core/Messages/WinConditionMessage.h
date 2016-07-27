/***********************************************
* Filename:  		WinConditionMessage.h
* Date:      		8/21/2015
* Mod. Date: 		8/21/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			The message for the win condition
************************************************/

#ifndef CWINCONDITIONMESSAGE_H
#define CWINCONDITIONMESSAGE_H


#include "../Messages/Message.h"


class CWinConditionMessage : public CMessage
{
private:

	bool m_bGameOver;

public:

	/// <summary>
	/// Constructor for EnemySpawnMessage
	/// </summary>
	/// <param> none</param>
	CWinConditionMessage(bool gameOver);

	/// <summary>
	/// Default destructor for EnemySpawnMessage
	/// </summary>
	/// <param> none</param>
	~CWinConditionMessage() = default;

};

#endif