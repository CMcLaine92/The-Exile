/***********************************************
* Filename:  		AddObjectMessage.h
* Date:      		8/21/2015
* Mod. Date: 		8/21/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			The message for adding an object
************************************************/

#ifndef CADDOBJECTMESSAGE_H
#define CADDOBJECTMESSAGE_H


#include "../Messages/Message.h"
#include "../../Object Manager/ObjectManager.h"


class CAddObjectMessage : public CMessage
{
private:

	IObject* m_cpTheObject;
	CObjectManager::eObjectType m_eObjectType;

public:

	/// <summary>
	/// Constructor for EnemySpawnMessage
	/// </summary>
	/// <param name = "object"> The object beging added. </param>
	/// <param name = "type"> The type of object being added. </param>
	CAddObjectMessage(IObject* object, CObjectManager::eObjectType type);

	/// <summary>
	/// Default destructor for EnemySpawnMessage
	/// </summary>
	/// <param> none</param>
	~CAddObjectMessage() = default;

	/***************
	*  Accessors
	***************/

	/// <summary>
	/// Returns the object being added
	/// </summary>
	/// <param> none</param>
	IObject* GetTheObject() const;

	/// <summary>
	/// Returns the object type of the object beign added
	/// </summary>
	/// <param> none</param>
	CObjectManager::eObjectType GetTheObjectType() const;

};

#endif
