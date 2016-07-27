#define AMBIENT 0.25f
#define MAX_LIGHTS 31
#define SPECULAR_POWER 128.0f

struct TPixel
{
	float4 m_fOldCoordinate : OLD_POSITION;
	float4 m_fCoordinate : SV_POSITION;
	float4 m_fNormal : NORMALS;
	float4 m_fTexCoord : TEXCOORD0;
	float4 m_fTangent : TANGENTS;
};

struct TPointLight
{
	float4 m_fPosition;
	float3 m_fColor;
	float m_fRadius;
	float m_fBrightness;
	float3 m_fPadding;
};

struct TDirectLight
{
	float4 m_fDirection;
	float4 m_fColor;
};

struct TLightPack
{
	TPointLight tPointLight[MAX_LIGHTS];
	TDirectLight tDirectLight;
};

cbuffer bLightList : register(b5)
{
	TLightPack tLights;
}

cbuffer bCameraPosition : register(b7)
{
	float4 fCameraPos;
}

cbuffer bSpecularData : register(b8)
{
	float4 fSpecAmount;
}

inline float3 CalcPointLight(float3 fLightPos, float3 fPixelPos, float3 fNormal, float fRadius, float3 fColor, float3 fDiffuse)
{
	float3 fPixelToLight = fLightPos - fPixelPos;
	float3 fLightDir = normalize(fPixelToLight);
	float fIntensity = saturate(dot(fLightDir, fNormal));
	float fAttenuation = 1.0f - saturate(length(fPixelToLight) / fRadius);
	fIntensity *= fAttenuation * fAttenuation;

	// Specular //
	float3 fSpecFinal = {0, 0, 0};
	if (fIntensity > 0.0001f)
	{
		float3 fViewDir = normalize(fCameraPos.xyz - fPixelPos);
		float3 fHalfVec = normalize(fLightDir + fViewDir);
		float fSpecIntensity = pow(saturate(dot(fNormal, fHalfVec)), SPECULAR_POWER);

		fSpecFinal = fColor * fSpecIntensity * fAttenuation * fDiffuse * fSpecAmount[0];
	}

	return (fIntensity * fColor * fDiffuse) + fSpecFinal;
}

inline float3 CalcDirectLight(float3 fLightDir, float3 fNormal, float3 fDiffuse, float3 fColor)
{
	float fIntensity = saturate(dot(normalize(-fLightDir), fNormal));
	return (fIntensity * fColor * fDiffuse);
}

inline float4 ComputeLighting(TPixel tPixel, float4 fDiffuse, float3 fNormals) // Don't know if inline actually matters. Figure it can't make it slower.
{
	float3 fTotalLight = { 0, 0, 0 };
	float3 fPointLight = { 0, 0, 0 };
	float3 fSpecular = { 0, 0, 0 };
	float3 fDirectLight = { 0, 0, 0 };
	
	// Point Lights //
	[unroll]
	for (int i = 0; i < MAX_LIGHTS; i++)
		fPointLight += CalcPointLight(tLights.tPointLight[i].m_fPosition.xyz, tPixel.m_fOldCoordinate.xyz, fNormals.xyz, tLights.tPointLight[i].m_fRadius, tLights.tPointLight[i].m_fColor * tLights.tPointLight[i].m_fBrightness, fDiffuse.xyz);

	// Directional Light //
	fDirectLight += CalcDirectLight(tLights.tDirectLight.m_fDirection.xyz, fNormals.xyz, fDiffuse.xyz, tLights.tDirectLight.m_fColor.xyz); // All directional lights will cast shadows.

	fTotalLight = fPointLight + fSpecular + fDirectLight + (AMBIENT * fDiffuse.xyz); // Ambient applied after everything. Makes entire scene brighter.

	return float4(fTotalLight, fDiffuse.w);
}