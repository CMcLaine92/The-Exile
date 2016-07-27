/***********************************************
* Filename:  		QuadNode.cpp
* Date:      		8/12/2015
* Mod. Date: 		-----
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Node class that makes up the Quad Tree
************************************************/

#include "QuadNode.h"
/*****************************************************************
* CQuadNode(XMFLOAT2,XMFLOAT2,unsigned int,unsigned int,CQuadNode*)	Initializes a node with passed in values
*
* Ins:					f2TopLeft		- The top left of the objects bounds.
*						f2BotRight		- The bottom right of the objects bounds.
*						unMaxLevel		- The maximum depth level of the tree.
*						unCurrentLevel	- The current depth level in the tree.
*						pcParentNode	- The nodes parent in the tree
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    08/13/2015
* Mod. Initials:	    MZ
*****************************************************************/
CQuadNode::CQuadNode(XMFLOAT2 f2TopLeft, XMFLOAT2 f2BotRight, unsigned int unMaxLevel, unsigned int unCurrentLevel, CQuadNode* pcParentNode)
{
	m_f2TopLeft = f2TopLeft;
	m_f2BotRight = f2BotRight;
	
	for (unsigned int i = 0; i < 4; i++)
	{
		m_pcChildNodes[i] = nullptr;
	}
	m_unMaxLevel = unMaxLevel;
	m_unCurrentLevel = unCurrentLevel;

	m_pcParentNode = pcParentNode;
}

/*****************************************************************
* CreateChildren()		Initializes the child nodes based on current node's values
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
void CQuadNode::CreateChildren()
{
	if (m_unCurrentLevel >= m_unMaxLevel)
	{
		return;
	}

	XMFLOAT2 f2CenterPoint;
	f2CenterPoint.x = (m_f2TopLeft.x + m_f2BotRight.x) / 2.0f;
	f2CenterPoint.y = (m_f2TopLeft.y + m_f2BotRight.y) / 2.0f;

	m_pcChildNodes[TopLeft] = new CQuadNode(m_f2TopLeft, f2CenterPoint,m_unMaxLevel,m_unCurrentLevel+1,this);
	m_pcChildNodes[TopRight] = new CQuadNode(XMFLOAT2(f2CenterPoint.x, m_f2TopLeft.y), XMFLOAT2(m_f2BotRight.x, f2CenterPoint.y), m_unMaxLevel, m_unCurrentLevel + 1,this);
	m_pcChildNodes[BotLeft] = new CQuadNode(XMFLOAT2(m_f2TopLeft.x, f2CenterPoint.y), XMFLOAT2(f2CenterPoint.x, m_f2BotRight.y), m_unMaxLevel, m_unCurrentLevel + 1,this);
	m_pcChildNodes[BotRight] = new CQuadNode(f2CenterPoint, m_f2BotRight, m_unMaxLevel, m_unCurrentLevel + 1,this);

	m_pcChildNodes[TopLeft]->CreateChildren();
	m_pcChildNodes[TopRight]->CreateChildren();
	m_pcChildNodes[BotLeft]->CreateChildren();
	m_pcChildNodes[BotRight]->CreateChildren();
}

/*****************************************************************
* SortObject(XMFLOAT2,XMFLOAT2)			Finds the node to that the object should be put in.
*
* Ins:					f2TopLeft		- The top left of the objects bounds.
*						f2BotRight		- The bottom right of the objects bounds.
*
* Outs:					None
*
* Returns:				CQuadNode*		- The node to place the object into
*
* Mod. Date:		    08/13/2015
* Mod. Initials:	    MZ
*****************************************************************/
CQuadNode* CQuadNode::SortObject(XMFLOAT2 f2TopLeft,XMFLOAT2 f2BotRight)
{
	XMFLOAT2 f2CenterPoint;
	f2CenterPoint.x = (m_f2TopLeft.x + m_f2BotRight.x) / 2.0f;
	f2CenterPoint.y = (m_f2TopLeft.y + m_f2BotRight.y) / 2.0f;

	//Leaf Node
	if (m_pcChildNodes[0] == nullptr)
	{
		return this;
	}
	//Not Fully Contained In Any Child Nodes 
	if ((f2TopLeft.x <= f2CenterPoint.x && f2BotRight.x >= f2CenterPoint.x) || (f2TopLeft.y >= f2CenterPoint.y && f2BotRight.y <= f2CenterPoint.y))
	{
		return this;
	}
	if (f2BotRight.x <= f2CenterPoint.x) //Left Side
	{
		
		if (f2BotRight.y >= f2CenterPoint.y) //Top
		{
			return m_pcChildNodes[TopLeft]->SortObject(f2TopLeft, f2BotRight);
		}
		else //Bottom
		{
			return m_pcChildNodes[BotLeft]->SortObject(f2TopLeft, f2BotRight);
		}
	}
	if (f2TopLeft.x >= f2CenterPoint.x) //Right Side
	{
		if (f2BotRight.y >= f2CenterPoint.y) //Top
		{
			return m_pcChildNodes[TopRight]->SortObject(f2TopLeft, f2BotRight);
		}
		else//Bottom
		{
			return m_pcChildNodes[BotRight]->SortObject(f2TopLeft, f2BotRight);
		}
	}
	//assert(0 && "Sort Object is about to return nullptr.  This normally means that bounds or velocity are incorrect.");
	return nullptr; //Something went Wrong if we hit this.
}

