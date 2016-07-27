#pragma once

/***********************************************
* Filename:  		Renderer.h
* Date:      		08/12/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Nick Dalton
* Purpose:   		Initializes DirectX, and
*					manages all rendering
************************************************/

#include "RenderNode.h"
#include <unordered_map>
#include "Lights.h"

// Forward Declarations //
class Shader;
class CLineMesh;
class CRenderMesh;
class CTextRenderer;
struct TPointLight;
struct TDirectLight;

struct TScene // Remove after camera is implemented
{
	XMFLOAT4X4 m_d3dViewMatrix;
	XMFLOAT4X4 m_d3dProjectionMatrix;
};

enum eTextMod { Left, Center, Right };

struct TText
{
	std::wstring m_chText;
	float2 m_fPosition;
	float4 m_fColor;
	float2 m_fScale;
	eTextMod m_nAlign;
	unsigned int m_nFontIndex;
};

class CRenderer
{
	// Debug Variables //
	int m_nLightIndex = 0;
	int m_nNumLights = 0;
	int m_nNumLines = 0;
	float m_fFrameTime = 0;
	int m_nTotalObjects = 0;
	int m_nTotalRendered = 0;
	int m_nFrame = 0;
	std::vector<float> m_fDrawTime;

	// Camera Variables //
	bool m_bDebugCamera = false;
	float m_fCameraAngle = 0;

	POINT m_lCurrentPos;
	POINT m_lPreviousPos;

	int m_nScreenWidth = SCREEN_WIDTH;
	int m_nScreenHeight = SCREEN_HEIGHT;

	float fFarPlane = 20000.0f;
	float fNearPlane = 0.1f;

	XMFLOAT4 m_fGamma = { 0, 0, 0, 0 };
	CRenderMesh* cBillboard = nullptr;

	// Scene //
	TScene m_tScene;

	// Renderer Variables //
	std::vector<TText> m_cTextBuffer;
	CTextRenderer* m_cText = nullptr;

	bool m_bDepth = true;
	bool m_bFullScreen = false;
	CLineMesh* m_cLines = nullptr; // Debug Lines
	CRenderNode m_cMapRenderToTexture;
	CRenderNode m_cSkybox; // Render First // Skybox
	std::vector<CRenderMesh*> m_cEGObjects; // Render Second // Edge Glow Objects
	std::vector<CRenderNode> m_cRenderList; // Render Third // Normal Objects
	std::vector<CRenderNode> m_cTransList; // Render Fourth // Transparent Objects
	std::vector<CRenderNode> m_cHUDList; // Render Last // HUD Objects

	std::vector<CRenderMesh*> m_cAllObjects;

	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> m_d3dTextureList;

	TDirectLight* m_tDirectLight = nullptr;
	std::vector<TPointLight*> m_tPointLightArr;

	// DirectX Variables //
	bool m_bDebugMode;
	float m_fClearColor[4];

	UINT m_nMultiSampleCount = 8;
	UINT m_nMultiSampleQuality = 16;

	ID3D11SamplerState* m_d3dSamplerState = nullptr;

	ID3D11InputLayout* m_d3dInputLayout3D = nullptr;
	ID3D11InputLayout* m_d3dInputLayout2D = nullptr;
	ID3D11InputLayout* m_d3dInputLayoutLine = nullptr;
	ID3D11InputLayout* m_d3dInputLayoutParticle = nullptr;
	ID3D11InputLayout* m_d3dInputLayoutAnim = nullptr;

	ID3D11BlendState* m_d3dBlendState;
	ID3D11RasterizerState* m_d3dSolidRasterizer;
	ID3D11RasterizerState* m_d3dEdgeRasterizer;
	ID3D11RasterizerState* m_d3dOutlineRasterizer;
	ID3D11RasterizerState* m_d3dWireframeRasterizer;
	ID3D11DepthStencilState* m_d3dDepthStencilState;
	ID3D11DepthStencilState* m_d3dDepthStencilStateObject;
	ID3D11DepthStencilState* m_d3dDepthStencilStateOutline;

	D3D11_VIEWPORT m_d3dViewport;
	IDXGISwapChain* m_d3dSwapChain;

	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dContext;

	// Back Buffer //
	ID3D11Texture2D* m_d3dBackBuffer = nullptr;
	ID3D11RenderTargetView* m_d3dBackBufferRTV = nullptr;

	// Scene Texture // - Main scene gets rendered to this
	ID3D11Texture2D* m_d3dSceneTexture = nullptr; // Main scene
	ID3D11RenderTargetView* m_d3dSceneTextureRTV = nullptr; // To bind as render target
	ID3D11ShaderResourceView* m_d3dSceneTextureSRV = nullptr; // To bind as shader resource

