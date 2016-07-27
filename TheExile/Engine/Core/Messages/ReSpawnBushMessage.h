/***********************************************
* Filename:  		ReSpawnBushMessage.h
* Date:      		9/13/2015
* Mod. Date: 		9/13/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Respawn a bush message
************************************************/

#ifndef CRESPAWNBUSHMESSAGE_H
#define CRESPAWNBUSHMESSAGE_H

#include "../Messages/Message.h"
#include "../../Object Manager/IObject.h"


class CReSpawnBushMessage : public CMessage
{
private:

	IObject* m_cpBushObject;

public:

	CReSpawnBushMessage(IObject* object);

	~CReSpawnBushMessage() = default;

	IObject* GetBushObject();

};

#endif
