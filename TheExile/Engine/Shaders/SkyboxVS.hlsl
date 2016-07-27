#pragma pack_matrix(row_major)

struct TVertexInput
{
	float4 m_fCoordinate : POSITIONS;
	float4 m_fNormal : NORMALS;
	float4 m_fTexCoord : TEXCOORDS;
	float4 m_fTanget : TANGENTS;
};

struct TVertexOutput
{
	float4 m_fOldCoordinate : OLD_POSITION;
	float4 m_fCoordinate : SV_POSITION;
	float4 m_fNormal : NORMALS;
	float4 m_fTexCoord : TEXCOORD0;
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

TVertexOutput main(TVertexInput tVertexIn)
{
	TVertexOutput tVertexOut;

	float4 fLocalPosition = float4(tVertexIn.m_fCoordinate.xyz, 1);
	float4x4 fNormalTransform = fTransformMatrix;
	fNormalTransform[3] = float4(0, 0, 0, 0); // Zero out position	

	// Perspective Divide //
	fLocalPosition = mul(fLocalPosition, fTransformMatrix);
	fLocalPosition = mul(fLocalPosition, fViewMatrix);
	fLocalPosition = mul(fLocalPosition, fProjectionMatrix);

	tVertexOut.m_fOldCoordinate = tVertexIn.m_fCoordinate;
	tVertexOut.m_fCoordinate = fLocalPosition;
	tVertexOut.m_fNormal = mul(tVertexIn.m_fNormal, fNormalTransform);
	tVertexOut.m_fTexCoord = tVertexIn.m_fTexCoord;

	return tVertexOut;
}