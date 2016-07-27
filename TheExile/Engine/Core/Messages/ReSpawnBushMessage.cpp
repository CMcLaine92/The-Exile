/***********************************************
* Filename:  		ReSpawnBushMessage.cpp
* Date:      		9/13/2015
* Mod. Date: 		9/13/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Respawn a bush message
************************************************/

#include "ReSpawnBushMessage.h"

CReSpawnBushMessage::CReSpawnBushMessage(IObject* object) : CMessage(MessageID::MSG_RESPAWN_BUSH)
{
	m_cpBushObject = object;
}

IObject* CReSpawnBushMessage::GetBushObject()
{
	return m_cpBushObject;
}