/*****************************************************************
* AddObject(IObject*,XMFLOAT2,XMFLOAT2)	Adds the object to the correct node.
*
* Ins:					pObj			- The object to add.
*						f2TopLeft		- The top left of the objects bounds.
*						f2BotRight		- The bottom right of the objects bounds.
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/13/2015
* Mod. Initials:	    MZ
*****************************************************************/
void CQuadNode::AddObject(IObject* pObj, XMFLOAT2 f2TopLeftBounds, XMFLOAT2 f2BotRightBounds)
{
	SortObject(f2TopLeftBounds, f2BotRightBounds)->m_vpcContainedObjects.push_back(pObj);
}

/*****************************************************************
* GetObjects(vector<IObject*>,XMFLOAT2,XMFLOAT2)	Gets all the objects that could collide with the passed in bounds.
*
* Ins:					vpcObjects		- IN_OUT Adds potential collidable objects to the passed in vector.
*						f2TopLeft		- The top left of the objects bounds.
*						f2BotRight		- The bottom right of the objects bounds.
*
* Outs:					vpcObjects		- IN_OUT Adds potential collidable objects to the passed in vector.
*
* Returns:				void
*
* Mod. Date:		    08/13/2015
* Mod. Initials:	    MZ
*****************************************************************/
void CQuadNode::GetObjects(std::vector<IObject*>& vpcObjects, XMFLOAT2 f2TopLeftBounds, XMFLOAT2 f2BotRightBounds)
{
	CQuadNode* pcFullyContainedNode = SortObject(f2TopLeftBounds, f2BotRightBounds);
	if (pcFullyContainedNode == nullptr)
	{
		return;
	}
	pcFullyContainedNode->GetObjectsInChildren(vpcObjects);

	pcFullyContainedNode->GetObjectsInParent(vpcObjects);
	

}


/*****************************************************************
* GetObjectsinChildrens(vector<IObject*>)	Gets objects in all current Nodes and down the tree.
*
* Ins:					vpcObjects	- IN_OUT  Adds potential collidable objects to the passed in vector.
*
* Outs:					vpcObjects	- IN_OUT  Adds potential collidable objects to the passed in vector.
*
* Returns:				void
*
* Mod. Date:		    08/13/2015
* Mod. Initials:	    MZ
*****************************************************************/
void CQuadNode::GetObjectsInChildren(std::vector<IObject*>& vpcObjects)
{


	for (unsigned int i = 0; i < m_vpcContainedObjects.size(); i++)
	{
		vpcObjects.push_back(m_vpcContainedObjects[i]);
	}

	if (m_pcChildNodes[0] == nullptr)
	{
		return;
	}

	for (unsigned int i = 0; i < 4; i++)
	{
		m_pcChildNodes[i]->GetObjectsInChildren(vpcObjects);
	}
}

/*****************************************************************
* GetObjectsInParent(vector<IObject*>)	Gets objects in all current Nodes and up the tree.
*
* Ins:					vpcObjects	- IN_OUT  Adds potential collidable objects to the passed in vector.
*
* Outs:					vpcObjects	- IN_OUT  Adds potential collidable objects to the passed in vector.
*
* Returns:				void
*
* Mod. Date:		    08/13/2015
* Mod. Initials:	    MZ
*****************************************************************/
void CQuadNode::GetObjectsInParent(std::vector<IObject*>& vpcObjects)
{
	for (unsigned int i = 0; i < m_vpcContainedObjects.size(); i++)
	{
		vpcObjects.push_back(m_vpcContainedObjects[i]);
	}
	
	if (m_pcParentNode == nullptr)
	{
		return;
	}

	m_pcParentNode->GetObjectsInParent(vpcObjects);
}

/*****************************************************************
* Destroy()				Deletes all dynamic memory in the node and all children
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
void CQuadNode::Destroy()
{
	
	for (unsigned int i = 0; i < 4; i++)
	{
		if (m_pcChildNodes[i] == nullptr)
			continue;
		m_pcChildNodes[i]->Destroy();
		SAFE_DELETE(m_pcChildNodes[i]);
	}
}