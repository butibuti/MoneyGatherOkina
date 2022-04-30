#include"DefaultShader.hlsli"
float4 PSMain(Pixel_UV_Normal pixel) : SV_TARGET
{
	float3 lightdir = normalize(lightDir.xyz);
	float3 n = normalize(pixel.normal);

	float light =1- dot(n, -lightdir);

	float2 lightUV;
	lightUV.y = 0;
	lightUV.x = light;

	float4 shadingColor =subTexture_1.Sample(mainSampler, lightUV);

	float4 Tex = (mainTexture.Sample(mainSampler,pixel.uv).rgba);


	return  float4((Tex * shadingColor).rgb, ambient.a);
}