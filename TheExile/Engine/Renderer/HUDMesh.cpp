#include "HUDMesh.h"

#include "Renderer.h"


CHUDMesh::CHUDMesh(std::vector<unsigned int> tIndexArr, std::vector<TVertex2D> tVertex2DArr,
	ID3D11VertexShader* d3dVertexShader, ID3D11PixelShader* d3dPixelShader,
	ID3D11HullShader* d3dHullShader, ID3D11DomainShader* d3dDomainShader,
	ID3D11GeometryShader* d3dGeometryShader, const wchar_t* chTexturePath)
{
	ID3D11Device* d3dDevice = GRAPHICS->GetDevice();
	m_cpMesh = nullptr;

	m_d3dInputLayout = GRAPHICS->GetInputLayout2D();
	IdentityMatrix(m_d3dTransformMatrix);

	m_d3dTopologyType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_nIndexArr = tIndexArr;
	m_tVertex2DArr = tVertex2DArr;

	m_d3dVertexShader = d3dVertexShader;
	m_d3dPixelShader = d3dPixelShader;

	m_d3dHullShader = d3dHullShader;
	m_d3dDomainShader = d3dDomainShader;
	m_d3dGeometryShader = d3dGeometryShader;

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
	d3dBufferResource.pSysMem = &m_tVertex2DArr[0];
	d3dBufferResource.SysMemPitch = NULL;
	d3dBufferResource.SysMemSlicePitch = NULL;

	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.StructureByteStride = sizeof(TVertex2D);
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = (UINT)m_tVertex2DArr.size() * sizeof(TVertex2D);

	d3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferResource, &m_d3dVertexBuffer);

	// Texture Creation //
	if (chTexturePath)
		SetTexture(chTexturePath);
	else
		SetTexture(L"../Game/Assets/Art/2D/Textures/Gray.dds");
}
CHUDMesh::CHUDMesh(std::vector<unsigned int> tIndexArr, std::vector<TVertex2D> tVertexArr,
	ID3D11VertexShader* d3dVertexShader, ID3D11PixelShader* d3dPixelShader,
	ID3D11HullShader* d3dHullShader , ID3D11DomainShader* d3dDomainShader ,
	ID3D11GeometryShader* d3dGeometryShader , ID3D11ShaderResourceView* d3dDiffuseTexture )
{
	ID3D11Device* d3dDevice = GRAPHICS->GetDevice();
	m_cpMesh = nullptr;

	m_d3dInputLayout = GRAPHICS->GetInputLayout2D();
	IdentityMatrix(m_d3dTransformMatrix);

	m_d3dTopologyType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_nIndexArr = tIndexArr;
	m_tVertex2DArr = tVertexArr;

	m_d3dVertexShader = d3dVertexShader;
	m_d3dPixelShader = d3dPixelShader;

	m_d3dHullShader = d3dHullShader;
	m_d3dDomainShader = d3dDomainShader;
	m_d3dGeometryShader = d3dGeometryShader;

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
	d3dBufferResource.pSysMem = &m_tVertex2DArr[0];
	d3dBufferResource.SysMemPitch = NULL;
	d3dBufferResource.SysMemSlicePitch = NULL;

	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.StructureByteStride = sizeof(TVertex2D);
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = (UINT)m_tVertex2DArr.size() * sizeof(TVertex2D);

	d3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferResource, &m_d3dVertexBuffer);

	// Texture Creation //
	m_d3dDiffuse = d3dDiffuseTexture;
}
CHUDMesh::~CHUDMesh()
{
	m_tVertex2DArr.clear();
	m_nIndexArr.clear();
}

void CHUDMesh::Draw()
{
	// Grab Render Components //
	ID3D11DeviceContext* d3dContext = GRAPHICS->GetDeviceContext();

	// Set Shader Resource View //
	d3dContext->PSSetShaderResources(0, 1, &m_d3dDiffuse);

	if (m_bNormalMapped)
		d3dContext->PSSetShaderResources(1, 1, &m_d3dNormals);

	// Set Vertex + Index Buffers //
	unsigned int nOffset = 0;
	unsigned int nStrideSize = sizeof(TVertex2D);

	d3dContext->IASetIndexBuffer(m_d3dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	d3dContext->IASetVertexBuffers(0, 1, &m_d3dVertexBuffer, &nStrideSize, &nOffset);

	// Map Constant Buffers //
	MapBuffers();

	if (m_nInstances)
		d3dContext->DrawIndexedInstanced(m_nIndexArr.size(), m_nInstances, 0, 0, 0);
	else
		d3dContext->DrawIndexed(m_nIndexArr.size(), 0, 0);
}

void CHUDMesh::MapBuffers()
{
	CRenderMesh::MapBuffers();

	if (m_bRemap)
	{
		m_bRemap = false;
		D3D11_MAPPED_SUBRESOURCE d3dMapSubresource;

		// Grab Render Components //
		ID3D11DeviceContext* d3dContext = GRAPHICS->GetDeviceContext();

		// Send New Vertices To GPU //
		ZeroMemory(&d3dMapSubresource, sizeof(d3dMapSubresource));
		d3dContext->Map(m_d3dVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &d3dMapSubresource);
		memcpy(d3dMapSubresource.pData, &m_tVertex2DArr[0], sizeof(TVertex2D) * m_tVertex2DArr.size());
		d3dContext->Unmap(m_d3dVertexBuffer, 0);
	}
}