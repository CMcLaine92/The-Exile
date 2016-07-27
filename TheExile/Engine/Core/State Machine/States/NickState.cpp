#include "NickState.h"
#include "../StateMachine.h"

#include "../../../Renderer/Lights.h"
#include "../../../Renderer/Renderer.h"
#include "../../../Renderer/RenderMesh.h"
#include "../../../Renderer/ShaderCompilation.h"
#include "../../../Animation/Mesh.h"

void CNickState::Enter()
{
	m_cpPlaneMesh = new CMesh;
	m_cpCube = new CMesh;
	m_cpSkyBox = new CMesh;

	RenderMeshEx(); // Create first cube

	TransparencyEx(); // Create second cube

	//DirectLightEx(); // Create directional light

	PointLightEx(); // Create point light

	SkyboxEx(); // Create skybox

	CreatePlatforms();

	LoadFontEx();

	fStartTime = CURRENT_TIME();
	fStartTime2 = CURRENT_TIME();
}

void CNickState::Exit()
{
	SAFE_DELETE(cCube);
	SAFE_DELETE(cCube2);
	SAFE_DELETE(cCube3);
	SAFE_DELETE(cCube4);
	SAFE_DELETE(cSkybox);

	SAFE_DELETE(cTop);
	SAFE_DELETE(cLeft);
	SAFE_DELETE(cRight);
	SAFE_DELETE(cFront);
	SAFE_DELETE(cBack);
	SAFE_DELETE(cBottom);

	SAFE_DELETE(tPointLight);
	SAFE_DELETE(tPointLight2);
	SAFE_DELETE(tPointLight3);
	SAFE_DELETE(tDirectLight);
	SAFE_DELETE(m_cpCube);
	SAFE_DELETE(m_cpSkyBox);
	SAFE_DELETE(m_cpPlaneMesh);

	
}
CNickState::CNickState()
{
	fStartTime = 0.0f;
	fStartTime2 = 0.0f;
	fEndTime = 0.0f;
	size = 0;
	nFontIndex = 0;

	bReversed1 = 0;
	bReversed2 = 0;
	bReversed3 = 0;
	bEdgeGlow = 0;
	
	cCube = nullptr;
	cCube2 = nullptr;
	cCube3 = nullptr;
	cCube4 = nullptr;

	cSkybox = nullptr;

	cTop = nullptr;
	cLeft = nullptr;
	cRight = nullptr;
	cFront = nullptr;
	cBack = nullptr;
	cBottom = nullptr;

	tPointLight = nullptr;
	tPointLight2 = nullptr;
	tPointLight3 = nullptr;
	tDirectLight = nullptr;
	m_cpCube = nullptr;
	m_cpSkyBox = nullptr;
	m_cpPlaneMesh = nullptr;
}

CNickState::~CNickState() // Clean Up
{
	
}

void CNickState::Update()
{
	//DrawLineEx();

	//RenderTextEx();

	//MoveLights();

	if (GetAsyncKeyState(VK_F11) || (GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(VK_RETURN)))
	{
		Sleep(250);
		GRAPHICS->ToggleFullScreen();
	}

	RotateLocalY(cCube->GetTransformMatrix(), 45 * DELTA_TIME());
	RotateLocalZ(cCube3->GetTransformMatrix(), 22 * DELTA_TIME());
	TranslateGlobalY(cTop->GetTransformMatrix(), -10 * DELTA_TIME());
	//TranslateGlobalY(cCube4->GetTransformMatrix(), 0.5f * DELTA_TIME());
	//
	//for (unsigned int i = 0; i < 3; i++)
	//	TranslateLocalY(GRAPHICS->GetLightViewMatrix(i), 0.5f * DELTA_TIME());

	//GRAPHICS->DirectionalLightLookAt({ 0, -1, 1 });
	//SnapPosition(GRAPHICS->GetLightViewMatrix(), GRAPHICS->GetViewMatrix());

	//cCube4->GetTransformMatrix() = GRAPHICS->GetLightViewMatrix();
	//RotateGlobalX(m_tShadowScene.m_d3dViewMatrix, DELTA_TIME() * 10.0f);

	//if (CURRENT_TIME() - fStartTime > 1.0f)
	//{
	//	if (bEdgeGlow)
	//	{
	//		cCube->SetEdgeGlow(false);
	//		bEdgeGlow = false;
	//	}
	//	else
	//	{
	//		cCube->SetEdgeGlow(true);
	//		bEdgeGlow = true;
	//	}
	//
	//	fStartTime = CURRENT_TIME();
	//}

	//if (CURRENT_TIME() - fStartTime2 > 0.0025f)
	//{
	//	float2 position;
	//	position.x = ((rand() % 600) + 1) - 300;
	//	position.y = ((rand() % 600) + 1) - 300;
	//
	//	float3 color;
	//	color.x = ((rand() % 1000) + 1) / 1000.0f;
	//	color.y = ((rand() % 1000) + 1) / 1000.0f;
	//	color.z = ((rand() % 1000) + 1) / 1000.0f;
	//
	//	TPointLight* newLight = new TPointLight
	//	{
	//		{ position.x, 5, position.y, 1 },
	//		{ color.x, color.y, color.z },
	//		{ 200 },
	//		{ 2.25f }
	//	};
	//
	//	lightArr.push_back(newLight);
	//	GRAPHICS->AddLight(newLight);
	//
	//	// Reset Time //
	//	fStartTime2 = CURRENT_TIME();
	//}
}

