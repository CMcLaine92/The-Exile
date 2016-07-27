#include "RenderMesh.h"
#include "../Animation/Mesh.h"
#include "Renderer.h"

#define EDGE_AMOUNT 1.08f
#define VECTOR_REMOVE(vector, object) { for (unsigned int i = 0; i < vector.size(); i++) { auto iter = vector.begin(); while (iter != vector.end()) { if (*iter == object) { vector.erase(iter); return; } iter++; } } }

/*****************************************************************
* CRenderMesh():	Initializes the CRenderMesh class
*
* Ins:			    tIndexArr
*					tVertexArr
*					d3dVertexShader
*					d3dPixelShader
*					d3dHullShader
*					d3dDomainShader
*					d3dGeometryShader
*					chTexturePath
*
* Outs:				N/A
*
* Returns:		    N/A
*
* Mod. Date:		----------
* Mod. Initials:	--
*****************************************************************/
CRenderMesh::CRenderMesh(CMesh* _mesh,
	ID3D11VertexShader* d3dVertexShader, ID3D11PixelShader* d3dPixelShader,
	ID3D11HullShader* d3dHullShader, ID3D11DomainShader* d3dDomainShader,
	ID3D11GeometryShader* d3dGeometryShader, const wchar_t* chTexturePath)
{
	Create( _mesh,
		 d3dVertexShader, d3dPixelShader,
		 d3dHullShader,  d3dDomainShader,
		 d3dGeometryShader,  chTexturePath);
}

std::vector<unsigned int>& CRenderMesh::GetIndexArr() { return m_cpMesh->GetIndices(); }
std::vector<TVertex3D>& CRenderMesh::GetVertexArr() { return m_cpMesh->GetVertices(); }

/*****************************************************************
* ~CRenderMesh():	Cleans up the CRenderMesh class
*
* Ins:			    N/A
*
* Outs:				N/A
*
* Returns:		    N/A
*
* Mod. Date:		----------
* Mod. Initials:	--
*****************************************************************/
CRenderMesh::~CRenderMesh()
{
	SAFE_RELEASE(m_d3dIndexBuffer);
	SAFE_RELEASE(m_d3dVertexBuffer);

//	SAFE_RELEASE(m_d3dNormals);
}

