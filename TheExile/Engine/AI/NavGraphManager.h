/***********************************************
* Filename:  		NavGrapManager.h
* Date:      		09/06/2015
* Mod. Date: 		----------
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:   		Handles all of the NavGraphs made
************************************************/
#ifndef _NAVGRAPHMANAGER_H
#define _NAVGRAPHMANAGER_H
#include <unordered_map>
class CNavGraph;

class CNavGraphManager
{

public:
	~CNavGraphManager();
	CNavGraph* GetNavGraph(const char* name);
	void AddNavGraph(const char* name, CNavGraph* newGraph);
	void Clear();

	static CNavGraphManager& GetReference();

private:
	CNavGraphManager() = default;
	CNavGraphManager(const CNavGraphManager&) = delete;

	static CNavGraphManager m_sInstance;


	std::unordered_map<std::string, CNavGraph*> m_umNavGraphs;


};


#endif