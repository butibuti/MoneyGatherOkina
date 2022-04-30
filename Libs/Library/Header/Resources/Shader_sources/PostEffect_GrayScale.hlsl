#include"DefaultShader.hlsli"

float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
	float4 Tex = (mainTexture.Sample(mainSampler,pixel.uv));
	float scale = Tex.r * 0.299 + Tex.g * 0.587 + Tex.b * 0.114;

	return float4(scale, scale, scale,Tex.a);
}