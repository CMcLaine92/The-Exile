/***********************************************
* Filename:  		DestroyBushMessage.cpp
* Date:      		9/13/2015
* Mod. Date: 		9/13/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Destroy a bush message
************************************************/

#include "DestroyBushMessage.h"

CDestroyBushMessage::CDestroyBushMessage(CBush* object) : CMessage(MessageID::MSG_DESTROY_BUSH)
{
	m_cpBushObject = object;
}

CBush* CDestroyBushMessage::GetBushObject()
{
	return m_cpBushObject;
}
