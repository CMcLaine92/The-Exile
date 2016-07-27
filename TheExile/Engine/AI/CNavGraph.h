/***********************************************
* Filename:  		NavGrap.h
* Date:      		08/16/2015
* Mod. Date: 		----------
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:   		Generates nodes from a navigation
*					mesh for Agents to pathfind.
************************************************/
#ifndef _NAVMESH_H
#define _NAVMESH_H

#include "Node.h"

class CMesh;

struct TTriangle
{
	vector<int> nvVertIndex;
};

struct TTri
{
	float3 verts[3];
	float3 centroid;
	vector<int> edges;

};


class CNavGraph
{

protected:
	// The Node Graph
	vector<CNode*> m_vNodes;

	// Triangles for node generation
	vector<TTriangle> m_vTriangles;

public:
	CNavGraph() : cNavMesh(nullptr){}
	CNavGraph(CMesh* _navMesh);

	~CNavGraph();


	/*********************************
		ACCESSORS
	************************************/
	vector<CNode*>& GetNodes() { return m_vNodes; }
	CNode* GetNodeAtIndex(int _index){ return m_vNodes[_index]; }

	bool BuildNavGraph(CMesh* _navMesh);
	void AddNode(CNode * temp) { m_vNodes.push_back(temp); }
	 CMesh* GetNavMesh()  { return cNavMesh; }
	void Clear();
	void InsertNode(CNode* node);

private:

	// Only instance of the Graph
	
	CMesh* cNavMesh;
	

	/*********************************
		HELPER
	************************************/
	void GenerateNavGraph();
	//float CalculateEdgeCost(CNode* to, CNode* from);

	void BuildTriangles(CMesh& _navMesh);
	float CalculateCost(XMFLOAT3 _to, XMFLOAT3 _from);
	XMFLOAT3 CalculateCenter(TTriangle _tri, CMesh& _navMesh);

};


#endif