#include"DefaultShader.hlsli"
//lighDir
///x	����offset
///y	�c��offset
///z	�c��scaleSize
///w	�c��scaleSize
float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
	float2 uv = pixel.uv;
	uv *= lightDir.zw;
	uv += lightDir.xy;
	float4 Tex = (mainTexture.Sample(mainSampler,uv));
	return Tex;
}