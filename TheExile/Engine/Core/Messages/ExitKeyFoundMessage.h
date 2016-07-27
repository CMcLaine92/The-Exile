/***********************************************
* Filename:  		ExitKeyFoundMessage.h
* Date:      		12/4/2015
* Mod. Date: 		12/4/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			The message for deactivating all exit keys after one is found
************************************************/

#ifndef CEXITKEYFOUNDMESSAGE_H
#define CEXITKEYFOUNDMESSAGE_H

#include "../Messages/Message.h"
#include "../../Object Manager/ObjectManager.h"

class CExitKeyFoundMessage : public CMessage
{
private:

	bool m_bDeActivateOthers;

public:

	CExitKeyFoundMessage(bool DeActivateOthers);
	~CExitKeyFoundMessage() = default;

	bool GetDeActivateOthers();

};

#endif
