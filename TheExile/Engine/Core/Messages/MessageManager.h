/***********************************************
* Filename:  		IMessageManager.h
* Date:      		8/10/2015
* Mod. Date: 		8/10/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:   		Manages all messages for use by
*					the registered message proc
************************************************/

#ifndef IMESSAGEMANAGER_H
#define IMESSAGEMANAGER_H

#include <queue>

class CMessage;

class CMessageManager
{
private:

	CMessageManager() = default;
	virtual ~CMessageManager() = default;

	static CMessageManager* m_cpInstance;
	
	std::queue<const CMessage*> m_cMessages;

	typedef void(*fpMessageProc)(const CMessage*);
	fpMessageProc m_pMessageProc;//the message proc function pointer

public:

	/// <summary>
	/// Returns the only instane of the MessageManager.
	/// </summary>
	/// <param> none</param>
	static CMessageManager* GetInstance();

	/// <summary>
	/// Initializes the MessageManager.
	/// Returns true if successfully initialized.
	/// </summary>
	/// <param name = "fpMessageProc"> The function that is going to act as the message proc. 
	/// It must have a Cmessage* parameter. </param>
	bool Initialize(void(*MessageProc)(const CMessage*));

	/// <summary>
	/// Shuts down the MessageManager.
	/// Returns true if successfully shutdown.
	/// </summary>
	/// <param> none</param>
	bool Shutdown();

	/// <summary>
	/// Sends a message to the message proc.
	/// Returns true if successfully sent.
	/// </summary>
	/// <param name = "pMsg"> The message being sent. </param>
	bool Send(const CMessage* pMsg);

	void Update();

};

#endif