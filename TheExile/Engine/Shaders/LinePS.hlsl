struct TPixelInput
{
	float4 m_fCoordinate : SV_POSITION;
	float4 m_fColor : COLORS;
};

float4 main(TPixelInput tPixelIn) : SV_TARGET
{
	return tPixelIn.m_fColor;
}