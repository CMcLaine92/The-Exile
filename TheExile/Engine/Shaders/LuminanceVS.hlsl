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
	TPixelPack tVertical : VERTICAL;
};

TVertexOutput main(TVertexInput tVertexIn)
{
	// No need to do any transformations
	// Just a basic shader pass through to the PS	

	TVertexOutput tVertexOut;
	tVertexOut.m_fCoordinate = tVertexIn.m_fCoordinate;
	tVertexOut.m_fTexCoord = tVertexIn.m_fTexCoord.xy;

	//float fBlurMagnitude = 4.5f;
	//
	//float fTexelSizeX = 1.0f / 1920.0f; // Hardcoded HD width
	//float fTexelSizeY = 1.0f / 1080.0f; // Hardcoded HD height
	//
	//tVertexOut.tHorizontal.m_fTexCoord1 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX * -4.0f * fBlurMagnitude, 0.0f);
	//tVertexOut.tHorizontal.m_fTexCoord2 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX * -3.0f * fBlurMagnitude, 0.0f);
	//tVertexOut.tHorizontal.m_fTexCoord3 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX * -2.0f * fBlurMagnitude, 0.0f);
	//tVertexOut.tHorizontal.m_fTexCoord4 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX * -1.0f * fBlurMagnitude, 0.0f);
	//tVertexOut.tHorizontal.m_fTexCoord5 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX *  0.0f * fBlurMagnitude, 0.0f);
	//tVertexOut.tHorizontal.m_fTexCoord6 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX *  1.0f * fBlurMagnitude, 0.0f);
	//tVertexOut.tHorizontal.m_fTexCoord7 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX *  2.0f * fBlurMagnitude, 0.0f);
	//tVertexOut.tHorizontal.m_fTexCoord8 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX *  3.0f * fBlurMagnitude, 0.0f);
	//tVertexOut.tHorizontal.m_fTexCoord9 = tVertexIn.m_fTexCoord.xy + float2(fTexelSizeX *  4.0f * fBlurMagnitude, 0.0f);
	//
	//tVertexOut.tVertical.m_fTexCoord1 = tVertexIn.m_fTexCoord.xy + float2(0.0f, fTexelSizeY * -4.0f * fBlurMagnitude);
	//tVertexOut.tVertical.m_fTexCoord2 = tVertexIn.m_fTexCoord.xy + float2(0.0f, fTexelSizeY * -3.0f * fBlurMagnitude);
	//tVertexOut.tVertical.m_fTexCoord3 = tVertexIn.m_fTexCoord.xy + float2(0.0f, fTexelSizeY * -2.0f * fBlurMagnitude);
	//tVertexOut.tVertical.m_fTexCoord4 = tVertexIn.m_fTexCoord.xy + float2(0.0f, fTexelSizeY * -1.0f * fBlurMagnitude);
	//tVertexOut.tVertical.m_fTexCoord5 = tVertexIn.m_fTexCoord.xy + float2(0.0f, fTexelSizeY *  0.0f * fBlurMagnitude);
	//tVertexOut.tVertical.m_fTexCoord6 = tVertexIn.m_fTexCoord.xy + float2(0.0f, fTexelSizeY *  1.0f * fBlurMagnitude);
	//tVertexOut.tVertical.m_fTexCoord7 = tVertexIn.m_fTexCoord.xy + float2(0.0f, fTexelSizeY *  2.0f * fBlurMagnitude);
	//tVertexOut.tVertical.m_fTexCoord8 = tVertexIn.m_fTexCoord.xy + float2(0.0f, fTexelSizeY *  3.0f * fBlurMagnitude);
	//tVertexOut.tVertical.m_fTexCoord9 = tVertexIn.m_fTexCoord.xy + float2(0.0f, fTexelSizeY *  4.0f * fBlurMagnitude);

	return tVertexOut;
}