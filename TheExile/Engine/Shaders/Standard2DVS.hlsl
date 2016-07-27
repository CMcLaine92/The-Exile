#pragma pack_matrix(row_major)

struct TVertexInput
{
	float4 m_fCoordinate : POSITIONS;
	float4 m_fTexCoord : TEXCOORDS;
};

struct TVertexOutput
{
	float4 m_fCoordinate : SV_POSITION;
	float4 m_fTexCoord : TEXCOORD0;
};

cbuffer bObject : register(b0)
{
	float4x4 fPositionMatrix;
}

cbuffer bScene : register(b1)
{
	float4x4 fViewMatrix;
	float4x4 fProjectionMatrix;
}

TVertexOutput main(TVertexInput tVertexIn)
{
	TVertexOutput tVertexOut;

	float4 fLocalPosition = float4(tVertexIn.m_fCoordinate.xy, 0, 1);

	// Perspective Divide //
	fLocalPosition = float4(mul(fLocalPosition, fPositionMatrix).xy,0,1);
	//fLocalPosition = mul(fLocalPosition, fProjectionMatrix);

	tVertexOut.m_fCoordinate = fLocalPosition;
	tVertexOut.m_fTexCoord = tVertexIn.m_fTexCoord;

	return tVertexOut;
}