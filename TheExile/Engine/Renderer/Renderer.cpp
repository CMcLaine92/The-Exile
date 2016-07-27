#include "Renderer.h"

#include "../Animation/Mesh.h"
#include "Lights.h"
#include "LineMesh.h"
#include "DirectXTK.h"
#include "RenderMesh.h"
#include "ParticleMesh.h"
#include "DX11Helpers.inl"
#include "ShaderCompilation.h"
#include "../Core/MathWrapper.inl"
#include "../Animation/AnimationMesh.h"

#pragma comment(lib, "DirectXTK.lib")
#pragma comment( lib, "dxguid.lib")

// Shader Includes //
#include "../Shaders/Compiled Shaders/LineVS.csh"
#include "../Shaders/Compiled Shaders/HBlurVS.csh"
#include "../Shaders/Compiled Shaders/VBlurVS.csh"
#include "../Shaders/Compiled Shaders/SkyboxVS.csh"
#include "../Shaders/Compiled Shaders/ParticleVS.csh"
#include "../Shaders/Compiled Shaders/SkinningVS.csh"
#include "../Shaders/Compiled Shaders/StandardVS.csh"
#include "../Shaders/Compiled Shaders/Standard2DVS.csh"
#include "../Shaders/Compiled Shaders/LuminanceVS.csh"
#include "../Shaders/Compiled Shaders/PostProcessorVS.csh"

#include "../Shaders/Compiled Shaders/HBlurPS.csh"
#include "../Shaders/Compiled Shaders/VBlurPS.csh"
#include "../Shaders/Compiled Shaders/LinePS.csh"
#include "../Shaders/Compiled Shaders/SkyboxPS.csh"
#include "../Shaders/Compiled Shaders/ParticlePS.csh"
#include "../Shaders/Compiled Shaders/StandardPS.csh"
#include "../Shaders/Compiled Shaders/NormalMapPS.csh"
#include "../Shaders/Compiled Shaders/Standard2DPS.csh"
#include "../Shaders/Compiled Shaders/DepthPrepass.csh"
#include "../Shaders/Compiled Shaders/LuminancePS.csh"
#include "../Shaders/Compiled Shaders/PostProcessorPS.csh"

#include "../Shaders/Compiled Shaders/ParticleGS.csh"
#include "../Shaders/Compiled Shaders/SunMoonGS.csh"

#include "../Shaders/Compiled Shaders/StandardCS.csh"

bool compare(TPointLight* a, TPointLight* b)
{
	return a->DistanceToCamera(GRAPHICS->GetViewMatrix()) < b->DistanceToCamera(GRAPHICS->GetViewMatrix());
}

#define DX_NAME(variable, name) (variable)->SetPrivateData(WKPDID_D3DDebugObjectName, (unsigned int)string(name).size(), string(name).c_str())
#define SHADOW_PASSES 0
#define SHADOW_FOV 90

// Global Accessor //
const void* gGlobalGraphics = nullptr;

