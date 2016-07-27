#pragma pack_matrix(row_major)

struct TVertexInput
{
	float4 m_fCoordinate : POSITIONS;
	float4 m_fNormal : NORMALS;
	float4 m_fTexCoord : TEXCOORDS;
	float4 m_fTangent : TANGENTS;
};

struct TVertexOutput
{
	float4 m_fCoordinate : SV_POSITION;
	float2 m_fTexCoord : TEXCOORD0;
};

TVertexOutput main(TVertexInput tVertexIn)
{
	// No need to do any transformations
	// Just a basic shader pass through to the PS	

	TVertexOutput tVertexOut;
	tVertexOut.m_fCoordinate = tVertexIn.m_fCoordinate;
	tVertexOut.m_fTexCoord = tVertexIn.m_fTexCoord.xy;

	return tVertexOut;
}