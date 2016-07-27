#pragma once

/***********************************************
* Filename:  		RenderContext.h
* Date:      		08/12/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Nick Dalton
* Purpose:   		Stores and applies context
*					settings
************************************************/

class Shader;

class CRenderContext
{
	ID3D11InputLayout* m_d3dInputLayout;

	// Shaders //
	ID3D11VertexShader* m_d3dVertexShader = nullptr;
	Shader* m_d3dVarPixelShader = nullptr;
	ID3D11PixelShader* m_d3dPixelShader = nullptr;

	ID3D11HullShader* m_d3dHullShader = nullptr;
	ID3D11DomainShader* m_d3dDomainShader = nullptr;
	ID3D11GeometryShader* m_d3dGeometryShader = nullptr;

	// Render Options //
	bool m_bWireframe = false;
	bool m_bTransparent = false;
	D3D11_PRIMITIVE_TOPOLOGY m_d3dTopologyType;

public:
	CRenderContext() = default;
	~CRenderContext() = default;

	void Apply();
	void Revert();

	// Accessors //

	ID3D11InputLayout* GetInputLayout() { return m_d3dInputLayout; }
	ID3D11VertexShader* GetVertexShader() { return m_d3dVertexShader; }
	Shader* GetVarPixelShader() { return m_d3dVarPixelShader; }
	ID3D11PixelShader* GetPixelShader() { return m_d3dPixelShader; }

	ID3D11HullShader* GetHullShader() { return m_d3dHullShader; }
	ID3D11DomainShader* GetDomainShader() { return m_d3dDomainShader; }
	ID3D11GeometryShader* GetGeometryShader() { return m_d3dGeometryShader; }

	bool GetWireframe() { return m_bWireframe; }
	bool GetTransparent() { return m_bTransparent; }
	D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return m_d3dTopologyType; }

	// Mutators //
	void SetInputLayout(ID3D11InputLayout* d3dInputLayout) { m_d3dInputLayout = d3dInputLayout; }

	void SetVertexShader(ID3D11VertexShader* d3dVertexShader) { m_d3dVertexShader = d3dVertexShader; }
	void SetVarPixelShader(Shader* d3dVarPixelShader) { m_d3dVarPixelShader = d3dVarPixelShader; }
	void SetPixelShader(ID3D11PixelShader* d3dPixelShader) { m_d3dPixelShader = d3dPixelShader; }

	void SetHullShader(ID3D11HullShader* d3dHullShader) { m_d3dHullShader = d3dHullShader; }
	void SetDomainShader(ID3D11DomainShader* d3dDomainShader) { m_d3dDomainShader = d3dDomainShader; }
	void SetGeometryShader(ID3D11GeometryShader* d3dGeometryShader) { m_d3dGeometryShader = d3dGeometryShader; }

	void SetWireframe(bool bWireframe) { m_bWireframe = bWireframe; }
	void SetTransparent(bool bTransparent) { m_bTransparent = bTransparent; }
	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY d3dTopologyType) { m_d3dTopologyType = d3dTopologyType; }
};