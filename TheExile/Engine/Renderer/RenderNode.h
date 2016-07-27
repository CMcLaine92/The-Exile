#pragma once

/***********************************************
* Filename:  		RenderNode.h
* Date:      		08/12/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Nick Dalton
* Purpose:   		Applies context and calls
*					draw on meshes with identical
*					context settings
************************************************/

#include "RenderContext.h"

// Forward Declarations //
class CRenderMesh;

class CRenderNode
{
	bool bVarPixelShader = false;
	CRenderContext m_cContext;
	std::vector<CRenderMesh*> m_cMeshArr;

public:
	CRenderNode() = default;
	~CRenderNode() = default;

	void Clear() { m_cMeshArr.clear(); }
	bool Remove(CRenderMesh* cRemoveMesh);
	void Render();
	bool IsMatch(CRenderMesh* cNewMesh);
	bool IsEmpty() { return !m_cMeshArr.size(); }
	void Initialize(CRenderMesh* cNewMesh);
	void AddMesh(CRenderMesh* cNewMesh) { m_cMeshArr.push_back(cNewMesh); }

	// Accessor //
	CRenderContext& GetContext() { return m_cContext; }
	std::vector<CRenderMesh*>& GetMeshArr() { return m_cMeshArr; }
};