#include "Lighting.hlsli"

texture2D tDiffuseMap : register(t0);
texture2D tNormalMap : register(t1);
SamplerState sSamplerState : register(s0);

inline float3 ConvertNormals(TPixel tPixel, float3 fNormal, float3 fTangent)
{
	float3x3 TBN = float3x3(fTangent, cross(fTangent, fNormal), fNormal);
	
	float3 fNewNormal = tNormalMap.Sample(sSamplerState, tPixel.m_fTexCoord.xy).xyz;
	fNewNormal *= 2.0f;
	fNewNormal -= 1.0f;
	
	fNewNormal = normalize(mul(fNewNormal, TBN));
	
	return fNewNormal;
}

float4 main(TPixel tPixelIn) : SV_TARGET
{
	float4 fDiffuse = tDiffuseMap.Sample(sSamplerState, tPixelIn.m_fTexCoord.xy);
	float3 fNormal = ConvertNormals(tPixelIn, tPixelIn.m_fNormal.xyz, tPixelIn.m_fTangent.xyz);

	if (fSpecAmount[1] >= 0)
		fDiffuse.w = fSpecAmount[1];

	float4 fTotalLight = ComputeLighting(tPixelIn, fDiffuse, fNormal); // Calculate normal lighting, then add directional light shadow

	return saturate(fTotalLight);
}