#include "NavGraphManager.h"
#include "CNavGraph.h"
CNavGraphManager CNavGraphManager::m_sInstance;

CNavGraphManager& CNavGraphManager::GetReference()
{
	static CNavGraphManager m_sInstance;
	return m_sInstance;
}

CNavGraphManager::~CNavGraphManager()
{
	Clear();
}
CNavGraph* CNavGraphManager::GetNavGraph(const char* name)
{
	if (name)
		return m_umNavGraphs[name];
	return nullptr;
}
void CNavGraphManager::AddNavGraph(const char* name, CNavGraph* newGraph)
{
	if (m_umNavGraphs.find(name) == m_umNavGraphs.end())
	{
		m_umNavGraphs[name] = newGraph;
	}
}

void CNavGraphManager::Clear()
{
	for (auto iter = m_umNavGraphs.begin(); iter != m_umNavGraphs.end(); iter++)
		SAFE_DELETE(iter->second);
	m_umNavGraphs.clear();
}