	// Depth Buffer //
	ID3D11Texture2D* m_d3dDepthBuffer = nullptr; // Main depth buffer
	ID3D11DepthStencilView* m_d3dDepthBufferDSV = nullptr; // To bind as depth buffer
	ID3D11ShaderResourceView* m_d3dDepthBufferSRV = nullptr; // To bind as shader resource

	// Luminance Texture // 
	ID3D11Texture2D* m_d3dLuminanceTexture = nullptr;
	ID3D11RenderTargetView* m_d3dLuminanceTextureRTV = nullptr;
	ID3D11ShaderResourceView* m_d3dLuminanceTextureSRV = nullptr;

	// HBlur Texture //
	ID3D11Texture2D* m_d3dHBlurTexture = nullptr; // Horizontal blur
	ID3D11RenderTargetView* m_d3dHBlurTextureRTV = nullptr;
	ID3D11ShaderResourceView* m_d3dHBlurTextureSRV = nullptr;

	// VBlur Texture //
	ID3D11Texture2D* m_d3dVBlurTexture = nullptr; // Vertical blur
	ID3D11RenderTargetView* m_d3dVBlurTextureRTV = nullptr;
	ID3D11ShaderResourceView* m_d3dVBlurTextureSRV = nullptr;

	ID3D11Texture2D* m_d3dMapTexture = nullptr;
	ID3D11RenderTargetView* m_d3dMapRenderTargetView = nullptr;
	ID3D11ShaderResourceView* m_d3dMapShaderResourceView = nullptr;

	// Constant Buffers //
	ID3D11Buffer* m_d3dTransformBuffer = nullptr;
	ID3D11Buffer* m_d3dSceneBuffer = nullptr;
	ID3D11Buffer* m_d3dBoneBuffer = nullptr;
	ID3D11Buffer* m_d3dLightBuffer = nullptr;
	ID3D11Buffer* m_d3dCameraBuffer = nullptr;
	ID3D11Buffer* m_d3dSpecularBuffer = nullptr;
	ID3D11Buffer* m_d3dParticleBuffer = nullptr;
	ID3D11Buffer* m_d3dGammaBuffer = nullptr;

	// Shaders //
	ID3D11VertexShader* m_d3dLineVS = nullptr;
	ID3D11VertexShader* m_d3dHBlurVS = nullptr;
	ID3D11VertexShader* m_d3dVBlurVS = nullptr;
	ID3D11VertexShader* m_d3dSkyboxVS = nullptr;
	ID3D11VertexShader* m_d3dParticleVS = nullptr;
	ID3D11VertexShader* m_d3dSkinningVS = nullptr;
	ID3D11VertexShader* m_d3dStandardVS = nullptr;
	ID3D11VertexShader* m_d3dStandard2DVS = nullptr;
	ID3D11VertexShader* m_d3dLuminanceVS = nullptr;
	ID3D11VertexShader* m_d3dPostProcessorVS = nullptr;

	ID3D11PixelShader* m_d3dHBlurPS = nullptr;
	ID3D11PixelShader* m_d3dVBlurPS = nullptr;
	ID3D11PixelShader* m_d3dLinePS = nullptr;
	ID3D11PixelShader* m_d3dSkyboxPS = nullptr;
	ID3D11PixelShader* m_d3dParticlePS = nullptr;
	ID3D11PixelShader* m_d3dStandardPS = nullptr;
	ID3D11PixelShader* m_d3dNormalMapPS = nullptr;
	ID3D11PixelShader* m_d3dStandard2DPS = nullptr;
	ID3D11PixelShader* m_d3dDepthPrepass = nullptr;
	ID3D11PixelShader* m_d3dLuminancePS = nullptr;
	ID3D11PixelShader* m_d3dPostProcessorPS = nullptr;

	ID3D11GeometryShader* m_d3dParticleGS = nullptr;
	ID3D11GeometryShader* m_d3dSunMoonGS = nullptr;

	ID3D11ComputeShader* m_d3dStandardCS = nullptr;
	ID3D11ComputeShader* m_d3dPostProcessCS = nullptr;

	// Variable Shaders //
	Shader* m_d3dPixelShaderVar = nullptr;

	// Helper Functions //
	void DrawAll();
	void MapBuffers();
	void UpdateCamera();
	void PostProcess();

