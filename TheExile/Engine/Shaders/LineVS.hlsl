#pragma pack_matrix(row_major)

struct TVertexInput
{
	float4 m_fCoordinate : POSITIONS;
	float4 m_fColor : COLORS;
};

struct TVertexOutput
{
	float4 m_fCoordinate : SV_POSITION;
	float4 m_fColor : COLORS;
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

	// Perspective Divide //
	fLocalPosition = mul(fLocalPosition, fTransformMatrix);
	fLocalPosition = mul(fLocalPosition, fViewMatrix);
	fLocalPosition = mul(fLocalPosition, fProjectionMatrix);

	tVertexOut.m_fCoordinate = fLocalPosition;
	tVertexOut.m_fColor = tVertexIn.m_fColor;

	return tVertexOut;
}