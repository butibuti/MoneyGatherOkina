#include"DefaultShader.hlsli"
float4 PSMain(Pixel_UV_Normal pixel) : SV_TARGET
{
	float3 lightdir = normalize(lightDir.xyz);
	float3 n = normalize(pixel.normal);

	float light = dot(n, -lightdir);

	float s =1.0- step(light, lightDir.a);
	s += 0.3 * (1.0 - s);

	float4 Tex = (mainTexture.Sample(mainSampler,pixel.uv).rgba);


	return  float4((Tex * s).rgb, ambient.a);
}