	void ClearScreen(ID3D11RenderTargetView* d3dRTV) { m_d3dContext->ClearRenderTargetView(d3dRTV, m_fClearColor); }
	void ClearDepth(ID3D11DepthStencilView* d3dDSV) { m_d3dContext->ClearDepthStencilView(d3dDSV, D3D11_CLEAR_DEPTH, 1.0f, 0); }

	void UnbindVertexShaderSRV(unsigned int nSlot) { ID3D11ShaderResourceView* d3dBlankSRV = nullptr; m_d3dContext->VSSetShaderResources(nSlot, 1, &d3dBlankSRV); }
	void UnbindPixelShaderSRV(unsigned int nSlot) { ID3D11ShaderResourceView* d3dBlankSRV = nullptr; m_d3dContext->PSSetShaderResources(nSlot, 1, &d3dBlankSRV); }
	void UnbindHullShaderSRV(unsigned int nSlot) { ID3D11ShaderResourceView* d3dBlankSRV = nullptr; m_d3dContext->HSSetShaderResources(nSlot, 1, &d3dBlankSRV); }
	void UnbindDomainShaderSRV(unsigned int nSlot) { ID3D11ShaderResourceView* d3dBlankSRV = nullptr; m_d3dContext->DSSetShaderResources(nSlot, 1, &d3dBlankSRV); }
	void UnbindGeometryShaderSRV(unsigned int nSlot) { ID3D11ShaderResourceView* d3dBlankSRV = nullptr; m_d3dContext->GSSetShaderResources(nSlot, 1, &d3dBlankSRV); }
	void UnbindComputeShaderSRV(unsigned int nSlot) { ID3D11ShaderResourceView* d3dBlankSRV = nullptr; m_d3dContext->CSSetShaderResources(nSlot, 1, &d3dBlankSRV); }

public:
	unsigned int indices = 0;
	unsigned int vertices = 0;
	CRenderer(HWND hWindow);
	~CRenderer();

	void ToggleFullScreen(unsigned int nWidth = 0, unsigned int nHeight = 0);

	void Update();
	void ClearScene();
	void ToggleDepth() { if (m_bDepth) { m_d3dContext->OMSetDepthStencilState(m_d3dDepthStencilState, NULL); m_bDepth = false; } else { m_d3dContext->OMSetDepthStencilState(NULL, NULL); m_bDepth = true; } }

	void AddLight(TPointLight* tNewLight) { m_tPointLightArr.push_back(tNewLight); }
	void RemoveLight(TPointLight* tRemoveLight) { VECTOR_REMOVE(m_tPointLightArr, tRemoveLight); }

	void AddRenderMesh(CRenderMesh* cNewMesh);
	void RemoveRenderMesh(CRenderMesh* cRemoveMesh);
	void AddRenderToTextureMap(CRenderMesh* cNewMesh);

	void DrawLine(float3 d3dStart, float3 d3dEnd, float3 d3dColor);
	unsigned int LoadFont(const wchar_t* chFontPath);
	void RenderText(const wchar_t* chText, float2 fPosition, float4 fColor = { 1, 1, 1, 1 }, float2 fScale = { 1, 1 }, eTextMod eAlign = Left, int nFontIndex = 0);
	void SetDirectionalLight(TDirectLight* tNewLight) { m_tDirectLight = tNewLight; }
	void LookAt(float3 fDirection, XMFLOAT4X4& fMatrix);
	
	void RenderToTextureMap();

	void SetGamma(float fGamma);

	// Accessors //
	int GetNumLights() { return m_nNumLights; }
	bool& GetFullScreen() { return m_bFullScreen; }
	bool& GetDebugCamera() { return m_bDebugCamera; }
	float GetFarPlane() { return fFarPlane; }
	float GetNearPlane() { return fNearPlane; }

	std::vector<CRenderMesh*>& GetEGObjects() { return m_cEGObjects; }

	std::unordered_map<std::wstring, ID3D11ShaderResourceView*>& GetTextureList() { return m_d3dTextureList; }

	XMFLOAT4X4& GetViewMatrix() { return m_tScene.m_d3dViewMatrix; }

	ID3D11InputLayout* GetInputLayout2D() { return m_d3dInputLayout2D; }
	ID3D11InputLayout* GetInputLayout3D() { return m_d3dInputLayout3D; }
	ID3D11InputLayout* GetInputLayoutLine() { return m_d3dInputLayoutLine; }
	ID3D11InputLayout* GetInputLayoutParticle() { return m_d3dInputLayoutParticle; }
	ID3D11InputLayout* GetInputLayoutAnim() { return m_d3dInputLayoutAnim; }

