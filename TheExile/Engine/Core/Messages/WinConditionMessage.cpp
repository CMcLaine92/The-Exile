/***********************************************
* Filename:  		WinConditionMessage.h
* Date:      		8/21/2015
* Mod. Date: 		8/21/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			The message for the win condition
************************************************/

#include "WinConditionMessage.h"

CWinConditionMessage::CWinConditionMessage(bool gameOver) : CMessage(MessageID::MSG_WIN_GAME)
{
	m_bGameOver = gameOver;
}
