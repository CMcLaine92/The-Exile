#include "ParticleMesh.h"
#include "RenderMesh.h"
#include <iostream>
#include "../Particles/Emitter.h"
#include "Renderer.h"
#include "DDSTextureLoader.h"

/***********************************************
* Filename:  		ParticleMesh.cpp
* Date:      		08/25/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Used to get Particles to the Renderer
*
************************************************/
CParticleMesh::CParticleMesh(unsigned int nNumVerts, const wchar_t* chTexturePath,  ID3D11Buffer* pVertexBuffer, CEmitter* pcEmitter,ID3D11GeometryShader* d3dGS)
{
	m_pcEmitter = pcEmitter;
	m_cType = typeid(CParticleMesh);

	m_d3dInputLayout = GRAPHICS->GetInputLayoutParticle();
	
	if (d3dGS == nullptr)
		m_d3dGeometryShader = GRAPHICS->GetParticleGS();
	else
		m_d3dGeometryShader = d3dGS;

	m_d3dPixelShader = GRAPHICS->GetParticlePS();
	m_d3dVertexShader = GRAPHICS->GetParticleVS();

	IdentityMatrix(m_d3dTransformMatrix);

	m_d3dTopologyType = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	m_unNumVerts = nNumVerts;
	m_d3dVertexBuffer = pVertexBuffer;
	//// Buffer Creations //
	//D3D11_BUFFER_DESC d3dBufferDesc;

	//ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	//d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//d3dBufferDesc.StructureByteStride = sizeof(TParticleVertex);
	//d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//d3dBufferDesc.ByteWidth = (UINT)nNumVerts * sizeof(TParticleVertex);

	//if (d3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_d3dVertexBuffer) != S_OK)
	//{
	//	cout << "VertexBuffer";
	//	wprintf(chTexturePath);
	//	cout << "\n";
	//}

	// Texture Creation //
	if (chTexturePath)
		SetTexture(chTexturePath);
	else
		SetTexture(L"../Game/Assets/Art/2D/Textures/Gray.dds");

	m_bTransparent = true;
}


void CParticleMesh::Draw()
{
	ID3D11DeviceContext* d3dContext = GRAPHICS->GetDeviceContext();
	// Set Shader Resource View //
	d3dContext->PSSetShaderResources(0, 1, &m_d3dDiffuse);

	// Set Buffers //
	unsigned int nOffset = 0;
	unsigned int nStrideSize = sizeof(TParticleVertex);

	if (m_pcEmitter != nullptr)
	{
		m_pcEmitter->MapVertexBuffer();
	}


	d3dContext->IASetVertexBuffers(0, 1, &m_d3dVertexBuffer, &nStrideSize, &nOffset);
	ID3D11Buffer* d3dParticleBuffer = GRAPHICS->GetParticleBuffer();
	// Send Position To GPU //
	MapBuffers();
	//MAP PARTICLE CONSTANT BUFFER //
	D3D11_MAPPED_SUBRESOURCE d3dMapSubresource;
	ZeroMemory(&d3dMapSubresource, sizeof(d3dMapSubresource));
	d3dContext->Map(d3dParticleBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &d3dMapSubresource);
	memcpy(d3dMapSubresource.pData, &m_tParticleBufferData, sizeof(m_tParticleBufferData));
	d3dContext->Unmap(d3dParticleBuffer, 0);

	d3dContext->Draw(m_unNumVerts, 0);
}
void CParticleMesh::MapBuffers()
{
	D3D11_MAPPED_SUBRESOURCE d3dMapSubresource;

	// Grab Render Components //
	ID3D11DeviceContext* d3dContext = GRAPHICS->GetDeviceContext();
	ID3D11Buffer* d3dTransformBuffer = GRAPHICS->GetTransformBuffer();

	// Send Transform Matrix To GPU //
	ZeroMemory(&d3dMapSubresource, sizeof(d3dMapSubresource));
	d3dContext->Map(d3dTransformBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &d3dMapSubresource);
	memcpy(d3dMapSubresource.pData, &m_d3dTransformMatrix, sizeof(m_d3dTransformMatrix));
	d3dContext->Unmap(d3dTransformBuffer, 0);
}