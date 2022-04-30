#include"DefaultShader.hlsli"
float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
	float2 uv = pixel.uv;
	uv.y = frac(uv.y + Time);
	uv.x = frac(uv.x + sin(Time));
	uv *= Tiling;
	float4 Tex = (mainTexture.Sample(mainSampler,uv));
	return Tex;
}