/***********************************************
* Filename:  		ParticleManager.cpp
* Date:      		08/25/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Keeps track of all emitters
*					Updating Emitters and deallocating
*					Emitters
************************************************/
#include "ParticleManager.h"
#include "Emitter.h"
#include "tinyxml\tinyxml.h"
#include "Particle.h"
#include "../Renderer/Renderer.h"
CParticleManager::CParticleManager(CObjectManager* pObject)
{
	m_pObjectManager = pObject;
	ID3D11Device* d3dDevice = GRAPHICS->GetDevice();

	D3D11_BUFFER_DESC d3dBufferDesc;

	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.StructureByteStride = sizeof(TParticleVertex);
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = (UINT)2000 * sizeof(TParticleVertex);

	HRESULT hr = d3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pVertexBuffer);

	assert(SUCCEEDED(hr));


}
CParticleManager::~CParticleManager()
{

	for (unsigned int i = 0; i < m_vpEmitters.size(); i++)
		SAFE_DELETE(m_vpEmitters[i]);
	SAFE_RELEASE(m_pVertexBuffer);
	m_vpEmitters.clear();
}
void CParticleManager::Update(void)
{
	for (unsigned int i = 0; i < m_vpEmitters.size(); i++)
	{
		m_vpEmitters[i]->Update();
	}
}
unsigned int CParticleManager::LoadEmitter(std::string szFilepath)
{
	unsigned int unID = 0;
	TiXmlDocument Doc;
	
	std::string szFullFilepath;
	szFullFilepath = "../Game/Assets/Scripts/Particles/" + szFilepath;
	if (Doc.LoadFile(szFullFilepath.c_str(), TiXmlEncoding::TIXML_ENCODING_UTF8))
	{
		TiXmlElement* FlyweightData = Doc.FirstChildElement("ParticleEffect")->FirstChildElement("FlyWeight");		
		TiXmlElement* EmitterData = Doc.FirstChildElement("ParticleEffect")->FirstChildElement("Emitter");
		unsigned int unMaxParticles = std::stoi(EmitterData->FirstChildElement("MaxParticles")->GetText());
		if (unMaxParticles > MAX_PARTICLES)
		{
			unMaxParticles = MAX_PARTICLES;
		}
		CEmitter* pcEmitter = new CEmitter(m_pObjectManager,unMaxParticles);
		pcEmitter->GetFlyweight().f2Life[0] = std::stof(FlyweightData->FirstChildElement("MinLife")->GetText());
		pcEmitter->GetFlyweight().f2Life[1] = std::stof(FlyweightData->FirstChildElement("MaxLife")->GetText());
		pcEmitter->GetFlyweight().f2Size[0] = std::stof(FlyweightData->FirstChildElement("StartSize")->GetText());
		pcEmitter->GetFlyweight().f2SizeMax[0] = std::stof(FlyweightData->FirstChildElement("StartSizeMax")->GetText());
		std::string szConstantSizeCheck = FlyweightData->FirstChildElement("EndSize")->GetText();
		if (szConstantSizeCheck == "c")
		{
			pcEmitter->GetFlyweight().f2Size[1] = pcEmitter->GetFlyweight().f2Size[0];
			pcEmitter->GetFlyweight().f2SizeMax[1] = pcEmitter->GetFlyweight().f2SizeMax[0];
		}
		else
		{
			pcEmitter->GetFlyweight().f2Size[1] = std::stof(FlyweightData->FirstChildElement("EndSize")->GetText());
			pcEmitter->GetFlyweight().f2SizeMax[1] = std::stof(FlyweightData->FirstChildElement("EndSizeMax")->GetText());
		}
		pcEmitter->GetFlyweight().f3MinStartVelocity[0] = std::stof(FlyweightData->FirstChildElement("MinStartXVelocity")->GetText());
		pcEmitter->GetFlyweight().f3MinStartVelocity[1] = std::stof(FlyweightData->FirstChildElement("MinStartYVelocity")->GetText());
		pcEmitter->GetFlyweight().f3MinStartVelocity[2] = std::stof(FlyweightData->FirstChildElement("MinStartZVelocity")->GetText());
		pcEmitter->GetFlyweight().f3MaxStartVelocity[0] = std::stof(FlyweightData->FirstChildElement("MaxStartXVelocity")->GetText());
		pcEmitter->GetFlyweight().f3MaxStartVelocity[1] = std::stof(FlyweightData->FirstChildElement("MaxStartYVelocity")->GetText());
		pcEmitter->GetFlyweight().f3MaxStartVelocity[2] = std::stof(FlyweightData->FirstChildElement("MaxStartZVelocity")->GetText());
		std::string szConstantCheck = FlyweightData->FirstChildElement("MinEndXVelocity")->GetText();
		if (szConstantCheck == "c")
		{
			pcEmitter->GetFlyweight().bConstantVelocity = true;
		}
		else
		{
			pcEmitter->GetFlyweight().bConstantVelocity = false;
			pcEmitter->GetFlyweight().f3MinEndVelocity[0] = std::stof(FlyweightData->FirstChildElement("MinEndXVelocity")->GetText());
			pcEmitter->GetFlyweight().f3MinEndVelocity[1] = std::stof(FlyweightData->FirstChildElement("MinEndYVelocity")->GetText());
			pcEmitter->GetFlyweight().f3MinEndVelocity[2] = std::stof(FlyweightData->FirstChildElement("MinEndZVelocity")->GetText());
			pcEmitter->GetFlyweight().f3MaxEndVelocity[0] = std::stof(FlyweightData->FirstChildElement("MaxEndXVelocity")->GetText());
			pcEmitter->GetFlyweight().f3MaxEndVelocity[1] = std::stof(FlyweightData->FirstChildElement("MaxEndYVelocity")->GetText());
			pcEmitter->GetFlyweight().f3MaxEndVelocity[2] = std::stof(FlyweightData->FirstChildElement("MaxEndZVelocity")->GetText());
		}
		pcEmitter->GetFlyweight().f4StartColor[3] = std::stof(FlyweightData->FirstChildElement("StartColorA")->GetText()) / 255.0f;
		pcEmitter->GetFlyweight().f4StartColor[0] = std::stof(FlyweightData->FirstChildElement("StartColorR")->GetText()) / 255.0f;
		pcEmitter->GetFlyweight().f4StartColor[1] = std::stof(FlyweightData->FirstChildElement("StartColorG")->GetText()) / 255.0f;
		pcEmitter->GetFlyweight().f4StartColor[2] = std::stof(FlyweightData->FirstChildElement("StartColorB")->GetText()) / 255.0f;
		pcEmitter->GetFlyweight().f4EndColor[3] = std::stof(FlyweightData->FirstChildElement("EndColorA")->GetText()) / 255.0f;
		pcEmitter->GetFlyweight().f4EndColor[0] = std::stof(FlyweightData->FirstChildElement("EndColorR")->GetText()) / 255.0f;
		pcEmitter->GetFlyweight().f4EndColor[1] = std::stof(FlyweightData->FirstChildElement("EndColorG")->GetText()) / 255.0f;
		pcEmitter->GetFlyweight().f4EndColor[2] = std::stof(FlyweightData->FirstChildElement("EndColorB")->GetText()) / 255.0f;
		std::string szShape;
		szShape = EmitterData->FirstChildElement("Shape")->GetText();
		if (szShape == "Point" || szShape == "Mesh")
		{
			pcEmitter->GetShape() = Shape::Point;
		}
		else if (szShape == "Box")
		{
			pcEmitter->GetShape() = Shape::Box;
		}
		else if (szShape == "Sphere")
		{
			pcEmitter->GetShape() = Shape::Sphere;
		}
		pcEmitter->GetScale(1) = pcEmitter->GetScale(2) = pcEmitter->GetScale(0) = std::stof(EmitterData->FirstChildElement("Scale")->GetText());
		pcEmitter->GetSpawnRate() = std::stof(EmitterData->FirstChildElement("SpawnRate")->GetText());
		pcEmitter->GetMinParticles() = std::stoi(EmitterData->FirstChildElement("MinParticles")->GetText());
		pcEmitter->GetMaxParticles() = unMaxParticles;

		if (pcEmitter->GetMinParticles() > pcEmitter->GetMaxParticles())
		{
			pcEmitter->GetMinParticles() = pcEmitter->GetMaxParticles();
		}
		std::string szLooping = EmitterData->FirstChildElement("Looping")->GetText();
		if (szLooping == "True")
		{
			pcEmitter->GetLooping() = true;
		}
		else
		{
			pcEmitter->GetLooping() = false;
		}


	//	std::vector<unsigned int> vIndexVector;
	//	std::vector<TParticleVertex> vVertexVector;
	//	TParticleVertex tDefaultVert = { 0, 0, 0, 0, 0, 0, 0, 0 };
		//vIndexVector.resize(pcEmitter->GetMaxParticles(),0);
	//	vVertexVector.resize(pcEmitter->GetMaxParticles(),tDefaultVert);
	//	for (unsigned int i = 0; i < pcEmitter->GetMaxParticles(); i++)
		//{
		//	vIndexVector[i] = i;
			//vIndexVector.push_back(i);
			//for (unsigned int xyzw = 0; xyzw < 3; xyzw++)
			//{
			//	tDefaultVert.m_fPosition[xyzw] = pcEmitter->GetParticles(i)->GetPosition(xyzw);
			////	vVertexVector[i].m_fPosition[xyzw] = pcEmitter->GetParticles(i)->GetPosition(xyzw);
			//}
			////vVertexVector[i].m_fPosition[3] = 1.0f;
			////vVertexVector[i].m_fCurrentLife = pcEmitter->GetParticles(i)->GetCurrentLife();
			////vVertexVector[i].m_fStartLife = pcEmitter->GetParticles(i)->GetStartLife();
			//tDefaultVert.m_fPosition[3] = 1.0f;
			//tDefaultVert.m_fCurrentLife = pcEmitter->GetParticles(i)->GetCurrentLife();
			//tDefaultVert.m_fStartLife = pcEmitter->GetParticles(i)->GetStartLife();
	//		vVertexVector.push_back(tDefaultVert);
	//	}

		//Texture Loading
		std::string szImagePath = EmitterData->FirstChildElement("ImagePath")->GetText();
		szImagePath = "../Game/Assets/Art/2D/FX/" + szImagePath;
		std::wstring WString(szImagePath.begin(),szImagePath.end());


		pcEmitter->GetParticleMesh() = new CParticleMesh(pcEmitter->GetMaxParticles(), WString.c_str(),m_pVertexBuffer,pcEmitter);


		TParticleBuffer tPartBuffer = pcEmitter->GetParticleMesh()->GetParticleBuffer();
		for (unsigned int i = 0; i < 4; i++)
		{
			tPartBuffer.m_fEndColor[i] = pcEmitter->GetFlyweight().f4EndColor[i];
			tPartBuffer.m_fStartColor[i] = pcEmitter->GetFlyweight().f4StartColor[i];
		}
		tPartBuffer.m_fStartSize = pcEmitter->GetFlyweight().f2Size[0];
		tPartBuffer.m_fEndSize = pcEmitter->GetFlyweight().f2Size[1];
		pcEmitter->GetParticleMesh()->SetParticleBuffer(tPartBuffer);

		pcEmitter->GetIsActive() = true;
		GRAPHICS->AddRenderMesh(pcEmitter->GetParticleMesh());
		unID = m_vpEmitters.size();
		m_vpEmitters.push_back(pcEmitter);

		Doc.Clear();
	}
	return unID;
}

CEmitter* CParticleManager::GetEmitter(unsigned int unID)
{
	return m_vpEmitters[unID];
}
