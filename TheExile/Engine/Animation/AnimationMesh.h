#pragma once
#include "../Renderer/RenderMesh.h"
#include "Mesh.h"
#include <unordered_map>


struct TBones
{
	XMFLOAT4X4 m_fBone[100];
	XMFLOAT4X4 m_fInvBone[100];
};


class CAnimationMesh : public CRenderMesh
{

	TBones m_tBones;
	/*unsigned int m_nAnimVertexCount;
	std::vector<TAnimVertex> m_tAnimVertexArr;*/

	ID3D11Buffer* m_d3dAnimVertexBuffer;
	unordered_map<string, XMFLOAT4X4> m_mapJoints;

	vector<XMFLOAT4X4> m_tSkinningPose;



public:
	//CMesh * m_cMesh;
	CAnimationMesh(CMesh* mesh, 
				   ID3D11VertexShader* d3dVertexShader, ID3D11PixelShader* d3dPixelShader, 
				   ID3D11HullShader* d3dHullShader = nullptr, ID3D11DomainShader* d3dDomainShader = nullptr, 
				   ID3D11GeometryShader* d3dGeometryShader = nullptr, const wchar_t* chTexturePath = nullptr,
				   D3D11_PRIMITIVE_TOPOLOGY d3dTopologyType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	~CAnimationMesh();

	void Create(CMesh* _mesh,
		ID3D11VertexShader* d3dVertexShader, ID3D11PixelShader* d3dPixelShader,
		ID3D11HullShader* d3dHullShader, ID3D11DomainShader* d3dDomainShader,
		ID3D11GeometryShader* d3dGeometryShader, const wchar_t* chTexturePath,
		D3D11_PRIMITIVE_TOPOLOGY d3dTopologyType);

	void Draw() override;

	// Accessors //
	unsigned int GetAnimVertexCount();// { return m_cpMesh->GetAnimVertices().size(); }
	//std::vector<TAnimVertex>& GetAnimVertexArr() { return m_tAnimVertexArr; }
	XMFLOAT4X4 GetJointMatrix(string _key) { return m_mapJoints[_key]; }
	vector<XMFLOAT4X4>& GetPose() { return m_tSkinningPose; }
	const vector<XMFLOAT4X4>& GetPose() const { return m_tSkinningPose; }

	TBones GetBones() { return m_tBones; }
	void SetBones(vector<XMFLOAT4X4>& _fBones, vector<Joint>& _bindPose);
};
