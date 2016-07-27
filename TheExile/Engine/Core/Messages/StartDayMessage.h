/***********************************************
* Filename:  		StartDayMessage.h
* Date:      		9/1/2015
* Mod. Date: 		-------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:			The message for the start of day
************************************************/

#ifndef STARTDAYMESSAGE_H_
#define STARTDAYMESSAGE_H_


#include "../Messages/Message.h"


class CStartDayMessage : public CMessage
{
private:
public:

	/// <summary>
	/// Constructor for Start Day Message
	/// </summary>
	/// <param> none</param>
	CStartDayMessage();

};

#endif