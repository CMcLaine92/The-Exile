/***********************************************
* Filename:  		IMessageManager.cpp
* Date:      		8/10/2015
* Mod. Date: 		8/10/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Manages all messages for use by
*					the registered message proc
************************************************/

#include "MessageManager.h"
#include "Message.h"
#include "RemoveObjectMessage.h"
CMessageManager* CMessageManager::m_cpInstance;


/*****************************************************************
* GetInstance()			Returns the only instance of the MessageManager
*
* Ins:					None
*
* Outs:					None
*
* Returns:				IMessageManager*
*
* Mod. Date:		    08/10/2015
* Mod. Initials:	    NH
*****************************************************************/
CMessageManager* CMessageManager::GetInstance()
{
	if (m_cpInstance == nullptr)
	{
		m_cpInstance = new CMessageManager;
	}

	return m_cpInstance;
}

/*****************************************************************
* Initialize()			Initializes the MessageManager
*
* Ins:					fpMessageProc
*
* Outs:					None
*
* Returns:				bool
*
* Mod. Date:		    08/10/2015
* Mod. Initials:	    NH
*****************************************************************/
bool CMessageManager::Initialize(void(*MessageProc)(const CMessage*))
{
	if (MessageProc == nullptr)
	{
		return false;
	}

	m_pMessageProc = MessageProc;
	return true;
}

/*****************************************************************
* Shutdown()			Shuts down the MessageManager
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
bool CMessageManager::Shutdown()
{
	Update();
	m_pMessageProc = nullptr;
	delete m_cpInstance;
	m_cpInstance = nullptr;

	return true;
}

/*****************************************************************
* Send()				Sends a message to the message proc
*
* Ins:					pMsg
*
* Outs:					None
*
* Returns:				bool
*
* Mod. Date:		    08/10/2015
* Mod. Initials:	    NH
*****************************************************************/
bool CMessageManager::Send(const CMessage* pMsg)
{
	if (pMsg)
	{
		m_cMessages.push(pMsg);
		return true;
	}

	return false;
}

void CMessageManager::Update()
{
	while (m_cMessages.size() > 0)
	{
		const CMessage * pMsg = m_cMessages.front();
		m_cMessages.pop();

		if (pMsg)
		{
			(*m_pMessageProc)(pMsg);
		}
		delete pMsg;
	}

}
