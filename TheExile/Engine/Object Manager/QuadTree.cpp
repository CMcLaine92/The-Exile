#include "QuadTree.h"
#include "../Core/Collision/AABB.h"
#include "../Core/Collision/Capsule.h"
#include "../Core/Collision/Plane.h"
#include "../Renderer/Renderer.h"
/***********************************************
* Filename:  		QuadTree.h
* Date:      		8/12/2015
* Mod. Date: 		-----
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Used for spatial partitioning
************************************************/

/*****************************************************************
* BuildTree(float,float,float,float,unsigned int)	Initializes the Tree
*
* Ins:					fXMin		- The minimum x value any object can occupy
*						fZMin		- The minimum z value any object can occupy
*						fXMax		- The maximum x value any object can occupy
*						fZMax		- The maximum z value any object can occupy	
*						unMaxLevels - The number of depths the tree will have
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/12/2015
* Mod. Initials:	    MZ
*****************************************************************/
void CQuadTree::BuildTree(float fXMin, float fZMin, float fXMax, float fZMax, unsigned int unMaxLevels)
{
	XMFLOAT2 f2TopLeft = { fXMin, fZMax };
	XMFLOAT2 f2BotRight = { fXMax, fZMin };
	m_pcRoot = new CQuadNode(f2TopLeft, f2BotRight, unMaxLevels, 0,nullptr);
	m_pcRoot->CreateChildren();
	XMFLOAT3 f3Start(f2TopLeft.x, 100, f2TopLeft.y);
	XMFLOAT3 f3End(f2BotRight.x, 100, f2BotRight.y);
	f3Start.x = 200;
	f3Start.z = 200;
	f3End.x = -200;
	f3End.z = 200;
	//GRAPHICS->DrawLine(f3Start, f3End, XMFLOAT3(0, 0, 1));
}

/*****************************************************************
* AddObjectToTree(IObject*,float,float,float,float)	Adds the object into the tree
*
* Ins:					pObj		- Object to add
*						fXMin		- The minimum x value the object can occupy
*						fZMin		- The minimum z value the object can occupy
*						fXMax		- The maximum x value the object can occupy
*						fZMax		- The maximum z value the object can occupy
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/12/2015
* Mod. Initials:	    MZ
*****************************************************************/
void CQuadTree::AddObjectToTree(IObject* pObj, float fXMin, float fZMin, float fXMax, float fZMax)
{
	XMFLOAT2 f2TopLeft = { fXMin, fZMax };
	XMFLOAT2 f2BotRight = { fXMax, fZMin };
	m_pcRoot->AddObject(pObj, f2TopLeft, f2BotRight);
}

/*****************************************************************
* GetNearbyObjects(float,float,float,float)	Returns all objects that could/do
*											collide with the specified bounds
*
* Ins:					fXMin		- The minimum x value of the bounds
*						fZMin		- The minimum z value of the bounds
*						fXMax		- The maximum x value of the bounds
*						fZMax		- The maximum z value of the bounds
*
* Outs:					None
*
* Returns:				vector<IObject*>
*
* Mod. Date:		    08/12/2015
* Mod. Initials:	    MZ
*****************************************************************/

std::vector<IObject*> CQuadTree::GetNearbyObjects(float fXMin, float fZMin, float fXMax, float fZMax)
{
	XMFLOAT2 f2TopLeft = { fXMin, fZMax };
	XMFLOAT2 f2BotRight = { fXMax, fZMin };

	std::vector<IObject*> vpcOutput;
	m_pcRoot->GetObjects(vpcOutput, f2TopLeft, f2BotRight);
	return vpcOutput;
}


/*****************************************************************
* GetNearbyObjects(CBounds*)				Returns all objects that could/do
*											collide with the specified bounds
*
* Ins:					cBounds - The bounds of the object.
*
* Outs:					None
*
* Returns:				vector<IObject*>
*
* Mod. Date:		    08/12/2015
* Mod. Initials:	    MZ
*****************************************************************/
std::vector<IObject*>CQuadTree::GetNearbyObjects(CBounds* cBounds)
{
	Bounds::eBoundsType eType = cBounds->GetType();

	float xMin = 0.0f, zMin = 0.0f, xMax = 0.0f, zMax = 0.0f;

	if (eType == Bounds::AABB)
	{
		xMin = ((CAABB *)cBounds)->Min().x;
		zMin = ((CAABB *)cBounds)->Min().z;
		xMax = ((CAABB *)cBounds)->Max().x;
		zMax = ((CAABB *)cBounds)->Max().z;
	}
	else if (eType == Bounds::Capsule)
	{
		float fRadius = ((CCapsule*)cBounds)->GetRadius();
		xMin = ((CCapsule*)cBounds)->GetStart().x - fRadius;
		zMin = ((CCapsule*)cBounds)->GetStart().z - fRadius;
		xMax = ((CCapsule*)cBounds)->GetStart().x + fRadius;
		zMax = ((CCapsule*)cBounds)->GetStart().z + fRadius;

		float testXMin = ((CCapsule*)cBounds)->GetEnd().x - fRadius;
		float testZMin = ((CCapsule*)cBounds)->GetEnd().z - fRadius;
		float testXMax = ((CCapsule*)cBounds)->GetEnd().x + fRadius;
		float testZMax = ((CCapsule*)cBounds)->GetEnd().z + fRadius;
		
		if (testXMin < xMin)
		{
			xMin = testXMin;
		}
		if (testZMin < zMin)
		{
			zMin = testZMin;
		}
		if (testXMax > xMax)
		{
			xMax = testXMax;
		}
		if (testZMax > zMax)
		{
			zMax = testZMax;
		}
	}
	else if (eType == Bounds::Plane)
	{
		xMin = ((CPlane *)cBounds)->Min().x;
		zMin = ((CPlane *)cBounds)->Min().z;
		xMax = ((CPlane *)cBounds)->Max().x;
		zMax = ((CPlane *)cBounds)->Max().z;
	}
	else
	{
		assert(0 && "Type Not Supported.  Let Matt Know About This Please.");
	}

	return GetNearbyObjects(xMin,zMin,xMax,zMax);
}

/*****************************************************************
* Destroy()				Deletes all dynamic memory in the tree.
*
* Ins:					None
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/13/2015
* Mod. Initials:	    MZ
*****************************************************************/
void CQuadTree::Destroy()
{
	m_pcRoot->Destroy();
	SAFE_DELETE(m_pcRoot);
	
}