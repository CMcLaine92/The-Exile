/***********************************************
* Filename:  		AddObjectMessage.cpp
* Date:      		8/21/2015
* Mod. Date: 		8/21/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			The message for adding an object
************************************************/

#include "AddObjectMessage.h"

/*****************************************************************
* CAddObjectMessage()	The constructor for AddObjectMessage
*
* Ins:					object
*						type
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    08/21/2015
* Mod. Initials:	    NH
*****************************************************************/
CAddObjectMessage::CAddObjectMessage(IObject* object, CObjectManager::eObjectType type) : CMessage(MessageID::MSG_ADD_OBJECT)
{
	m_cpTheObject = object;
	m_eObjectType = type;
}

/*****************************************************************
* GetTheObject()		Returns the object being added
*
* Ins:					None
*
* Outs:					None
*
* Returns:				IObject*
*
* Mod. Date:		    08/21/2015
* Mod. Initials:	    NH
*****************************************************************/
IObject* CAddObjectMessage::GetTheObject() const
{
	return m_cpTheObject;
}

/*****************************************************************
* GetTheObjectType()	Returns the object type of the object beign added
*
* Ins:					None
*
* Outs:					None
*
* Returns:				CObjectManager::eObjectType
*
* Mod. Date:		    08/21/2015
* Mod. Initials:	    NH
*****************************************************************/
CObjectManager::eObjectType CAddObjectMessage::GetTheObjectType() const
{
	return m_eObjectType;
}
