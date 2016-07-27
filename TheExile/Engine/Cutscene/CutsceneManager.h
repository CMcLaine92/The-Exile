#ifndef _CUTSCENEMANAGER_H_
#define _CUTSCENEMANAGER_H_

class CCutscene;
class CPlayer;
class CCamera;
class CCutsceneManager
{
private:
	std::vector<CCutscene*> m_vpCutscenes;
	bool m_bCutsceneActive;
	unsigned int m_unActiveCutscene;
	CPlayer* m_pcPlayer;
	CCamera* m_pcCamera;
public:
	CCutsceneManager(CPlayer* pPlayer);
	~CCutsceneManager();
	void Update();
	unsigned int LoadCutscene(std::string szFileName);
	CCutscene* GetCutscene(unsigned int unIndex);
	void PlayCutscene(unsigned int unIndex);
	void Stop();

	void SetCamera(CCamera* _camera) { m_pcCamera = _camera; }
	unsigned int GetActiveCutscene() { return m_unActiveCutscene; }
	bool IsCutsceneActive() { return m_bCutsceneActive; }
	CPlayer* GetPlayer() {return m_pcPlayer;}
};

#endif