void CNickState::MoveLights()
{
	// First Light //
	if (!bReversed1)
	{
		tPointLight->m_fPosition[0] += 100 * DELTA_TIME();
		tPointLight->m_fPosition[2] += 100 * DELTA_TIME();

		if (tPointLight->m_fPosition[0] > 100 || tPointLight->m_fPosition[2] > 100)
			bReversed1 = true;
	}
	else
	{
		tPointLight->m_fPosition[0] += -100 * DELTA_TIME();
		tPointLight->m_fPosition[2] += -100 * DELTA_TIME();

		if (tPointLight->m_fPosition[0] < -100 || tPointLight->m_fPosition[2] < -100)
			bReversed1 = false;
	}

	// Second Light //
	if (!bReversed2)
	{
		tPointLight2->m_fPosition[0] += -100 * DELTA_TIME();
		tPointLight2->m_fPosition[2] += 100 * DELTA_TIME();

		if (tPointLight2->m_fPosition[0] < -100 || tPointLight2->m_fPosition[2] > 100)
			bReversed2 = true;
	}
	else
	{
		tPointLight2->m_fPosition[0] += 100 * DELTA_TIME();
		tPointLight2->m_fPosition[2] += -100 * DELTA_TIME();

		if (tPointLight2->m_fPosition[0] > 100 || tPointLight2->m_fPosition[2] < -100)
			bReversed2 = false;
	}

	tPointLight3->m_fPosition[0] = sinf(CURRENT_TIME() / 5) * 200;
	tPointLight3->m_fPosition[2] = cosf(CURRENT_TIME() / 5) * 200;
}

