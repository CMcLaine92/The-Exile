#include "DayNight.h"
#include "../../../Engine/Core/Messages/StartDayMessage.h"
#include "../../../Engine/Core/Messages/StartNightMessage.h"
#include <iostream>
#include "../../../Engine/Renderer/Renderer.h"
#include "../Camera/Camera.h"
#include "../../../Engine/Particles/tinyxml/tinyxml.h"
#include "../../../Engine/Particles/ParticleManager.h"
#include "../../../Engine/Particles/Emitter.h"
#include "../../../Engine/Audio/Source/AudioSystemWwise.h"
#include "../../../Engine/Audio/Source/Wwise_IDs.h"
#include "../../../Engine/Object Manager/DoorObject.h"
#include "../../../Engine/Core/Collision/AABB.h"
#include "../../../Engine/Core/Input/InputManager.h"

#define SECONDS_PER_DAY (m_fMinutesPerDay * 60.0f)
#define SECONDS_PER_NIGHT (m_fMinutesPerNight * 60.0f)
#define HALF_DAY 180.0f
#define SKY_OBJECT_SIZE 1000.0f

void CDayNight::LoadParticles()
{
	m_vunParticleIndex.push_back(m_pcParticleManager->LoadEmitter("DoorSeemDust1.xml"));
	m_vunParticleIndex.push_back(m_pcParticleManager->LoadEmitter("DoorSeemDust2.xml"));
	for (unsigned int i = 0; i < 2; i++)
	{
		m_pcParticleManager->GetEmitter(m_vunParticleIndex[i])->GetPosition(0) = -1000.0f;
		m_pcParticleManager->GetEmitter(m_vunParticleIndex[i])->GetPosition(2) = 3200.0f;
	}
	m_vunParticleIndex.push_back(m_pcParticleManager->LoadEmitter("DoorSeemDust1.xml"));
	m_vunParticleIndex.push_back(m_pcParticleManager->LoadEmitter("DoorSeemDust2.xml"));
	for (unsigned int i = 2; i <4; i++)
	{
		m_pcParticleManager->GetEmitter(m_vunParticleIndex[i])->GetPosition(0) = 0.0f;
		m_pcParticleManager->GetEmitter(m_vunParticleIndex[i])->GetPosition(2) = -4200.0f;
		m_pcParticleManager->GetEmitter(m_vunParticleIndex[i])->GetFlyweight().f3MaxStartVelocity[0] *= -1.0f;

	}
	m_vunParticleIndex.push_back(m_pcParticleManager->LoadEmitter("DoorSeemDust3.xml"));
	m_vunParticleIndex.push_back(m_pcParticleManager->LoadEmitter("DoorSeemDust4.xml"));
	for (unsigned int i = 4; i < 6; i++)
	{
		m_pcParticleManager->GetEmitter(m_vunParticleIndex[i])->GetPosition(0) = 3200.0f;
		m_pcParticleManager->GetEmitter(m_vunParticleIndex[i])->GetPosition(2) = 0.0f;
	}

	m_vunParticleIndex.push_back(m_pcParticleManager->LoadEmitter("DoorSeemDust3.xml"));
	m_vunParticleIndex.push_back(m_pcParticleManager->LoadEmitter("DoorSeemDust4.xml"));
	for (unsigned int i = 6; i < 8; i++)
	{
		m_pcParticleManager->GetEmitter(m_vunParticleIndex[i])->GetPosition(0) = -4200.0f;
		m_pcParticleManager->GetEmitter(m_vunParticleIndex[i])->GetPosition(2) = -1000.0f;
		m_pcParticleManager->GetEmitter(m_vunParticleIndex[i])->GetFlyweight().f3MaxStartVelocity[2] *= -1.0f;
		m_pcParticleManager->GetEmitter(m_vunParticleIndex[i])->GetFlyweight().f3MinStartVelocity[2] *= -1.0f;
	}

	for (unsigned int i = 0; i < m_vunParticleIndex.size(); i++)
	{
		m_pcParticleManager->GetEmitter(m_vunParticleIndex[i])->GetPosition(1) = 1000.0f;
		m_pcParticleManager->GetEmitter(m_vunParticleIndex[i])->GetScale(1) = 1000;
	}

	m_vunBottomParticles.push_back(m_pcParticleManager->LoadEmitter("DoorBotDust1.xml"));
	m_vunBottomParticles.push_back(m_pcParticleManager->LoadEmitter("DoorBotDust1.xml"));
	m_vunBottomParticles.push_back(m_pcParticleManager->LoadEmitter("DoorBotDust1.xml"));
	m_vunBottomParticles.push_back(m_pcParticleManager->LoadEmitter("DoorBotDust1.xml"));

	for (unsigned int i = 0; i < m_vunBottomParticles.size(); i++)
	{
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[i])->Stop();
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[i])->GetPosition(0) = 0.0f;
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[i])->GetPosition(1) = 0.0f;
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[i])->GetPosition(2) = 0.0f;
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[i])->GetScale(1) = 10.0f;
	}
}
void CDayNight::PlayParticles()
{
	for (unsigned int i = 0; i < m_vunParticleIndex.size(); i++)
	{
		m_pcParticleManager->GetEmitter(m_vunParticleIndex[i])->ePlay();
	}
}
void CDayNight::SetTimer(float fDoorTimer)
{
	m_fDoorTimer = fDoorTimer;
	for (unsigned int i = 0; i < 4; i++)
	{
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[i])->ePlay();
	}
}
void CDayNight::UpdateMeshes()
{
	float fAngle = m_fTimeOfDay + HALF_DAY;
	fAngle = RADIANS(fAngle);

	const XMFLOAT3* f3CameraPos = m_pcCamera->GetPosition();
	
	m_vMoonVertex[0].m_fPosition[0] = m_fSkyObjectDistance * cosf(fAngle) + f3CameraPos->x;
	m_vMoonVertex[0].m_fPosition[1] = m_fSkyObjectDistance * sinf(fAngle) + f3CameraPos->y;

	fAngle = RADIANS(m_fTimeOfDay);
	m_vSunVertex[0].m_fPosition[0] = m_fSkyObjectDistance * cosf(fAngle) + f3CameraPos->x;
	m_vSunVertex[0].m_fPosition[1] = m_fSkyObjectDistance * sinf(fAngle) + f3CameraPos->y;



	TParticleBuffer tParticleBuffer = m_pcMoonMesh->GetParticleBuffer();
	tParticleBuffer.m_fCameraPos[0] = f3CameraPos->x;
	tParticleBuffer.m_fCameraPos[1] = f3CameraPos->y;
	tParticleBuffer.m_fCameraPos[2] = f3CameraPos->z;
	m_pcMoonMesh->SetParticleBuffer(tParticleBuffer);
	//Map Vertex Buffers
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ID3D11DeviceContext* d3dContext = GRAPHICS->GetDeviceContext();
	d3dContext->Map(m_pcMoonMesh->m_d3dVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &m_vMoonVertex[0], sizeof(TParticleVertex));
	d3dContext->Unmap(m_pcMoonMesh->m_d3dVertexBuffer, 0);

	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	d3dContext->Map(m_pcSunMesh->m_d3dVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &m_vSunVertex[0], sizeof(TParticleVertex));
	d3dContext->Unmap(m_pcSunMesh->m_d3dVertexBuffer, 0);
}
CDayNight::CDayNight(CCamera* pcCamera, CParticleManager* pcParticleManager)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		m_pcHavenDoors[i] = nullptr;
	}
	m_pcParticleManager = pcParticleManager;
	LoadParticles();
	
	TiXmlDocument Doc;
	std::string szFilepath;
	szFilepath = "../Game/Assets/Scripts/Config/DayNight.xml";
	if (Doc.LoadFile(szFilepath.c_str(), TiXmlEncoding::TIXML_ENCODING_UTF8))
	{
		TiXmlElement TimeData(*Doc.FirstChildElement("DayNight")->FirstChildElement("Time"));
		m_fMinutesPerDay = std::stof(TimeData.FirstChildElement("DayLength")->GetText());
		m_fMinutesPerNight = std::stof(TimeData.FirstChildElement("NightLength")->GetText());
	}
	m_pcCamera = pcCamera;
	std::vector<unsigned int> vIndexArr;
	vIndexArr.push_back(0);
	TParticleVertex tMoonVert;
	tMoonVert.m_fCurrentLife = 1.0f;
	tMoonVert.m_fStartLife = 1.0f;
	tMoonVert.m_fPosition[0] = m_fSkyObjectDistance;
	tMoonVert.m_fPosition[1] = 100.0f;
	tMoonVert.m_fPosition[2] = 0.0f;
	tMoonVert.m_fPosition[3] = 1.0f;
	tMoonVert.m_fSize[0] = SKY_OBJECT_SIZE;
	tMoonVert.m_fSize[1] = SKY_OBJECT_SIZE;

	m_vMoonVertex.push_back(tMoonVert);
	m_vSunVertex.push_back(tMoonVert);
	m_pcSunMesh = new CParticleMesh(1, L"../Game/Assets/Art/2D/FX/Sun.dds", m_pcParticleManager->GetVertexBuffer(),nullptr, GRAPHICS->GetSunMoonGS());
	m_pcMoonMesh = new CParticleMesh(1, L"../Game/Assets/Art/2D/FX/Moon.dds", m_pcParticleManager->GetVertexBuffer(), nullptr, GRAPHICS->GetSunMoonGS());
	TParticleBuffer tParticleBuffer = m_pcMoonMesh->GetParticleBuffer();
	tParticleBuffer.m_fStartSize = SKY_OBJECT_SIZE;
	tParticleBuffer.m_fEndSize = SKY_OBJECT_SIZE;
	tParticleBuffer.m_fStartColor[0] = 1.0f;
	tParticleBuffer.m_fStartColor[1] = 1.0f;
	tParticleBuffer.m_fStartColor[2] = 1.0f;
	tParticleBuffer.m_fStartColor[3] = 1.0f;
	tParticleBuffer.m_fEndColor[0] = 1.0f;
	tParticleBuffer.m_fEndColor[1] = 1.0f;
	tParticleBuffer.m_fEndColor[2] = 1.0f;
	tParticleBuffer.m_fEndColor[3] = 1.0f;
	tParticleBuffer.m_fCameraPos[0] = m_pcCamera->GetPosition()->x;
	tParticleBuffer.m_fCameraPos[1] = m_pcCamera->GetPosition()->y;
	tParticleBuffer.m_fCameraPos[2] = m_pcCamera->GetPosition()->z;
	tParticleBuffer.m_fCameraPos[3] = 1.0f;
	m_pcMoonMesh->SetParticleBuffer(tParticleBuffer);
	m_pcSunMesh->SetParticleBuffer(tParticleBuffer);
	GRAPHICS->AddRenderMesh(m_pcMoonMesh);
	GRAPHICS->AddRenderMesh(m_pcSunMesh);

	m_f3LightColors[0] = XMFLOAT3(1, .5f, 0);
	m_f3LightColors[1] = XMFLOAT3(1, 1, 1);
	m_f3LightColors[2] = XMFLOAT3(.5f, .2f, 0);
	m_f3LightColors[3] = XMFLOAT3(.2f, .4f, .8f);


	m_fSkyObjectDistance = GRAPHICS->GetFarPlane() - SKY_OBJECT_SIZE;
}
CDayNight::~CDayNight()
{
	AudioSystemWwise::Get()->PostEvent(AK::EVENTS::STOPBACKGROUND,XMFLOAT3(0,0,0));
	//AudioSystemWwise::Get()->PostEvent(AK::EVENTS::STOPMUSIC, XMFLOAT3(0, 0, 0));
	if (m_pcMoonMesh)
	{
		delete m_pcMoonMesh;
	}
	if (m_pcSunMesh)
	{
		delete m_pcSunMesh;
	}
}
void CDayNight::SendDayNightMessage()
{
	if (m_bIsDay == true)
	{
		// Send Start Day Message
		CStartDayMessage* pStartDay = new CStartDayMessage();
		pStartDay->Send();
		//ePlay Daytime Audio
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::STOPBACKGROUND);

		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::DAYTIMEBIRDS);
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::DAYTIMESONG);
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::DAYTIMEWIND);
	}
	else
	{
		// Send Start Night Message
		CStartNightMessage* pStartNight = new CStartNightMessage();
		pStartNight->Send();
		//ePlay Nightime Audio
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::STOPBACKGROUND);

		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::NIGHTTIMEFROGS);
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::NIGHTTIMERUMBLE);
		AudioSystemWwise::Get()->PostEvent(AK::EVENTS::NIGHTTIMESONG);
	}
}
void CDayNight::Update()
{
	float fDelta = (float)DELTA_TIME();
	CInputManager* cInputManager = CInputManager::GetInstance();
	if (cInputManager->IsKeyDown("Ctrl") && cInputManager->IsKeyDown("FastForward"))
	{
		if (m_fDoorTimer < 0.0f)
		{
			fDelta *= 30.0f;
		}
	}
	float fTimeToAdd;

	if (m_bTimeOfDayChanged)
		m_bTimeOfDayChanged = !m_bTimeOfDayChanged;
	
	if (m_bIsDay)
	{
		fTimeToAdd = fDelta / SECONDS_PER_DAY * HALF_DAY;
	}
	else
	{
		fTimeToAdd = fDelta / SECONDS_PER_NIGHT * HALF_DAY;
	}
	m_fTimeOfDay += fTimeToAdd;
	if (m_bIsDay == true)
	{
		if (m_fTimeOfDay > HALF_DAY)
		{
			m_bIsDay = false;
			m_bTimeOfDayChanged = true;
			SendDayNightMessage();
			std::cout << "\tStart Night\n";		
		}
	}
	if (m_fTimeOfDay > (HALF_DAY * 2.0f))
	{
		m_fTimeOfDay -= (HALF_DAY * 2.0f);
		m_bIsDay = true;
		m_bTimeOfDayChanged = true;

		SendDayNightMessage();
		std::cout << "\tStart Day\n";
	}
	if (m_fDoorTimer > 0.0f)
	{
		m_fDoorTimer -= fDelta;
		if (m_fDoorTimer < 0.0f)
		{
			for (unsigned int i = 0; i < 4; i++)
			{
				m_pcParticleManager->GetEmitter(m_vunBottomParticles[i])->Stop();
			}
			if (m_bIsDay == false)
			{
				//ePlay Seem Particles
				PlayParticles();
			}
		}
		
		//ePlay Looping Particles
		for (unsigned int i = 0; i < 4; i++)
		{
			XMFLOAT3 f3Velocity = m_pcHavenDoors[i]->GetWorldVelocity();
			f3Velocity.x *= fDelta;
			f3Velocity.z *= fDelta;
			m_pcParticleManager->GetEmitter(m_vunBottomParticles[i])->UpdatePosition(f3Velocity);
		}
	}
	UpdateMeshes();
}
bool CDayNight::IsDay()
{
	return m_bIsDay;
}
bool CDayNight::IsNight()
{
	return !m_bIsDay;
}
void CDayNight::AddDoors(unsigned int unIndex,IObject* pcDoor)
{
	m_pcHavenDoors[unIndex] = pcDoor;
	unsigned int unDirection = reinterpret_cast<CDoorObject*>(pcDoor)->GetDirection();
	//enum EDirection{ eForward, eBackward, eLeft, eRight };
	CAABB* pDoorBounds = reinterpret_cast<CAABB*> (pcDoor->GetColliders()[0]->GetBounds());
	switch (unDirection)
	{
	case 0://forward
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[unIndex])->GetPosition(0) = pDoorBounds->GetCenter().x;
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[unIndex])->GetPosition(2) = pDoorBounds->GetCenter().z;
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[unIndex])->GetScale(2) = 1100.0f;
		break;
	case 1://backward
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[unIndex])->GetPosition(0) = pDoorBounds->GetCenter().x;
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[unIndex])->GetPosition(2) = pDoorBounds->GetCenter().z;
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[unIndex])->GetScale(2) = 1100.0f;
		break;
	case 2://left
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[unIndex])->GetPosition(0) = pDoorBounds->GetCenter().x;
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[unIndex])->GetPosition(2) = pDoorBounds->GetCenter().z;
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[unIndex])->GetScale(0) = 1100.0f;
		break;
	case 3://right
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[unIndex])->GetPosition(0) = pDoorBounds->GetCenter().x;
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[unIndex])->GetPosition(2) = pDoorBounds->GetCenter().z;
		m_pcParticleManager->GetEmitter(m_vunBottomParticles[unIndex])->GetScale(0) = 1100.0f;
		break;
	}
}
void CDayNight::UpdateDirectionalLight(TDirectLight* tDirLight)
{
	float fAngle;
	if (m_fTimeOfDay > HALF_DAY)
	{
		fAngle = m_fTimeOfDay - HALF_DAY;
	}
	else
	{
		fAngle = m_fTimeOfDay;
	}
	if (fAngle < 30.0f)
	{
		fAngle = 30.0f;
	}
	else if (fAngle > HALF_DAY - 30.0f)
	{
		fAngle = HALF_DAY - 30.0f;
	}
	fAngle = RADIANS(fAngle);
	tDirLight->m_fDirection[0] = -cosf(fAngle);
	tDirLight->m_fDirection[1] = -sinf(fAngle);
	tDirLight->m_fDirection[2] = 0.0f;

	float fLightRatio;

	unsigned int unLightStart, unLightEnd;

	if (m_fTimeOfDay < 90.0f)
	{
		fLightRatio = m_fTimeOfDay / 90.0f;
		unLightStart = 0;
		unLightEnd = 1;
	}
	else if (m_fTimeOfDay < 180.0f)
	{
		fLightRatio = (m_fTimeOfDay - 90.0f) / 90.0f;
		unLightStart = 1;
		unLightEnd = 2;
	}
	else if (m_fTimeOfDay < 270.0f)
	{
		fLightRatio = (m_fTimeOfDay - 180.0f) / 90.0f;
		unLightStart = 2;
		unLightEnd = 3;
	}
	else
	{
		fLightRatio = (m_fTimeOfDay - 270.0f) / 90.0f;
		unLightStart = 3;
		unLightEnd = 0;
	}

	XMFLOAT3 f3LightDifference;
	f3LightDifference.x = m_f3LightColors[unLightEnd].x - m_f3LightColors[unLightStart].x;
	f3LightDifference.y = m_f3LightColors[unLightEnd].y - m_f3LightColors[unLightStart].y;
	f3LightDifference.z = m_f3LightColors[unLightEnd].z - m_f3LightColors[unLightStart].z;

	tDirLight->m_fColor[0] = m_f3LightColors[unLightStart].x + f3LightDifference.x * fLightRatio;
	tDirLight->m_fColor[1] = m_f3LightColors[unLightStart].y + f3LightDifference.y * fLightRatio;
	tDirLight->m_fColor[2] = m_f3LightColors[unLightStart].z + f3LightDifference.z * fLightRatio;

}
void CDayNight::UpdateSkybox(CRenderMesh* pcSkyboxMesh)
{
	float fTimeRatio;
	static char cCurrentSection = 0;
	static char cPreviousSection = -1;
	if (m_fTimeOfDay < 90.0f)
	{
		fTimeRatio = m_fTimeOfDay / 90.0f;
		cCurrentSection = 0;
	}
	else if (m_fTimeOfDay < 180.0f)
	{
		fTimeRatio = (m_fTimeOfDay - 90.0f) / 90.0f;
		cCurrentSection = 1;
	}
	else if (m_fTimeOfDay < 270.0f)
	{
		fTimeRatio = (m_fTimeOfDay - 180.0f) / 90.0f;
		cCurrentSection = 2;
	}
	else
	{
		fTimeRatio = (m_fTimeOfDay - 270.0f) / 90.0f;
		cCurrentSection = 3;
	}
	pcSkyboxMesh->GetTransparencyOverride() = fTimeRatio;
	if (cPreviousSection != cCurrentSection)
	{
		cPreviousSection = cCurrentSection;
		//Change Textures and Normals
		switch (cCurrentSection)
		{
		case 0:
			pcSkyboxMesh->SetTexture(L"../Game/Assets/Art/2D/Textures/SkyBoxDawn.dds");
			pcSkyboxMesh->SetNormals(L"../Game/Assets/Art/2D/Textures/Skybox.dds");
			break;
		case 1:
			pcSkyboxMesh->SetTexture(L"../Game/Assets/Art/2D/Textures/Skybox.dds");
			pcSkyboxMesh->SetNormals(L"../Game/Assets/Art/2D/Textures/SkyBoxDusk.dds");
			break;
		case 2:
			pcSkyboxMesh->SetTexture(L"../Game/Assets/Art/2D/Textures/SkyBoxDusk.dds");
			pcSkyboxMesh->SetNormals(L"../Game/Assets/Art/2D/Textures/SkyBoxNight.dds");
			break;
		case 3:
			pcSkyboxMesh->SetTexture(L"../Game/Assets/Art/2D/Textures/SkyBoxNight.dds");
			pcSkyboxMesh->SetNormals(L"../Game/Assets/Art/2D/Textures/SkyBoxDawn.dds");
			break;
		}
	}
}