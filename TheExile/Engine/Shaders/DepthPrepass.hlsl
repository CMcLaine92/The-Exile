#include "Lighting.hlsli"

float4 main(TPixel tPixelIn) : SV_TARGET
{
	//return float4(tPixelIn.m_fDepth, tPixelIn.m_fDepth, tPixelIn.m_fDepth, 1.0f);
	return float4(1, 1, 1, 1);
}