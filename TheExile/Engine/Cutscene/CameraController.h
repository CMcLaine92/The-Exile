#ifndef _CAMERACONTROLLER_H_
#define _CAMERACONTROLLER_H_
class CCameraFrame;
class CCutscene;
class CCamera;
class CCameraController
{
private:
	std::vector<CCameraFrame*> m_vpCameraFrames;
	unsigned int m_unCurrentFrame;
	CCutscene* m_pCutscene;
	//Matrix to Reset Camera To After Cutscene Ends
	XMFLOAT4X4 m_f44CameraReturnLocation;
	CCamera* m_pcCamera = nullptr;
	void LerpCamera();
public:
	CCameraController(CCutscene* pOwnerCutscene);
	~CCameraController();
	void SetCamera(CCamera* _camera);
	void Update();
	void Reset();

	std::vector<CCameraFrame*>& GetFrames() { return m_vpCameraFrames; }
	XMFLOAT4X4& GetCameraReset(){ return m_f44CameraReturnLocation; }
};

#endif