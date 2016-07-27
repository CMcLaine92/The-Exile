#include "CameraFrame.h"
CCameraFrame::CCameraFrame(float fFrameTime, int nInterpolate, XMFLOAT4X4 f44StartLocation)
{
	m_fFrameTime = fFrameTime;
	m_fElapsedTime = 0.0f;
	m_nInterpolate = nInterpolate;
	m_f44StartWorld = f44StartLocation;
	m_f44World = f44StartLocation;
}
CCameraFrame::~CCameraFrame()
{

}
void CCameraFrame::Update()
{
	m_fElapsedTime += DELTA_TIME();
}
void CCameraFrame::Reset()
{
	m_fElapsedTime = 0.0f;
	m_f44World = m_f44StartWorld;
}