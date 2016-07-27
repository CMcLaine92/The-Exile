#include "AnimationMesh.h"

#include "../Animation/Mesh.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/DDSTextureLoader.h"


CAnimationMesh::CAnimationMesh(CMesh* _mesh, 
							   ID3D11VertexShader* d3dVertexShader, ID3D11PixelShader* d3dPixelShader, 
							   ID3D11HullShader* d3dHullShader, ID3D11DomainShader* d3dDomainShader, 
							   ID3D11GeometryShader* d3dGeometryShader, const wchar_t* chTexturePath, 
							   D3D11_PRIMITIVE_TOPOLOGY d3dTopologyType)
{
	Create(_mesh,
		d3dVertexShader, d3dPixelShader,
		d3dHullShader, d3dDomainShader,
		d3dGeometryShader, chTexturePath,
		d3dTopologyType);
}
void CAnimationMesh::Create(CMesh* _mesh,
	ID3D11VertexShader* d3dVertexShader, ID3D11PixelShader* d3dPixelShader,
	ID3D11HullShader* d3dHullShader, ID3D11DomainShader* d3dDomainShader,
	ID3D11GeometryShader* d3dGeometryShader, const wchar_t* chTexturePath,
	D3D11_PRIMITIVE_TOPOLOGY d3dTopologyType)
{
	m_cType = typeid(CAnimationMesh);

	XMStoreFloat4x4(&m_d3dTransformMatrix, XMMatrixIdentity());
	m_cpMesh = _mesh;
	m_d3dTopologyType = d3dTopologyType;

	vector<unsigned int>& m_nIndexArr = m_cpMesh->GetIndices();
	vector<TAnimVertex>& m_tAnimVertexArr = m_cpMesh->GetAnimVertices();

	m_unIndexCount = m_nIndexArr.size();

	m_d3dInputLayout = GRAPHICS->GetInputLayoutAnim();

	m_d3dVertexShader = d3dVertexShader;
	m_d3dPixelShader = d3dPixelShader;

	m_d3dHullShader = d3dHullShader;
	m_d3dDomainShader = d3dDomainShader;
	m_d3dGeometryShader = d3dGeometryShader;

	// Index Buffer Creation //
	D3D11_SUBRESOURCE_DATA d3dIndexResource;
	ZeroMemory(&d3dIndexResource, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dIndexResource.pSysMem = &m_nIndexArr[0];
	d3dIndexResource.SysMemPitch = NULL;
	d3dIndexResource.SysMemSlicePitch = NULL;

	D3D11_BUFFER_DESC d3dIndexBufferDesc;
	ZeroMemory(&d3dIndexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dIndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	d3dIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d3dIndexBufferDesc.StructureByteStride = sizeof(unsigned int);
	d3dIndexBufferDesc.CPUAccessFlags = NULL;
	d3dIndexBufferDesc.ByteWidth = (UINT)m_nIndexArr.size() * sizeof(unsigned int);

	GRAPHICS->GetDevice()->CreateBuffer(&d3dIndexBufferDesc, &d3dIndexResource, &m_d3dIndexBuffer);

	// Animation Vertex Buffer Creation //
	D3D11_SUBRESOURCE_DATA d3dAnimVertexResource;
	ZeroMemory(&d3dAnimVertexResource, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dAnimVertexResource.pSysMem = &m_tAnimVertexArr[0];
	d3dAnimVertexResource.SysMemPitch = NULL;
	d3dAnimVertexResource.SysMemSlicePitch = NULL;

	D3D11_BUFFER_DESC d3dAnimVertexBufferDesc;
	ZeroMemory(&d3dAnimVertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dAnimVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	d3dAnimVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dAnimVertexBufferDesc.StructureByteStride = sizeof(TAnimVertex);
	d3dAnimVertexBufferDesc.CPUAccessFlags = NULL;
	d3dAnimVertexBufferDesc.ByteWidth = (UINT)m_tAnimVertexArr.size() * sizeof(TAnimVertex);

	GRAPHICS->GetDevice()->CreateBuffer(&d3dAnimVertexBufferDesc, &d3dAnimVertexResource, &m_d3dAnimVertexBuffer);

	// Texture Creation //
	if (chTexturePath)
		SetTexture(chTexturePath);
	else
		SetTexture(L"../Game/Assets/Art/2D/Textures/Gray.dds");
}
CAnimationMesh::~CAnimationMesh()
{
	//SAFE_RELEASE(m_d3dAnimVertexBuffer);
}


void CAnimationMesh::Draw()
{
	// Set Shader Resource View //
	GRAPHICS->GetDeviceContext()->PSSetShaderResources(0, 1, &m_d3dDiffuse);

	if (m_bNormalMapped)
		GRAPHICS->GetDeviceContext()->PSSetShaderResources(1, 1, &m_d3dNormals);

	// Set Buffers //
	unsigned int nOffset = 0;
	unsigned int nStrideSize = sizeof(TAnimVertex);

	GRAPHICS->GetDeviceContext()->IASetIndexBuffer(m_d3dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	GRAPHICS->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_d3dAnimVertexBuffer, &nStrideSize, &nOffset);

	// Send Position To GPU //
	D3D11_MAPPED_SUBRESOURCE d3dMapSubResource;
	ZeroMemory(&d3dMapSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	GRAPHICS->GetDeviceContext()->Map(GRAPHICS->GetTransformBuffer(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &d3dMapSubResource);
	memcpy(d3dMapSubResource.pData, &m_d3dTransformMatrix, sizeof(XMFLOAT4X4));
	GRAPHICS->GetDeviceContext()->Unmap(GRAPHICS->GetTransformBuffer(), 0);

	// Send Bones To GPU //
	ZeroMemory(&d3dMapSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	GRAPHICS->GetDeviceContext()->Map(GRAPHICS->GetBoneBuffer(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &d3dMapSubResource);
	memcpy(d3dMapSubResource.pData, &m_tBones, sizeof(TBones));
	GRAPHICS->GetDeviceContext()->Unmap(GRAPHICS->GetBoneBuffer(), 0);


	if (m_nInstances)
		GRAPHICS->GetDeviceContext()->DrawIndexedInstanced(m_unIndexCount, m_nInstances, 0, 0, 0);
	else
		GRAPHICS->GetDeviceContext()->DrawIndexed(m_unIndexCount, 0, 0);
}

void CAnimationMesh::SetBones(vector<XMFLOAT4X4>& _fBones, vector<Joint>& _bindPose)
{
	for (unsigned int bone = 0; bone < _fBones.size(); bone++)
	{
		Joint& joint = _bindPose[bone];
		m_tBones.m_fBone[bone] = _fBones[bone];
		XMMATRIX invBindPose = XMLoadFloat4x4(&joint.bind_pose_transform);
		XMVECTOR det = XMMatrixDeterminant(invBindPose);
		invBindPose = XMMatrixInverse(&det, invBindPose);
		XMStoreFloat4x4(&m_tBones.m_fInvBone[bone], invBindPose);
		m_mapJoints[joint.name] = _fBones[bone];
	}
}