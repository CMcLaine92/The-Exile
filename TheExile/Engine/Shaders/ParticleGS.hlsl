#pragma pack_matrix(row_major)
struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv  : TEXCOORD0;
};

struct TParticleToGS
{
	float4 m_fCoordinate : POSITIONS;
	float m_fStartLife : STARTLIFE;
	float m_fCurrentLife : CURRENTLIFE;
	float m_fSize : SIZE0;
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
cbuffer Flyweight : register(b10)
{
	float4 f4StartColor;
	float4 f4EndColor;
	float4 f4CameraPos;
	float fStartSize;
	float fEndSize;
	float2 padding;
}


[maxvertexcount(4)]
void main(point TParticleToGS input[1], inout TriangleStream< GSOutput > OutputStream)
{
	float fLifeRatio = 1.0f - ( input[0].m_fCurrentLife / input[0].m_fStartLife);
	float width = input[0].m_fSize;
	float3 cameraPos = f4CameraPos.xyz;
	float3 PlaneNormal = input[0].m_fCoordinate.xyz - cameraPos;
	PlaneNormal.y = 0.0f;
	PlaneNormal = normalize(PlaneNormal);
	float3 upVector = float3(0, 1.0f, 0);
	float3 rightVector = normalize(cross( PlaneNormal,upVector));
	rightVector = rightVector * width * .5f;
	upVector *= width * .5f;

	float3 vertPos[4];
	vertPos[0] = input[0].m_fCoordinate.xyz - rightVector - upVector;//Bottom Left
	vertPos[1] = input[0].m_fCoordinate.xyz + rightVector - upVector; //Bottom Right
	vertPos[2] = input[0].m_fCoordinate.xyz - rightVector + upVector; //TopLeft
	vertPos[3] = input[0].m_fCoordinate.xyz + rightVector + upVector; //TopRight

	float2 texCoord[4];
	texCoord[0] = float2(0, 1);
	texCoord[1] = float2(1, 1);
	texCoord[2] = float2(0, 0);
	texCoord[3] = float2(1, 0);

	GSOutput output;
	output.color = lerp(f4StartColor, f4EndColor, fLifeRatio);
	for (unsigned int i = 0; i < 4; i++)
	{
		output.pos = float4(vertPos[i],input[0].m_fCoordinate.w);
		output.uv = texCoord[i];		
		output.pos = mul(output.pos, fPositionMatrix);
		output.pos = mul(output.pos, fViewMatrix);
		output.pos = mul(output.pos, fProjectionMatrix);
		
		OutputStream.Append(output);
	}
}