	ID3D11BlendState* GetBlendState() { return m_d3dBlendState; }
	ID3D11RasterizerState* GetSolidRasterizer() { return m_d3dSolidRasterizer; }
	ID3D11RasterizerState* GetOutlineRasterizer() { return m_d3dOutlineRasterizer; }
	ID3D11RasterizerState* GetWireframeRasterizer() { return m_d3dWireframeRasterizer; }
	ID3D11DepthStencilState* GetDepthStencilState() { return m_d3dDepthStencilState; }

	ID3D11Texture2D* GetBackBuffer() { return m_d3dBackBuffer; }
	ID3D11RenderTargetView* GetBackBufferRTV() { return m_d3dBackBufferRTV; }

	ID3D11Texture2D* GetShadowBuffer() { return m_d3dDepthBuffer; }
	ID3D11DepthStencilView* GetShadowBufferDSV() { return m_d3dDepthBufferDSV; }
	ID3D11ShaderResourceView* GetShadowBufferSRV() { return m_d3dDepthBufferSRV; }

	ID3D11Texture2D* GetSceneTexture() { return m_d3dSceneTexture; }
	ID3D11RenderTargetView* GetSceneTextureRTV() { return m_d3dSceneTextureRTV; }
	ID3D11ShaderResourceView* GetSceneTextureSRV() { return m_d3dSceneTextureSRV; }

	ID3D11Texture2D* GetDepthBuffer() { return m_d3dDepthBuffer; }
	ID3D11DepthStencilView* GetDepthBufferDSV() { return m_d3dDepthBufferDSV; }
	ID3D11ShaderResourceView* GetDepthBufferSRV() { return m_d3dDepthBufferSRV; }

	ID3D11Device* GetDevice() { return m_d3dDevice; }
	ID3D11DeviceContext* GetDeviceContext() { return m_d3dContext; }
	IDXGISwapChain* GetSwapChain() { return m_d3dSwapChain; }
	ID3D11ShaderResourceView* GetMapShaderResourceView(){ return m_d3dMapShaderResourceView; }

	ID3D11Buffer* GetTransformBuffer() { return m_d3dTransformBuffer; }
	ID3D11Buffer* GetSceneBuffer() { return m_d3dSceneBuffer; }
	ID3D11Buffer* GetBoneBuffer() { return m_d3dBoneBuffer; }
	ID3D11Buffer* GetCameraBuffer() { return m_d3dCameraBuffer; }
	ID3D11Buffer* GetSpecularBuffer() { return m_d3dSpecularBuffer; }
	ID3D11Buffer* GetParticleBuffer() { return m_d3dParticleBuffer; }

	ID3D11VertexShader* GetLineVS() { return m_d3dLineVS; }
	ID3D11VertexShader* GetSkyboxVS() { return m_d3dSkyboxVS; }
	ID3D11VertexShader* GetParticleVS() { return m_d3dParticleVS; }
	ID3D11VertexShader* GetSkinningVS() { return m_d3dSkinningVS; }
	ID3D11VertexShader* GetStandardVS() { return m_d3dStandardVS; }
	ID3D11VertexShader* GetVertexShader() { return m_d3dStandardVS; } // Legacy
	ID3D11VertexShader* GetStandard2DVS() { return m_d3dStandard2DVS; }
	ID3D11VertexShader* GetVertexShader2D() { return m_d3dStandard2DVS; } // Legacy

	ID3D11PixelShader* GetLinePS() { return m_d3dLinePS; }
	ID3D11PixelShader* GetSkyboxPS() { return m_d3dSkyboxPS; }
	ID3D11PixelShader* GetParticlePS() { return m_d3dParticlePS; }
	ID3D11PixelShader* GetStandardPS() { return m_d3dStandardPS; }
	ID3D11PixelShader* GetPixelShader() { return m_d3dStandardPS; } // Legacy
	ID3D11PixelShader* GetNormalMapPS() { return m_d3dNormalMapPS; }
	ID3D11PixelShader* GetStandard2DPS() { return m_d3dStandard2DPS; }
	ID3D11PixelShader* GetPixelShader2D() { return m_d3dStandard2DPS; } // Legacy

	ID3D11GeometryShader* GetParticleGS() { return m_d3dParticleGS; }
	ID3D11GeometryShader* GetSunMoonGS(){ return m_d3dSunMoonGS; }
	
	ID3D11ComputeShader* GetComputeShader() { return m_d3dStandardCS; }

	Shader* GetPixelShaderVar() { return m_d3dPixelShaderVar; }
};