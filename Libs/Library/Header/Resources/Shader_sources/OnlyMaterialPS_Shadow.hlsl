#include"DefaultShader.hlsli"
float4 PSMain(Pixel_UV_Normal_Shadow pixel) : SV_TARGET
{ 
	float3 lightdir = normalize(lightDir.xyz);
	float3 N1 = normalize(pixel.normal);
	float4 Light = saturate(dot(N1, -lightdir) * diffuse + emissive);

	float sm = (depthTexture_1.Sample(mainSampler, pixel.shadowPos.xy));
	bool stepped = step(sm, pixel.shadowPos.z - 0.000001f);
	Light.rgb += specular.rgb * specular.a;
	Light.rgb *= (1 - 0.5 * (stepped));
	Light.a = 1.0;
	return  Light;//,0.5f);
}