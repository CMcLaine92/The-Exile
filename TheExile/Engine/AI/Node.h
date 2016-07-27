#pragma once
#include "Edge.h"

class CNode
{

	int m_nIndex;

	XMFLOAT3 m_vecPosition;
	
	// Vector of connections // maybe hold indexes?
	vector<CEdge*> m_vEdges;

	// Cost to this edge for each search
	float m_fCostToNode = 0;

	float m_fHeuristic;
	float m_fWeight;
	float m_fFinalCost;

public:
	CNode();
	CNode(int _index, XMFLOAT3 _pos);
	~CNode();

	void addEdge(CEdge* _edge);

	bool operator< (const CNode& _rhs) const;


	// Accessors
	float GetWeight() { return m_fWeight; }
	int GetIndex() { return m_nIndex; }
	float GetCost() { return m_fCostToNode; }
	float GetFinalCost(){ return m_fFinalCost; }
	float GetHeuristic() { return m_fHeuristic; }
	XMFLOAT3 GetPosition() { return m_vecPosition; }
	vector<CEdge*> GetEdges() { return m_vEdges; }


	// Mutators
	void SetCost(float _cost) { m_fCostToNode = _cost; }
	void SetWeight(float _weight) { m_fWeight = _weight; }
	void SetFinalCost(float _cost) { m_fFinalCost = _cost; }
	void SetHeuristic(float _heur) { m_fHeuristic = _heur; }
};

