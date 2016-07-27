#pragma once
#include "State.h"


// Forward Declarations //
class CRenderMesh;
class CMesh;
struct TPointLight;
struct TDirectLight;

class CNickState : public IState
{
	float fTime;

	CMesh* m_cpPlaneMesh;
	CMesh* m_cpCube;
	CMesh* m_cpSkyBox;

	float fStartTime;
	float fStartTime2;
	float fEndTime;
	std::vector<TPointLight*> lightArr;

	int size = 0;
	CRenderMesh* cCube = nullptr;
	CRenderMesh* cCube2 = nullptr;
	CRenderMesh* cCube3 = nullptr;
	CRenderMesh* cCube4 = nullptr;
	CRenderMesh* cSkybox = nullptr;

	CRenderMesh* cTop = nullptr;
	CRenderMesh* cLeft = nullptr;
	CRenderMesh* cRight = nullptr;
	CRenderMesh* cFront = nullptr;
	CRenderMesh* cBack = nullptr;
	CRenderMesh* cBottom = nullptr;

	unsigned int nFontIndex;

	bool bReversed1 = false;
	bool bReversed2 = true;
	bool bReversed3 = false;
	bool bEdgeGlow = false;
	TPointLight* tPointLight = nullptr;
	TPointLight* tPointLight2 = nullptr;
	TPointLight* tPointLight3 = nullptr;
	TDirectLight* tDirectLight = nullptr;

	void MoveLights();
	void CreatePlatforms();

	// Examples //
	void RenderMeshEx(); // Basic RenderMesh creation
	void TransparencyEx(); // Same as RenderMesh, except you set a bool
	void DirectLightEx(); // Basic directional light creation
	void PointLightEx(); // Basic point light creation
	void SkyboxEx(); // Skybox creation
	void DrawLineEx(); // Drawing debug lines
	void LoadFontEx(); // Loading fonts
	void RenderTextEx(); // Rendering text

public:
	CNickState();
	~CNickState();
	eStateType GetStateType() { return eNONE; }
	virtual void Enter();
	virtual void Exit();
	void Update();
};

