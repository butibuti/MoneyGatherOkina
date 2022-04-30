#include"DefaultShader.hlsli"

float4 PSMain(Pixel pixel):SV_TARGET
{
	return float4((ExInfo).xyz,1.0f);
	
}