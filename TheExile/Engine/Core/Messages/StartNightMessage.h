/***********************************************
* Filename:  		StartNightMessage.h
* Date:      		9/1/2015
* Mod. Date: 		-------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:			The message for the start of night
************************************************/

#ifndef STARTNIGHTMESSAGE_H_
#define STARTNIGHTMESSAGE_H_


#include "../Messages/Message.h"


class CStartNightMessage : public CMessage
{
private:
public:

	/// <summary>
	/// Constructor for Start Night Message
	/// </summary>
	/// <param> none</param>
	CStartNightMessage();

};

#endif