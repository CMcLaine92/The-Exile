#include "LineMesh.h"

#include "Renderer.h"

CLineMesh::CLineMesh()
{
	ID3D11Device* d3dDevice = GRAPHICS->GetDevice();

	m_cType = typeid(CLineMesh);

	m_d3dInputLayout = GRAPHICS->GetInputLayoutLine();
	IdentityMatrix(m_d3dTransformMatrix);

	m_d3dTopologyType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	m_tLineVertexArr.reserve(MAX_LINES * 2);

	m_d3dVertexShader = GRAPHICS->GetLineVS();
	m_d3dPixelShader = GRAPHICS->GetLinePS();

	// Buffer Creation //
	D3D11_BUFFER_DESC d3dBufferDesc;

	// Vertex Buffer //
	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.StructureByteStride = sizeof(TLineVertex);
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = ((UINT)MAX_LINES * 2) * sizeof(TLineVertex);

	d3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_d3dVertexBuffer);
}

void CLineMesh::Draw()
{
	if (!m_tLineVertexArr.size()) // No Lines
		return;

	// Grab Render Components //
	ID3D11DeviceContext* d3dContext = GRAPHICS->GetDeviceContext();

	// Set Vertex + Index Buffers //
	unsigned int nOffset = 0;
	unsigned int nStrideSize = sizeof(TLineVertex);

	d3dContext->IASetVertexBuffers(0, 1, &m_d3dVertexBuffer, &nStrideSize, &nOffset);

	// Map Constant Buffers //
	MapBuffers();

	d3dContext->Draw(m_tLineVertexArr.size(), 0);
}

void CLineMesh::MapBuffers()
{
	CRenderMesh::MapBuffers();

	m_bRemap = false;
	D3D11_MAPPED_SUBRESOURCE d3dMapSubresource;
	
	// Grab Render Components //
	ID3D11DeviceContext* d3dContext = GRAPHICS->GetDeviceContext();
	
	// Send New Vertices To GPU //
	ZeroMemory(&d3dMapSubresource, sizeof(d3dMapSubresource));
	d3dContext->Map(m_d3dVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &d3dMapSubresource);
	memcpy(d3dMapSubresource.pData, &m_tLineVertexArr[0], sizeof(TLineVertex) * m_tLineVertexArr.size());
	d3dContext->Unmap(m_d3dVertexBuffer, 0);
}