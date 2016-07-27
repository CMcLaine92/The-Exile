#pragma pack_matrix(row_major)

struct TVertexInput
{
	float4 m_fCoordinate : POSITIONS;
	float4 m_fNormal : NORMALS;
	float4 m_fTexCoord : TEXCOORDS;
	float4 m_fTangent : TANGENTS;
};

struct TPixelPack
{
	// Tex Coords for neighbors
	float2 m_fTexCoord1;
	float2 m_fTexCoord2;
	float2 m_fTexCoord3;
	float2 m_fTexCoord4;
	float2 m_fTexCoord5;
	float2 m_fTexCoord6;
	float2 m_fTexCoord7;
	float2 m_fTexCoord8;
	float2 m_fTexCoord9;
};

struct TVertexOutput
{
	float4 m_fCoordinate : SV_POSITION;
	float2 m_fTexCoord : TEXCOORD0;

	TPixelPack tHorizontal : HORIZONTAL;
};

TVertexOutput main(TVertexInput tVertexIn)
{
	TVertexOutput tVertexOut;
	tVertexOut.m_fCoordinate = tVertexIn.m_fCoordinate;
	tVertexOut.m_fTexCoord = tVertexIn.m_fTexCoord.xy;

	float fBlurMagnitude = 10.0f;
	
	float fTexelSizeX = 1.0f / 960;
	
	tVertexOut.tHorizontal.m_fTexCoord1 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX * -4.0f * fBlurMagnitude, 0.0f);
	tVertexOut.tHorizontal.m_fTexCoord2 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX * -3.0f * fBlurMagnitude, 0.0f);
	tVertexOut.tHorizontal.m_fTexCoord3 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX * -2.0f * fBlurMagnitude, 0.0f);
	tVertexOut.tHorizontal.m_fTexCoord4 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX * -1.0f * fBlurMagnitude, 0.0f);
	tVertexOut.tHorizontal.m_fTexCoord5 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX *  0.0f * fBlurMagnitude, 0.0f);
	tVertexOut.tHorizontal.m_fTexCoord6 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX *  1.0f * fBlurMagnitude, 0.0f);
	tVertexOut.tHorizontal.m_fTexCoord7 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX *  2.0f * fBlurMagnitude, 0.0f);
	tVertexOut.tHorizontal.m_fTexCoord8 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX *  3.0f * fBlurMagnitude, 0.0f);
	tVertexOut.tHorizontal.m_fTexCoord9 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX *  4.0f * fBlurMagnitude, 0.0f);

	return tVertexOut;
}