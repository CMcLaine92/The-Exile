#include "ObjectManager.h"
#include "IObject.h"
#include <cassert>
#include "../Core/Collision/Collision.h"
#include "../../Engine/Renderer/Renderer.h"
#define MAX_MAP_SIZE 70000.0f

/*****************************************************************
* CObjectManager:	Initializes the Object Manager
*
* Ins:				N/A
*
* Outs:				N/A
*
* Returns:		    N/A
*
* Mod. Date:		9/2/2015
* Mod. Initials:	MZ
*****************************************************************/
CObjectManager::CObjectManager()
{
	float halfMap = (MAX_MAP_SIZE / 2);
	m_cQuadTree = new CQuadTree();
	m_cQuadTree->BuildTree(-halfMap, -halfMap, halfMap, halfMap, 4);
	m_ptFrustum = new TFrustum();
}

/*****************************************************************
* ~CObjectManager:	Releases all dynamic memory
*
* Ins:				N/A
*
* Outs:				N/A
*
* Returns:		    N/A
*
* Mod. Date:		8/10/2015
* Mod. Initials:	SS
*****************************************************************/
CObjectManager::~CObjectManager()
{
	m_cQuadTree->Destroy();
	SAFE_DELETE(m_cQuadTree);
	SAFE_DELETE(m_ptFrustum);
	
}

/*****************************************************************
* AddObject():		Pushes the specified object into the correct
*					list
*
* Ins:				IObject * _iObject, eObjectType _eType
*
* Outs:				N/A
*
* Returns:		    N/A
*
* Mod. Date:		9/2/2015
* Mod. Initials:	MZ
*****************************************************************/
void CObjectManager::AddObject(IObject * _iObject, eObjectType _eType)
{
	m_vpObjects[_eType].push_back(_iObject);
	if (_eType == Static)
	{
		float xMin, zMin, xMax, zMax;
		if (_iObject->GetColliders()[0]->GetBounds()->GetType() == Bounds::AABB)
		{
			xMin = ((CAABB *)_iObject->GetColliders()[0]->GetBounds())->Min().x;
			zMin = ((CAABB *)_iObject->GetColliders()[0]->GetBounds())->Min().z;
			xMax = ((CAABB *)_iObject->GetColliders()[0]->GetBounds())->Max().x;
			zMax = ((CAABB *)_iObject->GetColliders()[0]->GetBounds())->Max().z;
		}
		else if (_iObject->GetColliders()[0]->GetBounds()->GetType() == Bounds::Plane)
		{
			xMin = ((CPlane *)_iObject->GetColliders()[0]->GetBounds())->Min().x;
			zMin = ((CPlane *)_iObject->GetColliders()[0]->GetBounds())->Min().z;
			xMax = ((CPlane *)_iObject->GetColliders()[0]->GetBounds())->Max().x;
			zMax = ((CPlane *)_iObject->GetColliders()[0]->GetBounds())->Max().z;
		}
		else
		{
			xMin = -MAX_MAP_SIZE;
			zMin = -MAX_MAP_SIZE;
			xMax = MAX_MAP_SIZE;
			zMax = MAX_MAP_SIZE;
		}
		m_cQuadTree->AddObjectToTree(_iObject, xMin, zMin, xMax, zMax);
	}
}

/*****************************************************************
* RemoveObject():	Searches the correct list for the specified
*					object and removes it if it exists
*
* Ins:				IObject * _iObject, eObjectType _eType
*
* Outs:				N/A
*
* Returns:		    bool
*
* Mod. Date:		8/10/2015
* Mod. Initials:	SS
*****************************************************************/
bool CObjectManager::RemoveObject(IObject * _iObject, eObjectType _eType)
{
	if (_iObject == nullptr)
	{
		return false;
	}

	//Getting the correct, specified list
	vector<IObject *>& vCurrentList = m_vpObjects[_eType];

	//Looping through all the object in the list, comparing them to the specified object
	auto iter = vCurrentList.begin();
	while (iter != vCurrentList.end())
	{

		if ((*iter) == _iObject)
		{
			//IObject * temp = (*iter);

			vCurrentList.erase(iter);

			//SPENCER_TODO - Use the temp to uninitialize if necessary
			return true;
		}

		iter++;
	}

	//Return false if the specified object was not in the specified list
	return false;
}