void CNickState::CreatePlatforms()
{
	// - Platform - //

	// Index Initialization //
	std::vector<unsigned int> nPlatformIndex;
	
	m_cpPlaneMesh->GetIndices().push_back(0);
	m_cpPlaneMesh->GetIndices().push_back(0 + 1);
	m_cpPlaneMesh->GetIndices().push_back(0 + 2);
	
	m_cpPlaneMesh->GetIndices().push_back(0);
	m_cpPlaneMesh->GetIndices().push_back(0 + 2);
	m_cpPlaneMesh->GetIndices().push_back(0 + 3);

	// Vertex Initialization //
	std::vector<TVertex3D> tPlatformVertex;

	m_cpPlaneMesh->GetVertices().push_back
	({
		{ -300, 0, 300, 1 },
		{ 0, 1, 0 },
		{ -4, 4 },
		{ 1, 0, 0, 1 }
	});

	m_cpPlaneMesh->GetVertices().push_back
	({
		{ 300, 0, 300, 1 },
		{ 0, 1, 0 },
		{ 4, 4 },
		{ 1, 0, 0, 1 }
	});

	m_cpPlaneMesh->GetVertices().push_back
	({
		{ 300, 0, -300, 1 },
		{ 0, 1, 0 },
		{ 4, -4 },
		{ 1, 0, 0, 1 }
	});

	m_cpPlaneMesh->GetVertices().push_back
	({
		{ -300, 0, -300, 1 },
		{ 0, 1, 0 },
		{ -4, -4 },
		{ 1, 0, 0, 1 }
	});

	cTop = new CRenderMesh(m_cpPlaneMesh, GRAPHICS->GetStandardVS(), GRAPHICS->GetNormalMapPS());
	cLeft = new CRenderMesh(m_cpPlaneMesh, GRAPHICS->GetStandardVS(), GRAPHICS->GetNormalMapPS());
	cRight = new CRenderMesh(m_cpPlaneMesh, GRAPHICS->GetStandardVS(), GRAPHICS->GetNormalMapPS());
	cFront = new CRenderMesh(m_cpPlaneMesh, GRAPHICS->GetStandardVS(), GRAPHICS->GetNormalMapPS());
	cBack = new CRenderMesh(m_cpPlaneMesh, GRAPHICS->GetStandardVS(), GRAPHICS->GetNormalMapPS());
	cBottom = new CRenderMesh(m_cpPlaneMesh, GRAPHICS->GetStandardVS(), GRAPHICS->GetNormalMapPS());

	cTop->GetVarPixelShader() = GRAPHICS->GetPixelShaderVar();
	cLeft->GetVarPixelShader() = GRAPHICS->GetPixelShaderVar();
	cRight->GetVarPixelShader() = GRAPHICS->GetPixelShaderVar();
	cFront->GetVarPixelShader() = GRAPHICS->GetPixelShaderVar();
	cBack->GetVarPixelShader() = GRAPHICS->GetPixelShaderVar();
	cBottom->GetVarPixelShader() = GRAPHICS->GetPixelShaderVar();

	cTop->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Hex.dds");
	cLeft->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Hex.dds");
	cRight->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Hex.dds");
	cFront->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Hex.dds");
	cBack->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Hex.dds");
	cBottom->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Hex.dds");

	cTop->SetTexture(L"../Game/Assets/Art/2D/Textures/Hex.dds");
	cLeft->SetTexture(L"../Game/Assets/Art/2D/Textures/Hex.dds");
	cRight->SetTexture(L"../Game/Assets/Art/2D/Textures/Hex.dds");
	cFront->SetTexture(L"../Game/Assets/Art/2D/Textures/Hex.dds");
	cBack->SetTexture(L"../Game/Assets/Art/2D/Textures/Hex.dds");
	cBottom->SetTexture(L"../Game/Assets/Art/2D/Textures/Hex.dds");

	cTop->GetSpecular() = 2.0f;
	cLeft->GetSpecular() = 2.0f;
	cRight->GetSpecular() = 2.0f;
	cFront->GetSpecular() = 2.0f;
	cBack->GetSpecular() = 2.0f;
	cBottom->GetSpecular() = 2.0f;

	TranslateLocalY(cBottom->GetTransformMatrix(), -2.5);

	TranslateLocalY(cTop->GetTransformMatrix(), 300);
	RotateLocalZ(cTop->GetTransformMatrix(), 180);

	TranslateLocalZ(cBack->GetTransformMatrix(), 300);
	RotateLocalX(cBack->GetTransformMatrix(), -90);

	TranslateLocalZ(cFront->GetTransformMatrix(), -300);
	RotateLocalX(cFront->GetTransformMatrix(), 90);

	TranslateLocalX(cLeft->GetTransformMatrix(), -300);
	RotateLocalZ(cLeft->GetTransformMatrix(), -90);

	TranslateLocalX(cRight->GetTransformMatrix(), 300);
	RotateLocalZ(cRight->GetTransformMatrix(), 90);

	GRAPHICS->AddRenderMesh(cTop);
	GRAPHICS->AddRenderMesh(cLeft);
	GRAPHICS->AddRenderMesh(cRight);
	GRAPHICS->AddRenderMesh(cFront);
	GRAPHICS->AddRenderMesh(cBack);
	GRAPHICS->AddRenderMesh(cBottom);
}

