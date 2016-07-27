#include "CameraController.h"
#include "CameraFrame.h"
#include "../Core/State Machine/StateMachine.h"
#include "../Core/State Machine/States/GameplayState.h"
#include "../Object Manager/ObjectManager.h"
#include "../../Game/Source/Camera/Camera.h"
#include "../Renderer/Renderer.h"
#define XMF3DotProduct(a,b) (a.x*b.x + a.y*b.y + a.z*b.z)
#define XMF44XAxisDotProduct(a,b) (a._11*b._11 + a._12*b._12 + a._13*b._13)
#define XMF44YAxisDotProduct(a,b) (a._21*b._21 + a._22*b._22 + a._23*b._23)
#define XMF44ZAxisDotProduct(a,b) (a._31*b._31 + a._32*b._32 + a._33*b._33)
#define XMF44WAxisDotProduct(a,b) (a._41*b._41 + a._42*b._42 + a._43*b._43)
void Slerp(XMFLOAT3& f3Start, XMFLOAT3& f3End,float fRatio, XMFLOAT3& f3Output)
{
	//Slerp Around Origin
	float fStartDistance = sqrt(XMF3DotProduct(f3Start, f3Start));
	float fEndDistance = sqrt(XMF3DotProduct(f3End, f3End));
	float fCurrentDistance = fStartDistance + (fEndDistance - fStartDistance)*fRatio;

	XMFLOAT3 f3StartNormed = f3Start;
	XMFLOAT3 f3EndNormed = f3End;

	f3StartNormed.x /= fStartDistance;
	f3StartNormed.y /= fStartDistance;
	f3StartNormed.z /= fStartDistance;

	f3EndNormed.x /= fEndDistance;
	f3EndNormed.y /= fEndDistance;
	f3EndNormed.z /= fEndDistance;

	float fDot = XMF3DotProduct(f3StartNormed, f3EndNormed);

	fDot = min(1.0f, fDot);
	fDot = max(-1.0f, fDot);

	float fTheta = acosf(fDot) * fRatio;
	XMFLOAT3 f3RelativeVec = f3EndNormed;
	f3RelativeVec.x -= f3StartNormed.x * fDot;
	f3RelativeVec.y -= f3StartNormed.y * fDot;
	f3RelativeVec.z -= f3StartNormed.z * fDot;

	float fRelDist = sqrt(XMF3DotProduct(f3RelativeVec, f3RelativeVec));
	f3RelativeVec.x /= fRelDist;
	f3RelativeVec.y /= fRelDist;
	f3RelativeVec.z /= fRelDist;

	f3Output.x = ((f3StartNormed.x * cos(fTheta)) + (f3RelativeVec.x * sin(fTheta)));
	f3Output.y = ((f3StartNormed.y * cos(fTheta)) + (f3RelativeVec.y * sin(fTheta)));
	f3Output.z = ((f3StartNormed.z * cos(fTheta)) + (f3RelativeVec.z * sin(fTheta)));

	f3Output.x *= fCurrentDistance;
	f3Output.y *= fCurrentDistance;
	f3Output.z *= fCurrentDistance;
}
void Slerp(XMFLOAT4X4& f44Start, XMFLOAT4X4& f44End, float fRatio, XMFLOAT4X4& f44Output)
{
	XMFLOAT3 f3Start = XMFLOAT3(f44Start._41, f44Start._42, f44Start._43);
	XMFLOAT3 f3End = XMFLOAT3(f44End._41, f44End._42, f44End._43);
	XMFLOAT3 f3Ouput;

	Slerp(f3Start, f3End, fRatio, f3Ouput);
	f44Output._41 = f3Ouput.x;
	f44Output._42 = f3Ouput.y;
	f44Output._43 = f3Ouput.z;
}
void CCameraController::LerpCamera()
{
	unsigned int unNextFrame = m_unCurrentFrame+1;
	if (unNextFrame >= m_vpCameraFrames.size())
	{
		unNextFrame = m_unCurrentFrame;
	}
	CCameraFrame* pCurrentFrame = m_vpCameraFrames[m_unCurrentFrame];
	float fRatio = pCurrentFrame->GetElapsedTime() / pCurrentFrame->GetFrameTime();
	XMFLOAT4X4& f44World = pCurrentFrame->GetWorld();
	XMFLOAT4X4 f44StartWorld = pCurrentFrame->GetStart();
	XMFLOAT4X4 f44EndWorld = m_vpCameraFrames[unNextFrame]->GetStart();
	//Lerp Position
	if (pCurrentFrame->GetInterpolate() == 1)
	{
		f44World._41 = f44StartWorld._41 + (f44EndWorld._41 - f44StartWorld._41)*fRatio;
		f44World._42 = f44StartWorld._42 + (f44EndWorld._42 - f44StartWorld._42)*fRatio;
		f44World._43 = f44StartWorld._43 + (f44EndWorld._43 - f44StartWorld._43)*fRatio;
	}
	else
	{
		Slerp(f44StartWorld, f44EndWorld, fRatio, f44World);
	}
	//Lerp Rotation or Slerp Rotation or Nlerp Rotation
	//NLerp
	//XAxis
	f44World._11 = f44StartWorld._11 + (f44EndWorld._11 - f44StartWorld._11)*fRatio;
	f44World._12 = f44StartWorld._12 + (f44EndWorld._12 - f44StartWorld._12)*fRatio;
	f44World._13 = f44StartWorld._13 + (f44EndWorld._13 - f44StartWorld._13)*fRatio;
	//Normalize
	float fDot = sqrt(XMF44XAxisDotProduct(f44World, f44World));
	f44World._11 /= fDot;
	f44World._12 /= fDot;
	f44World._13 /= fDot;
	//YAxis
	f44World._21 = f44StartWorld._21 + (f44EndWorld._21 - f44StartWorld._21)*fRatio;
	f44World._22 = f44StartWorld._22 + (f44EndWorld._22 - f44StartWorld._22)*fRatio;
	f44World._23 = f44StartWorld._23 + (f44EndWorld._23 - f44StartWorld._23)*fRatio;
	//Normalize
	fDot = sqrt(XMF44YAxisDotProduct(f44World, f44World));
	f44World._21 /= fDot;
	f44World._22 /= fDot;
	f44World._23 /= fDot;
	//ZAxis
	f44World._31 = f44StartWorld._31 + (f44EndWorld._31 - f44StartWorld._31)*fRatio;
	f44World._32 = f44StartWorld._32 + (f44EndWorld._32 - f44StartWorld._32)*fRatio;
	f44World._33 = f44StartWorld._33 + (f44EndWorld._33 - f44StartWorld._33)*fRatio;
	//Normalize
	fDot = sqrt(XMF44ZAxisDotProduct(f44World, f44World));
	f44World._31 /= fDot;
	f44World._32 /= fDot;
	f44World._33 /= fDot;
}
CCameraController::CCameraController(CCutscene* pOwnerCutscene)
{
	m_unCurrentFrame = 0;
	m_pCutscene = pOwnerCutscene;
	
}
void CCameraController::SetCamera(CCamera* _camera)
{
	m_pcCamera = _camera;
	m_f44CameraReturnLocation = m_pcCamera->GetWorldMatrix();

}

