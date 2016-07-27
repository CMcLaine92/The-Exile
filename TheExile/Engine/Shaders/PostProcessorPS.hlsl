texture2D tTexture : register(t4);
SamplerState sSamplerState : register(s0);

struct TPixel
{
	float4 m_fCoordinate : SV_POSITION;
	float2 m_fTexCoord : TEXCOORD0;
};

cbuffer Gamma : register(c9)
{
	float4 fGamma;
}

float4 main(TPixel tPixelIn) : SV_TARGET
{
	float4 fColor = tTexture.Sample(sSamplerState, tPixelIn.m_fTexCoord.xy);

	//fColor.x = fColor.x / 2.0f;
	//fColor.y = fColor.y / 2.0f;
	//fColor.z = fColor.z / 2.0f;

	return fColor;
}