void CNickState::RenderMeshEx() // Basic RenderMesh creation
{
	// - Cube - //	

	// Index Initialization //
	std::vector<unsigned int> nCubeIndex;
	
	unsigned int faces = 6;
	for (unsigned int i = 0; i < faces; i++)
	{
		unsigned int a = i * 4;
		m_cpCube->GetIndices().push_back(a);
		m_cpCube->GetIndices().push_back(a + 1);
		m_cpCube->GetIndices().push_back(a + 2);
		
		m_cpCube->GetIndices().push_back(a);
		m_cpCube->GetIndices().push_back(a + 2);
		m_cpCube->GetIndices().push_back(a + 3);
	}
	
	// Vertex Initialization //
	std::vector<TVertex3D> tCubeVertex;

	// Front Face // 
	m_cpCube->GetVertices().push_back
	({
		{ -1, 1, -1, 1 },
		{ 0, 0, -1, 0 },
		{ 0, 0 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ 1, 1, -1, 1 },
		{ 0, 0, -1 },
		{ 1, 0 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ 1, -1, -1, 1 },
		{ 0, 0, -1 },
		{ 1, 1 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ -1, -1, -1, 1 },
		{ 0, 0, -1 },
		{ 0, 1 }
	});

	// Left Face //
	m_cpCube->GetVertices().push_back
	({
		{ -1, 1, 1, 1 },
		{ -1, 0, 0 },
		{ 0, 0 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ -1, 1, -1, 1 },
		{ -1, 0, 0 },
		{ 1, 0 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ -1, -1, -1, 1 },
		{ -1, 0, 0 },
		{ 1, 1 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ -1, -1, 1, 1 },
		{ -1, 0, 0 },
		{ 0, 1 }
	});

	// Right Face //
	m_cpCube->GetVertices().push_back
	({
		{ 1, 1, -1, 1 },
		{ 1, 0, 0 },
		{ 0, 0 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ 1, 1, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 0 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ 1, -1, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 1 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ 1, -1, -1, 1 },
		{ 1, 0, 0 },
		{ 0, 1 }
	});

	// Back Face //
	m_cpCube->GetVertices().push_back
	({
		{ 1, 1, 1, 1 },
		{ 0, 0, 1 },
		{ 0, 0 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ -1, 1, 1, 1 },
		{ 0, 0, 1 },
		{ 1, 0 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ -1, -1, 1, 1 },
		{ 0, 0, 1 },
		{ 1, 1 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ 1, -1, 1, 1 },
		{ 0, 0, 1 },
		{ 0, 1 }
	});

	// Top Face //
	m_cpCube->GetVertices().push_back
	({
		{ -1, 1, 1, 1 },
		{ 0, 1, 0 },
		{ 0, 0 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ 1, 1, 1, 1 },
		{ 0, 1, 0 },
		{ 1, 0 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ 1, 1, -1, 1 },
		{ 0, 1, 0 },
		{ 1, 1 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ -1, 1, -1, 1 },
		{ 0, 1, 0 },
		{ 0, 1 }
	});

	// Bottom Face //
	m_cpCube->GetVertices().push_back
	({
		{ -1, -1, -1, 1 },
		{ 0, -1, 0 },
		{ 0, 0 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ 1, -1, -1, 1 },
		{ 0, -1, 0 },
		{ 1, 0 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ 1, -1, 1, 1 },
		{ 0, -1, 0 },
		{ 1, 1 }
	});

	m_cpCube->GetVertices().push_back
	({
		{ -1, -1, 1, 1 },
		{ 0, -1, 0 },
		{ 0, 1 }
	});
	cCube3 = new CRenderMesh(m_cpCube, GRAPHICS->GetStandardVS(), GRAPHICS->GetStandardPS());
	TranslateLocalX(cCube3->GetTransformMatrix(), -5); // Translate left 5
	TranslateLocalZ(cCube3->GetTransformMatrix(), -5);
	GRAPHICS->AddRenderMesh(cCube3);

	// Allocate RenderMesh //
	cCube = new CRenderMesh(m_cpCube, GRAPHICS->GetStandardVS(), GRAPHICS->GetStandardPS()); // Default texture
	cCube->GetVarPixelShader() = GRAPHICS->GetPixelShaderVar();

	TranslateLocalX(cCube->GetTransformMatrix(), -5); // Translate left 5
	cCube->GetSpecular() = 2.0f;

	// Add To Renderer // Does not handle clean up
	GRAPHICS->AddRenderMesh(cCube);

	cCube->SetEdgeGlow(true);
	cCube3->SetEdgeGlow(true);

	// Directional Light Cube //
	cCube4 = new CRenderMesh(m_cpCube, GRAPHICS->GetStandardVS(), GRAPHICS->GetStandardPS());
	RotateLocalX(cCube4->GetTransformMatrix(), 25);

	GRAPHICS->AddRenderMesh(cCube4); // Should render a little behind the directional light
}

