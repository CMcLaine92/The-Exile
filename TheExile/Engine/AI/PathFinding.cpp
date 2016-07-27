/***********************************************
* Filename:  		PathFinding.cpp
* Date:      		08/16/2015
* Mod. Date: 		----------
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:   		Finds a path using a generated Node Graph
*					for Agents to move a long through A* Pathfinding.
************************************************/
#include "PathFinding.h"
#include "CNavGraph.h"
#include "NavGraphManager.h"

#include "../Object Manager/ObjectManager.h"
#include "../Core/Collision/Capsule.h"
#include "../Core/Collision/Collider.h"
#include "../Core/Collision/Collision.h"
#include "../Animation/Mesh.h"

/*****************************************************************
* CPathFinding()		Initializes the PathFinding class.
*
* Ins:					None
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    08/16/2015
* Mod. Initials:	    NS
*****************************************************************/
CPathFinding::CPathFinding( const char* navGraphName, CObjectManager* objManager, bool render)
{

	m_bIsRendering = false;
	m_pGraph = nullptr;
	m_pNavRenderMesh = nullptr;
	if (navGraphName)
	{
		m_pGraph = CNavGraphManager::GetReference().GetNavGraph(navGraphName);
		if (m_pGraph && render)
		{
			m_bIsRendering = render;
			CMesh* navMesh = m_pGraph->GetNavMesh();
			m_pNavRenderMesh = new CRenderMesh(navMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetPixelShader(), NULL, NULL, NULL, L"../Game/Assets/Art/2D/Textures/Ground_Grass.dds");
			m_pNavRenderMesh->GetWireframe() = true;
			GRAPHICS->AddRenderMesh(m_pNavRenderMesh);
		}
	}

	m_pObjectManager = objManager;
	// Set up access to the graph.

	
	m_fHeuristicWeight = 1.20f;
}

/*****************************************************************
* ~CPathFinding()		Un-Initializes the PathFinding class.
*
* Ins:					None
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    08/16/2015
* Mod. Initials:	    NS
*****************************************************************/
CPathFinding::~CPathFinding()
{
	SAFE_DELETE(m_pNavRenderMesh);


	m_pGraph = nullptr;
	// Clean up
	m_pObjectManager = nullptr;
	GetReadyForNewSearch();
	//m_vPath.clear();
	m_lPath.clear();
	m_umVisitedNodes.clear();
	m_pqNodeQue.clear();
}

