/***********************************************
* Filename:  		AddObjectMessage.cpp
* Date:      		8/25/2015
* Mod. Date: 		8/25/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			The message for removing an object
************************************************/

#include "RemoveObjectMessage.h"

/*****************************************************************
* CRemoveObjectMessage()	The constructor for RemoveObjectMessage
*
* Ins:						object
*							type
*
* Outs:						None
*
* Returns:					None
*
* Mod. Date:				08/25/2015
* Mod. Initials:			NH
*****************************************************************/
CRemoveObjectMessage::CRemoveObjectMessage(IObject* object, CObjectManager::eObjectType type, bool deleteObject) : CMessage(MessageID::MSG_REMOVE_OBJECT)
{
	m_cpTheObject = object;
	m_eObjectType = type;
	m_bDeleteObject = deleteObject;
}

/*****************************************************************
* GetTheObject()		Returns the object being removed
*
* Ins:					None
*
* Outs:					None
*
* Returns:				IObject*
*
* Mod. Date:		    08/25/2015
* Mod. Initials:	    NH
*****************************************************************/
IObject* CRemoveObjectMessage::GetTheObject() const
{
	return m_cpTheObject;
}

/*****************************************************************
* GetTheObjectType()	Returns the object type of the object beign removed
*
* Ins:					None
*
* Outs:					None
*
* Returns:				CObjectManager::eObjectType
*
* Mod. Date:		    08/25/2015
* Mod. Initials:	    NH
*****************************************************************/
CObjectManager::eObjectType CRemoveObjectMessage::GetTheObjectType() const
{
	return m_eObjectType;
}

/*****************************************************************
* GetObjectDelete()		Returns the bool that tells the message proc 
*						to delete the object or not
*
* Ins:					None
*
* Outs:					None
*
* Returns:				CObjectManager::eObjectType
*
* Mod. Date:		    08/25/2015
* Mod. Initials:	    NH
*****************************************************************/
bool CRemoveObjectMessage::GetObjectDelete() const
{
	return m_bDeleteObject;
}