#ifndef _QUADNODE_H_
#define _QUADNODE_H_

/***********************************************
* Filename:  		QuadNode.h
* Date:      		8/12/2015
* Mod. Date: 		-----
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Node class that makes up the Quad Tree
************************************************/


#include <DirectXMath.h>
using namespace DirectX;
#include "IObject.h"
#include <vector>

enum eQuadChild { TopLeft, TopRight, BotLeft,BotRight, Self };
class CQuadNode
{
private:
	XMFLOAT2 m_f2TopLeft;
	XMFLOAT2 m_f2BotRight;
	std::vector<IObject*> m_vpcContainedObjects;
	CQuadNode* m_pcChildNodes[4];	
	CQuadNode* m_pcParentNode;
	unsigned int m_unMaxLevel;
	unsigned int m_unCurrentLevel;
	
	/// <summary>
	/// Finds out which node an object should be put into.
	/// </summary>
	/// <param name = "f2TopLeft"> The minimum xz value the object can occupy.</param>
	/// <param name = "f2BotRight"> The maximum xz value any object can occupy.</param>
	CQuadNode* SortObject(XMFLOAT2 f2TopLeft, XMFLOAT2 f2BotRight);

	/// <summary>
	/// Finds all objects in child nodes.
	/// </summary>
	/// <param name = "vpcObjects"> IN_OUT The objects in child nodes.</param>
	void GetObjectsInChildren(std::vector<IObject*>& vpcObjects);

	/// <summary>
	/// Finds all objects in parent nodes.
	/// </summary>
	/// <param name = "vpcObjects"> IN_OUT The objects in parent nodes.</param>
	void GetObjectsInParent(std::vector<IObject*>& vpcObjects);

public:

	/// <summary>
	/// Initializes Quad Node data
	/// </summary>
	/// <param name = "f2TopLeft"> Top Left of object bounds.</param>
	/// <param name = "f2BotRight"> Bottom Right of object bounds.</param>
	/// <param name = "unMaxLevel"> Max level of depth in the tree.</param>
	/// <param name = "unCurrentLevel"> Current depth level in the tree.</param>
	/// <param name = "pcParentNode"> Parent node in the tree.</param>
	CQuadNode(XMFLOAT2 f2TopLeft, XMFLOAT2 f2BotRight, unsigned int unMaxLevel, unsigned int unCurrentLevel, CQuadNode* pcParentNode);

	/// <summary>
	/// Initializes child data based on parent data
	/// </summary>
	/// <param> none</param>
	void CreateChildren();

	/// <summary>
	/// Adds object to the proper node.  Should be called on root of tree.
	/// </summary>
	/// <param name = "pObj"> The object to add.</param>
	/// <param name = "f2TopLeftBounds"> The top left of the objects bounds.</param>
	/// <param name = "f2BotRightBounds"> The bottom right of the objects bounds.</param>
	void AddObject(IObject* pObj, XMFLOAT2 f2TopLeftBounds, XMFLOAT2 f2BotRightBounds);


	/// <summary>
	/// Returns all objects that could collide with the passed in bounds
	/// </summary>
	/// <param name = "vpcObjects"> IN_OUT The objects that have been found.</param>
	/// <param name = "f2TopLeftBounds"> The top left of the objects bounds.</param>
	/// <param name = "f2BotRightBounds"> The bottom right of the objects bounds.</param>
	void GetObjects(std::vector<IObject*>& vpcObjects, XMFLOAT2 f2TopLeftBounds, XMFLOAT2 f2BotRightBounds);

	/// <summary>
	/// Deallocates the dynamic memory in the quad node.
	/// </summary>
	/// <param name = ""> </param>
	void Destroy();
};





#endif