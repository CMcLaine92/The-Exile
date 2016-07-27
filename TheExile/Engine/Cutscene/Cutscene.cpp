#include "Cutscene.h"
#include "CameraController.h"
#include "CameraFrame.h"
#include "../Particles/tinyxml/tinyxml.h"
CCutscene::CCutscene()
{
	m_fElapsedTime = 0.0f;
	m_fCutsceneTime = 0.0f;
	m_bIsOver = false;
	m_cCameraController = nullptr;
}
CCutscene::~CCutscene()
{
	delete m_cCameraController;
}
void CCutscene::Update()
{
	m_fElapsedTime += DELTA_TIME();
	if (m_fElapsedTime > m_fCutsceneTime)
	{
		m_bIsOver = true;
		return;
	}
	m_cCameraController->Update();
}
void CCutscene::Reset()
{
	m_fElapsedTime = 0.0f;
	m_bIsOver = false;
	m_cCameraController->Reset();
}
void CCutscene::Initialize(std::string szFilename)
{
	m_cCameraController = new CCameraController(this);
	if (szFilename == "Test")
	{
		XMFLOAT4X4 f44World = { 
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 500, 0, 1 };
		CCameraFrame* pCameraFrame = new CCameraFrame(5.0f, false, f44World);
		m_cCameraController->GetFrames().push_back(pCameraFrame);
		m_fCutsceneTime = 3.0f;
	}
	else
	{
		//Load from XML TODO
		std::string szFullFilepath;
		szFullFilepath = "../Game/Assets/Scripts/Cutscene/" + szFilename;
		TiXmlDocument Doc;
		if (Doc.LoadFile(szFullFilepath.c_str(), TiXmlEncoding::TIXML_ENCODING_UTF8))
		{
			TiXmlElement * root = Doc.RootElement();
			TiXmlElement * CameraFrameElmt = root->FirstChildElement("CameraFrame");
			m_fCutsceneTime = 0.0f;
			while (CameraFrameElmt != nullptr)
			{
				double dLook[3];
				double dPos[3];
				double dFrameTime;
				int nShouldInterpolate;
				CameraFrameElmt->Attribute("lookDirX", &dLook[0]);
				CameraFrameElmt->Attribute("lookDirY", &dLook[1]);
				CameraFrameElmt->Attribute("lookDirZ", &dLook[2]);
				CameraFrameElmt->Attribute("posX", &dPos[0]);
				CameraFrameElmt->Attribute("posY", &dPos[1]);
				CameraFrameElmt->Attribute("posZ", &dPos[2]);
				CameraFrameElmt->Attribute("frameTime", &dFrameTime);
				CameraFrameElmt->Attribute("shouldInterpolate", &nShouldInterpolate);

				//Do Look At Algorithm to find Matrix of Frame

				XMFLOAT4X4 f44WorldMatrix;
				XMVECTOR vecZAxis = { (float)dLook[0], (float)dLook[1], (float)dLook[2], 0 };
				vecZAxis = XMVector3Normalize(vecZAxis);
				XMVECTOR vecUp = { 0, 1, 0, 0 };
				XMVECTOR vecXAxis = XMVector3Cross(vecUp, vecZAxis);
				vecXAxis = XMVector3Normalize(vecXAxis);
				XMVECTOR vecYAxis = XMVector3Cross(vecZAxis, vecXAxis);
				vecYAxis = XMVector2Normalize(vecYAxis);
				f44WorldMatrix.m[0][0] = vecXAxis.m128_f32[0];
				f44WorldMatrix.m[0][1] = vecXAxis.m128_f32[1];
				f44WorldMatrix.m[0][2] = vecXAxis.m128_f32[2];
				f44WorldMatrix.m[0][3] = 0;
				f44WorldMatrix.m[1][0] = vecYAxis.m128_f32[0];
				f44WorldMatrix.m[1][1] = vecYAxis.m128_f32[1];
				f44WorldMatrix.m[1][2] = vecYAxis.m128_f32[2];
				f44WorldMatrix.m[1][3] = 0;
				f44WorldMatrix.m[2][0] = vecZAxis.m128_f32[0];
				f44WorldMatrix.m[2][1] = vecZAxis.m128_f32[1];
				f44WorldMatrix.m[2][2] = vecZAxis.m128_f32[2];
				f44WorldMatrix.m[2][3] = 0;
				f44WorldMatrix.m[3][0] = (float)dPos[0];
				f44WorldMatrix.m[3][1] = (float)dPos[1];
				f44WorldMatrix.m[3][2] = (float)dPos[2];
				f44WorldMatrix.m[3][3] = 1;
				CCameraFrame* pCameraFrame = new CCameraFrame((float)dFrameTime, nShouldInterpolate,f44WorldMatrix);
				m_cCameraController->GetFrames().push_back(pCameraFrame);
				m_fCutsceneTime += (float)dFrameTime;
				CameraFrameElmt = CameraFrameElmt->NextSiblingElement("CameraFrame");
			}
		}
	}
}