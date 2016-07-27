/***********************************************
* Filename:  		DestroyBushMessage.h
* Date:      		9/13/2015
* Mod. Date: 		9/13/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Destroy a bush message
************************************************/

#ifndef CDESTROYBUSHMESSAGE_H
#define CDESTROYBUSHMESSAGE_H

#include "../Messages/Message.h"
#include "../../../Game/Source/Environment/Bush.h"

class CDestroyBushMessage : public CMessage
{
private:

	CBush* m_cpBushObject;

public:

	CDestroyBushMessage(CBush* object);

	~CDestroyBushMessage() = default;

	CBush* GetBushObject();

};

#endif
