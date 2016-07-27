TextureCube tTexture : register(t0);
TextureCube tTexture2: register(t1);

SamplerState sSamplerState : register(s0);

struct TPixelInput
{
	float4 m_fOldCoordinate : OLD_POSITION;
	float4 m_fCoordinate : SV_POSITION;
	float4 m_fNormal : NORMALS;
	float4 m_fTexCoord : TEXCOORD0;
};
cbuffer bSpecularData : register(b8)
{
	float4 fSpecAmount;
}
float4 main(TPixelInput tPixelIn) : SV_TARGET
{
	float4 f4Output,f4Color1,f4Color2;
	f4Color1 = tTexture.Sample(sSamplerState, tPixelIn.m_fOldCoordinate.xyz);
	f4Color2 = tTexture2.Sample(sSamplerState, tPixelIn.m_fOldCoordinate.xyz);
	f4Output = lerp(f4Color1, f4Color2, fSpecAmount[1]);
	return f4Output;
}