/*****************************************************************
* GeneratePath()		Uses A* PathFinding to generate a path from
*						the start position to the goal position
*
* Ins:					int
*						int
*
* Outs:					None
*
* Returns:				vector<CNode*>
*
* Mod. Date:		    08/16/2015
* Mod. Initials:	    NS
*****************************************************************/
vector<XMFLOAT3> CPathFinding::GeneratePath(int _start, int _goal, bool smoothed)
{
	GetReadyForNewSearch();

	// Create the first PathNode
	CPathNode* curPathNode = new CPathNode(m_pGraph->GetNodes()[_start]);
	m_pqNodeQue.push(curPathNode);
	m_umVisitedNodes[_start] = curPathNode;

	// Create all local variables here for attempt at optimization
	CPathNode* theNode = nullptr;
	CPathNode* visitedNode = nullptr;
	CPathNode* nextPathNode = nullptr;
	CNode* curNode;
	CNode* adjNode;
	CEdge* curEdge;
	float newFinalCost;
	float newCost;
	float oldCost;
	float curCost;
	float newHeuristic;
	int adjNodeIndex;
	int curNodeIndex;
	vector<XMFLOAT3> path;

	// Loop while there are edges to explore
	while (!m_pqNodeQue.empty())
	{
		// Get the next node.
		curPathNode = m_pqNodeQue.front();
		m_pqNodeQue.pop();

		// Access the node and it's index for future use
		curNode = curPathNode->GetNode();
		curNodeIndex = curNode->GetIndex();

		// If the destination is found
		if (curNodeIndex == _goal)
		{
			// Clear the old path
			m_lPath.clear();

			// Build the path
			theNode = m_umVisitedNodes[curNode->GetIndex()];

			// Untill we trace back to the _start node
			while (theNode != nullptr)
			{
				// Add the node to the path
				m_lPath.push_back(theNode->GetNode());

				// Advance to the next connected node
				theNode = theNode->m_cpParent;
			}
			if (smoothed)
				path = SmoothPath(m_lPath);
			else
			{
				for (size_t currNode = 0; currNode < m_lPath.size(); currNode++)
					path.push_back(m_lPath[currNode]->GetPosition());
			}

			
			

			
			
				/*m_umVisitedNodes.clear();
				^^^^^^^^^^^^^^^^^^^^^^^^^^
				This stupid line of code caused us so many headaches that I can't bring
				 myself to actually delete it. Forever it shall live in the belly of our 
				 pathfinding code*/

			return path;
		}

		// For each edge the node has
		for (size_t edge = 0; edge < curNode->GetEdges().size(); edge++)
		{
			// Get the current Edge and the node attatched to it
			curEdge = curNode->GetEdges()[edge];
			adjNodeIndex = curEdge->GetAdjNode();
			adjNode = m_pGraph->GetNodes()[adjNodeIndex];

			// Calculate the cost to the adj Node from _start Node (Total cost so far)
			curCost = curNode->GetCost() + curEdge->GetEdgeCost();

			// If the node has been visited
			if (m_umVisitedNodes[adjNodeIndex] != nullptr)
			{
				visitedNode = m_umVisitedNodes[adjNodeIndex];

				// The cost it took previously to get to the node
				oldCost = visitedNode->GetNode()->GetCost();

				// If the new cost is less the the old one
				if (curCost < oldCost)
				{
					m_pqNodeQue.remove(visitedNode);
					visitedNode->GetNode()->SetCost(curCost);

					newFinalCost = visitedNode->GetNode()->GetCost() + visitedNode->GetNode()->GetHeuristic() * m_fHeuristicWeight;
					visitedNode->GetNode()->SetFinalCost(newFinalCost);

					visitedNode->m_cpParent = curPathNode;
					m_pqNodeQue.push(visitedNode);
				}

			}
			else // If it has not been visited
			{
				// Set the cost adj notes cost (Total of what it takes to get here)
				newCost = curNode->GetCost() + curEdge->GetEdgeCost();
				adjNode->SetCost(newCost);

				// Set the HeuristicCost
				newHeuristic = CalculateCost(m_pGraph->GetNodes()[_goal]->GetPosition(), adjNode->GetPosition());
				adjNode->SetHeuristic(newHeuristic);

				// Set the new FinalCost // ?? 
				newFinalCost = newCost + newHeuristic * m_fHeuristicWeight;
				adjNode->SetFinalCost(newFinalCost);

				// Create a PathNode for the adj node (Note adj to the one I'm on)
				nextPathNode = new CPathNode(adjNode);

				// Set it's parent to the one I'm on (Coming from)
				nextPathNode->m_cpParent = curPathNode;

				// Add it to the que to be checked.
				m_pqNodeQue.push(nextPathNode);

				// Mark it as visited
				m_umVisitedNodes[adjNodeIndex] = nextPathNode;
			}
		}
	}
	return path;
}

/*****************************************************************
* SmoothPath()			Removes uneeded nodes from given path to smooth it out
*						it out.
*
* Ins:					vector<CNode*>&
*
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    09/01/2015
* Mod. Initials:	    CM
*****************************************************************/
vector<XMFLOAT3> CPathFinding::SmoothPath(vector<CNode*>& path)
{
	vector<XMFLOAT3> smoothedPath;
	if (path.size() > 2)
	{
		XMFLOAT3 checkPoint = path[0]->GetPosition();
		XMFLOAT3 currPoint = path[1]->GetPosition();

		smoothedPath.push_back(path[0]->GetPosition());
		for (size_t currNode = 1; currNode + 2 < path.size(); currNode++)
		{


			CCapsule* tempCapsule = new CCapsule(path[currNode + 2]->GetPosition(), checkPoint, 100.0f);
			CCollider* tempCollider = new CCollider(true, tempCapsule);

			std::vector<IObject*> colliders = m_pObjectManager->GetQuadTree()->GetNearbyObjects(tempCollider->GetBounds());
			std::vector<IObject*> hit;
			CheckCollision(tempCollider, colliders, hit);

			bool skip = false;
			for (size_t currHit = 0; currHit < hit.size(); currHit++)
			{
				if (hit[currHit]->GetTag() == "Wall")
				{
					checkPoint = currPoint;
					currPoint = path[currNode + 1]->GetPosition();
					skip = true;
					smoothedPath.push_back(path[currNode]->GetPosition());
					break;
				}
			}
			if (skip == false)
			{

				currPoint = path[currNode + 1]->GetPosition();
				
			}

			delete tempCollider;
		}

		
		//currIndex++;


	}
	smoothedPath.push_back(path[path.size() - 1]->GetPosition());
	return smoothedPath;

}