void CRenderMesh::Create(CMesh* _mesh,
	ID3D11VertexShader* d3dVertexShader, ID3D11PixelShader* d3dPixelShader,
	ID3D11HullShader* d3dHullShader, ID3D11DomainShader* d3dDomainShader,
	ID3D11GeometryShader* d3dGeometryShader, const wchar_t* chTexturePath)
{
	ID3D11Device* d3dDevice = GRAPHICS->GetDevice();
	m_cpMesh = _mesh;
	m_d3dInputLayout = GRAPHICS->GetInputLayout3D();
	IdentityMatrix(m_d3dTransformMatrix);

	m_d3dTopologyType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	vector<unsigned int>& m_nIndexArr = m_cpMesh->GetIndices();
	vector<TVertex3D>& m_tVertexArr = m_cpMesh->GetVertices();
	m_unIndexCount = m_nIndexArr.size();

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
	GRAPHICS->indices += m_nIndexArr.size();

	d3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferResource, &m_d3dIndexBuffer);

	// Vertex Buffer //
	ZeroMemory(&d3dBufferResource, sizeof(d3dBufferResource));
	d3dBufferResource.pSysMem = &m_tVertexArr[0];
	d3dBufferResource.SysMemPitch = NULL;
	d3dBufferResource.SysMemSlicePitch = NULL;

	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.StructureByteStride = sizeof(TVertex3D);
	d3dBufferDesc.CPUAccessFlags = NULL;
	d3dBufferDesc.ByteWidth = (UINT)m_tVertexArr.size() * sizeof(TVertex3D);
	GRAPHICS->vertices += m_tVertexArr.size();

	d3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferResource, &m_d3dVertexBuffer);

	// Texture Creation //
	if (chTexturePath)
		SetTexture(chTexturePath);
	else
		SetTexture(L"../Game/Assets/Art/2D/Textures/Gray.dds");

	m_d3dBlack = GRAPHICS->GetTextureList()[L"../Game/Assets/Art/2D/HUD/Gold.dds"];
}
/*****************************************************************
* Draw():			Draws the current RenderMesh and set necessary
*					context settings
*
* Ins:			    N/A
*
* Outs:				N/A
*
* Returns:		    N/A
*
* Mod. Date:		----------
* Mod. Initials:	--
*****************************************************************/
void CRenderMesh::Draw()
{
	// Grab Render Components //
	ID3D11DeviceContext* d3dContext = GRAPHICS->GetDeviceContext();

	// Set Vertex + Index Buffers //
	unsigned int nOffset = 0;
	unsigned int nStrideSize = sizeof(TVertex);

	d3dContext->IASetIndexBuffer(m_d3dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	d3dContext->IASetVertexBuffers(0, 1, &m_d3dVertexBuffer, &nStrideSize, &nOffset);

	// Map Constant Buffers //
	MapBuffers();

	// Set Shader Resource Views //
	d3dContext->PSSetShaderResources(0, 1, &m_d3dDiffuse);

	if (m_bNormalMapped)
		d3dContext->PSSetShaderResources(1, 1, &m_d3dNormals);

	if (m_nInstances)
		d3dContext->DrawIndexedInstanced(m_unIndexCount, m_nInstances, 0, 0, 0);
	else
		d3dContext->DrawIndexed(m_unIndexCount, 0, 0);
}

/*****************************************************************
* SetTexture():		Sets new texture
*
* Ins:			    chTexturePath
*
* Outs:				N/A
*
* Returns:		    N/A
*
* Mod. Date:		----------
* Mod. Initials:	--
*****************************************************************/
void CRenderMesh::SetTexture(const wchar_t* chTexturePath)
{
	//m_d3dDiffuse = GRAPHICS->GetTextureList()[std::wstring(chTexturePath)];

	auto iterTextureList = GRAPHICS->GetTextureList().find(std::wstring(chTexturePath));

	if (iterTextureList != GRAPHICS->GetTextureList().end())
	{ 
		//m_d3dDiffuse = NULL;
		m_d3dDiffuse = GRAPHICS->GetTextureList()[std::wstring(chTexturePath)];
		return;
	}
	else
	{
		HRESULT result = CreateDDSTextureFromFile(GRAPHICS->GetDevice(), chTexturePath, nullptr, &m_d3dDiffuse);

		// Error Check //
		if (result == S_OK)
		{
			OUTPUT("Texture Load Successful\n");
			GRAPHICS->GetTextureList()[std::wstring(chTexturePath)] = m_d3dDiffuse;
		}
		else
			m_d3dDiffuse = GRAPHICS->GetTextureList()[L"../Game/Assets/Art/2D/Textures/Error.dds"];
	}
}

void CRenderMesh::SetNormals(const wchar_t* chNormalsPath)
{
	m_bNormalMapped = true; 
	auto iterTextureList = GRAPHICS->GetTextureList().find(std::wstring(chNormalsPath));

	if (iterTextureList != GRAPHICS->GetTextureList().end())
	{
		m_d3dNormals = GRAPHICS->GetTextureList()[std::wstring(chNormalsPath)];
		return;
	}
	HRESULT result = CreateDDSTextureFromFile(GRAPHICS->GetDevice(), chNormalsPath, nullptr, &m_d3dNormals);

	// Error Check //
	if (result == S_OK)
	{
		OUTPUT("Texture Load Successful\n");
		GRAPHICS->GetTextureList()[std::wstring(chNormalsPath)] = m_d3dNormals;
	}
	else
		m_d3dNormals = GRAPHICS->GetTextureList()[L"../Game/Assets/Art/2D/Textures/Error.dds"];

}

/*****************************************************************
* MapBuffers():		Maps necessary data to buffers
*
* Ins:			    N/A
*
* Outs:				N/A
*
* Returns:		    N/A
*
* Mod. Date:		----------
* Mod. Initials:	--
*****************************************************************/
void CRenderMesh::MapBuffers() // Default
{
	D3D11_MAPPED_SUBRESOURCE d3dMapSubresource;

	// Grab Render Components //
	ID3D11DeviceContext* d3dContext = GRAPHICS->GetDeviceContext();
	ID3D11Buffer* d3dTransformBuffer = GRAPHICS->GetTransformBuffer();
	ID3D11Buffer* d3dSpecularBuffer = GRAPHICS->GetSpecularBuffer();

	// Send Transform Matrix To GPU //
	ZeroMemory(&d3dMapSubresource, sizeof(d3dMapSubresource));
	d3dContext->Map(d3dTransformBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &d3dMapSubresource);
	memcpy(d3dMapSubresource.pData, &m_d3dTransformMatrix, sizeof(m_d3dTransformMatrix));
	d3dContext->Unmap(d3dTransformBuffer, 0);

	float4 fSpecularVec = { m_fSpecular, m_fTransparencyOverride, m_fSpecular, m_fSpecular };

	ZeroMemory(&d3dMapSubresource, sizeof(d3dMapSubresource));
	d3dContext->Map(d3dSpecularBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &d3dMapSubresource);
	memcpy(d3dMapSubresource.pData, &fSpecularVec, sizeof(fSpecularVec));
	d3dContext->Unmap(d3dSpecularBuffer, 0);
}

void CRenderMesh::DrawEdge()
{
	D3D11_MAPPED_SUBRESOURCE d3dMapSubresource;
	ID3D11DeviceContext* d3dContext = GRAPHICS->GetDeviceContext();
	ID3D11Buffer* d3dTransformBuffer = GRAPHICS->GetTransformBuffer();

	// Set Vertex + Index Buffers //
	unsigned int nOffset = 0;
	unsigned int nStrideSize = sizeof(TVertex);

	d3dContext->IASetIndexBuffer(m_d3dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	d3dContext->IASetVertexBuffers(0, 1, &m_d3dVertexBuffer, &nStrideSize, &nOffset);

	d3dContext->PSSetShaderResources(0, 1, &m_d3dBlack);

	// Temporarily Scale Up Matrix //
	XMFLOAT4X4 tempMatrix = m_d3dTransformMatrix;
	ScaleMatrix(m_d3dTransformMatrix, EDGE_AMOUNT);

	MapBuffers();

	m_d3dTransformMatrix = tempMatrix;

	if (m_bTransparent)
		d3dContext->OMSetBlendState(NULL, NULL, 0xfffffff);

	d3dContext->DrawIndexed(m_unIndexCount, 0, 0);

	if (m_bTransparent)
		d3dContext->OMSetBlendState(GRAPHICS->GetBlendState(), NULL, 0xfffffff);
}

void CRenderMesh::SetEdgeGlow(bool bEdgeGlow)
{
	if (bEdgeGlow && !m_bEdgeGlow)
	{
		m_bEdgeGlow = true;
		GRAPHICS->GetEGObjects().push_back(this);
	}
	else if (!bEdgeGlow && m_bEdgeGlow)
	{
		m_bEdgeGlow = false;
		VECTOR_REMOVE(GRAPHICS->GetEGObjects(), this);
	}
}

void CRenderMesh::ApplyContextOverride()
{
	ID3D11DeviceContext* d3dContext = GRAPHICS->GetDeviceContext();

	if (m_d3dVertexShader)
		d3dContext->VSSetShader(m_d3dVertexShader, nullptr, 0);
	if (m_d3dPixelShader)
		d3dContext->PSSetShader(m_d3dPixelShader, nullptr, 0);
	if (m_d3dHullShader)
		d3dContext->HSSetShader(m_d3dHullShader, nullptr, 0);
	if (m_d3dDomainShader)
		d3dContext->DSSetShader(m_d3dDomainShader, nullptr, 0);
	if (m_d3dGeometryShader)
		d3dContext->GSSetShader(m_d3dGeometryShader, nullptr, 0);
	if (m_bTransparent)
		d3dContext->OMSetBlendState(GRAPHICS->GetBlendState(), NULL, 0xfffffff);
	if (m_d3dPixelShader == GRAPHICS->GetPixelShader2D())
		d3dContext->OMSetDepthStencilState(GRAPHICS->GetDepthStencilState(), NULL);

	d3dContext->IASetInputLayout(m_d3dInputLayout);
	d3dContext->IASetPrimitiveTopology(m_d3dTopologyType);
}

void CRenderMesh::RevertContextOverride()
{
	ID3D11DeviceContext* d3dContext = GRAPHICS->GetDeviceContext();

	if (m_d3dVertexShader)
		d3dContext->VSSetShader(nullptr, nullptr, 0);
	if (m_d3dPixelShader)
		d3dContext->PSSetShader(nullptr, nullptr, 0);
	if (m_d3dHullShader)
		d3dContext->HSSetShader(nullptr, nullptr, 0);
	if (m_d3dDomainShader)
		d3dContext->DSSetShader(nullptr, nullptr, 0);
	if (m_d3dGeometryShader)
		d3dContext->GSSetShader(nullptr, nullptr, 0);

	d3dContext->RSSetState(GRAPHICS->GetSolidRasterizer());
	d3dContext->OMSetBlendState(NULL, NULL, 0xfffffff);
	d3dContext->OMSetDepthStencilState(NULL, NULL);
}

void CRenderMesh::ConvertNormals()
{
	//for (unsigned int i = 0; i < m_nIndexArr.size() - 3; i += 3)
	//{
	//	TVertex3D& v0 = m_tVertexArr[m_nIndexArr[i + 0]];
	//	TVertex3D& v1 = m_tVertexArr[m_nIndexArr[i + 1]];
	//	TVertex3D& v2 = m_tVertexArr[m_nIndexArr[i + 2]];

	//	XMVECTOR v0Pos = { v0.m_fPosition[0], v0.m_fPosition[1], v0.m_fPosition[2] };
	//	XMVECTOR v1Pos = { v1.m_fPosition[0], v1.m_fPosition[1], v1.m_fPosition[2] };
	//	XMVECTOR v2Pos = { v2.m_fPosition[0], v2.m_fPosition[1], v2.m_fPosition[2] };

	//	float3 v0Tex = { v0.m_fTexCoord[0], v0.m_fTexCoord[1], v0.m_fTexCoord[2] };
	//	float3 v1Tex = { v1.m_fTexCoord[0], v1.m_fTexCoord[1], v1.m_fTexCoord[2] };
	//	float3 v2Tex = { v2.m_fTexCoord[0], v2.m_fTexCoord[1], v2.m_fTexCoord[2] };

	//	float3 Edge1;
	//	float3 Edge2;

	//	XMStoreFloat3(&Edge1, v1Pos - v0Pos);
	//	XMStoreFloat3(&Edge2, v2Pos - v0Pos);

	//	float DeltaU1 = v1Tex.x - v0Tex.x;
	//	float DeltaV1 = v1Tex.y - v0Tex.y;
	//	float DeltaU2 = v2Tex.x - v0Tex.x;
	//	float DeltaV2 = v2Tex.y - v0Tex.y;
	//	float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

	//	v0.m_fTangent[0] = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
	//	v0.m_fTangent[1] = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
	//	v0.m_fTangent[2] = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

	//	v1.m_fTangent[0] = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
	//	v1.m_fTangent[1] = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
	//	v1.m_fTangent[2] = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

	//	v2.m_fTangent[0] = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
	//	v2.m_fTangent[1] = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
	//	v2.m_fTangent[2] = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);
	//}

	//for (unsigned int i = 0; i < m_tVertexArr.size(); i++) // Normalize
	//{
	//	XMVECTOR tangent = { m_tVertexArr[i].m_fTangent[0], m_tVertexArr[i].m_fTangent[1], m_tVertexArr[i].m_fTangent[2] };
	//	tangent = XMVector3Normalize(tangent);

	//	m_tVertexArr[i].m_fTangent[0] = tangent.m128_f32[0];
	//	m_tVertexArr[i].m_fTangent[1] = tangent.m128_f32[1];
	//	m_tVertexArr[i].m_fTangent[2] = tangent.m128_f32[2];
	//}
}

void CRenderMesh::CalculateTangentVectors(std::vector<TVertex>& _vertsvec, std::vector<unsigned int> & _indexvec)
{
	//Loosely Based Off of Code available at http://www.terathon.com/code/tangent.html 
	//Credit goes to
	//Lengyel, Eric. “Computing Tangent Space Basis Vectors for an Arbitrary Mesh”. Terathon Software 3D Graphics Library, 2001. http://www.terathon.com/code/tangent.html


	//std::vector<XMFLOAT4> tan1;
	//std::vector<XMFLOAT4> tan2;
	//XMFLOAT4 temp = { 0, 0, 0, 0 };
	//for (unsigned int i = 0; i < _vertsvec.size(); i++)
	//{
	//	tan1.push_back(temp);
	//	tan2.push_back(temp);
	//}

	//for (unsigned int index = 0; index < _indexvec.size(); index += 3)
	//{
	//	float xyz1[3];
	//	float xyz2[3];
	//	float st1[2];
	//	float st2[2];

	//	TVertex& _v1 = _vertsvec[_indexvec[index]];
	//	TVertex& _v2 = _vertsvec[_indexvec[index + 1]];
	//	TVertex& _v3 = _vertsvec[_indexvec[index + 2]];

	//	for (unsigned int i = 0; i < 3; i++)
	//	{
	//		xyz1[i] = _v2.m_fPosition[i] - _v1.m_fPosition[i];
	//		xyz2[i] = _v3.m_fPosition[i] - _v1.m_fPosition[i];
	//	}
	//	for (unsigned int i = 0; i < 2; i++)
	//	{
	//		st1[i] = _v2.m_fTexCoord[i] - _v1.m_fTexCoord[i];
	//		st2[i] = _v3.m_fTexCoord[i] - _v1.m_fTexCoord[i];
	//	}

	//	float r = 1.0f / (float)((st1[0] * st2[1]) - (st2[0] * st1[1]));
	//	XMVECTOR sdir = { (st2[1] * xyz1[0] - st1[1] * xyz2[0]) * r,
	//		(st2[1] * xyz1[1] - st1[1] * xyz2[1]) * r,
	//		(st2[1] * xyz1[2] - st1[1] * xyz2[2]) * r };

	//	XMVECTOR tdir = { (st1[0] * xyz2[0] - st2[0] * xyz1[0]) * r,
	//		(st1[0] * xyz2[1] - st2[0] * xyz1[1]) * r,
	//		(st1[0] * xyz2[2] - st2[0] * xyz1[2]) * r };

	//	// sdir = XMVector3Normalize(sdir);
	//	// tdir = XMVector3Normalize(tdir);

	//	tan1[_indexvec[index]].x += sdir.m128_f32[0];
	//	tan1[_indexvec[index]].y += sdir.m128_f32[1];
	//	tan1[_indexvec[index]].z += sdir.m128_f32[2];

	//	tan2[_indexvec[index]].x += tdir.m128_f32[0];
	//	tan2[_indexvec[index]].y += tdir.m128_f32[1];
	//	tan2[_indexvec[index]].z += tdir.m128_f32[2];

	//	tan1[_indexvec[index + 1]].x += sdir.m128_f32[0];
	//	tan1[_indexvec[index + 1]].y += sdir.m128_f32[1];
	//	tan1[_indexvec[index + 1]].z += sdir.m128_f32[2];

	//	tan2[_indexvec[index + 1]].x += tdir.m128_f32[0];
	//	tan2[_indexvec[index + 1]].y += tdir.m128_f32[1];
	//	tan2[_indexvec[index + 1]].z += tdir.m128_f32[2];

	//	tan1[_indexvec[index + 2]].x += sdir.m128_f32[0];
	//	tan1[_indexvec[index + 2]].y += sdir.m128_f32[1];
	//	tan1[_indexvec[index + 2]].z += sdir.m128_f32[2];

	//	tan2[_indexvec[index + 2]].x += tdir.m128_f32[0];
	//	tan2[_indexvec[index + 2]].y += tdir.m128_f32[1];
	//	tan2[_indexvec[index + 2]].z += tdir.m128_f32[2];

	//}
	//for (unsigned int index = 0; index < _vertsvec.size(); index++)
	//{
	//	XMVECTOR n;
	//	XMVECTOR t;
	//	t.m128_f32[0] = tan1[index].x;
	//	t.m128_f32[1] = tan1[index].y;
	//	t.m128_f32[2] = tan1[index].z;
	//	t.m128_f32[3] = tan1[index].w;

	//	XMVECTOR tang;
	//	for (unsigned int a = 0; a < 4; a++)
	//	{
	//		n.m128_f32[a] = _vertsvec[index].m_fNormal[a];
	//	}

	//	n = XMVector3Normalize(n);
	//	t = XMVector3Normalize(t);
	//	//Gram-Schmidt orthogonalize
	//	tang = (t - n * XMVector3Dot(n, t));
	//	tang = XMVector3Normalize(tang);


	//	XMVECTOR t2;
	//	t2.m128_f32[0] = tan2[index].x;
	//	t2.m128_f32[1] = tan2[index].y;
	//	t2.m128_f32[2] = tan2[index].z;

	//	//calc handedness
	//	tang.m128_f32[3] = (XMVector3Dot(XMVector3Cross(n, t), t2)).m128_f32[0] < 0.0f ? -1.0f : 1.0f;
	//	for (unsigned int i = 0; i < 4; i++)
	//	{
	//		_vertsvec[index].m_fTangent[i] = tang.m128_f32[i];

	//	}
	//}
}

/*
void CRenderMesh::ConvertNormals()
{
	// I have no clue what any of this does
	for (unsigned int i = 0; i < m_nIndexArr.size(); i++)
	{
		float3 v1 = { m_tVertexArr[i + 0].m_fPosition[0], m_tVertexArr[i + 0].m_fPosition[1], m_tVertexArr[i + 0].m_fPosition[2] };
		float3 v2 = { m_tVertexArr[i + 1].m_fPosition[0], m_tVertexArr[i + 1].m_fPosition[1], m_tVertexArr[i + 1].m_fPosition[2] };
		float3 v3 = { m_tVertexArr[i + 2].m_fPosition[0], m_tVertexArr[i + 2].m_fPosition[1], m_tVertexArr[i + 2].m_fPosition[2] };

		float2 w1 = { m_tVertexArr[i + 0].m_fTexCoord[0], m_tVertexArr[i + 0].m_fTexCoord[1] };
		float2 w2 = { m_tVertexArr[i + 1].m_fTexCoord[0], m_tVertexArr[i + 1].m_fTexCoord[1] };
		float2 w3 = { m_tVertexArr[i + 2].m_fTexCoord[0], m_tVertexArr[i + 2].m_fTexCoord[1] };

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;

		float r = 1.0f / (s1 * t2 - s2 * t1);
		float3 sDir = { (t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r };
		float3 tDir = { (s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r };

		m_tVertexArr[i + 0].m_fTangent[0] += sDir.x;
		m_tVertexArr[i + 1].m_fTangent[0] += sDir.x;
		m_tVertexArr[i + 2].m_fTangent[0] += sDir.x;

		m_tVertexArr[i + 0].m_fTangent[1] += sDir.y;
		m_tVertexArr[i + 1].m_fTangent[1] += sDir.y;
		m_tVertexArr[i + 2].m_fTangent[1] += sDir.y;

		m_tVertexArr[i + 0].m_fTangent[2] += sDir.z;
		m_tVertexArr[i + 1].m_fTangent[2] += sDir.z;
		m_tVertexArr[i + 2].m_fTangent[2] += sDir.z;

		m_tVertexArr[i + 0].m_fBitangent[0] += tDir.x;
		m_tVertexArr[i + 1].m_fBitangent[0] += tDir.x;
		m_tVertexArr[i + 2].m_fBitangent[0] += tDir.x;

		m_tVertexArr[i + 0].m_fBitangent[1] += tDir.y;
		m_tVertexArr[i + 1].m_fBitangent[1] += tDir.y;
		m_tVertexArr[i + 2].m_fBitangent[1] += tDir.y;

		m_tVertexArr[i + 0].m_fBitangent[2] += tDir.z;
		m_tVertexArr[i + 1].m_fBitangent[2] += tDir.z;
		m_tVertexArr[i + 2].m_fBitangent[2] += tDir.z;
	}

	for (unsigned int i = 0; i < m_tVertexArr.size(); i++)
	{
		XMVECTOR n = { m_tVertexArr[i].m_fNormal[0], m_tVertexArr[i].m_fNormal[1], m_tVertexArr[i].m_fNormal[2] };
		XMVECTOR t = { m_tVertexArr[i].m_fTangent[0], m_tVertexArr[i].m_fTangent[1], m_tVertexArr[i].m_fTangent[2] };
		
		XMVECTOR tangent = (t - n * XMVector3Dot(n, t));

		bool bZeroTangent = false;

		if (XMVector3Length(tangent).m128_f32[0] > 0.00001f)
			tangent = XMVector3Normalize(tangent);
		else if (XMVector3Length(n).m128_f32[0] > 0.00001f)
		{
			// Perpendicular //
			XMVECTOR perp;
			float x = std::abs(n.m128_f32[0]);
			float y = std::abs(n.m128_f32[1]);
			float z = std::abs(n.m128_f32[2]);
			float minVal = std::fmin(x, y);
			minVal = std::fmin(minVal, z);

			if (minVal == x)
			{
				XMVECTOR xVec = { 1.0f, 0.0f, 0.0f };
				perp = XMVector3Cross(n, xVec);
			}
			else if (minVal == y)
			{
				XMVECTOR yVec = { 0.0f, 1.0f, 0.0f };
				perp = XMVector3Cross(n, yVec);
			}
			else
			{
				XMVECTOR zVec = { 0.0f, 0.0f, 1.0f };
				perp = XMVector3Cross(n, zVec);
			}

			tangent = XMVector3Normalize(perp);

			bZeroTangent = true;
		}

		float sign = 1.0f;

		if (!bZeroTangent)
		{
			XMVECTOR bitangent = { m_tVertexArr[i].m_fBitangent[0], m_tVertexArr[i].m_fBitangent[1], m_tVertexArr[i].m_fBitangent[2] };

			XMVECTOR b;
			b = XMVector3Cross(n, t);
			sign = (XMVector3Dot(b, bitangent).m128_f32[0] < 0.0f) ? -1.0f : 1.0f;
		}

		tangent = XMVector3Normalize(tangent);
		m_tVertexArr[i].m_fTangent[0] = tangent.m128_f32[0];
		m_tVertexArr[i].m_fTangent[1] = tangent.m128_f32[1];
		m_tVertexArr[i].m_fTangent[2] = tangent.m128_f32[2];

		XMVECTOR bitangent = XMVector3Normalize(XMVector3Cross(n, tangent));
		m_tVertexArr[i].m_fBitangent[0] = bitangent.m128_f32[0] *= sign;
		m_tVertexArr[i].m_fBitangent[1] = bitangent.m128_f32[1] *= sign;
		m_tVertexArr[i].m_fBitangent[2] = bitangent.m128_f32[2] *= sign;
	}
}
*/

XMFLOAT3 CRenderMesh::GetPosition() const
{
	return XMFLOAT3(m_d3dTransformMatrix._41, m_d3dTransformMatrix._42, m_d3dTransformMatrix._43);
}
void CRenderMesh::SetPosition(XMFLOAT3 newPos)
{
	m_d3dTransformMatrix._41 = newPos.x;
	m_d3dTransformMatrix._42 = newPos.y;
	m_d3dTransformMatrix._43 = newPos.z;
}