CCameraController::~CCameraController()
{
	for (unsigned int i = 0; i < m_vpCameraFrames.size(); i++)
	{
		delete m_vpCameraFrames[i];
	}
	m_vpCameraFrames.clear();
}
void CCameraController::Update()
{
	/*if (m_pcCamera == nullptr)
	{
		CGameplayState* pGameplayState = reinterpret_cast<CGameplayState*>(CStateMachine::GetInstance()->GetCurrentState());
		m_pcCamera = reinterpret_cast<CCamera*>(pGameplayState->GetObjectManager()->GetCamera());
	}
*/
	CCameraFrame* pCurrentFrame = m_vpCameraFrames[m_unCurrentFrame];
	if (pCurrentFrame->GetInterpolate() == 0)
	{
		pCurrentFrame->Update();
	}
	else
	{
		//Update Camera
		pCurrentFrame->Update();
		LerpCamera();
	}

	//Update Rendering Camera
	GRAPHICS->GetViewMatrix() = pCurrentFrame->GetWorld();

	//Go to Next Frame
	if (pCurrentFrame->GetFrameTime() < pCurrentFrame->GetElapsedTime())
	{
		pCurrentFrame->Reset();
		m_unCurrentFrame++;
		if (m_unCurrentFrame >= m_vpCameraFrames.size())
		{
			m_unCurrentFrame = 0;
		}
	}
}
void CCameraController::Reset()
{
	m_unCurrentFrame = 0;
	for (unsigned int i = 0; i < m_vpCameraFrames.size(); i++)
	{
		m_vpCameraFrames[i]->Reset();
	}
	GRAPHICS->GetViewMatrix() = m_f44CameraReturnLocation;
}