/*****************************************************************
* FindObjectsWithTag():		Searches the specified list for all
*							objects with the given tag and
*							returns them in a vector
*
* Ins:						string _iObject, eObjectType _eType
*
* Outs:						N/A
*
* Returns:					N/A
*
* Mod. Date:				8/11/2015
* Mod. Initials:			SS
*****************************************************************/
vector<IObject *> CObjectManager::FindObjectsWithTag(string _szTag, eObjectType _eType)
{

	//Getting the correct, specified list
	vector<IObject *>&vCurrentList = m_vpObjects[_eType];

	vector<IObject *> vOutList;

	//Looping through all the object in the list, comparing them to the specified object
	for (unsigned int object = 0; object < vCurrentList.size(); object++)
	{
		if (vCurrentList[object]->GetTag() == _szTag)
		{
			//SPENCER_TODO - Use the temp to uninitialize if necessary
			vOutList.push_back(vCurrentList[object]);
		}
	}

	//returning the list of objects with a matching tag
	return vOutList;
}

/*****************************************************************
* GetCamera():		Returns the Camera object
*
* Ins:				N/A
*
* Outs:				N/A
*
* Returns:			IObject *
*
* Mod. Date:		8/11/2015
* Mod. Initials:	SS
*****************************************************************/
IObject * CObjectManager::GetCamera()
{
	vector<IObject *> vCamera = FindObjectsWithTag("Camera", Dynamic);

	assert(vCamera.size() != 0 && "CObjectManager::GetCamera() - FindObjectsWithTag() didn't find a camera");

	return vCamera[0];
}

/*****************************************************************
* GetPlayer():		Returns the Player object
*
* Ins:				N/A
*
* Outs:				N/A
*
* Returns:			IObject *
*
* Mod. Date:		8/11/2015
* Mod. Initials:	SS
*****************************************************************/
IObject * CObjectManager::GetPlayer()
{
	vector<IObject *> vPlayer = FindObjectsWithTag("Player", Dynamic);

	assert(vPlayer.size() != 0 && "CObjectManager::GetPlayer() - FindObjectsWithTag() didn't find a player");

	return  vPlayer[0];
}

/*****************************************************************
* Update():			Updates all the objects in the object manager
*
* Ins:				N/A
*
* Outs:				N/A
*
* Returns:			N/A
*
* Mod. Date:		8/11/2015
* Mod. Initials:	SS
*****************************************************************/
void CObjectManager::UpdateAll()
{
	float fFireTrapTime = 0.0f;
	int nFireTrapUpdates = 0;
	BuildFrustum(m_ptFrustum, RADIANS(FOV), GRAPHICS->GetNearPlane(), GRAPHICS->GetFarPlane(), RATIO, GRAPHICS->GetViewMatrix());

	for (int list = Static; list < Count; list++)
	{
		//Grabs the current list to update
		vector<IObject *>& vCurrentList = m_vpObjects[list];
		for (unsigned int object = 0; object < vCurrentList.size(); object++)
		{
			//Update the current object
			vCurrentList[object]->Update();
			if (vCurrentList[object]->GetColliders().size() > 0)
			{
				for (unsigned int colliderIndex = 0; colliderIndex < vCurrentList[object]->GetColliders().size(); colliderIndex++)
				{
					vCurrentList[object]->GetColliders()[colliderIndex]->SetResetVelocity(vCurrentList[object]->GetWorldVelocity());
				}
			
				//SPENCER_TODO
			
				///////////////////////////////////////////////
				/////////Taking care of collisions/////////////
				///////////////////////////////////////////////
				if (list != Static)
				{
					
					vector<IObject *> vpCollidedObjects;
					vector<IObject*> vpTestCollisionObjects = m_cQuadTree->GetNearbyObjects(vCurrentList[object]->GetColliders()[0]->GetBounds());
					for (unsigned int i = 0; i < m_vpObjects[Dynamic].size(); i++)
					{
						vpTestCollisionObjects.push_back(m_vpObjects[Dynamic][i]);
					}
			
					for (unsigned int i = 0; i < m_vpObjects[AI].size(); i++)
					{
						vpTestCollisionObjects.push_back(m_vpObjects[AI][i]);
					}
					if (CheckCollision(vCurrentList[object], vpTestCollisionObjects, vpCollidedObjects))
					{
						for (unsigned int collided = 0; collided < vpCollidedObjects.size(); collided++)
						{
							vCurrentList[object]->CollisionResponse(vpCollidedObjects[collided]);
							vpCollidedObjects[collided]->CollisionResponse(vCurrentList[object]);
						}
					}
				}
			}
			vCurrentList[object]->UpdatePosition();	

		}
		if (list == Static)
		{
			FrustumCull(m_ptFrustum, vCurrentList);
		}
	}
}