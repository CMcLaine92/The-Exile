#ifndef PARTICLERENDERMESH_H_
#define PARTICLERENDERMESH_H_
#include "RenderMesh.h"
/***********************************************
* Filename:  		ParticleMesh.h
* Date:      		08/25/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Used to get Particles to the Renderer
*					
************************************************/
struct TParticleVertex
{
	float m_fPosition[4];
	float m_fStartLife;
	float m_fCurrentLife;
	float m_fSize[2];
};
struct TParticleBuffer
{
	float m_fStartColor[4];
	float m_fEndColor[4];
	float m_fCameraPos[4];
	float m_fStartSize;
	float m_fEndSize;
	float m_f2Padding[2];
};

class CParticleMesh : public CRenderMesh
{
private:
	std::vector<TParticleVertex> m_tParticleVertexArr;
	TParticleBuffer m_tParticleBufferData;
	unsigned int m_unNumVerts = 0;
	friend class CEmitter;
	friend class CDayNight;
	CEmitter* m_pcEmitter = nullptr;
	virtual void MapBuffers() override;
public:
	TParticleBuffer GetParticleBuffer()
	{
		return m_tParticleBufferData;
	}
	void SetParticleBuffer(TParticleBuffer tParticleBufferData)
	{
		m_tParticleBufferData = tParticleBufferData;
	}
	


	CParticleMesh(unsigned int nNumVerts, const wchar_t* chTexturePath,ID3D11Buffer* pVertexBuffer,CEmitter* pcEmitter,ID3D11GeometryShader* d3dGS = nullptr);
	virtual ~CParticleMesh(){ m_d3dVertexBuffer = NULL; }
	void Draw() override;
};


#endif