/*****************************************************************
* Walkable()		Checks to see if a segment of the path is blocked by something
*
* Ins:					XMFLOAT3
*						XMFLOAT3
*
* Outs:					None
*
* Returns:				bool
*
* Mod. Date:		    09/02/2015
* Mod. Initials:	    CM
*****************************************************************/
bool CPathFinding::Walkable(XMFLOAT3 start, XMFLOAT3 end)
{
	CCapsule c_pTestCapsule(start, end, 1.0f);
	CCollider c_pTestCollider(false, &c_pTestCapsule);

	//m_pObjectManager->GetQuadTree()->GetNearbyObjects()


	return true;
}





/*****************************************************************
* CalculateCost()		Calculates the cost in form of distnace between
*						two points.
* Ins:					XMFLOAT3
*						XMFLOAT3
*
* Outs:					None
*
* Returns:				Float
*
* Mod. Date:		    08/16/2015
* Mod. Initials:	    NS
*****************************************************************/
float CPathFinding::CalculateCost(XMFLOAT3 _to, XMFLOAT3 _from)
{
	XMFLOAT3 vTemp;

	vTemp.x = pow((_from.x - _to.x), 2);
	vTemp.y = pow((_from.y - _to.y), 2);
	vTemp.z = pow((_from.z - _to.z), 2);

	return (float)sqrt(vTemp.x + vTemp.y + vTemp.z);
}

/*****************************************************************
* FindClosestNode()		Finds the closest graph node to a spesified
*						position by going through all nodes.
* Ins:					XMFLOAT3
*
* Outs:					int
*
* Returns:				Float
*
* Mod. Date:		    08/16/2015
* Mod. Initials:	    NS
*****************************************************************/
int CPathFinding::FindClosestNode(const XMFLOAT3* _position)
{
	// remember the closes node
	float fShortestDistance = FLT_MAX;
	int nClosestNode = -1;

	// Loop through all the nodes on the graph
	for (size_t nNode = 0; nNode < m_pGraph->GetNodes().size(); nNode++)
	{
		// Calculate the distance from the node to the target
		float fCurDistance = CalculateCost(*_position, m_pGraph->GetNodes()[nNode]->GetPosition());
		if (fCurDistance < fShortestDistance || nClosestNode < 0.0f)
		{
			// Update the closest node
			nClosestNode = (int)nNode;

			// Update the distance to the closest node
			fShortestDistance = fCurDistance;
		}
	}
	return nClosestNode;
}

/*****************************************************************
* GetReadyForNewSearch()	Clean up memory for next search
*
* Ins:						XMFLOAT3
*
* Outs:						int
*
* Returns:					Float
*
* Mod. Date:			    08/16/2015
* Mod. Initials:		    NS
*****************************************************************/
void CPathFinding::GetReadyForNewSearch()
{
	//// Get ready for a new search
	m_pqNodeQue.clear();

	auto iter = m_umVisitedNodes.begin();
	while (iter != m_umVisitedNodes.end())
	{
		SAFE_DELETE(iter->second)
		iter++;
	}


	m_umVisitedNodes.clear();
	m_lPath.clear();

	//	for (size_t node = 0; node < m_pGraph->GetNodes().size(); node++)
	//		m_pGraph->GetNodes()[node]->SetCost(0.0f);

	//m_umVisitedNodes.reserve(400);
	//m_vPath.resize(100);
}

/*****************************************************************
* isGreater()		Finds the closest graph node to a spesified
*						position by going through all nodes.
* Ins:					XMFLOAT3
*
* Outs:					int
*
* Returns:				Float
*
* Mod. Date:		    08/16/2015
* Mod. Initials:	    NS
*****************************************************************/
bool isGreater(CPathNode* const &_lhs, CPathNode* const &_rhs)
{
	return (_lhs->GetNode()->GetFinalCost() > _rhs->GetNode()->GetFinalCost());
}


CPathNode::CPathNode(CNode* _node) : m_cpNode(_node)
{
}

CPathNode::~CPathNode()
{

}

bool CPathNode::operator() (const CPathNode* _lhs, const CPathNode* _rhs) const
{

	return (_lhs->GetNode()->GetFinalCost() > _rhs->GetNode()->GetFinalCost());

}