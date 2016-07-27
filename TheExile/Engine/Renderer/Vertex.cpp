#include "Vertex.h"

// TVertex2D Constructor //
IVertex::IVertex(float2 fPosition, float2 fTexCoord) : cType(typeid(TVertex2D))
{
	m_tType1.m_fPosition[0] = fPosition.x;
	m_tType1.m_fPosition[1] = fPosition.y;

	m_tType1.m_fTexCoord[0] = fTexCoord.x;
	m_tType1.m_fTexCoord[1] = fTexCoord.y;
}

// TVertex3D Constructor //
IVertex::IVertex(float4 fPosition, float4 fNormal, float4 fTexCoord, float4 fTangent) : cType(typeid(TVertex3D))
{
	m_tType2.m_fPosition[0] = fPosition.x;
	m_tType2.m_fPosition[1] = fPosition.y;
	m_tType2.m_fPosition[2] = fPosition.z;
	m_tType2.m_fPosition[3] = fPosition.w;

	m_tType2.m_fNormal[0] = fNormal.x;
	m_tType2.m_fNormal[1] = fNormal.y;
	m_tType2.m_fNormal[2] = fNormal.z;
	m_tType2.m_fNormal[3] = fNormal.w;

	m_tType2.m_fTexCoord[0] = fTexCoord.x;
	m_tType2.m_fTexCoord[1] = fTexCoord.y;
	m_tType2.m_fTexCoord[2] = fTexCoord.z;
	m_tType2.m_fTexCoord[3] = fTexCoord.w;

	m_tType2.m_fTangent[0] = fTangent.x;
	m_tType2.m_fTangent[1] = fTangent.y;
	m_tType2.m_fTangent[2] = fTangent.z;
	m_tType2.m_fTangent[3] = fTangent.w;
}

// TLineVertex Constructor //
IVertex::IVertex(float4 fPosition, float4 fColor) : cType(typeid(TLineVertex))
{
	m_tType3.m_fPosition[0] = fPosition.x;
	m_tType3.m_fPosition[1] = fPosition.y;
	m_tType3.m_fPosition[2] = fPosition.z;
	m_tType3.m_fPosition[3] = fPosition.w;

	m_tType3.m_fColor[0] = fColor.x;
	m_tType3.m_fColor[1] = fColor.y;
	m_tType3.m_fColor[2] = fColor.z;
	m_tType3.m_fColor[3] = fColor.w;
}

// TParticleVertex Constructor //
IVertex::IVertex(float4 fPosition, float fStartLife, float fCurrentLife) : cType(typeid(TParticleVertex))
{
	m_tType4.m_fPosition[0] = fPosition.x;
	m_tType4.m_fPosition[1] = fPosition.y;
	m_tType4.m_fPosition[2] = fPosition.z;
	m_tType4.m_fPosition[3] = fPosition.w;

	m_tType4.m_fStartLife = fStartLife;
	m_tType4.m_fCurrentLife = fCurrentLife;
}

// TAnimVertex Constructor //
IVertex::IVertex(float4 fPosition, float4 fNormal, float4 fTexCoord, float4 fTangent, uint4 uBoneIndex, float4 fBoneWeight) : cType(typeid(TAnimVertex))
{
	m_tType5.m_fPosition[0] = fPosition.x;
	m_tType5.m_fPosition[1] = fPosition.y;
	m_tType5.m_fPosition[2] = fPosition.z;
	m_tType5.m_fPosition[3] = fPosition.w;

	m_tType5.m_fNormal[0] = fNormal.x;
	m_tType5.m_fNormal[1] = fNormal.y;
	m_tType5.m_fNormal[2] = fNormal.z;
	m_tType5.m_fNormal[3] = fNormal.w;

	m_tType5.m_fTexCoord[0] = fTexCoord.x;
	m_tType5.m_fTexCoord[1] = fTexCoord.y;
	m_tType5.m_fTexCoord[2] = fTexCoord.z;
	m_tType5.m_fTexCoord[3] = fTexCoord.w;

	m_tType5.m_fTangent[0] = fTangent.x;
	m_tType5.m_fTangent[1] = fTangent.y;
	m_tType5.m_fTangent[2] = fTangent.z;
	m_tType5.m_fTangent[3] = fTangent.w;

	m_tType5.m_uBoneIndex[0] = uBoneIndex.x;
	m_tType5.m_uBoneIndex[1] = uBoneIndex.y;
	m_tType5.m_uBoneIndex[2] = uBoneIndex.z;
	m_tType5.m_uBoneIndex[3] = uBoneIndex.w;

	m_tType5.m_fBoneWeight[0] = fBoneWeight.x;
	m_tType5.m_fBoneWeight[1] = fBoneWeight.y;
	m_tType5.m_fBoneWeight[2] = fBoneWeight.z;
	m_tType5.m_fBoneWeight[3] = fBoneWeight.w;
}

unsigned int IVertex::Size()
{
	if (cType == typeid(TVertex2D))
		return sizeof(TVertex2D);
	else if (cType == typeid(TVertex3D))
		return sizeof(TVertex3D);
	else if (cType == typeid(TLineVertex))
		return sizeof(TLineVertex);
	else if (cType == typeid(TParticleVertex))
		return sizeof(TParticleVertex);
	else
		return sizeof(TAnimVertex);
}