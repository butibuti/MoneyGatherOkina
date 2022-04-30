#include"DefaultShader.hlsli"
float4 PSMain(Pixel_UV_Normal_Tangent pixel) : SV_TARGET
{
	float3 mappedNormal = subTexture_1.Sample(mainSampler,pixel.uv).rgb*2.0- float3(1.0,1.0,1.0);

	mappedNormal = float3(mappedNormal.x * pixel.tangent + mappedNormal.y * pixel.binormal + mappedNormal.z * pixel.normal);
	mappedNormal = mul(mappedNormal, (float3x3) modelMatrix);


	float3 lightdir = normalize(lightDir.xyz);



	float lightPower = dot(pixel.normal, -lightdir);
	float4 Light = saturate(lightPower * diffuse + emissive);



	float4 Tex = (mainTexture.Sample(mainSampler,pixel.uv).rgba);


	return  float4((Tex*Light).rgb, Tex.a * ambient.a);
}