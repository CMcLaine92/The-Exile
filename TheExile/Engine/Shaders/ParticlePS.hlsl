struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv  : TEXCOORD0;
};

texture2D tTexture : register(t0);
SamplerState sSamplerState : register(s0);

float4 main(GSOutput input) : SV_TARGET
{
	float4 fPixel = tTexture.Sample(sSamplerState, input.uv.xy);
	fPixel *= input.color;
	
	clip(fPixel.a - 0.01f);

	return fPixel;
}