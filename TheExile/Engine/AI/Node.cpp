#include "Node.h"

CNode::CNode()
{
}

CNode::CNode(int _index, XMFLOAT3 _pos) : m_nIndex(_index), m_vecPosition(_pos)
{

}


CNode::~CNode()
{
	VECTOR_DELETE_ALL(m_vEdges);
	m_vEdges.clear();
}

void CNode::addEdge(CEdge* _edge)
{
	m_vEdges.push_back(_edge);
	
}

bool CNode::operator< (const CNode& _rhs) const
{

	return (this->m_fCostToNode < _rhs.m_fCostToNode);

}

