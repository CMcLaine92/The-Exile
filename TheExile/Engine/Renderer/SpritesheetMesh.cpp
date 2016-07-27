#include "SpritesheetMesh.h"
#include "RenderMesh.h"
#include <iostream>

#include "Renderer.h"
#include "DDSTextureLoader.h"
/***********************************************
* Filename:  		SpritesheetMesh.cpp
* Date:      		09/03/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Used to get Particles to the Renderer
*
************************************************/
CSpritesheetMesh::CSpritesheetMesh(std::vector<unsigned int> tIndexArr, std::vector<TVertex3D> tVertexArr, TSpritesheetData tSpritesheetData, const wchar_t* chTexturePath)
{
	m_cType = typeid(CSpritesheetMesh);
	m_tSpritesheetData = tSpritesheetData;
	m_fCurrentTime = tSpritesheetData.m_fFrameTime;
	ID3D11Device* d3dDevice = GRAPHICS->GetDevice();

	m_d3dInputLayout = GRAPHICS->GetInputLayout3D();
	m_d3dPixelShader = GRAPHICS->GetPixelShader();
	m_d3dVertexShader = GRAPHICS->GetVertexShader();

	IdentityMatrix(m_d3dTransformMatrix);

	m_d3dTopologyType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_nIndexArr = tIndexArr;
	m_tVertexArr = tVertexArr;
	InitializeUVs();
	// Buffer Creations //
	D3D11_BUFFER_DESC d3dBufferDesc;
	D3D11_SUBRESOURCE_DATA d3dBufferResource;

	// Index Buffer //
	ZeroMemory(&d3dBufferResource, sizeof(d3dBufferResource));
	d3dBufferResource.pSysMem = &m_nIndexArr[0];
	d3dBufferResource.SysMemPitch = NULL;
	d3dBufferResource.SysMemSlicePitch = NULL;

	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d3dBufferDesc.StructureByteStride = sizeof(unsigned int);
	d3dBufferDesc.CPUAccessFlags = NULL;
	d3dBufferDesc.ByteWidth = (UINT)m_nIndexArr.size() * sizeof(unsigned int);

	d3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferResource, &m_d3dIndexBuffer);

	// Vertex Buffer //
	ZeroMemory(&d3dBufferResource, sizeof(d3dBufferResource));
	d3dBufferResource.pSysMem = &m_tVertexArr[0];
	d3dBufferResource.SysMemPitch = NULL;
	d3dBufferResource.SysMemSlicePitch = NULL;

	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.StructureByteStride = sizeof(TVertex3D);
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = (UINT)m_tVertexArr.size() * sizeof(TVertex3D);

	d3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferResource, &m_d3dVertexBuffer);

	// Texture Creation //
	if (chTexturePath)
		SetTexture(chTexturePath);
	else
		SetTexture(L"../Game/Assets/Art/2D/Textures/Gray.dds");

}
void CSpritesheetMesh::Draw()
{
	// Set Shader Resource View //
	GRAPHICS->GetDeviceContext()->PSSetShaderResources(0, 1, &m_d3dDiffuse);

	// Set Buffers //
	unsigned int nOffset = 0;
	unsigned int nStrideSize = sizeof(TVertex3D);

	GRAPHICS->GetDeviceContext()->IASetIndexBuffer(m_d3dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	GRAPHICS->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_d3dVertexBuffer, &nStrideSize, &nOffset);

	// Send Position To GPU //
	MapBuffers();

	//MAP VERTEX BUFFERS 
	MapVertexBuffer(GRAPHICS->GetDeviceContext());

	GRAPHICS->GetDeviceContext()->DrawIndexed(m_nIndexArr.size(), 0, 0);
}
void CSpritesheetMesh::Draw(ID3D11DeviceContext* d3dContext)
{
	d3dContext->PSSetShaderResources(0, 1, &m_d3dDiffuse);

	// Set Buffers //
	unsigned int nOffset = 0;
	unsigned int nStrideSize = sizeof(TVertex3D);

	d3dContext->IASetIndexBuffer(m_d3dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	d3dContext->IASetVertexBuffers(0, 1, &m_d3dVertexBuffer, &nStrideSize, &nOffset);

	// Send Position To GPU //
	MapBuffers();

	//MAP VERTEX BUFFERS 
	MapVertexBuffer(d3dContext);

	d3dContext->DrawIndexed(m_nIndexArr.size(), 0, 0);
}
void CSpritesheetMesh::Update()
{
	m_fCurrentTime -= DELTA_TIME();
	if (m_fCurrentTime < 0.0f)
	{
		m_fCurrentTime = m_tSpritesheetData.m_fFrameTime;
		MoveOverFrame();
	}
}
void CSpritesheetMesh::MapVertexBuffer(ID3D11DeviceContext* d3dContext)
{
	//Map Vertex Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	d3dContext->Map(m_d3dVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &m_tVertexArr[0], sizeof(TVertex3D) * m_tVertexArr.size());
	d3dContext->Unmap(m_d3dVertexBuffer, 0);
}
void CSpritesheetMesh::InitializeUVs()
{

	float fUScale = m_tSpritesheetData.m_fWidth / m_tSpritesheetData.m_fTextureWidth;
	float fVScale = m_tSpritesheetData.m_fHeight / m_tSpritesheetData.m_fTextureHeight;
	m_fUVScale[0] = fUScale;
	m_fUVScale[1] = fVScale;

	for (unsigned int i = 0; i < m_tVertexArr.size(); i++)
	{
		m_vOriginalTexCoordsU.push_back(m_tVertexArr[i].m_fTexCoord[0]);
		m_vOriginalTexCoordsV.push_back(m_tVertexArr[i].m_fTexCoord[1]);
		m_tVertexArr[i].m_fTexCoord[0] *= fUScale;
		m_tVertexArr[i].m_fTexCoord[1] *= fVScale;
	}

}
void CSpritesheetMesh::MoveOverFrame()
{
	m_unCurrentFrame++;
	//if after final frame, reset to beginning
	if (m_unCurrentFrame >= m_tSpritesheetData.m_unColumns * m_tSpritesheetData.m_unRows)
	{
		m_unCurrentFrame = 0;
	}
	unsigned int unCurrentColumn = m_unCurrentFrame % m_tSpritesheetData.m_unColumns;
	unsigned int unCurrentRow = m_unCurrentFrame / m_tSpritesheetData.m_unColumns;
	for (unsigned int i = 0; i < m_tVertexArr.size(); i++)
	{
		m_tVertexArr[i].m_fTexCoord[0] = m_vOriginalTexCoordsU[i] * m_fUVScale[0] + m_fUVScale[0] * unCurrentColumn;
		m_tVertexArr[i].m_fTexCoord[1] = m_vOriginalTexCoordsV[i] * m_fUVScale[1] + m_fUVScale[1] * unCurrentRow;
	}

}