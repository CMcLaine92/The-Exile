/***********************************************
* Filename:  		ExitKeyFoundMessage.h
* Date:      		12/4/2015
* Mod. Date: 		12/4/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			The message for deactivating all exit keys after one is found
************************************************/

#include "ExitKeyFoundMessage.h"

CExitKeyFoundMessage::CExitKeyFoundMessage(bool DeActivateOthers) : CMessage(MessageID::MSG_EXITKEY_FOUND)
{
	m_bDeActivateOthers = DeActivateOthers;
}

bool CExitKeyFoundMessage::GetDeActivateOthers()
{
	return m_bDeActivateOthers;
}
