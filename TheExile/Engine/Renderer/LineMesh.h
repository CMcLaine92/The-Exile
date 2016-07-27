#pragma once

#include "RenderMesh.h"

#define MAX_LINES 1000000

struct TLineVertex
{
	float m_fPosition[4];
	float m_fColor[4];
};

class CLineMesh : public CRenderMesh
{
	bool m_bRemap = false;
	std::vector<TLineVertex> m_tLineVertexArr;

	// Helper Functions //
	void MapBuffers() override;

public:
	CLineMesh();
	~CLineMesh() = default;

	void Draw() override;
	void Clear() { m_tLineVertexArr.clear(); }
	void Resize(int nSize) { m_tLineVertexArr.resize(nSize); }

	// Accessors //
	bool& GetRemap() { return m_bRemap; }
	std::vector<TLineVertex>& GetLineVertexArr() { return m_tLineVertexArr; }
};