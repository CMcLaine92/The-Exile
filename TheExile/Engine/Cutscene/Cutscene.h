#ifndef _CUTSCENE_H_
#define _CUTSCENE_H_
class CCameraController;
class CCutscene
{
private:
	float m_fElapsedTime;
	float m_fCutsceneTime;
	bool m_bIsOver;
	CCameraController* m_cCameraController;
public:
	CCutscene();
	~CCutscene();
	void Update();
	void Reset();
	void Initialize(std::string szFilename);

	void SetCameraController(CCameraController* cCameraController) { m_cCameraController = cCameraController; }
	CCameraController* GetCameraController() { return m_cCameraController; }

	bool IsOver(){ return m_bIsOver; }
};
#endif