/*****************************************************************
* CRenderer():		Initializes the CRenderer class and DirectX
*
* Ins:			    hWindow
*
* Outs:				N/A
*
* Returns:		    N/A
*
* Mod. Date:		----------
* Mod. Initials:	--
*****************************************************************/
CRenderer::CRenderer(HWND hWindow)
{
	// Global Accessor Initialization //
	gGlobalGraphics = this;

	// DirectX Initialization //
	#if _DEBUG
		m_bDebugMode = true;
		m_nMultiSampleCount = 1;
		m_nMultiSampleQuality = 0;
	#else
		m_bDebugMode = false;
		m_nMultiSampleCount = 1;
		m_nMultiSampleQuality = 0;
	#endif

	m_fClearColor[0] = 
	m_fClearColor[1] = 
	m_fClearColor[2] = 
	m_fClearColor[3] = 0;


	// Swap Chain Description Creation //
	DXGI_SWAP_CHAIN_DESC d3dSwapChainDescription;
	ZeroMemory(&d3dSwapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC));
	d3dSwapChainDescription.BufferCount = 1;
	d3dSwapChainDescription.BufferDesc.Width = (UINT)WINDOW_WIDTH;
	d3dSwapChainDescription.BufferDesc.Height = (UINT)WINDOW_HEIGHT;
	d3dSwapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dSwapChainDescription.BufferDesc.RefreshRate.Numerator = 60;
	d3dSwapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
	d3dSwapChainDescription.Windowed = true;
	d3dSwapChainDescription.OutputWindow = hWindow;
	d3dSwapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	d3dSwapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	d3dSwapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	d3dSwapChainDescription.SampleDesc.Count = m_nMultiSampleCount;
	d3dSwapChainDescription.SampleDesc.Quality = m_nMultiSampleQuality;

	// Feature Levels //
	const D3D_FEATURE_LEVEL d3dFeatureLevels[3] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_9_3,
	};

	// Device and Swap Chain Description //
	D3D11CreateDeviceAndSwapChain(nullptr, // Default adapter
								  D3D_DRIVER_TYPE_HARDWARE, // Render with hardware
								  nullptr, // No need for software rasterizer
								  (m_bDebugMode) ? D3D11_CREATE_DEVICE_DEBUG : 0, // Debug mode check
								  d3dFeatureLevels, // Supported DirectX versions array
								  3, // Number of supported DirectX versions
								  D3D11_SDK_VERSION, // No other options
								  &d3dSwapChainDescription, // Swap chain options
								  &m_d3dSwapChain, // Swap chain output param
								  &m_d3dDevice, // Device output param
								  NULL, // Current DirectX version
								  &m_d3dContext); // Device context output param


	// Disable Alt+Enter //
	IDXGIFactory1* d3dFactory = nullptr;
	if (SUCCEEDED(m_d3dSwapChain->GetParent(__uuidof(d3dFactory), (void**)(&d3dFactory))))
		d3dFactory->MakeWindowAssociation(hWindow, DXGI_MWA_NO_ALT_ENTER);
		
	SAFE_RELEASE(d3dFactory);

	
	// Viewport Creation //
	ZeroMemory(&m_d3dViewport, sizeof(D3D11_VIEWPORT));
	m_d3dViewport.Width = (FLOAT)WINDOW_WIDTH;
	m_d3dViewport.Height = (FLOAT)WINDOW_HEIGHT;
	m_d3dViewport.MinDepth = 0;
	m_d3dViewport.MaxDepth = 1;
	m_d3dViewport.TopLeftX = 0;
	m_d3dViewport.TopLeftY = 0;

	m_d3dContext->RSSetViewports(1, &m_d3dViewport);

	// Depth Stencil State Creation //
	D3D11_DEPTH_STENCIL_DESC d3dDepthStencil;
	ZeroMemory(&d3dDepthStencil, sizeof(D3D11_DEPTH_STENCIL_DESC));
	d3dDepthStencil.DepthEnable = false;
	d3dDepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencil.DepthFunc = D3D11_COMPARISON_LESS;
	d3dDepthStencil.StencilEnable = false;
	d3dDepthStencil.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	d3dDepthStencil.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	d3dDepthStencil.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	d3dDepthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencil.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencil.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
	d3dDepthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencil.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencil.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

	m_d3dDevice->CreateDepthStencilState(&d3dDepthStencil, &m_d3dDepthStencilState);

	// Depth Stencil State Outline Creation //
	ZeroMemory(&d3dDepthStencil, sizeof(D3D11_DEPTH_STENCIL_DESC));
	d3dDepthStencil.DepthEnable = true;
	d3dDepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencil.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
	d3dDepthStencil.StencilEnable = false;
	d3dDepthStencil.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	d3dDepthStencil.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	d3dDepthStencil.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	d3dDepthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencil.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencil.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
	d3dDepthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencil.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencil.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

	m_d3dDevice->CreateDepthStencilState(&d3dDepthStencil, &m_d3dDepthStencilStateOutline);
	m_d3dContext->OMSetDepthStencilState(NULL, NULL);

	// Blend State Creation //
	D3D11_BLEND_DESC d3dBlendStateDesc;
	ZeroMemory(&d3dBlendStateDesc, sizeof(D3D11_BLEND_DESC));
	for (unsigned int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
	{
		d3dBlendStateDesc.RenderTarget[i].BlendEnable = true;
		d3dBlendStateDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		d3dBlendStateDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		d3dBlendStateDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		d3dBlendStateDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		d3dBlendStateDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		d3dBlendStateDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		d3dBlendStateDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	m_d3dDevice->CreateBlendState(&d3dBlendStateDesc, &m_d3dBlendState);

	// Wireframe Rasterizer Creation //
	D3D11_RASTERIZER_DESC d3dWireframeRasterizerDesc;
	ZeroMemory(&d3dWireframeRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dWireframeRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3dWireframeRasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3dWireframeRasterizerDesc.FrontCounterClockwise = false;
	d3dWireframeRasterizerDesc.DepthBias = 0;
	d3dWireframeRasterizerDesc.DepthBiasClamp = 0;
	d3dWireframeRasterizerDesc.SlopeScaledDepthBias = true;
	d3dWireframeRasterizerDesc.ScissorEnable = false;
	d3dWireframeRasterizerDesc.MultisampleEnable = true;
	d3dWireframeRasterizerDesc.AntialiasedLineEnable = false;

	m_d3dDevice->CreateRasterizerState(&d3dWireframeRasterizerDesc, &m_d3dWireframeRasterizer);

	// Outline Rasterizer Creation //
	D3D11_RASTERIZER_DESC d3dOutlineRasterizerDesc;
	ZeroMemory(&d3dOutlineRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dOutlineRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3dOutlineRasterizerDesc.CullMode = D3D11_CULL_FRONT;
	d3dOutlineRasterizerDesc.FrontCounterClockwise = false;
	d3dOutlineRasterizerDesc.DepthBias = 0;
	d3dOutlineRasterizerDesc.DepthBiasClamp = 0;
	d3dOutlineRasterizerDesc.SlopeScaledDepthBias = true;
	d3dOutlineRasterizerDesc.ScissorEnable = false;
	d3dOutlineRasterizerDesc.MultisampleEnable = true;
	d3dOutlineRasterizerDesc.AntialiasedLineEnable = false;

	m_d3dDevice->CreateRasterizerState(&d3dOutlineRasterizerDesc, &m_d3dOutlineRasterizer);

	// Solid Rasterizer Creation //
	D3D11_RASTERIZER_DESC d3dSolidRasterizerDesc;
	ZeroMemory(&d3dSolidRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dSolidRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3dSolidRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dSolidRasterizerDesc.FrontCounterClockwise = false;
	d3dSolidRasterizerDesc.DepthBias = 0;
	d3dSolidRasterizerDesc.DepthBiasClamp = 0;
	d3dSolidRasterizerDesc.SlopeScaledDepthBias = true;
	d3dSolidRasterizerDesc.ScissorEnable = false;
	d3dSolidRasterizerDesc.MultisampleEnable = true;
	d3dSolidRasterizerDesc.AntialiasedLineEnable = false;

	m_d3dDevice->CreateRasterizerState(&d3dSolidRasterizerDesc, &m_d3dSolidRasterizer);
	m_d3dContext->RSSetState(m_d3dSolidRasterizer);

	// Edge Rasterizer Creation //
	D3D11_RASTERIZER_DESC d3dEdgeRasterizerDesc;
	ZeroMemory(&d3dSolidRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dEdgeRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3dEdgeRasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3dEdgeRasterizerDesc.FrontCounterClockwise = false;
	d3dEdgeRasterizerDesc.DepthBias = 0;
	d3dEdgeRasterizerDesc.DepthBiasClamp = 0;
	d3dEdgeRasterizerDesc.SlopeScaledDepthBias = true;
	d3dEdgeRasterizerDesc.ScissorEnable = false;
	d3dEdgeRasterizerDesc.MultisampleEnable = true;
	d3dEdgeRasterizerDesc.AntialiasedLineEnable = false;

	m_d3dDevice->CreateRasterizerState(&d3dEdgeRasterizerDesc, &m_d3dEdgeRasterizer);
	m_d3dContext->RSSetState(m_d3dEdgeRasterizer);


	// Back Buffer Creation //
	HRESULT hResult = m_d3dSwapChain->GetBuffer(NULL, __uuidof(m_d3dBackBuffer), (void**)(&m_d3dBackBuffer)); // Grab texture
	assert("Back buffer retrieval failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dBackBuffer, "Back Buffer");

	hResult = m_d3dDevice->CreateRenderTargetView(m_d3dBackBuffer, nullptr, &m_d3dBackBufferRTV); // Create RTV
	assert("Back buffer RTV creation failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dBackBufferRTV, "Back Buffer RTV");

	D3D11_TEXTURE2D_DESC d3dTextureDesc;
	CreateTextureDesc(2048, 2048, DXGI_FORMAT_R32_TYPELESS, m_nMultiSampleCount, m_nMultiSampleQuality,
		D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, NULL, NULL, d3dTextureDesc);

	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDSVDesc;
	CreateDSVDesc(NULL, DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2D, d3dDSVDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc;
	CreateSRVDesc(DXGI_FORMAT_R32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2D, d3dSRVDesc);


	// Scene Texture Creation //
	CreateTextureDesc(2048, 2048, DXGI_FORMAT_R32G32B32A32_FLOAT, m_nMultiSampleCount, m_nMultiSampleQuality,
					  D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, NULL, NULL, d3dTextureDesc);

	hResult = m_d3dDevice->CreateTexture2D(&d3dTextureDesc, NULL, &m_d3dSceneTexture); // Create texture
	assert("Scene texture creation failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dSceneTexture, "Scene Texture");

	hResult = m_d3dDevice->CreateRenderTargetView(m_d3dSceneTexture, nullptr, &m_d3dSceneTextureRTV); // Create RTV
	assert("Scene texture RTV creation failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dSceneTextureRTV, "Scene Texture RTV");

	hResult = m_d3dDevice->CreateShaderResourceView(m_d3dSceneTexture, nullptr, &m_d3dSceneTextureSRV); // Create SRV
	assert("Scene texture SRV creation failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dSceneTextureSRV, "Scene Texture SRV");


	// Depth Buffer Creation //
	CreateTextureDesc(WINDOW_WIDTH, WINDOW_HEIGHT, DXGI_FORMAT_D32_FLOAT, m_nMultiSampleCount, m_nMultiSampleQuality,
					  D3D11_BIND_DEPTH_STENCIL, NULL, NULL, d3dTextureDesc);

	CreateDSVDesc(NULL, DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2D, d3dDSVDesc);

	CreateSRVDesc(DXGI_FORMAT_R32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2D, d3dSRVDesc);

	hResult = m_d3dDevice->CreateTexture2D(&d3dTextureDesc, NULL, &m_d3dDepthBuffer); // Create texture
	assert("Depth buffer creation failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dDepthBuffer, "Depth Buffer");

	hResult = m_d3dDevice->CreateDepthStencilView(m_d3dDepthBuffer, &d3dDSVDesc, &m_d3dDepthBufferDSV); // Create DSV
	assert("Depth buffer DSV creation failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dDepthBufferDSV, "Depth Buffer DSV");

	// Luminance Texture Creation //
	CreateTextureDesc(WINDOW_WIDTH, WINDOW_HEIGHT, DXGI_FORMAT_R32G32B32A32_FLOAT, m_nMultiSampleCount, m_nMultiSampleQuality,
					  D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, NULL, NULL, d3dTextureDesc);

	hResult = m_d3dDevice->CreateTexture2D(&d3dTextureDesc, NULL, &m_d3dLuminanceTexture); // Create texture
	assert("Luminance texture creation failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dLuminanceTexture, "Luminance Texture");

	hResult = m_d3dDevice->CreateRenderTargetView(m_d3dLuminanceTexture, nullptr, &m_d3dLuminanceTextureRTV); // Create RTV
	assert("Luminance texture RTV creation failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dLuminanceTextureRTV, "Luminance Texture RTV");

	hResult = m_d3dDevice->CreateShaderResourceView(m_d3dLuminanceTexture, nullptr, &m_d3dLuminanceTextureSRV); // Create SRV
	assert("Luminance texture SRV creation failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dLuminanceTextureSRV, "Luminance Texture SRV");

	// HBlur Texture Creation //
	CreateTextureDesc(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, DXGI_FORMAT_R32G32B32A32_FLOAT, m_nMultiSampleCount, m_nMultiSampleQuality,
					  D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, NULL, NULL, d3dTextureDesc);

	hResult = m_d3dDevice->CreateTexture2D(&d3dTextureDesc, NULL, &m_d3dHBlurTexture); // Create texture
	assert("HBlur texture creation failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dHBlurTexture, "HBlur Texture");

	hResult = m_d3dDevice->CreateRenderTargetView(m_d3dHBlurTexture, nullptr, &m_d3dHBlurTextureRTV); // Create RTV
	assert("HBlur texture RTV creation failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dHBlurTextureRTV, "HBlur Texture RTV");

	hResult = m_d3dDevice->CreateShaderResourceView(m_d3dHBlurTexture, nullptr, &m_d3dHBlurTextureSRV); // Create SRV
	assert("HBlur texture SRV creation failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dHBlurTextureSRV, "HBlur Texture SRV");
	

	// VBlur Texture Creation //
	CreateTextureDesc(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, DXGI_FORMAT_R32G32B32A32_FLOAT, m_nMultiSampleCount, m_nMultiSampleQuality,
					  D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, NULL, NULL, d3dTextureDesc);

	hResult = m_d3dDevice->CreateTexture2D(&d3dTextureDesc, NULL, &m_d3dVBlurTexture); // Create texture
	assert("VBlur texture creation failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dVBlurTexture, "VBlur Texture");

	hResult = m_d3dDevice->CreateRenderTargetView(m_d3dVBlurTexture, nullptr, &m_d3dVBlurTextureRTV); // Create RTV
	assert("VBlur texture RTV creation failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dVBlurTextureRTV, "VBlur Texture RTV");

	hResult = m_d3dDevice->CreateShaderResourceView(m_d3dVBlurTexture, nullptr, &m_d3dVBlurTextureSRV); // Create SRV
	assert("VBlur texture SRV creation failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dVBlurTextureSRV, "VBlur Texture SRV");
	

	//hResult = m_d3dDevice->CreateShaderResourceView(m_d3dDepthBuffer, &d3dSRVDesc, &m_d3dDepthBufferSRV); // Create SRV
	//assert("Depth buffer SRV creation failed!" && SUCCEEDED(hResult));
	//DX_NAME(m_d3dDepthBufferSRV, "Depth Buffer SRV");



	// Shader Creation //
	m_d3dDevice->CreateVertexShader(LineVS, sizeof(LineVS), nullptr, &m_d3dLineVS);
	m_d3dDevice->CreateVertexShader(HBlurVS, sizeof(HBlurVS), nullptr, &m_d3dHBlurVS);
	m_d3dDevice->CreateVertexShader(VBlurVS, sizeof(VBlurVS), nullptr, &m_d3dVBlurVS);
	m_d3dDevice->CreateVertexShader(SkyboxVS, sizeof(SkyboxVS), nullptr, &m_d3dSkyboxVS);
	m_d3dDevice->CreateVertexShader(ParticleVS, sizeof(ParticleVS), nullptr, &m_d3dParticleVS);
	m_d3dDevice->CreateVertexShader(SkinningVS, sizeof(SkinningVS), nullptr, &m_d3dSkinningVS);
	m_d3dDevice->CreateVertexShader(StandardVS, sizeof(StandardVS), nullptr, &m_d3dStandardVS);
	m_d3dDevice->CreateVertexShader(Standard2DVS, sizeof(Standard2DVS), nullptr, &m_d3dStandard2DVS);
	m_d3dDevice->CreateVertexShader(LuminanceVS, sizeof(LuminanceVS), nullptr, &m_d3dLuminanceVS);
	m_d3dDevice->CreateVertexShader(PostProcessorVS, sizeof(PostProcessorVS), nullptr, &m_d3dPostProcessorVS);

	m_d3dDevice->CreatePixelShader(HBlurPS, sizeof(HBlurPS), nullptr, &m_d3dHBlurPS);
	m_d3dDevice->CreatePixelShader(VBlurPS, sizeof(VBlurPS), nullptr, &m_d3dVBlurPS);
	m_d3dDevice->CreatePixelShader(LinePS, sizeof(LinePS), nullptr, &m_d3dLinePS);
	m_d3dDevice->CreatePixelShader(SkyboxPS, sizeof(SkyboxPS), nullptr, &m_d3dSkyboxPS);
	m_d3dDevice->CreatePixelShader(ParticlePS, sizeof(ParticlePS), nullptr, &m_d3dParticlePS);
	m_d3dDevice->CreatePixelShader(StandardPS, sizeof(StandardPS), nullptr, &m_d3dStandardPS);
	m_d3dDevice->CreatePixelShader(NormalMapPS, sizeof(NormalMapPS), nullptr, &m_d3dNormalMapPS);
	m_d3dDevice->CreatePixelShader(Standard2DPS, sizeof(Standard2DPS), nullptr, &m_d3dStandard2DPS);
	m_d3dDevice->CreatePixelShader(DepthPrepass, sizeof(DepthPrepass), nullptr, &m_d3dDepthPrepass);
	m_d3dDevice->CreatePixelShader(LuminancePS, sizeof(LuminancePS), nullptr, &m_d3dLuminancePS);
	m_d3dDevice->CreatePixelShader(PostProcessorPS, sizeof(PostProcessorPS), nullptr, &m_d3dPostProcessorPS);

	m_d3dDevice->CreateGeometryShader(ParticleGS, sizeof(ParticleGS), nullptr, &m_d3dParticleGS);
	m_d3dDevice->CreateGeometryShader(SunMoonGS, sizeof(SunMoonGS), nullptr, &m_d3dSunMoonGS);

	m_d3dDevice->CreateComputeShader(StandardCS, sizeof(StandardCS), nullptr, &m_d3dStandardCS);
	
	// Input Layout Creation // - 2D
	D3D11_INPUT_ELEMENT_DESC d3d2DElementDesc[] =
	{
		{ "POSITIONS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORDS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	GRAPHICS->GetDevice()->CreateInputLayout(d3d2DElementDesc, 2, Standard2DVS, sizeof(Standard2DVS), &m_d3dInputLayout2D);

	// Input Layout Creation // - 3D
	D3D11_INPUT_ELEMENT_DESC d3dElementDesc[] =
	{
		{ "POSITIONS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORDS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	GRAPHICS->GetDevice()->CreateInputLayout(d3dElementDesc, 4, StandardVS, sizeof(StandardVS), &m_d3dInputLayout3D);

	// Input Layout Creation // - Lines
	D3D11_INPUT_ELEMENT_DESC d3dLineElementDesc[] =
	{
		{ "POSITIONS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLORS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	GRAPHICS->GetDevice()->CreateInputLayout(d3dLineElementDesc, 2, LineVS, sizeof(LineVS), &m_d3dInputLayoutLine);

	// Input Layout Creation // - Particle
	D3D11_INPUT_ELEMENT_DESC d3dParticleElementDesc[] =
	{
		{ "POSITIONS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "STARTLIFE", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "CURRENTLIFE", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	GRAPHICS->GetDevice()->CreateInputLayout(d3dParticleElementDesc, 4, ParticleVS, sizeof(ParticleVS), &m_d3dInputLayoutParticle);

	// Input Layout Creation // - Animations
	D3D11_INPUT_ELEMENT_DESC d3dAnimElementDesc[] =
	{
		{ "POSITIONS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORDS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	GRAPHICS->GetDevice()->CreateInputLayout(d3dAnimElementDesc, 6, SkinningVS, sizeof(SkinningVS), &m_d3dInputLayoutAnim);

	// Sampler State Creation //
	CD3D11_SAMPLER_DESC d3dSamplerDesc = CD3D11_SAMPLER_DESC(CD3D11_DEFAULT());
	d3dSamplerDesc.AddressU = 
	d3dSamplerDesc.AddressV =
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	m_d3dDevice->CreateSamplerState(&d3dSamplerDesc, &m_d3dSamplerState);
	m_d3dContext->PSSetSamplers(0, 1, &m_d3dSamplerState);

	// - Buffer Creation - //
	D3D11_BUFFER_DESC d3dBufferDesc;

	// Transform Buffer //
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	d3dBufferDesc.MiscFlags = 0;
	d3dBufferDesc.StructureByteStride = 0;

	m_d3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_d3dTransformBuffer);
	m_d3dContext->VSSetConstantBuffers(0, 1, &m_d3dTransformBuffer);
	m_d3dContext->PSSetConstantBuffers(0, 1, &m_d3dTransformBuffer);
	m_d3dContext->GSSetConstantBuffers(0, 1, &m_d3dTransformBuffer);
	// Scene Buffer //
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(TScene);
	d3dBufferDesc.MiscFlags = 0;
	d3dBufferDesc.StructureByteStride = 0;

	m_d3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_d3dSceneBuffer);
	m_d3dContext->VSSetConstantBuffers(1, 1, &m_d3dSceneBuffer);
	m_d3dContext->GSSetConstantBuffers(1, 1, &m_d3dSceneBuffer);

	// Bone Buffer //
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(TBones);
	d3dBufferDesc.MiscFlags = 0;
	d3dBufferDesc.StructureByteStride = 0;

	m_d3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_d3dBoneBuffer);
	m_d3dContext->VSSetConstantBuffers(2, 1, &m_d3dBoneBuffer);

	// Light Buffer //
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(TLightPack);
	d3dBufferDesc.MiscFlags = 0;
	d3dBufferDesc.StructureByteStride = 0;

	m_d3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_d3dLightBuffer);
	m_d3dContext->PSSetConstantBuffers(5, 1, &m_d3dLightBuffer);

	// Camera Buffer //
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(float) * 4;
	d3dBufferDesc.MiscFlags = 0;
	d3dBufferDesc.StructureByteStride = 0;

	m_d3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_d3dCameraBuffer);
	m_d3dContext->PSSetConstantBuffers(7, 1, &m_d3dCameraBuffer);

	// Specular Buffer //
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(float) * 4;
	d3dBufferDesc.MiscFlags = 0;
	d3dBufferDesc.StructureByteStride = 0;

	m_d3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_d3dSpecularBuffer);
	m_d3dContext->PSSetConstantBuffers(8, 1, &m_d3dSpecularBuffer);

	// Particle Buffer //
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(TParticleBuffer);
	d3dBufferDesc.MiscFlags = 0;
	d3dBufferDesc.StructureByteStride = 0;

	m_d3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_d3dParticleBuffer);
	m_d3dContext->GSSetConstantBuffers(10, 1, &m_d3dParticleBuffer);

	// Gamma Buffer //
	CreateBufferDesc(sizeof(float) * 4, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE,
					 0, d3dBufferDesc);

	m_d3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_d3dGammaBuffer);
	m_d3dContext->PSSetConstantBuffers(9, 1, &m_d3dGammaBuffer);

	// View Matrix //
	m_tScene.m_d3dViewMatrix =
	{
		 1, 0,  0, 0,
		 0, 1,  0, 0,
		 0, 0,  1, 0,
		 0, 2, -7, 1,
	};

	// Projection Matrix //
	float yScale = 1 / (tan(RADIANS(FOV) / 2)); // 1 / Cotangent of half the FOV (90)
	float xScale = yScale / RATIO; // Width / Height

	m_tScene.m_d3dProjectionMatrix =
	{
		xScale,		 0,													   0, 0,
			 0, yScale,													   0, 0,
			 0,		 0,				  (fFarPlane / (fFarPlane - fNearPlane)), 1,
			 0,		 0, -(fFarPlane * fNearPlane) / (fFarPlane - fNearPlane), 0
	};

	// Line List Creation //
	m_cLines = new CLineMesh();
	AddRenderMesh(m_cLines);
	
	m_fDrawTime.resize(60);
	for (unsigned int i = 0; i < 60; i++) m_fDrawTime[i] = 0;
	
	m_cText = new CTextRenderer(m_d3dContext, { (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT });
	LoadFont(L"Diablo.font");

	// Render To Texture //
	D3D11_TEXTURE2D_DESC d3dMapTextureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC d3dMapRenderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC d3dMapShaderResourceViewDesc;

	// Map Texture //
	ZeroMemory(&d3dMapTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	d3dMapTextureDesc.Width = WINDOW_WIDTH;
	d3dMapTextureDesc.Height = WINDOW_HEIGHT;
	d3dMapTextureDesc.MipLevels = 1;
	d3dMapTextureDesc.ArraySize = 1;
	d3dMapTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	d3dMapTextureDesc.SampleDesc.Count = m_nMultiSampleCount;
	d3dMapTextureDesc.SampleDesc.Quality = m_nMultiSampleQuality;
	d3dMapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dMapTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	d3dMapTextureDesc.CPUAccessFlags = 0;
	d3dMapTextureDesc.MiscFlags = 0;

	m_d3dDevice->CreateTexture2D(&d3dMapTextureDesc, NULL, &m_d3dMapTexture);

	d3dMapRenderTargetViewDesc.Format = d3dMapTextureDesc.Format;
	d3dMapRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	d3dMapRenderTargetViewDesc.Texture2D.MipSlice = 0;

	m_d3dDevice->CreateRenderTargetView(m_d3dMapTexture, &d3dMapRenderTargetViewDesc, &m_d3dMapRenderTargetView);

	ZeroMemory(&d3dMapShaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dMapShaderResourceViewDesc.Format = d3dMapTextureDesc.Format;
	d3dMapShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	d3dMapShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	d3dMapShaderResourceViewDesc.Texture2D.MipLevels = 1;

	m_d3dDevice->CreateShaderResourceView(m_d3dMapTexture, &d3dMapShaderResourceViewDesc, &m_d3dMapShaderResourceView);

	std::vector<ShaderMacro> tMacroSet = 
	{
		{ "NUM_LIGHTS", 51 },
		{ "NORMAL_MAPPED", 2 },
		{ "SPECULAR_MAPPED", 2 }
	};

	CMesh cBillboardMesh;

	cBillboardMesh.GetIndices().push_back(0);
	cBillboardMesh.GetIndices().push_back(1);
	cBillboardMesh.GetIndices().push_back(2);

	cBillboardMesh.GetIndices().push_back(0);
	cBillboardMesh.GetIndices().push_back(2);
	cBillboardMesh.GetIndices().push_back(3);

	// Front Face // 
	cBillboardMesh.GetVertices().push_back
	({
		{ -1, 1, 0, 1 },
		{ 0, 0, -1 },
		{ 0, 0 }
	});

	cBillboardMesh.GetVertices().push_back
	({
		{ 1, 1, 0, 1 },
		{ 0, 0, -1 },
		{ 1, 0 }
	});

	cBillboardMesh.GetVertices().push_back
	({
		{ 1, -1, 0, 1 },
		{ 0, 0, -1 },
		{ 1, 1 }
	});

	cBillboardMesh.GetVertices().push_back
	({
		{ -1, -1, 0, 1 },
		{ 0, 0, -1 },
		{ 0, 1 }
	});

	cBillboard = new CRenderMesh(&cBillboardMesh, m_d3dPostProcessorVS, m_d3dPostProcessorPS);
	cBillboard->GetTransparent() = true;

	//Shader::Create<ID3D11PixelShader>(m_d3dPixelShaderVar, tMacroSet, L"PixelShader.hlsl", "main", "ps_5_0");

	//ToggleFullScreen();
}

/*****************************************************************
* ~CRenderer():		Cleans up the CRenderer class
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
CRenderer::~CRenderer()
{
	delete m_cText;
	delete m_cLines;
	SAFE_DELETE(cBillboard);

	auto iter = m_d3dTextureList.begin();

	while (iter != m_d3dTextureList.end())
	{
		SAFE_RELEASE((*iter).second);
		iter++;
	}

	SAFE_RELEASE(m_d3dSceneBuffer);

	SAFE_RELEASE(m_d3dSamplerState);

	SAFE_RELEASE(m_d3dInputLayout2D);
	SAFE_RELEASE(m_d3dInputLayout3D);
	SAFE_RELEASE(m_d3dInputLayoutLine);
	SAFE_RELEASE(m_d3dInputLayoutParticle);
	SAFE_RELEASE(m_d3dInputLayoutAnim);

	SAFE_RELEASE(m_d3dBlendState);
	SAFE_RELEASE(m_d3dEdgeRasterizer);
	SAFE_RELEASE(m_d3dSolidRasterizer);
	SAFE_RELEASE(m_d3dOutlineRasterizer);
	SAFE_RELEASE(m_d3dWireframeRasterizer);
	SAFE_RELEASE(m_d3dDepthStencilState);
	SAFE_RELEASE(m_d3dDepthStencilStateOutline);

	SAFE_RELEASE(m_d3dBackBuffer);
	SAFE_RELEASE(m_d3dBackBufferRTV);

	SAFE_RELEASE(m_d3dSceneTexture);
	SAFE_RELEASE(m_d3dSceneTextureRTV);
	SAFE_RELEASE(m_d3dSceneTextureSRV);

	SAFE_RELEASE(m_d3dDepthBuffer);
	SAFE_RELEASE(m_d3dDepthBufferDSV);
	SAFE_RELEASE(m_d3dDepthBufferSRV);

	SAFE_RELEASE(m_d3dLuminanceTexture);
	SAFE_RELEASE(m_d3dLuminanceTextureRTV);
	SAFE_RELEASE(m_d3dLuminanceTextureSRV);

	SAFE_RELEASE(m_d3dHBlurTexture);
	SAFE_RELEASE(m_d3dHBlurTextureRTV);
	SAFE_RELEASE(m_d3dHBlurTextureSRV);

	SAFE_RELEASE(m_d3dVBlurTexture);
	SAFE_RELEASE(m_d3dVBlurTextureRTV);
	SAFE_RELEASE(m_d3dVBlurTextureSRV);

	SAFE_RELEASE(m_d3dSwapChain);
	
	SAFE_RELEASE(m_d3dDevice);
	SAFE_RELEASE(m_d3dContext);

	SAFE_RELEASE(m_d3dMapTexture);
	SAFE_RELEASE(m_d3dMapRenderTargetView);
	SAFE_RELEASE(m_d3dMapShaderResourceView);

	SAFE_RELEASE(m_d3dTransformBuffer);
	SAFE_RELEASE(m_d3dSceneBuffer);
	SAFE_RELEASE(m_d3dBoneBuffer);
	SAFE_RELEASE(m_d3dLightBuffer);
	SAFE_RELEASE(m_d3dCameraBuffer);
	SAFE_RELEASE(m_d3dSpecularBuffer);
	SAFE_RELEASE(m_d3dParticleBuffer);
	SAFE_RELEASE(m_d3dGammaBuffer);

	SAFE_RELEASE(m_d3dHBlurVS);
	SAFE_RELEASE(m_d3dHBlurPS);
	SAFE_RELEASE(m_d3dVBlurPS);
	SAFE_RELEASE(m_d3dLineVS);
	SAFE_RELEASE(m_d3dSkyboxVS);
	SAFE_RELEASE(m_d3dParticleVS);
	SAFE_RELEASE(m_d3dSkinningVS);
	SAFE_RELEASE(m_d3dStandardVS);
	SAFE_RELEASE(m_d3dStandard2DVS);
	SAFE_RELEASE(m_d3dLuminanceVS);
	SAFE_RELEASE(m_d3dPostProcessorVS);

	SAFE_RELEASE(m_d3dLinePS);
	SAFE_RELEASE(m_d3dSkyboxPS);
	SAFE_RELEASE(m_d3dParticlePS);
	SAFE_RELEASE(m_d3dStandardPS);
	SAFE_RELEASE(m_d3dNormalMapPS);
	SAFE_RELEASE(m_d3dStandard2DPS);
	SAFE_RELEASE(m_d3dDepthPrepass);
	SAFE_RELEASE(m_d3dLuminancePS);
	SAFE_RELEASE(m_d3dPostProcessorPS);

	SAFE_RELEASE(m_d3dParticleGS);
	SAFE_RELEASE(m_d3dSunMoonGS);

	SAFE_RELEASE(m_d3dStandardCS);
}

/*****************************************************************
* Update():			Clears screen, executes RenderNodes, and
*					presents result
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
void CRenderer::Update()
{
	// Update Debug Info //
	//float fCurrentTime = CURRENT_TIME();
	
	if (m_bDebugCamera)
		UpdateCamera();
	
	sort(m_tPointLightArr.begin(), m_tPointLightArr.end(), compare);
	
	// Draw //
	MapBuffers();
	DrawAll();
	
	// Post Process //
	PostProcess();
	
	// Clear Text & Lines //
	m_cLines->Clear();
	m_cTextBuffer.clear();
	
	// Present //
	m_d3dSwapChain->Present(0, 0);
	//
	//OUTPUT("Total Objects: " + to_string(m_nTotalObjects) + '\n');
	//OUTPUT("Total Time: " + to_string(CURRENT_TIME() - fCurrentTime) + " seconds\n");
}

void CRenderer::DrawAll()
{
	ClearScreen(m_d3dSceneTextureRTV);
	ClearDepth(m_d3dDepthBufferDSV);

	m_d3dContext->OMSetRenderTargets(1, &m_d3dSceneTextureRTV, m_d3dDepthBufferDSV);


	if (!m_cSkybox.IsEmpty())
	{
		m_cSkybox.GetMeshArr()[0]->GetPositionMatrix().m[3][0] = m_tScene.m_d3dViewMatrix.m[3][0];
		m_cSkybox.GetMeshArr()[0]->GetPositionMatrix().m[3][1] = m_tScene.m_d3dViewMatrix.m[3][1];
		m_cSkybox.GetMeshArr()[0]->GetPositionMatrix().m[3][2] = m_tScene.m_d3dViewMatrix.m[3][2];
		m_cSkybox.GetMeshArr()[0]->GetPositionMatrix().m[3][3] = m_tScene.m_d3dViewMatrix.m[3][3];
		m_cSkybox.Render();
		ClearDepth(m_d3dDepthBufferDSV);
	}

	for (unsigned int i = 0; i < m_cEGObjects.size(); i++) // Edge glow objects
	{
		m_cEGObjects[i]->ApplyContextOverride();
	
		m_d3dContext->OMSetDepthStencilState(NULL, NULL);
		m_d3dContext->RSSetState(m_d3dEdgeRasterizer);
		m_cEGObjects[i]->DrawEdge();
	
		m_d3dContext->OMSetDepthStencilState(m_d3dDepthStencilStateOutline, NULL);
		m_cEGObjects[i]->Draw();
	
		m_cEGObjects[i]->RevertContextOverride();
	}

	m_d3dContext->OMSetDepthStencilState(NULL, NULL);

	for (unsigned int i = 0; i < m_cRenderList.size(); i++)
		m_cRenderList[i].Render();

	for (unsigned int i = 0; i < m_cTransList.size(); i++)
		m_cTransList[i].Render();

	ClearDepth(m_d3dDepthBufferDSV); // Clear for HUD objects

	for (unsigned int i = 0; i < m_cHUDList.size(); i++)
		m_cHUDList[i].Render();

	for (unsigned int i = 0; i < m_cTextBuffer.size(); i++)
		m_cText->RenderText(m_cTextBuffer[i].m_chText.c_str(), m_cTextBuffer[i].m_fPosition, 
							m_cTextBuffer[i].m_fColor, m_cTextBuffer[i].m_fScale, 
							m_cTextBuffer[i].m_nAlign, m_cTextBuffer[i].m_nFontIndex);
}

void CRenderer::PostProcess()
{
	cBillboard->ApplyContextOverride();

	ClearScreen(m_d3dBackBufferRTV);
	m_d3dContext->OMSetRenderTargets(1, &m_d3dBackBufferRTV, nullptr);
	m_d3dContext->PSSetShaderResources(4, 1, &m_d3dSceneTextureSRV);
	m_d3dContext->VSSetShader(m_d3dPostProcessorVS, nullptr, NULL);
	m_d3dContext->PSSetShader(m_d3dPostProcessorPS, nullptr, NULL);

	cBillboard->Draw();

	UnbindPixelShaderSRV(4);
}

void CRenderer::SetGamma(float fGamma)
{
	IDXGIOutput* d3dOutput = nullptr;
}

void CRenderer::LookAt(float3 fDirection, XMFLOAT4X4& fMatrix)
{
	if ((fDirection.x == 0 &&
		fDirection.y == 1 &&
		fDirection.z == 0) || 
		(fDirection.x == 0 && 
		fDirection.y == -1 && 
		fDirection.z == 0))
	{
		fDirection.y -= 0.01f;
		fDirection.z = 0.01f;
	}

	//IdentityMatrix(m_tShadowScene.m_d3dViewMatrix);
	float3 fUpVec = { 0, 1.0f, 0 };
	XMStoreFloat3(&fDirection, XMVector3Normalize(XMLoadFloat3(&fDirection)));

	float3 zAxis;
	zAxis.x = fDirection.x;
	zAxis.y = fDirection.y;
	zAxis.z = fDirection.z;
	XMStoreFloat3(&zAxis, XMVector3Normalize(XMLoadFloat3(&zAxis)));

	float3 xAxis;
	XMStoreFloat3(&xAxis, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&fUpVec), XMLoadFloat3(&zAxis))));

	float3 yAxis;
	XMStoreFloat3(&yAxis, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&zAxis), XMLoadFloat3(&xAxis))));

	fMatrix.m[0][0] = xAxis.x;
	fMatrix.m[0][1] = xAxis.y;
	fMatrix.m[0][2] = xAxis.z;
	fMatrix.m[0][3] = 0.0f;
	
	fMatrix.m[1][0] = yAxis.x;
	fMatrix.m[1][1] = yAxis.y;
	fMatrix.m[1][2] = yAxis.z;
	fMatrix.m[1][3] = 0.0f;
	
	fMatrix.m[2][0] = zAxis.x;
	fMatrix.m[2][1] = zAxis.y;
	fMatrix.m[2][2] = zAxis.z;
	fMatrix.m[2][3] = 0.0f;

	//TranslateLocalZ(m_tShadowScene.m_d3dViewMatrix, -200);
}

void CRenderer::ToggleFullScreen(unsigned int nWidth, unsigned int nHeight)
{
	if (nWidth && nHeight) // If a custom resolution gets passed in
	{
		CUSTOM_RES_WIDTH = nWidth;
		CUSTOM_RES_HEIGHT = nHeight;
		WINDOW_WIDTH = nWidth;
		WINDOW_HEIGHT = nHeight;
	}
	else if (WINDOW_WIDTH == SCREEN_WIDTH)
	{
		WINDOW_WIDTH = CUSTOM_RES_WIDTH;
		WINDOW_HEIGHT = CUSTOM_RES_HEIGHT;
	}
	else
	{
		WINDOW_WIDTH = SCREEN_WIDTH;
		WINDOW_HEIGHT = SCREEN_HEIGHT;
	}

	m_cText->Resize({ (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT });

	// Resize And Reposition Window //
	SetWindowPos(hWindow, NULL, WINDOW_START_X, WINDOW_START_Y, WINDOW_WIDTH, WINDOW_HEIGHT, SWP_NOOWNERZORDER | SWP_NOZORDER);
	OUTPUT(to_string(WINDOW_WIDTH) + '\n');
	OUTPUT(to_string(WINDOW_HEIGHT) + '\n');

	// Release Resolution-Dependant Variables //
	SAFE_RELEASE(m_d3dBackBuffer);
	SAFE_RELEASE(m_d3dBackBufferRTV);
	
	SAFE_RELEASE(m_d3dSceneTexture);
	SAFE_RELEASE(m_d3dSceneTextureRTV);
	SAFE_RELEASE(m_d3dSceneTextureSRV);
	
	SAFE_RELEASE(m_d3dDepthBuffer);
	SAFE_RELEASE(m_d3dDepthBufferDSV);
	SAFE_RELEASE(m_d3dDepthBufferSRV);

	SAFE_RELEASE(m_d3dLuminanceTexture);
	SAFE_RELEASE(m_d3dLuminanceTextureRTV);
	SAFE_RELEASE(m_d3dLuminanceTextureSRV);

	SAFE_RELEASE(m_d3dHBlurTexture);
	SAFE_RELEASE(m_d3dHBlurTextureRTV);
	SAFE_RELEASE(m_d3dHBlurTextureSRV);

	SAFE_RELEASE(m_d3dVBlurTexture);
	SAFE_RELEASE(m_d3dVBlurTextureRTV);
	SAFE_RELEASE(m_d3dVBlurTextureSRV);
	
	SAFE_RELEASE(m_d3dMapTexture);
	SAFE_RELEASE(m_d3dMapRenderTargetView);
	SAFE_RELEASE(m_d3dMapShaderResourceView);

	//m_d3dContext->ClearState();

	// Clear RTV & DSV //
	m_d3dContext->OMSetRenderTargets(NULL, nullptr, nullptr);

	HRESULT hResult;

	// Resize Swap Chain //
	hResult = m_d3dSwapChain->ResizeBuffers(1, WINDOW_WIDTH, WINDOW_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM, NULL);
	assert("Swap chain resize failed!" && SUCCEEDED(hResult));


	// Recreate Viewport //
	D3D11_VIEWPORT d3dViewport;
	CreateViewport(WINDOW_WIDTH, WINDOW_HEIGHT, d3dViewport);

	m_d3dContext->RSSetViewports(1, &d3dViewport);


	// Recreate Back Buffer //
	hResult = m_d3dSwapChain->GetBuffer(NULL, __uuidof(m_d3dBackBuffer), (void**)(&m_d3dBackBuffer)); // Grab texture
	assert("Back buffer retrieval failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dBackBuffer, "Back Buffer");

	hResult = m_d3dDevice->CreateRenderTargetView(m_d3dBackBuffer, nullptr, &m_d3dBackBufferRTV); // Create RTV
	assert("Back buffer RTV resize failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dBackBufferRTV, "Back Buffer RTV");


	// Recreate Shadow Buffer //
	D3D11_TEXTURE2D_DESC d3dTextureDesc;
	CreateTextureDesc(WINDOW_WIDTH, WINDOW_HEIGHT, DXGI_FORMAT_R32_TYPELESS, m_nMultiSampleCount, m_nMultiSampleQuality,
					  D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, NULL, NULL, d3dTextureDesc);

	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDSVDesc;
	CreateDSVDesc(NULL, DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2D, d3dDSVDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc;
	CreateSRVDesc(DXGI_FORMAT_R32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2D, d3dSRVDesc);

	// Recreate Scene Texture //
	CreateTextureDesc(WINDOW_WIDTH, WINDOW_HEIGHT, DXGI_FORMAT_R32G32B32A32_FLOAT, m_nMultiSampleCount, m_nMultiSampleQuality,
					  D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, NULL, NULL, d3dTextureDesc);

	hResult = m_d3dDevice->CreateTexture2D(&d3dTextureDesc, NULL, &m_d3dSceneTexture); // Create texture
	assert("Scene texture resize failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dSceneTexture, "Scene Texture");

	hResult = m_d3dDevice->CreateRenderTargetView(m_d3dSceneTexture, nullptr, &m_d3dSceneTextureRTV); // Create RTV
	assert("Scene texture RTV resize failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dSceneTextureRTV, "Scene Texture RTV");

	hResult = m_d3dDevice->CreateShaderResourceView(m_d3dSceneTexture, nullptr, &m_d3dSceneTextureSRV); // Create SRV
	assert("Scene texture SRV resize failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dSceneTextureSRV, "Scene Texture SRV");


	// Recreate Depth Buffer //
	CreateTextureDesc(WINDOW_WIDTH, WINDOW_HEIGHT, DXGI_FORMAT_R32_TYPELESS, m_nMultiSampleCount, m_nMultiSampleQuality,
					  D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, NULL, NULL, d3dTextureDesc);

	CreateDSVDesc(NULL, DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2D, d3dDSVDesc);

	CreateSRVDesc(DXGI_FORMAT_R32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2D, d3dSRVDesc);

	hResult = m_d3dDevice->CreateTexture2D(&d3dTextureDesc, NULL, &m_d3dDepthBuffer); // Create texture
	assert("Depth buffer resize failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dDepthBuffer, "Depth Buffer");

	hResult = m_d3dDevice->CreateDepthStencilView(m_d3dDepthBuffer, &d3dDSVDesc, &m_d3dDepthBufferDSV); // Create DSV
	assert("Depth buffer DSV resize failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dDepthBufferDSV, "Depth Buffer DSV");

	hResult = m_d3dDevice->CreateShaderResourceView(m_d3dDepthBuffer, &d3dSRVDesc, &m_d3dDepthBufferSRV); // Create SRV
	assert("Depth buffer SRV resize failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dDepthBufferSRV, "Depth Buffer SRV");


	// Luminance Texture Creation //
	CreateTextureDesc(WINDOW_WIDTH, WINDOW_HEIGHT, DXGI_FORMAT_R32G32B32A32_FLOAT, m_nMultiSampleCount, m_nMultiSampleQuality,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, NULL, NULL, d3dTextureDesc);

	hResult = m_d3dDevice->CreateTexture2D(&d3dTextureDesc, NULL, &m_d3dLuminanceTexture); // Create texture
	assert("Luminance texture resize failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dLuminanceTexture, "Luminance Texture");

	hResult = m_d3dDevice->CreateRenderTargetView(m_d3dLuminanceTexture, nullptr, &m_d3dLuminanceTextureRTV); // Create RTV
	assert("Luminance texture RTV resize failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dLuminanceTextureRTV, "Luminance Texture RTV");

	hResult = m_d3dDevice->CreateShaderResourceView(m_d3dLuminanceTexture, nullptr, &m_d3dLuminanceTextureSRV); // Create SRV
	assert("Luminance texture SRV resize failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dLuminanceTextureSRV, "Luminance Texture SRV");


	// HBlur Texture Creation //
	CreateTextureDesc(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, DXGI_FORMAT_R32G32B32A32_FLOAT, m_nMultiSampleCount, m_nMultiSampleQuality,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, NULL, NULL, d3dTextureDesc);

	hResult = m_d3dDevice->CreateTexture2D(&d3dTextureDesc, NULL, &m_d3dHBlurTexture); // Create texture
	assert("HBlur texture resize failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dHBlurTexture, "HBlur Texture");

	hResult = m_d3dDevice->CreateRenderTargetView(m_d3dHBlurTexture, nullptr, &m_d3dHBlurTextureRTV); // Create RTV
	assert("HBlur texture RTV resize failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dHBlurTextureRTV, "HBlur Texture RTV");

	hResult = m_d3dDevice->CreateShaderResourceView(m_d3dHBlurTexture, nullptr, &m_d3dHBlurTextureSRV); // Create SRV
	assert("HBlur texture SRV resize failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dHBlurTextureSRV, "HBlur Texture SRV");


	// VBlur Texture Creation //
	CreateTextureDesc(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, DXGI_FORMAT_R32G32B32A32_FLOAT, m_nMultiSampleCount, m_nMultiSampleQuality,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, NULL, NULL, d3dTextureDesc);

	hResult = m_d3dDevice->CreateTexture2D(&d3dTextureDesc, NULL, &m_d3dVBlurTexture); // Create texture
	assert("VBlur texture resize failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dVBlurTexture, "VBlur Texture");

	hResult = m_d3dDevice->CreateRenderTargetView(m_d3dVBlurTexture, nullptr, &m_d3dVBlurTextureRTV); // Create RTV
	assert("VBlur texture RTV resize failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dVBlurTextureRTV, "VBlur Texture RTV");

	hResult = m_d3dDevice->CreateShaderResourceView(m_d3dVBlurTexture, nullptr, &m_d3dVBlurTextureSRV); // Create SRV
	assert("VBlur texture SRV resize failed!" && SUCCEEDED(hResult));
	DX_NAME(m_d3dVBlurTextureSRV, "VBlur Texture SRV");

	// Recreate Projection Matrices //
	CreateProjectionMatrix(fNearPlane, fFarPlane, FOV, RATIO, m_tScene.m_d3dProjectionMatrix);

	// Recreate Render To Texture //
	D3D11_TEXTURE2D_DESC d3dMapTextureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC d3dMapRenderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC d3dMapShaderResourceViewDesc;

	// Map Texture //
	ZeroMemory(&d3dMapTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	d3dMapTextureDesc.Width = WINDOW_WIDTH;
	d3dMapTextureDesc.Height = WINDOW_HEIGHT;
	d3dMapTextureDesc.MipLevels = 1;
	d3dMapTextureDesc.ArraySize = 1;
	d3dMapTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	d3dMapTextureDesc.SampleDesc.Count = m_nMultiSampleCount;
	d3dMapTextureDesc.SampleDesc.Quality = m_nMultiSampleQuality;
	d3dMapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dMapTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	d3dMapTextureDesc.CPUAccessFlags = 0;
	d3dMapTextureDesc.MiscFlags = 0;
	m_d3dDevice->CreateTexture2D(&d3dMapTextureDesc, NULL, &m_d3dMapTexture);

	d3dMapRenderTargetViewDesc.Format = d3dMapTextureDesc.Format;
	d3dMapRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	d3dMapRenderTargetViewDesc.Texture2D.MipSlice = 0;

	m_d3dDevice->CreateRenderTargetView(m_d3dMapTexture, &d3dMapRenderTargetViewDesc, &m_d3dMapRenderTargetView);

	ZeroMemory(&d3dMapShaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dMapShaderResourceViewDesc.Format = d3dMapTextureDesc.Format;
	d3dMapShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	d3dMapShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	d3dMapShaderResourceViewDesc.Texture2D.MipLevels = 1;

	m_d3dDevice->CreateShaderResourceView(m_d3dMapTexture, &d3dMapShaderResourceViewDesc, &m_d3dMapShaderResourceView);

	RenderToTextureMap();
}

void CRenderer::MapBuffers()
{
	D3D11_MAPPED_SUBRESOURCE d3dMapResource;

	// Send Scene To GPU //
	InverseMatrix(m_tScene.m_d3dViewMatrix);

	ZeroMemory(&d3dMapResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_d3dContext->Map(m_d3dSceneBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &d3dMapResource);
	memcpy(d3dMapResource.pData, &m_tScene, sizeof(TScene));
	m_d3dContext->Unmap(m_d3dSceneBuffer, 0);

	InverseMatrix(m_tScene.m_d3dViewMatrix);

	// Send Camera Position To GPU //
	ZeroMemory(&d3dMapResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_d3dContext->Map(m_d3dCameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &d3dMapResource);
	memcpy(d3dMapResource.pData, &m_tScene.m_d3dViewMatrix.m[3], sizeof(float) * 4);
	m_d3dContext->Unmap(m_d3dCameraBuffer, 0);
	
	// Populate Light Pack //
	TLightPack tLightPack;
	ZeroMemory(&tLightPack, sizeof(TLightPack));
	for (unsigned int i = 0; i < m_tPointLightArr.size() && i < MAX_LIGHTS; i++)
			memcpy(&tLightPack.m_tPointLights[i], &*m_tPointLightArr[i], sizeof(TPointLight));

	if (m_tDirectLight)
		tLightPack.m_tDirectLight = *m_tDirectLight;

	// Send Lights To GPU //
	ZeroMemory(&d3dMapResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_d3dContext->Map(m_d3dLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &d3dMapResource);
	memcpy(d3dMapResource.pData, &tLightPack, sizeof(TLightPack));
	m_d3dContext->Unmap(m_d3dLightBuffer, 0);

	// Send Gamma Level To GPU //
	ZeroMemory(&d3dMapResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_d3dContext->Map(m_d3dGammaBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &d3dMapResource);
	memcpy(d3dMapResource.pData, &m_fGamma, sizeof(float) * 4);
	m_d3dContext->Unmap(m_d3dGammaBuffer, 0);
}

void CRenderer::ClearScene()
{
	ClearScreen(m_d3dSceneTextureRTV);
	ClearDepth(m_d3dDepthBufferDSV);

	m_cSkybox.Clear();
	m_cEGObjects.clear();
	m_cMapRenderToTexture.Clear();

	for (unsigned int i = 0; i < m_cRenderList.size(); i++)
		m_cRenderList[i].Clear();

	for (unsigned int i = 0; i < m_cTransList.size(); i++)
		m_cTransList[i].Clear();

	for (unsigned int i = 0; i < m_cHUDList.size(); i++)
		m_cHUDList[i].Clear();

	m_cLines->Clear();
	m_tPointLightArr.clear();
	m_nLightIndex = 0;
	m_nNumLights = 0;
	m_nTotalObjects = 0;
	m_tDirectLight = nullptr;
	AddRenderMesh(m_cLines);
}

/*****************************************************************
* DrawLine():		Draws a line on the screen
*
* Ins:			    d3dStart
*					d3dEnd
*
* Outs:				N/A
*
* Returns:		    N/A
*
* Mod. Date:		----------
* Mod. Initials:	--
*****************************************************************/
void CRenderer::DrawLine(float3 d3dStart, float3 d3dEnd, float3 d3dColor)
{
	//m_cLines->GetLineVertexArr().push_back // Start
	//({
	//	{ d3dStart.x, d3dStart.y, d3dStart.z, 1 },
	//	{ d3dColor.x, d3dColor.y, d3dColor.z, 1 }
	//});
	//
	//m_cLines->GetLineVertexArr().push_back // End
	//({
	//	{ d3dEnd.x, d3dEnd.y, d3dEnd.z, 1 },
	//	{ d3dColor.x, d3dColor.y, d3dColor.z, 1 }
	//});
}

unsigned int CRenderer::LoadFont(const wchar_t* chFontPath)
{
	return m_cText->LoadFont(m_d3dDevice, chFontPath);
}

void CRenderer::RenderText(const wchar_t* chText, float2 fPosition, float4 fColor, float2 fScale, eTextMod eAlign, int nFontIndex)
{
	m_cTextBuffer.push_back({ std::wstring(chText), fPosition, fColor, fScale, eAlign, nFontIndex });
}

/*****************************************************************
* UpdateCamera():	Updates the player camera
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
void CRenderer::UpdateCamera()
{
	if (GetCursorPos(&m_lCurrentPos) && GetAsyncKeyState(0x02))
	{
		float fDeltaX = (float)(m_lCurrentPos.x - m_lPreviousPos.x);
		float fDeltaY = (float)(m_lCurrentPos.y - m_lPreviousPos.y);

		// Cursor Wrap //
		if (m_lCurrentPos.x < 1) // Left
			SetCursorPos(m_nScreenWidth - 2, m_lCurrentPos.y);
		if (m_lCurrentPos.y < 1) // Top
			SetCursorPos(m_lCurrentPos.x, m_nScreenHeight - 2);
		if (m_lCurrentPos.x > m_nScreenWidth - 2) // Right
			SetCursorPos(1, m_lCurrentPos.y);
		if (m_lCurrentPos.y > m_nScreenHeight - 2) // Bottom
			SetCursorPos(m_lCurrentPos.x, 1);

		// - Rotate Camera - //
		if (fDeltaX || fDeltaY)
		{
			fDeltaX *= SENSITIVITY;
			fDeltaY *= SENSITIVITY;

			// Store Position //
			float fMatrixPosX = m_tScene.m_d3dViewMatrix.m[3][0];
			float fMatrixPosY = m_tScene.m_d3dViewMatrix.m[3][1];
			float fMatrixPosZ = m_tScene.m_d3dViewMatrix.m[3][2];
			float fMatrixPosW = m_tScene.m_d3dViewMatrix.m[3][3];

			// Set Position To World Center // Global rotation becomes same as local
			m_tScene.m_d3dViewMatrix.m[3][0] = 0;
			m_tScene.m_d3dViewMatrix.m[3][1] = 0;
			m_tScene.m_d3dViewMatrix.m[3][2] = 0;
			m_tScene.m_d3dViewMatrix.m[3][3] = 0;

			// Rotate Globally On Y-Axis //
			RotateGlobalY(m_tScene.m_d3dViewMatrix, fDeltaX);

			if (m_fCameraAngle + fDeltaY <  90 &&
				m_fCameraAngle + fDeltaY > -90) // No x-axis 360's
			{
				// Update Angle //
				m_fCameraAngle += fDeltaY;

				// Rotate Locally On X-Axis //
				RotateLocalX(m_tScene.m_d3dViewMatrix, fDeltaY);
			}

			// Restore Position //
			m_tScene.m_d3dViewMatrix.m[3][0] = fMatrixPosX;
			m_tScene.m_d3dViewMatrix.m[3][1] = fMatrixPosY;
			m_tScene.m_d3dViewMatrix.m[3][2] = fMatrixPosZ;
			m_tScene.m_d3dViewMatrix.m[3][3] = fMatrixPosW;
		}
	}

	// Snapping Fix //
	if (m_lCurrentPos.x < 1 ||
		m_lCurrentPos.y < 1 ||
		m_lCurrentPos.x > m_nScreenWidth - 2 ||
		m_lCurrentPos.y > m_nScreenHeight - 2)
	{
		GetCursorPos(&m_lCurrentPos);
	}

	m_lPreviousPos = m_lCurrentPos;

	// Distace Per Second //
	float fBoostSpeed = 90 * DELTA_TIME();
	float fNormalSpeed = 30 * DELTA_TIME();

	// - Key Movement - //
	if (GetAsyncKeyState(0x57)) // W
	{
		if (GetAsyncKeyState(0x10))
			TranslateLocalZ(m_tScene.m_d3dViewMatrix, fBoostSpeed); // Boost
		else
			TranslateLocalZ(m_tScene.m_d3dViewMatrix, fNormalSpeed); // Normal
	}
	if (GetAsyncKeyState(0x53)) // S
	{
		if (GetAsyncKeyState(0x10))
			TranslateLocalZ(m_tScene.m_d3dViewMatrix, -fBoostSpeed); // Boost
		else
			TranslateLocalZ(m_tScene.m_d3dViewMatrix, -fNormalSpeed); // Normal
	}
	if (GetAsyncKeyState(0x41)) // A
	{
		if (GetAsyncKeyState(0x10))
			TranslateLocalX(m_tScene.m_d3dViewMatrix, -fBoostSpeed); // Boost
		else
			TranslateLocalX(m_tScene.m_d3dViewMatrix, -fNormalSpeed); // Normal
	}
	if (GetAsyncKeyState(0x44)) // D
	{
		if (GetAsyncKeyState(0x10))
			TranslateLocalX(m_tScene.m_d3dViewMatrix, fBoostSpeed); // Boost
		else
			TranslateLocalX(m_tScene.m_d3dViewMatrix, fNormalSpeed); // Normal
	}
}

void CRenderer::RenderToTextureMap()
{
	//MATTHEW TODO
	if (m_cMapRenderToTexture.IsEmpty() == false)
	{
		m_d3dContext->OMSetRenderTargets(1, &m_d3dMapRenderTargetView, m_d3dDepthBufferDSV);

		m_d3dContext->ClearRenderTargetView(m_d3dMapRenderTargetView, m_fClearColor);

		m_cMapRenderToTexture.Render();

		ClearDepth(m_d3dDepthBufferDSV);
		//Reset Render Target
		m_d3dContext->OMSetRenderTargets(1, &m_d3dSceneTextureRTV, m_d3dDepthBufferDSV);
	}
}
/*****************************************************************
* AddRenderMesh():	Sorts incoming mesh into correct RenderNode
*
* Ins:			    cNewMesh
*
* Outs:				N/A
*
* Returns:		    N/A
*
* Mod. Date:		----------
* Mod. Initials:	--
*****************************************************************/
void CRenderer::AddRenderMesh(CRenderMesh* cNewMesh)
{
	m_nTotalObjects++;

	// Skybox //
	if (cNewMesh->GetPixelShader() == m_d3dSkyboxPS)
	{
		m_cSkybox.Initialize(cNewMesh);
		m_cSkybox.AddMesh(cNewMesh);
		return;
	}

	// HUD Object //
	else if (cNewMesh->GetVertexShader() == m_d3dStandard2DVS && 
			 cNewMesh->GetPixelShader() == m_d3dStandard2DPS)
	{
		// Search For Matching Context //
		for (unsigned int i = 0; i < m_cHUDList.size(); i++)
		{
			if (m_cHUDList[i].IsMatch(cNewMesh))
			{
				m_cHUDList[i].AddMesh(cNewMesh);
				return;
			}
		}

		// No Context Found - Create New Set //
		CRenderNode cNewSet;
		cNewSet.Initialize(cNewMesh);
		cNewSet.AddMesh(cNewMesh);
		m_cHUDList.push_back(cNewSet);
		return;
	}

	// Transparent Object //
	else if (cNewMesh->GetTransparent())
	{
		// Search For Matching Context //
		for (unsigned int i = 0; i < m_cTransList.size(); i++)
		{
			if (m_cTransList[i].IsMatch(cNewMesh))
			{
				m_cTransList[i].AddMesh(cNewMesh);
				return;
			}
		}

		// No Context Found - Create New Set //
		CRenderNode cNewSet;
		cNewSet.Initialize(cNewMesh);
		cNewSet.AddMesh(cNewMesh);
		m_cTransList.push_back(cNewSet);
		return;
	}

	// Normal Object //
	else
	{
		if (cNewMesh->GetType() != typeid(CLineMesh))
			m_cAllObjects.push_back(cNewMesh);

		// Search For Matching Context //
		for (unsigned int i = 0; i < m_cRenderList.size(); i++)
		{
			if (m_cRenderList[i].IsMatch(cNewMesh))
			{
				m_cRenderList[i].AddMesh(cNewMesh);
				return;
			}
		}

		// No Context Found - Create New Set //
		CRenderNode cNewSet;
		cNewSet.Initialize(cNewMesh);
		cNewSet.AddMesh(cNewMesh);
		m_cRenderList.push_back(cNewSet);
	}
}

void CRenderer::RemoveRenderMesh(CRenderMesh* cRemoveMesh)
{
	m_nTotalObjects--;
	VECTOR_REMOVE(m_cEGObjects, cRemoveMesh);
	VECTOR_REMOVE(m_cAllObjects, cRemoveMesh);

	if (m_cSkybox.Remove(cRemoveMesh))
		return;

	for (unsigned int i = 0; i < m_cRenderList.size(); i++)
	{
		if (m_cRenderList[i].Remove(cRemoveMesh))
			return;
	}

	for (unsigned int i = 0; i < m_cTransList.size(); i++)
	{
		if (m_cTransList[i].Remove(cRemoveMesh))
			return;
	}

	for (unsigned int i = 0; i < m_cHUDList.size(); i++)
	{
		if (m_cHUDList[i].Remove(cRemoveMesh))
			return;
	}
}

void CRenderer::AddRenderToTextureMap(CRenderMesh* cNewMesh)
{
	if (m_cMapRenderToTexture.IsEmpty())
	{
		m_cMapRenderToTexture.Initialize(cNewMesh);
	}
	m_cMapRenderToTexture.AddMesh(cNewMesh);
}