#pragma pack_matrix(row_major)
struct TParticleInput
{
	float4 m_fCoordinate : POSITIONS;
	float m_fStartLife : STARTLIFE;
	float m_fCurrentLife : CURRENTLIFE;
	float2 m_fSize : SIZE;
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

TParticleToGS main( TParticleInput input)
{
	TParticleToGS output;
	float fLifeRatio = 1.0f - (input.m_fCurrentLife / input.m_fStartLife);

	output.m_fStartLife = input.m_fStartLife;
	output.m_fCurrentLife = input.m_fCurrentLife;
	output.m_fCoordinate = input.m_fCoordinate;

	output.m_fSize = lerp(input.m_fSize[0], input.m_fSize[1], fLifeRatio);

	return output;
}