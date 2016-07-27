#pragma pack_matrix(row_major)

#define SHADOW_PASSES 6

struct TVertexInput
{
	float4 m_fCoordinate : POSITIONS;
	float4 m_fNormal : NORMALS;
	float4 m_fTexCoord : TEXCOORDS;
	float4 m_fTangent : TANGENTS;
};

struct TVertexOutput
{
	float4 m_fOldCoordinate : OLD_POSITION;
	float4 m_fCoordinate : SV_POSITION;
	float4 m_fNormal : NORMALS;
	float4 m_fTexCoord : TEXCOORD0;
	float4 m_fTangent : TANGENTS;
};

cbuffer bObject : register(b0)
{
	float4x4 fTransformMatrix;
}

cbuffer bScene : register(b1)
{
	float4x4 fViewMatrix;
	float4x4 fProjectionMatrix;
}

struct TScene
{
	float4x4 fViewMatrix;
	float4x4 fProjectionMatrix;
};

cbuffer bLightScene : register(b9)
{
	TScene tScene[SHADOW_PASSES];
}

TVertexOutput main(TVertexInput tVertexIn)
{
	TVertexOutput tVertexOut;
	tVertexOut.m_fOldCoordinate =
	tVertexOut.m_fCoordinate =
	tVertexOut.m_fNormal =
	tVertexOut.m_fTexCoord =
	tVertexOut.m_fTangent = float4(0, 0, 0, 0);

	float4 fLocalPosition = float4(tVertexIn.m_fCoordinate.xyz, 1);
	float4x4 fNormalTransform = fTransformMatrix;
	fNormalTransform[3] = float4(0, 0, 0, 1); // Zero out position	

	// Perspective Divide //
	fLocalPosition = mul(fLocalPosition, fTransformMatrix);
	fLocalPosition = mul(fLocalPosition, fViewMatrix);
	fLocalPosition = mul(fLocalPosition, fProjectionMatrix);

	tVertexOut.m_fOldCoordinate = mul(tVertexIn.m_fCoordinate, fTransformMatrix);
	tVertexOut.m_fCoordinate = fLocalPosition;
	tVertexOut.m_fTexCoord = tVertexIn.m_fTexCoord;

	tVertexOut.m_fNormal.xyz = mul(tVertexIn.m_fNormal.xyz, (float3x3)fTransformMatrix);

	float3 fTangent = tVertexIn.m_fTangent.xyz;
	fTangent = mul((fTangent * tVertexIn.m_fTangent.w), (float3x3)fTransformMatrix);
	fTangent = fTangent - dot(fTangent, tVertexOut.m_fNormal.xyz) * tVertexOut.m_fNormal.xyz;

	tVertexOut.m_fTangent.xyz = fTangent;

	return tVertexOut;
}