texture2D tTexture : register(t0);
SamplerState sSamplerState : register(s0);

struct TPixelInput
{
	float4 m_fCoordinate : SV_POSITION;
	float4 m_fTexCoord : TEXCOORD0;
};

cbuffer bSpecularData : register(b8)
{
	float4 fSpecAmount;
}

float4 main(TPixelInput tPixelIn) : SV_TARGET
{
	//float4 fPixel = { 0.3529f, 0.8118f, 0.9412f, 1 };
	float4 fPixel = tTexture.Sample(sSamplerState, tPixelIn.m_fTexCoord.xy);

	if (fSpecAmount[1] >= 0)
		fPixel.w *= fSpecAmount[1];

	return fPixel;
}