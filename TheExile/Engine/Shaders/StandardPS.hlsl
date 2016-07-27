#include "Lighting.hlsli"

texture2D tDiffuseMap : register(t0);
SamplerState sSamplerState : register(s0);

float4 main(TPixel tPixelIn) : SV_TARGET
{
	float4 fDiffuse = tDiffuseMap.Sample(sSamplerState, tPixelIn.m_fTexCoord.xy);

	if (fSpecAmount[1] >= 0)
		fDiffuse.w = fSpecAmount[1];

	float4 fTotalLight = ComputeLighting(tPixelIn, fDiffuse, tPixelIn.m_fNormal.xyz); // Calculate normal lighting, then add directional light shadow

	return saturate(fTotalLight);
}