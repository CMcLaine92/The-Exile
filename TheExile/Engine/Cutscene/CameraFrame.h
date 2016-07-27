#ifndef _CAMERAFRAME_H_
#define _CAMERAFRAME_H_
class CCameraFrame
{
private:
	//End Time of Frame
	float m_fFrameTime;
	//Time since Frame Started
	float m_fElapsedTime;
	//Int interpolate type 0 = jumpcut 1 = Lerp 2 = Slerp
	int m_nInterpolate;
	XMFLOAT4X4 m_f44World;
	XMFLOAT4X4 m_f44StartWorld;
public:
	CCameraFrame(float fFrameTime, int nInterpolate, XMFLOAT4X4 f44StartLocation);
	~CCameraFrame();
	void Update();
	void Reset();

	XMFLOAT4X4& GetWorld() { return m_f44World; }
	XMFLOAT4X4& GetStart() { return m_f44StartWorld; }
	int GetInterpolate() { return m_nInterpolate; }
	float& GetElapsedTime() { return m_fElapsedTime; }
	float& GetFrameTime() { return m_fFrameTime; }
};
#endif