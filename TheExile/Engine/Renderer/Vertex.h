#pragma once


struct TVertex2D
{
	float m_fPosition[4];
	float m_fTexCoord[4];
};

struct TVertex3D
{
	float m_fPosition[4];
	float m_fNormal[4];
	float m_fTexCoord[4];
	float m_fTangent[4];
};

struct TLineVertex
{
	float m_fPosition[4];
	float m_fColor[4];
}; // All lines sent in one huge vertex buffer // Each vertex is one end of a line

struct TParticleVertex
{
	float m_fPosition[4];

	float m_fStartLife;
	float m_fCurrentLife;
	float m_fPadding[2];
};

struct TAnimVertex
{
	float m_fPosition[4];
	float m_fNormal[4];
	float m_fTexCoord[4];
	float m_fTangent[4];

	unsigned int m_uBoneIndex[4];
	float m_fBoneWeight[4];
};

class IVertex
{
	std::type_index cType;

	union
	{
		TVertex2D		m_tType1;
		TVertex3D		m_tType2;
		TLineVertex		m_tType3;
		TParticleVertex m_tType4;
		TAnimVertex		m_tType5;
	};

public:
	IVertex(float2 fPosition, float2 fTexCoord); // Vertex2D
	IVertex(float4 fPosition, float4 fNormal, float4 fTexCoord, float4 fTangent); // Vertex3D
	IVertex(float4 fPosition, float4 fColor); // LineVertex
	IVertex(float4 fPosition, float fStartLife, float fCurrentLife); // ParticleVertex
	IVertex(float4 fPosition, float4 fNormal, float4 fTexCoord, float4 fTangent, uint4 uBoneIndex, float4 fBoneWeight); // AnimVertex

	// Accessors //
	TVertex2D& GetVertex2D() { return m_tType1; }
	TVertex3D& GetVertex3D() { return m_tType2; }
	TLineVertex& GetLineVertex() { return m_tType3; }
	TParticleVertex& GetParticleVertex() { return m_tType4; }
	TAnimVertex& GetAnimVertex() { return m_tType5; }

	std::type_index& Type() { return cType; }
	unsigned int Size();
};
// Need to write operator= overloads