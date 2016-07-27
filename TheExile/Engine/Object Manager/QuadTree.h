#ifndef _QUADTREE_H_
#define _QUADTREE_H_
#include "QuadNode.h"
#include "IObject.h"
class CBounds;
/***********************************************
* Filename:  		QuadTree.h
* Date:      		8/12/2015
* Mod. Date: 		-----
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Used for spatial partitioning of
*                   immobile objects
************************************************/
class CQuadTree
{
private:
	CQuadNode* m_pcRoot;

public:
	
	
	/// <summary>
	/// Initializes the quad tree
	/// </summary>
	/// <param name = "fXMin"> The minimum x value any object can occupy.</param>
	/// <param name = "fZMin"> The minimum z value any object can occupy.</param>
	/// <param name = "fXMax"> The maximum x value any object can occupy.</param>
	/// <param name = "fZMax"> The maximum z value any object can occupy.</param>
	/// <param name = "unMaxLevels"> The maximum depth number of the tree.</param>
	void BuildTree(float fXMin, float fZMin, float fXMax, float fZMax, unsigned int unMaxLevels);


	/// <summary>
	/// Add Object to the quad tree
	/// </summary>
	/// <param name = "pObj"> The object to be added to the tree.</param>
	/// <param name = "fXMin"> The minimum x value the object is at.</param>
	/// <param name = "fZMin"> The minimum z value the object is at.</param>
	/// <param name = "fXMax"> The maximum x value the object is at.</param>
	/// <param name = "fZMax"> The maximum z value the object is at.</param>
	void AddObjectToTree(IObject* pObj, float fXMin, float fZMin, float fXMax, float fZMax);


	/// <summary>
	/// Returns all objects that could/do collide with the specified bounds
	/// </summary>
	/// <param name = "fXMin"> The minimum x value of the bounds.</param>
	/// <param name = "fZMin"> The minimum z value of the bounds.</param>
	/// <param name = "fXMax"> The maximum x value of the bounds.</param>
	/// <param name = "fZMax"> The maximum z value of the bounds.</param>
	std::vector<IObject*>GetNearbyObjects(float fXMin, float fZMin, float fXMax, float fZMax);

	/// <summary>
	/// Returns all objects that could/do collide with the specified bounds
	/// </summary>
	/// <param name = "cBounds"> The object bounds.</param>
	std::vector<IObject*>GetNearbyObjects(CBounds* cBounds);

	/// <summary>
	/// Deallocates the dynamic memory in the quad tree.
	/// </summary>
	/// <param name = ""> </param>
	void Destroy();
};





#endif