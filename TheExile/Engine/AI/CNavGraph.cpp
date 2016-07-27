/***********************************************
* Filename:  		NavGrap.cpp
* Date:      		08/16/2015
* Mod. Date: 		----------
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:   		Generates nodes from a navigation
*					mesh for Agents to pathfind.
************************************************/
#include "CNavGraph.h"
#include "../Animation/Mesh.h"


/*****************************************************************
* ~CNavGraph()			Destructor
*
* Ins:					None
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    09/06/2015
* Mod. Initials:	    CM
*****************************************************************/
CNavGraph::~CNavGraph()
{
	Clear();
}
/*****************************************************************
* CNavGraph(CMesh* _navMesh)	Power constructor builds NavGraph
*								on Contruction.
*
* Ins:							CMesh*
*
* Outs:							None
*
* Returns:						None
*
* Mod. Date:					09/06/2015
* Mod. Initials:				CM
*****************************************************************/
CNavGraph::CNavGraph(CMesh* _navMesh)
{
	BuildNavGraph(_navMesh);
}

/*****************************************************************
* ~Clear()			    Cleans up the memory and clears out the graph
*						for deletion or reuse.
*
* Ins:					None
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    09/06/2015
* Mod. Initials:	    CM
*****************************************************************/
void CNavGraph::Clear()
{
	for (size_t node = 0; node < m_vNodes.size(); node++)
	{
		delete m_vNodes[node];
	}
	m_vNodes.clear();
	m_vTriangles.clear();
}





/*****************************************************************
* GenerateNavGraph()	Builds Nodes for each triangle on the mesh
*						and connects them with Edges
*
* Ins:					vector<CMesh>
*
* Outs:					None
*
* Returns:				Void
*
* Mod. Date:		    08/16/2015
* Mod. Initials:	    NS
*****************************************************************/
void CNavGraph::GenerateNavGraph()
{
	// Build the triangles
	BuildTriangles(*cNavMesh);

	// - Loop through all the triangles.
	for (size_t nCurTri = 0; nCurTri < m_vTriangles.size(); nCurTri++)
	{
		// For each triangle, calculate center and create a node.
		XMFLOAT3 vecNodePos = CalculateCenter(m_vTriangles[nCurTri], *cNavMesh);
		CNode* newNode = new CNode(nCurTri, vecNodePos);
		m_vNodes.push_back(newNode);

		bool bNextTriangle = false;

		// For every other triangle
		for (size_t nOtherTri = 0; nOtherTri < m_vTriangles.size(); nOtherTri++)
		{
			// Avoid compaing to self
			if (nCurTri == nOtherTri)
				continue;

			// For every vert in Current Triangle
			int sharedVerts = 0;

			for (size_t nCurVert = 0; nCurVert < 3; nCurVert++)
			{
				if (bNextTriangle)
				{
					bNextTriangle = false;
					sharedVerts = 0;
					break;
				}

				// For every vert in Other Triangle
				for (size_t nOtherVert = 0; nOtherVert < 3; nOtherVert++)
				{
					// Compare if Current Triangle shares any vert with any Other Triangle

					if (m_vTriangles[nCurTri].nvVertIndex[nCurVert] == m_vTriangles[nOtherTri].nvVertIndex[nOtherVert])
					{
						// avoid comaring to self
						sharedVerts++;
						if (sharedVerts > 1)
						{
							// Calculate the cost to travel the edge
							float fCost = CalculateCost((XMFLOAT3)cNavMesh->GetVertices()[nOtherVert].m_fPosition,
								(XMFLOAT3)cNavMesh->GetVertices()[nCurVert].m_fPosition);

							// Create the edge and add it to the Node && Unique list
							CEdge* newEdge = new CEdge(fCost, nOtherTri);
							m_vNodes[nCurTri]->addEdge(newEdge);

							// Move on to the next triangle
							bNextTriangle = true;
							break;
						}

					}
				}
			}
		}
	}
}


/*****************************************************************
* BuildTriangles()		Builds triangles from a FBX loaded mesh.
*
* Ins:					vector<CMesh>
*
* Outs:					None
*
* Returns:				Void
*
* Mod. Date:		    08/16/2015
* Mod. Initials:	    NS
*****************************************************************/
void CNavGraph::BuildTriangles(CMesh& _navMesh)
{

	int nCounter = 0;
	//m_vTriangles.push_back(TTriangle());
	//	TTriangle test;
	//m_vTriangles.push_back(test);
	size_t test = _navMesh.GetIndices().size();


	m_vTriangles.resize(_navMesh.GetIndices().size() / 3);

	for (size_t nCurVert = 0; nCurVert < _navMesh.GetIndices().size(); nCurVert++)
	{
		m_vTriangles[nCounter].nvVertIndex.push_back(_navMesh.GetIndices()[nCurVert]);

		// Incroment counter every three verts
		if ((nCurVert + 1) % 3 == 0)
		{
			nCounter++;
		}
	}

}
bool CNavGraph::BuildNavGraph(CMesh* _navMesh)
{
	if (_navMesh)
	{
		cNavMesh = _navMesh;
		GenerateNavGraph();
		return true;
	}
	return false;
}

/*****************************************************************
* BuildTriangles()		Builds triangles from a FBX loaded mesh.
*
* Ins:					TTriangle
*						vector<CMesh>
*
* Outs:					None
*
* Returns:				XMFLOAT3
*
* Mod. Date:		    08/16/2015
* Mod. Initials:	    NS
*****************************************************************/
XMFLOAT3 CNavGraph::CalculateCenter(TTriangle _tri, CMesh& _navMesh)
{
	XMFLOAT3 avgPos = XMFLOAT3(0, 0, 0);
	for (size_t i = 0; i < _tri.nvVertIndex.size(); i++)
	{
		int nIndex = _tri.nvVertIndex[i];
		avgPos.x += _navMesh.GetVertices()[nIndex].m_fPosition[0];
		avgPos.y += _navMesh.GetVertices()[nIndex].m_fPosition[1];
		avgPos.z += _navMesh.GetVertices()[nIndex].m_fPosition[2];
		_navMesh.GetVertices()[nIndex].m_fPosition[1] = 50.0f;
	}

	avgPos.x /= 3;
	avgPos.y = 0.0f;
	avgPos.z /= 3;
	return avgPos;
}

void CNavGraph::InsertNode(CNode* node)
{
	for (int _node = 0; _node < (int)GetNodes().size(); _node++)
	{
		CNode* currNode = GetNodes()[_node];


		if (node == currNode)
			continue;

		if ((node->GetPosition().x == currNode->GetPosition().x && fabs(node->GetPosition().z - currNode->GetPosition().z) == 1000)
			|| (node->GetPosition().z == currNode->GetPosition().z && fabs(node->GetPosition().x - currNode->GetPosition().x) == 1000))
		{
			CEdge* currNodeEdge = new CEdge(1.0f, _node);
			CEdge* newNodeEdge = new CEdge(1.0f, GetNodes().size());

			node->addEdge(newNodeEdge);
			currNode->addEdge(currNodeEdge);
		}
	}

	AddNode(node);
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
float CNavGraph::CalculateCost(XMFLOAT3 _to, XMFLOAT3 _from)
{
	XMFLOAT3 vTemp;

	vTemp.x = pow((_from.x - _to.x), 2);
	vTemp.y = pow((_from.y - _to.y), 2);
	vTemp.z = pow((_from.z - _to.z), 2);

	return (float)sqrt(vTemp.x + vTemp.y + vTemp.z);
}






