/***********************************************
* Filename:  		AddObjectMessage.h
* Date:      		8/25/2015
* Mod. Date: 		8/25/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			The message for removing an object
************************************************/

#ifndef CREMOVEOBJECTMESSAGE_H
#define CREMOVEOBJECTMESSAGE_H

#include "../Messages/Message.h"
#include "../../Object Manager/ObjectManager.h"

class CRemoveObjectMessage : public CMessage
{
private:

	IObject* m_cpTheObject;
	CObjectManager::eObjectType m_eObjectType;
	bool m_bDeleteObject;

public:

	/// <summary>
	/// Constructor for EnemySpawnMessage
	/// </summary>
	/// <param name = "object"> The object beging removed. </param>
	/// <param name = "type"> The type of object being removed. </param>
	CRemoveObjectMessage(IObject* object, CObjectManager::eObjectType type, bool deleteObject = false);

	/// <summary>
	/// Default destructor for EnemySpawnMessage
	/// </summary>
	/// <param> none</param>
	~CRemoveObjectMessage() = default;


	/***************
	*  Accessors
	***************/

	/// <summary>
	/// Returns the object being removed
	/// </summary>
	/// <param> none</param>
	IObject* GetTheObject() const;

	/// <summary>
	/// Returns the object type of the object beign removed
	/// </summary>
	/// <param> none</param>
	CObjectManager::eObjectType GetTheObjectType() const;

	/// <summary>
	/// Returns the bool that tells the message proc to delete the object or not
	/// </summary>
	/// <param> none</param>
	bool GetObjectDelete() const;

};

#endif
