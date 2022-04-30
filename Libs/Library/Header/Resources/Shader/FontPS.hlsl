#include"DefaultShader.hlsli"

float4 PSMain(Pixel_UV_Color pixel) : SV_TARGET
{
	float4 Tex = (floatTexture.Sample(mainSampler,pixel.uv))*pixel.color;
	Tex.a *= color.w;
	return Tex;
}
