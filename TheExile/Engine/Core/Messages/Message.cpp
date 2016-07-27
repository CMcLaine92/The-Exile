/***********************************************
* Filename:  		Message.cpp
* Date:      		8/10/2015
* Mod. Date: 		8/10/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Holds information to be passed 
*					through the MessageManager to 
*					the messageProc
************************************************/

#include "Message.h"
#include "MessageManager.h"


/*****************************************************************
* CMessage()			Overloaded constructor that sets the message id
*
* Ins:					id
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    08/10/2015
* Mod. Initials:	    NH
*****************************************************************/
CMessage::CMessage(MessageID id)
{
	m_eMessageID = id;
}

/*****************************************************************
* Send()				Sends this message to the message proc
*
* Ins:					None
*
* Outs:					None
*
* Returns:				bool
*
* Mod. Date:		    08/10/2015
* Mod. Initials:	    NH
*****************************************************************/
bool CMessage::Send() const
{
	return CMessageManager::GetInstance()->Send(this);
}

/*****************************************************************
* GetMessageID()		Returns the message id of this message
*
* Ins:					None
*
* Outs:					None
*
* Returns:				bool
*
* Mod. Date:		    08/10/2015
* Mod. Initials:	    NH
*****************************************************************/
MessageID CMessage::GetMessageID() const
{
	return m_eMessageID;
}
