/***********************************************
* Filename:  		Edge.cpp
* Date:      		08/16/2015
* Mod. Date: 		----------
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:   		Edge class to connect the nodes in the graph
*					
************************************************/
#include "Edge.h"


/*****************************************************************
* CEdge()				Constructor
*
* Ins:					None
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NS
*****************************************************************/
CEdge::CEdge()
{
}

/*****************************************************************
* CEdge()				Overloaded Constructor
*
* Ins:					float
*						int
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NS
*****************************************************************/
CEdge::CEdge(float _cost, int _node) : m_fCost(_cost),
									   m_nAdjNode(_node)
{

}

CEdge::~CEdge()
{
}

/*****************************************************************
* operator<()			Overloaded less then opperator.
*
* Ins:					CEdge&
*
* Outs:					None
*
* Returns:				bool
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NS
*****************************************************************/
bool CEdge::operator< (const CEdge& _rhs) const
{

	return (this->m_fCost < _rhs.m_fCost);

}
