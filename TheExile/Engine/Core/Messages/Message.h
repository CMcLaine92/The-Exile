/***********************************************
* Filename:  		Message.h
* Date:      		8/10/2015
* Mod. Date: 		8/10/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Holds information to be passed 
*					through the MessageManager to 
*					the messageProc
************************************************/

#ifndef CMESSAGE_H
#define CMESSAGE_H


enum MessageID
{
	MSG_NONE			= 0,
	MSG_SPAWN_ENEMY		= 1,
	MSG_DESPAWN_ENEMY	= 2,
	MSG_START_DAY		= 3,
	MSG_START_NIGHT		= 4,
	MSG_OPEN_DOOR		= 5,
	MSG_CLOSE_DOOR		= 6,
	MSG_WIN_GAME		= 7,
	MSG_ADD_OBJECT		= 8,
	MSG_REMOVE_OBJECT	= 9,
	MSG_RESPAWN_BUSH	= 10,
	MSG_DESTROY_BUSH	= 11,
	MSG_EXITKEY_FOUND	= 12,

};


class CMessage
{
private:

	MessageID m_eMessageID;

public:

	CMessage() = default;
	virtual ~CMessage() = default;

	/// <summary>
	/// OverLoaded constructor that sets the message id.
	/// </summary>
	/// <param> none</param>
	CMessage(MessageID id);

	/// <summary>
	/// Sends this message to the message proc.
	/// Returns true if successfully sent.
	/// </summary>
	/// <param> none</param>
	bool Send() const;



	/***************
	*  Accessors
	***************/

	/// <summary>
	/// Returns the message id of this message.
	/// </summary>
	/// <param> none</param>
	MessageID GetMessageID() const;


};

#endif