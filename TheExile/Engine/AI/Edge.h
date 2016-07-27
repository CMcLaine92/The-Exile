/***********************************************
* Filename:  		Edge.h
* Date:      		08/16/2015
* Mod. Date: 		----------
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:   		Edge class to connect the nodes in the graph
*
************************************************/
#ifndef EDGE_H
#define EDGE_H


class CEdge
{

	// Distance between the nodes
	float m_fCost;

	// Node the edge is connected to
	int m_nAdjNode;

	// Is the edge passable (bridges, cracked walls)
	bool m_bPassable;

public:
	CEdge();
	CEdge(float _cost, int _node);
	~CEdge();
	
	bool operator< (const CEdge& _rhs) const;

	// Assesors
	int GetAdjNode() { return m_nAdjNode; }
	float GetEdgeCost() { return m_fCost; }
};

#endif