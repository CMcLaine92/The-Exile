#include "RenderNode.h"

#include "Renderer.h"
#include "RenderMesh.h"
#include "ShaderCompilation.h"

#define VECTOR_REMOVE(vector, object) { for (unsigned int i = 0; i < vector.size(); i++) { auto iter = vector.begin(); while (iter != vector.end()) { if (*iter == object) { vector.erase(iter); return; } iter++; } } }

bool CRenderNode::Remove(CRenderMesh* cRemoveMesh)
{

		auto iter = m_cMeshArr.begin();
		while (iter != m_cMeshArr.end())
		{
			if (*iter == cRemoveMesh)
			{
				m_cMeshArr.erase(iter);
				return true;
			}

			iter++;
		}

	return false;
}

/*****************************************************************
* Render():			Applies context settings, renders all meshes, 
*					and reverts context settings
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
void CRenderNode::Render()
{
	m_cContext.Apply();

	for (unsigned int i = 0; i < m_cMeshArr.size(); i++)
		if (m_cMeshArr[i]->GetRender()) m_cMeshArr[i]->Draw();

	m_cContext.Revert();
}

bool CRenderNode::IsMatch(CRenderMesh* cNewMesh)
{
	if (cNewMesh->GetVertexShader() == m_cContext.GetVertexShader() &&
		cNewMesh->GetPixelShader() == m_cContext.GetPixelShader() &&
		cNewMesh->GetHullShader() == m_cContext.GetHullShader() &&
		cNewMesh->GetDomainShader() == m_cContext.GetDomainShader() &&
		cNewMesh->GetGeometryShader() == m_cContext.GetGeometryShader() &&
		cNewMesh->GetTopology() == m_cContext.GetTopology() &&
		cNewMesh->GetWireframe() == m_cContext.GetWireframe() &&
		cNewMesh->GetTransparent() == m_cContext.GetTransparent() &&
		cNewMesh->GetInputLayout() == m_cContext.GetInputLayout())
	{
		return true;
	}
	else
		return false;
}

void CRenderNode::Initialize(CRenderMesh* cNewMesh)
{
	m_cContext.SetVertexShader(cNewMesh->GetVertexShader());

	if (cNewMesh->GetVarPixelShader()) // Variable pixel shader
		m_cContext.SetVarPixelShader(cNewMesh->GetVarPixelShader()); // Only will get called if you have a variable shader set
		
	m_cContext.SetPixelShader(cNewMesh->GetPixelShader());
	
	m_cContext.SetHullShader(cNewMesh->GetHullShader());
	m_cContext.SetDomainShader(cNewMesh->GetDomainShader());
	m_cContext.SetGeometryShader(cNewMesh->GetGeometryShader());
	
	m_cContext.SetTopology(cNewMesh->GetTopology());
	m_cContext.SetWireframe(cNewMesh->GetWireframe());
	m_cContext.SetTransparent(cNewMesh->GetTransparent());
	m_cContext.SetInputLayout(cNewMesh->GetInputLayout());
}