void CNickState::TransparencyEx() // Same as RenderMesh, except you set a bool
{
	//std::vector<unsigned int> cCubeIndex = cCube->GetIndexArr(); // Steal from already created cube
	//std::vector<TVertex3D> cCubeVertex = cCube->GetVertexArr();

	// Allocate RenderMesh //
	cCube2 = new CRenderMesh(m_cpCube, GRAPHICS->GetStandardVS(), GRAPHICS->GetStandardPS());
	cCube2->SetTexture(L"../Game/Assets/Art/2D/HUD/HealthVignette.dds");
	cCube2->GetTransparent() = true; // Set object to use transparency
	//cCube2->GetTransparencyOverride() = 0.1f;
	cCube2->GetVarPixelShader() = GRAPHICS->GetPixelShaderVar();

	TranslateLocalX(cCube2->GetTransformMatrix(), 5); // Translate right 5

	// Add To Renderer // Does not handle clean up
	GRAPHICS->AddRenderMesh(cCube2);
}

void CNickState::DirectLightEx() // Basic directional light creation
{
	// Create Light //
	tDirectLight = new TDirectLight
	{
		{ 0, -1, 0, 1 }, // Direction
		{ 1, 1, 1 }				// Color
	};

	// Add To Renderer //
	GRAPHICS->SetDirectionalLight(tDirectLight); // Only one directional light at a time
	//GRAPHICS->DirectionalLightLookAt({ -1, -0.5f, -1 });
}

