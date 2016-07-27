#include "RenderContext.h"
#include "Renderer.h"
#include "ShaderCompilation.h"

/*****************************************************************
* Apply():			Applies the current context
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
void CRenderContext::Apply()
{
	if (m_d3dVertexShader)
		GRAPHICS->GetDeviceContext()->VSSetShader(m_d3dVertexShader, nullptr, 0);

	if (m_d3dVarPixelShader)
	{
		std::vector<unsigned int> values =
		{
			(unsigned int)GRAPHICS->GetNumLights(),
			(unsigned int)((m_d3dPixelShader == GRAPHICS->GetNormalMapPS()) ? 1 : 0),
			0
		}; // Grab values
		m_d3dVarPixelShader->SetOptions(values);
		GRAPHICS->GetDeviceContext()->PSSetShader(*m_d3dVarPixelShader, nullptr, 0);
	}
	else if (m_d3dPixelShader)
		GRAPHICS->GetDeviceContext()->PSSetShader(m_d3dPixelShader, nullptr, 0);

	if (m_d3dHullShader)
		GRAPHICS->GetDeviceContext()->HSSetShader(m_d3dHullShader, nullptr, 0);
	if (m_d3dDomainShader)
		GRAPHICS->GetDeviceContext()->DSSetShader(m_d3dDomainShader, nullptr, 0);
	if (m_d3dGeometryShader)
		GRAPHICS->GetDeviceContext()->GSSetShader(m_d3dGeometryShader, nullptr, 0);
	if (m_bWireframe)
		GRAPHICS->GetDeviceContext()->RSSetState(GRAPHICS->GetWireframeRasterizer());
	if (m_bTransparent)
		GRAPHICS->GetDeviceContext()->OMSetBlendState(GRAPHICS->GetBlendState(), NULL, 0xfffffff);
	if (m_d3dPixelShader == GRAPHICS->GetPixelShader2D())
		GRAPHICS->GetDeviceContext()->OMSetDepthStencilState(GRAPHICS->GetDepthStencilState(), NULL);


	GRAPHICS->GetDeviceContext()->IASetInputLayout(m_d3dInputLayout);
	GRAPHICS->GetDeviceContext()->IASetPrimitiveTopology(m_d3dTopologyType);
}

/*****************************************************************
* Revert():			Reverts the changed context settings
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
void CRenderContext::Revert()
{
	if (m_d3dVertexShader)
		GRAPHICS->GetDeviceContext()->VSSetShader(nullptr, nullptr, 0);
	if (m_d3dPixelShader)
		GRAPHICS->GetDeviceContext()->PSSetShader(nullptr, nullptr, 0);
	if (m_d3dHullShader)
		GRAPHICS->GetDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	if (m_d3dDomainShader)
		GRAPHICS->GetDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	if (m_d3dGeometryShader)
		GRAPHICS->GetDeviceContext()->GSSetShader(nullptr, nullptr, 0);

	GRAPHICS->GetDeviceContext()->RSSetState(GRAPHICS->GetSolidRasterizer());
	GRAPHICS->GetDeviceContext()->OMSetBlendState(NULL, NULL, 0xfffffff);
	GRAPHICS->GetDeviceContext()->OMSetDepthStencilState(NULL, NULL);
}
