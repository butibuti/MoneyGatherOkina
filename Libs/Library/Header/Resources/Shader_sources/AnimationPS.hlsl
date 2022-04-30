#include"DefaultShader.hlsli"
//lighDir
///x	â°ÇÃoffset
///y	ècÇÃoffset
///z	ècÇÃscaleSize
///w	ècÇÃscaleSize
float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
	float2 uv = pixel.uv;
	uv *= lightDir.zw;
	uv += lightDir.xy;
	float4 Tex = (mainTexture.Sample(mainSampler,uv));
	return Tex;
}