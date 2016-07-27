/***********************************************
* Filename:  		Emitter.cpp
* Date:      		08/25/2015
* Mod. Date: 		08/28/2015
* Mod. Initials:	MZ
* Author:    		Matthew Zanini
* Purpose:   		Spawns and updates particles
************************************************/
#include "Emitter.h"
#include "Particle.h"
#include "../Renderer/Renderer.h"
#include "../Object Manager/ObjectManager.h"
#include "../../Game/Source/Camera/Camera.h"
#include "../Core/Collision/Collision.h"

void CEmitter::BurstUpdate()
{
	if (m_unActiveParticles == 0)
	{
		m_bIsActive = false;
	}
}
void CEmitter::LoopingUpdate()
{
	//Update Time
	m_fSpawnTimer -= DELTA_TIME();
	//Spawn if Time to
	if (m_fSpawnTimer < 0.0f)
	{
		m_fSpawnTimer = m_fSpawnRate;
		//If Active is less than max
		if (m_un2NumParticles[1] > m_unActiveParticles)
		{
			//Spawn new particles
			for (unsigned int unParticle = 0; unParticle < m_un2NumParticles[1]; unParticle++)
			{
				if (m_pParticles[unParticle].GetActive() == false)
				{
					m_pParticles[unParticle].GetEmitter() = this;
					m_pParticles[unParticle].SpawnParticle();
					m_unActiveParticles++;
					break;
				}
			}
		}
	}
	//Spawn if not enough
	SpawnMinParticles();
}
void CEmitter::SharedUpdate()
{
	//Update Particles
	for (unsigned int i = 0; i < m_un2NumParticles[1]; i++)
	{
		m_pParticles[i].Update();
	}

	XMFLOAT3 f3Pos = XMFLOAT3(m_f3Position[0],m_f3Position[1],m_f3Position[2]);
	float fRadius = max(m_fScale[0], m_fScale[1]);
	fRadius = 1000.0f;
	CSphere cSphere(f3Pos,fRadius);	
	if (SphereToFrustum(m_pObjectManager->GetFrustum(), &cSphere))
	{
		m_pParticleMesh->GetRender() =true;
	}

	//Map Constant Buffer
	TParticleBuffer tPartBuffer = m_pParticleMesh->GetParticleBuffer();
	for (unsigned int i = 0; i < 4; i++)
	{
		tPartBuffer.m_fEndColor[i] = m_tFlyweight.f4EndColor[i];
		tPartBuffer.m_fStartColor[i] = m_tFlyweight.f4StartColor[i];
	}
	tPartBuffer.m_fStartSize = m_tFlyweight.f2Size[0];
	tPartBuffer.m_fEndSize = m_tFlyweight.f2Size[1];

	if (m_pCamera == nullptr)
	{
		m_pCamera = (CCamera*)m_pObjectManager->GetCamera();
	}
	const XMFLOAT3* f3CameraPos = m_pCamera->GetPosition();

	tPartBuffer.m_fCameraPos[0] = f3CameraPos->x;
	tPartBuffer.m_fCameraPos[1] = f3CameraPos->y;
	tPartBuffer.m_fCameraPos[2] = f3CameraPos->z;

	m_pParticleMesh->SetParticleBuffer(tPartBuffer);


	//MapVertexBuffer();
}

CEmitter::CEmitter(CObjectManager* pObjectManager,unsigned int unMaxParticles)
{
	m_un2NumParticles[1] = unMaxParticles;
	//for (unsigned int i = 0; i < unMaxParticles; i++)
	//{
	//	m_pParticles[i] = new CParticle(this);
	//}
	m_unActiveParticles = 0;
	m_pObjectManager = pObjectManager;
}

CEmitter::~CEmitter()
{
	/*for (unsigned int i = 0; i < m_un2NumParticles[1]; i++)
	{
		delete m_pParticles[i];
	}
*/
	SAFE_DELETE(m_pParticleMesh);
	
}
void CEmitter::Update(void)
{
	if (m_bIsActive == false)
	{
		return;
	}	
	if (m_bIsLooping)
	{
		LoopingUpdate();
	}
	else
	{
		BurstUpdate();
	}
	SharedUpdate();
}
void CEmitter::UpdatePosition(DirectX::XMFLOAT3 f3Velocity)
{
	m_f3Position[0] += f3Velocity.x;
	m_f3Position[1] += f3Velocity.y;
	m_f3Position[2] += f3Velocity.z;
}
void CEmitter::ePlay()
{
	m_bIsActive = true;
	if (m_bIsLooping == false)
	{
		SpawnMinParticles();
	}
	m_pParticleMesh->GetRender() = true;
}
void CEmitter::Stop()
{
	m_bIsActive = false;
	for (unsigned int i = 0; i < m_un2NumParticles[1]; i++)
	{
		m_pParticles[i].DespawnParticle();
	}
	m_pParticleMesh->GetRender() = false;
}
void CEmitter::SpawnMinParticles()
{
	if (m_unActiveParticles < m_un2NumParticles[0])
	{
		unsigned int unToSpawn = m_un2NumParticles[0] - m_unActiveParticles;
		unsigned int unParticle = 0;
		for (unsigned int i = 0; i < unToSpawn; i++)
		{
			for (; unParticle < m_un2NumParticles[1]; unParticle++)
			{
				if (m_pParticles[unParticle].GetActive() == false)
				{
					m_pParticles[unParticle].GetEmitter() = this;
					m_pParticles[unParticle].SpawnParticle();
					m_unActiveParticles++;
					break;
				}
			}
		}
	}
}
void CEmitter::MapVertexBuffer()
{
	//Map Vertex Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	std::vector<TParticleVertex> vVertexVector;
	vVertexVector.resize(m_un2NumParticles[1]);
	for (unsigned int i = 0; i < m_un2NumParticles[1]; i++)
	{
		for (unsigned int xyzw = 0; xyzw < 3; xyzw++)
		{
			vVertexVector[i].m_fPosition[xyzw] = GetParticles(i).GetPosition(xyzw);
		}
		if (GetParticles(i).GetActive())
		{
			vVertexVector[i].m_fPosition[3] = 1.0f;
		}
		else
		{
			vVertexVector[i].m_fPosition[3] = 0.0f;
		}
		vVertexVector[i].m_fCurrentLife = GetParticles(i).GetCurrentLife();
		vVertexVector[i].m_fStartLife = GetParticles(i).GetStartLife();
		vVertexVector[i].m_fSize[0] = GetParticles(i).GetStartSize();
		vVertexVector[i].m_fSize[1] = GetParticles(i).GetEndSize();
	}
	ID3D11DeviceContext* d3dContext = GRAPHICS->GetDeviceContext();
	d3dContext->Map(m_pParticleMesh->m_d3dVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &vVertexVector[0], sizeof(TParticleVertex) * vVertexVector.size());
	d3dContext->Unmap(m_pParticleMesh->m_d3dVertexBuffer, 0);
}