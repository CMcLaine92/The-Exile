#include "CutsceneManager.h"
#include "CameraController.h"
#include "Cutscene.h"
#include "../../Game/Source/Player/Player.h"
CCutsceneManager::CCutsceneManager(CPlayer* pcPlayer)
{
	m_bCutsceneActive = false;
	m_unActiveCutscene = 0;
	m_pcPlayer = pcPlayer;
}
CCutsceneManager::~CCutsceneManager()
{
	for (unsigned int i = 0; i < m_vpCutscenes.size(); i++)
	{
		delete m_vpCutscenes[i];
	}
	m_vpCutscenes.clear();
}
void CCutsceneManager::Update()
{
	if (m_bCutsceneActive == true)
	{
		//Update Active Cutscene
		m_vpCutscenes[m_unActiveCutscene]->Update();
		
		//Stop if over
		if (m_vpCutscenes[m_unActiveCutscene]->IsOver())
		{
			Stop();
		}
	}
}
unsigned int CCutsceneManager::LoadCutscene(std::string szFileName)
{
	//Load From XML
	CCutscene* pcCutscene = new CCutscene();
	pcCutscene->Initialize(szFileName);
	pcCutscene->GetCameraController()->SetCamera(m_pcCamera);
	m_vpCutscenes.push_back(pcCutscene);

	return m_vpCutscenes.size()-1;
}
CCutscene* CCutsceneManager::GetCutscene(unsigned int unIndex)
{
	//Invalid Index Check
	if (unIndex > m_vpCutscenes.size() - 1)
		return nullptr;

	return m_vpCutscenes[unIndex];
}
void CCutsceneManager::PlayCutscene(unsigned int unIndex)
{
	if (unIndex < m_vpCutscenes.size())
	{
		m_unActiveCutscene = unIndex;
		m_bCutsceneActive = true;
		m_pcPlayer->GetActive() = false;
	}
}
void CCutsceneManager::Stop()
{
	m_bCutsceneActive = false;
	m_vpCutscenes[m_unActiveCutscene]->Reset();
	m_pcPlayer->GetActive() = true;
}