void CNickState::PointLightEx() // Basic point light creation
{
	//// Create Light //
	//tPointLight = new TPointLight
	//{
	//	{ 0, 5, 0, 1 }, // Position
	//	{ 1, 0, 1 },	// Color
	//	{ 250 },		// Radius
	//	{ 2.5f }		// Brightness
	//};
	//
	//std::vector<int> test;
	//
	//// Add To Renderer //
	//GRAPHICS->AddLight(tPointLight);
	//
	//// Second Light //
	//tPointLight2 = new TPointLight
	//{
	//	{ -100, 5, 100, 1 }, // Position
	//	{ 0, 1, 1 },	   // Color
	//	{ 250 },		   // Radius
	//	{ 2.5f }		   // Brightness
	//};
	//
	//GRAPHICS->AddLight(tPointLight2);
	//
	//// Third Light //
	//tPointLight3 = new TPointLight
	//{
	//	{ 0, 5, 45, 1 }, // Position
	//	{ 1, 0.25f, 0 },	   // Color
	//	{ 250 },		   // Radius
	//	{ 3 }		   // Brightness
	//};
	//
	//GRAPHICS->AddLight(tPointLight3);

	TPointLight* shadowLight = new TPointLight
	{
		{ cCube4->GetTransformMatrix().m[3][0], cCube4->GetTransformMatrix().m[3][1], cCube4->GetTransformMatrix().m[3][2], 1 },
		{ (rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f },
		{ 250 },
		{ 3 }
	};

	GRAPHICS->AddLight(shadowLight);

	for (unsigned int i = 0; i < 100; i++)
	{
		TPointLight* newLight = new TPointLight
		{
			{ (rand() % 600) - 300, (rand() % 600) - 300, (rand() % 600) - 300, 1 },
			{ (rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f },
			{ 250 },
			{ 3 }
		};
	
		GRAPHICS->AddLight(newLight);
	}
}

void CNickState::SkyboxEx() // Skybox creation
{
	// SkyBox Indices Initialization //
	std::vector<unsigned int> nSkyIndex;

	// Front Face //
	m_cpSkyBox->GetIndices().push_back(1);
	m_cpSkyBox->GetIndices().push_back(0);
	m_cpSkyBox->GetIndices().push_back(3);

	m_cpSkyBox->GetIndices().push_back(1);
	m_cpSkyBox->GetIndices().push_back(3);
	m_cpSkyBox->GetIndices().push_back(2);

	// Left Face //
	m_cpSkyBox->GetIndices().push_back(0);
	m_cpSkyBox->GetIndices().push_back(4);
	m_cpSkyBox->GetIndices().push_back(7);

	m_cpSkyBox->GetIndices().push_back(0);
	m_cpSkyBox->GetIndices().push_back(7);
	m_cpSkyBox->GetIndices().push_back(3);

	// Right Face //
	m_cpSkyBox->GetIndices().push_back(5);
	m_cpSkyBox->GetIndices().push_back(1);
	m_cpSkyBox->GetIndices().push_back(2);

	m_cpSkyBox->GetIndices().push_back(5);
	m_cpSkyBox->GetIndices().push_back(2);
	m_cpSkyBox->GetIndices().push_back(6);

	// Back Face //
	m_cpSkyBox->GetIndices().push_back(4);
	m_cpSkyBox->GetIndices().push_back(5);
	m_cpSkyBox->GetIndices().push_back(6);

	m_cpSkyBox->GetIndices().push_back(4);
	m_cpSkyBox->GetIndices().push_back(6);
	m_cpSkyBox->GetIndices().push_back(7);

	// Top Face //
	m_cpSkyBox->GetIndices().push_back(0);
	m_cpSkyBox->GetIndices().push_back(1);
	m_cpSkyBox->GetIndices().push_back(5);

	m_cpSkyBox->GetIndices().push_back(0);
	m_cpSkyBox->GetIndices().push_back(5);
	m_cpSkyBox->GetIndices().push_back(4);

	// Bottom Face //
	m_cpSkyBox->GetIndices().push_back(7);
	m_cpSkyBox->GetIndices().push_back(6);
	m_cpSkyBox->GetIndices().push_back(2);

	m_cpSkyBox->GetIndices().push_back(7);
	m_cpSkyBox->GetIndices().push_back(2);
	m_cpSkyBox->GetIndices().push_back(3);
	
	// SkyBox Vertices Initialization //
	std::vector<TVertex3D> tSkyVertex;
	
	m_cpSkyBox->GetVertices().push_back({ { -1, 1, -1, 1 } }); // 0 // Front Left  Top
	m_cpSkyBox->GetVertices().push_back({ {  1,  1, -1, 1 } }); // 1 // Front Right Top
	m_cpSkyBox->GetVertices().push_back({ {  1, -1, -1, 1 } }); // 2 // Front Right Bottom
	m_cpSkyBox->GetVertices().push_back({ { -1, -1, -1, 1 } }); // 3 // Front Left  Bottom
	m_cpSkyBox->GetVertices().push_back({ { -1,  1,  1, 1 } }); // 4 // Back  Left  Top
	m_cpSkyBox->GetVertices().push_back({ {  1,  1,  1, 1 } }); // 5 // Back  Right Top
	m_cpSkyBox->GetVertices().push_back({ {  1, -1,  1, 1 } }); // 6 // Back  Right Bottom
	m_cpSkyBox->GetVertices().push_back({ { -1, -1,  1, 1 } }); // 7 // Back  Left  Bottom

	// Allocate RenderMesh //
	cSkybox = new CRenderMesh(m_cpSkyBox, GRAPHICS->GetSkyboxVS(), GRAPHICS->GetSkyboxPS());
	cSkybox->SetTexture(L"../Game/Assets/Art/2D/Textures/Skybox.dds"); // Set to skybox texture
	cSkybox->SetNormals(L"../Game/Assets/Art/2D/Textures/Skybox.dds");
	cSkybox->GetTransformMatrix() = GRAPHICS->GetViewMatrix(); // Snap to camera

	// Add To Renderer //
	GRAPHICS->AddRenderMesh(cSkybox);
}

void CNickState::DrawLineEx() // Drawing debug lines
{
	for (unsigned int i = 0; i < 1500; i++) // Draws 1,500 Lines Every Frame
		GRAPHICS->DrawLine({ -20000, (float)i, 0 }, { 20000, (float)i, 0 }, { 0, 1, 1 }); // Start, End, Color
}

void CNickState::LoadFontEx() // Loading fonts
{
	nFontIndex = GRAPHICS->LoadFont(L"Harlow.font"); // The int that LoadFont returns is your index into the font list
}

void CNickState::RenderTextEx() // Rendering text
{
	// Position is in screen space, with top-left corner being -1000, 1000 and bottom-right being 1000, -1000
	// Default position is 0, 0 at the center of the screen
	// Alignment options are Left (default), Center, Right

	// String, Position, Color (optional), Scale (optional), Alignment (optional), Font Index (optional)
	GRAPHICS->RenderText(L"The Exile", { 0, 900 }, { 1, 1, 1, 1 }, { 1, 1 }, Center);
	GRAPHICS->RenderText(L"Left Aligned", { -1000, 0 });
	GRAPHICS->RenderText(L"Right Aligned", { 1000, 0 }, { 1, 1, 1, 1 }, { 1, 1 }, Right);
	GRAPHICS->RenderText(L"Different Font!", { 0, -500 }, { 1, 1, 1, 1 }, { 1, 1 }, Center, nFontIndex); // Include index for different font
}