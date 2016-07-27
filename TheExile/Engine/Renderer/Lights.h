#pragma once



#define MAX_LIGHTS 31 // Per Tile

struct TPointLight
{
	float m_fPosition[4];
	float m_fColor[3];
	float m_fRadius;
	float m_fBrightness;
	float m_fPadding[3];

	float DistanceToCamera(XMFLOAT4X4& fCamera)
	{
		XMVECTOR fStart = { m_fPosition[0], m_fPosition[1], m_fPosition[2] };
		XMVECTOR fEnd = { fCamera.m[3][0], fCamera.m[3][1], fCamera.m[3][2] };

		return XMVector3LengthSq(fEnd - fStart).m128_f32[0];
	}
};

struct TDirectLight
{
	float m_fDirection[4];
	float m_fColor[4];
};

struct TLightPack
{
	TPointLight m_tPointLights[MAX_LIGHTS];
	TDirectLight m_tDirectLight;
};