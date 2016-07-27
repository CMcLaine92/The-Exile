/***********************************************
* Filename:  		PathFinding.h
* Date:      		08/16/2015
* Mod. Date: 		----------
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:   		Finds a path using a generated Node Graph
*					for Agents to move a long through A* Pathfinding.
************************************************/
#ifndef _PATHFINDING_H
#define _PATHFINDING_H

#include "PriorityQueue.h"

#include <unordered_map>
#include <functional>
#include <queue>

using namespace fullsail_ai;

class CPathNode;
class CObjectManager;
class CRenderMesh;
class CNavGraph;
class CNode;
bool isGreater(CPathNode* const &, CPathNode* const &);

class CPathFinding
{

protected:
	CNavGraph* m_pGraph = nullptr;

	// The Path to take
	//vector<CNode*> m_vPath;
	vector<CNode*> m_lPath;

	// Nodes that have been visited during this search
	unordered_map<int, CPathNode*> m_umVisitedNodes;

	CObjectManager* m_pObjectManager;

	CRenderMesh* m_pNavRenderMesh;
	// Nodes to be visited, sorted by Cost
	PriorityQueue<CPathNode*> m_pqNodeQue = PriorityQueue<CPathNode*>(isGreater);

	// Heuristic Weight
	float m_fHeuristicWeight;

public:

	CPathFinding(const char* navGraphName, CObjectManager* objManager, bool render = false);
	~CPathFinding();

	vector<XMFLOAT3> GeneratePath(int _start, int _goal, bool smoothed);
	int FindClosestNode(const XMFLOAT3* _position);

	/*********************************
		ACCESSOR
	************************************/
	CNavGraph* GetGraph() const { return m_pGraph; }

private:



	/*********************************
	HELPER
	************************************/
	float CalculateCost(XMFLOAT3 _to, XMFLOAT3 _from);

	vector<XMFLOAT3> SmoothPath(vector<CNode*>& path);
	bool Walkable(XMFLOAT3 start, XMFLOAT3 end);

	void GetReadyForNewSearch();

	bool m_bIsRendering;
};

class CPathNode
{
public:
	CPathNode(CNode* _node);
	~CPathNode();

	bool operator() (const CPathNode* _lhs, const CPathNode* _rhs) const;

	CPathNode* m_cpParent = nullptr;


	CNode* GetNode() const { return m_cpNode; }
private:
	CNode* m_cpNode;
};

#endif
