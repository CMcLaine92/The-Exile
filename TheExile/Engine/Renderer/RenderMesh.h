#pragma once

/***********************************************
* Filename:  		RenderMesh.h
* Date:      		08/12/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Nick Dalton
* Purpose:   		Stores the vertices, texture,
*					shaders, and position matrix,
*					and manages drawing
************************************************/

#include "../Core/MathWrapper.inl"
#include "DDSTextureLoader.h"
#include "../Animation/Mesh.h"
class Shader;

//struct TVertex3D
//{
//	float m_fPosition[4];
//	float m_fNormal[4];
//	float m_fTexCoord[4];
//	float m_fTangent[4];
//};


class CRenderMesh
{
protected:
	float m_fSpecular = 0.1f;
	float m_fTransparencyOverride = -1.0f;
	unsigned int m_unIndexCount = 0;
	bool m_bRender = true;
	bool m_bTransparent = false;
	bool m_bNormalMapped = false;
	std::type_index m_cType = typeid(CRenderMesh);

	// Render Options //
	bool m_bEdgeGlow = false;
	int m_nInstances = 0;
	bool m_bWireframe = false;
	D3D11_PRIMITIVE_TOPOLOGY m_d3dTopologyType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// DirectX Variables //
	XMFLOAT4X4 m_d3dTransformMatrix;
	ID3D11InputLayout* m_d3dInputLayout = nullptr;

	CMesh* m_cpMesh = nullptr;

	/*std::vector<unsigned int> m_nIndexArr;
	std::vector<TVertex3D> m_tVertexArr;
*/
	ID3D11Buffer* m_d3dIndexBuffer = nullptr;
	ID3D11Buffer* m_d3dVertexBuffer = nullptr;

	ID3D11VertexShader* m_d3dVertexShader = nullptr;
	ID3D11PixelShader* m_d3dPixelShader = nullptr;

	ID3D11HullShader* m_d3dHullShader = nullptr;
	ID3D11DomainShader* m_d3dDomainShader = nullptr;
	ID3D11GeometryShader* m_d3dGeometryShader = nullptr;

	ID3D11ShaderResourceView* m_d3dBlack = nullptr;
	ID3D11ShaderResourceView* m_d3dDiffuse = nullptr;
	ID3D11ShaderResourceView* m_d3dNormals = nullptr;

	Shader* m_d3dVarPixelShader = nullptr;

	// Helper Functions //
	virtual void MapBuffers();

public:
	CRenderMesh() = default;
	CRenderMesh(CMesh* _mesh,
		ID3D11VertexShader* d3dVertexShader, ID3D11PixelShader* d3dPixelShader,
		ID3D11HullShader* d3dHullShader = nullptr, ID3D11DomainShader* d3dDomainShader = nullptr,
		ID3D11GeometryShader* d3dGeometryShader = nullptr, const wchar_t* chTexturePath = nullptr);

	void Create(CMesh* _mesh,
		ID3D11VertexShader* d3dVertexShader, ID3D11PixelShader* d3dPixelShader,
		ID3D11HullShader* d3dHullShader, ID3D11DomainShader* d3dDomainShader,
		ID3D11GeometryShader* d3dGeometryShader, const wchar_t* chTexturePath);
	virtual ~CRenderMesh();

	virtual void Draw();
	void DrawEdge();
	virtual void SetTexture(const wchar_t* chTexturePath);
	virtual void SetTexture(ID3D11ShaderResourceView* d3dDiffuseTexture) { m_d3dDiffuse = d3dDiffuseTexture; }
	virtual void SetNormals(const wchar_t* chNormalsPath);
	void ConvertNormals();
	void CalculateTangentVectors(std::vector<TVertex3D>& _vertsvec, std::vector<unsigned int> & _indexvec);
	void ApplyContextOverride();
	void RevertContextOverride();

	// Accessors/Mutators //
	float& GetSpecular() { return m_fSpecular; }
	void SetEdgeGlow(bool bEdgeGlow);
	bool& GetRender() { return m_bRender; }
	bool& GetTransparent() { return m_bTransparent; }
	float& GetTransparencyOverride() { return m_fTransparencyOverride; }
	std::type_index& GetType() { return m_cType; }
	CMesh* GetMesh() { return m_cpMesh; }
	int& GetInstances() { return m_nInstances; }
	bool& GetWireframe() { return m_bWireframe; }
	D3D11_PRIMITIVE_TOPOLOGY& GetTopology() { return m_d3dTopologyType; }

	XMFLOAT4X4& GetTransformMatrix() { return m_d3dTransformMatrix; }
	XMFLOAT4X4& GetPositionMatrix() { return m_d3dTransformMatrix; } // Legacy
	ID3D11InputLayout* GetInputLayout() { return m_d3dInputLayout; }

	std::vector<unsigned int>& GetIndexArr();
	std::vector<TVertex3D>& GetVertexArr();

	ID3D11VertexShader* GetVertexShader() { return m_d3dVertexShader; }
	ID3D11PixelShader* GetPixelShader() { return m_d3dPixelShader; }

	ID3D11HullShader* GetHullShader() { return m_d3dHullShader; }
	ID3D11DomainShader* GetDomainShader() { return m_d3dDomainShader; }
	ID3D11GeometryShader* GetGeometryShader() { return m_d3dGeometryShader; }

	XMFLOAT3 GetPosition() const;
	void SetPosition(XMFLOAT3 newPos);

	Shader*& GetVarPixelShader() { return m_d3dVarPixelShader; }
};