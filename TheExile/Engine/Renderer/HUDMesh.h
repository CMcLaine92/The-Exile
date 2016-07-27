#pragma once

#include "RenderMesh.h"

struct TVertex2D
{
	float m_fPosition[4];
	float m_fTexCoord[4];
};

class CHUDMesh : public CRenderMesh
{
	bool m_bRemap = false;
	std::vector<TVertex2D> m_tVertex2DArr;
	vector<unsigned int> m_nIndexArr;
	// Helper Functions //
	void MapBuffers() override;

public:
	CHUDMesh(std::vector<unsigned int> tIndexArr, std::vector<TVertex2D> tVertexArr,
		ID3D11VertexShader* d3dVertexShader, ID3D11PixelShader* d3dPixelShader,
		ID3D11HullShader* d3dHullShader = nullptr, ID3D11DomainShader* d3dDomainShader = nullptr,
		ID3D11GeometryShader* d3dGeometryShader = nullptr, const wchar_t* chTexturePath = nullptr);

	CHUDMesh(std::vector<unsigned int> tIndexArr, std::vector<TVertex2D> tVertexArr,
		ID3D11VertexShader* d3dVertexShader, ID3D11PixelShader* d3dPixelShader,
		ID3D11HullShader* d3dHullShader, ID3D11DomainShader* d3dDomainShader,
		ID3D11GeometryShader* d3dGeometryShader, ID3D11ShaderResourceView* d3dDiffuseTexture);

	virtual ~CHUDMesh();

	void Draw() override;

	// Accessors //
	std::vector<TVertex2D>& GetVertex2DArr() { m_bRemap = true; return m_tVertex2DArr; }
};

