texture2D tScreenQuad : register(t4);
SamplerState sSamplerState : register(s0);

struct TPixelPack
{
	// Tex Coords for neighbors
	float2 m_fTexCoord1;
	float2 m_fTexCoord2;
	float2 m_fTexCoord3;
	float2 m_fTexCoord4;
	float2 m_fTexCoord5;
	float2 m_fTexCoord6;
	float2 m_fTexCoord7;
	float2 m_fTexCoord8;
	float2 m_fTexCoord9;
};

struct TPixel
{
	float4 m_fCoordinate : SV_POSITION;
	float2 m_fTexCoord : TEXCOORD0;

	TPixelPack tHorizontal : HORIZONTAL;
	TPixelPack tVertical : VERTICAL;
};

float4 main(TPixel tPixelIn) : SV_TARGET
{
	/* Currently to be used for bloom */

	float4 fPixel = tScreenQuad.Sample(sSamplerState, tPixelIn.m_fTexCoord.xy);

	float fThreshold = 0.5f;
	float fLightIntensity = (fPixel.x + fPixel.y + fPixel.z) / 3.0f;
	//float fLightIntensity = normalize(fPixel);
	if (fLightIntensity >= fThreshold)
		return fPixel;
	else
		return float4(0, 0, 0, 0);

	// Horizontal Blur //
	//float fWeight0 = 1.0f;
	//float fWeight1 = 0.9f;
	//float fWeight2 = 0.55f;
	//float fWeight3 = 0.18f;
	//float fWeight4 = 0.1f;
	//float fNormalization = (fWeight0 + 2.0f * (fWeight1 + fWeight2 + fWeight3 + fWeight4));
	//
	//fWeight0 = fWeight0 / fNormalization;
	//fWeight1 = fWeight1 / fNormalization;
	//fWeight2 = fWeight2 / fNormalization;
	//fWeight3 = fWeight3 / fNormalization;
	//fWeight4 = fWeight4 / fNormalization;
	//
	//float4 fColorX = { 0.0f, 0.0f, 0.0f, 0.0f }; // Horizontal
	//float4 fColorY = { 0.0f, 0.0f, 0.0f, 0.0f }; // Vertical
	//
	//fColorX += tScreenQuad.Sample(sSamplerState, tPixelIn.tHorizontal.m_fTexCoord1) * fWeight4;
	//fColorX += tScreenQuad.Sample(sSamplerState, tPixelIn.tHorizontal.m_fTexCoord2) * fWeight3;
	//fColorX += tScreenQuad.Sample(sSamplerState, tPixelIn.tHorizontal.m_fTexCoord3) * fWeight2;
	//fColorX += tScreenQuad.Sample(sSamplerState, tPixelIn.tHorizontal.m_fTexCoord4) * fWeight1;
	//fColorX += tScreenQuad.Sample(sSamplerState, tPixelIn.tHorizontal.m_fTexCoord5) * fWeight0;
	//fColorX += tScreenQuad.Sample(sSamplerState, tPixelIn.tHorizontal.m_fTexCoord6) * fWeight1;
	//fColorX += tScreenQuad.Sample(sSamplerState, tPixelIn.tHorizontal.m_fTexCoord7) * fWeight2;
	//fColorX += tScreenQuad.Sample(sSamplerState, tPixelIn.tHorizontal.m_fTexCoord8) * fWeight3;
	//fColorX += tScreenQuad.Sample(sSamplerState, tPixelIn.tHorizontal.m_fTexCoord9) * fWeight4;
	//
	//fColorX.a = 1.0f;
	//
	//fColorY += tScreenQuad.Sample(sSamplerState, tPixelIn.tVertical.m_fTexCoord1) * fWeight4;
	//fColorY += tScreenQuad.Sample(sSamplerState, tPixelIn.tVertical.m_fTexCoord2) * fWeight3;
	//fColorY += tScreenQuad.Sample(sSamplerState, tPixelIn.tVertical.m_fTexCoord3) * fWeight2;
	//fColorY += tScreenQuad.Sample(sSamplerState, tPixelIn.tVertical.m_fTexCoord4) * fWeight1;
	//fColorY += tScreenQuad.Sample(sSamplerState, tPixelIn.tVertical.m_fTexCoord5) * fWeight0;
	//fColorY += tScreenQuad.Sample(sSamplerState, tPixelIn.tVertical.m_fTexCoord6) * fWeight1;
	//fColorY += tScreenQuad.Sample(sSamplerState, tPixelIn.tVertical.m_fTexCoord7) * fWeight2;
	//fColorY += tScreenQuad.Sample(sSamplerState, tPixelIn.tVertical.m_fTexCoord8) * fWeight3;
	//fColorY += tScreenQuad.Sample(sSamplerState, tPixelIn.tVertical.m_fTexCoord9) * fWeight4;
	//
	//fColorY.a = 1.0f;
	//
	//return fColorY;
}