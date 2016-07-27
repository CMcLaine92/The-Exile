#ifndef CEMITTER_H
#define CEMITTER_H

/***********************************************
* Filename:  		Emitter.h
* Date:      		08/25/2015
* Mod. Date: 		08/28/2015
* Mod. Initials:	MZ
* Author:    		Matthew Zanini
* Purpose:   		Spawns and updates particles
************************************************/


#include "Flyweight.h"
#include <vector>
#include <DirectXMath.h>
#include "../Renderer/ParticleMesh.h"
#include "Particle.h"
#define MAX_PARTICLES 1600
namespace Shape
{
	enum eShape { Point,Box,Sphere };
}
class CParticle;
class CCamera;
class CObjectManager;
class CEmitter
{
private:
	//Is Emitter Active
	bool m_bIsActive;
	//Shape of Emitter
	Shape::eShape m_eShape;
	//Size of Emitter
	float m_fScale[3];
	//How frequently they spawn
	float m_fSpawnRate;
	//Current Spawn Timer
	float m_fSpawnTimer = 0.0f;

	//Min Particles/Max Particles
	unsigned int m_un2NumParticles[2];
	//Emitter looping or one shot
	bool m_bIsLooping;
	//Position of Emitter
	float m_f3Position[3];

	//Active Particles
	unsigned int m_unActiveParticles;

	//All Particles in the Emitter
	CParticle m_pParticles[MAX_PARTICLES];

	//Flyweight used for all the particles
	TFlyweight m_tFlyweight;

	//Render Mesh
	CParticleMesh* m_pParticleMesh = nullptr;

	//Object Manager
	CObjectManager* m_pObjectManager = nullptr;

	//Camera Reference
	CCamera* m_pCamera = nullptr;

	//Translate with parent velocity added on
	IObject* m_pParent = nullptr;

	//Helper Functions
	void SharedUpdate();
	void BurstUpdate();
	void LoopingUpdate();
	void SpawnMinParticles();
public:
	void MapVertexBuffer();

	//Accessor/Mutators
#pragma region Accessor/Mutators
	bool& GetIsActive()
	{
		return m_bIsActive;
	}
	Shape::eShape& GetShape()
	{
		return m_eShape;
	}

	float& GetScale(unsigned int unIndex)
	{
		return m_fScale[unIndex];
	}

	float& GetSpawnRate()
	{
		return m_fSpawnRate;
	}

	unsigned int& GetMinParticles()
	{
		return m_un2NumParticles[0];
	}
	
	unsigned int& GetMaxParticles()
	{
		return m_un2NumParticles[1];
	}

	float& GetPosition(unsigned int unIndex)
	{
		return m_f3Position[unIndex];
	}

	bool& GetLooping()
	{
		return m_bIsLooping;
	}

	CParticle& GetParticles(unsigned int unIndex)
	{
		return m_pParticles[unIndex];
	}

	TFlyweight& GetFlyweight()
	{
		return m_tFlyweight;
	}
	CParticleMesh*& GetParticleMesh()
	{
		return m_pParticleMesh;
	}
	unsigned int& GetNumActive()
	{
		return m_unActiveParticles;
	}
	IObject*& GetParent()
	{
		return m_pParent;
	}

#pragma endregion

	void ePlay();
	void Stop();
	CEmitter(CObjectManager* pObjectManager,unsigned int unMaxParticles);
	~CEmitter();
	void Update(void);
	void UpdatePosition(DirectX::XMFLOAT3 f3Velocity);
};





#endif