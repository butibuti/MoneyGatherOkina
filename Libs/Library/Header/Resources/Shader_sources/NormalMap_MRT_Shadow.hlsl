#include"DefaultShader.hlsli"
PixelOutput_Default PSMain(Pixel_UV_Normal_Phong_Tangent_Shadow pixel)
{
	PixelOutput_Default output;
	float3 mappedNormal = subTexture_1.Sample(mainSampler,pixel.uv).rgb*2.0f- float3(1.0f,1.0f,1.0f);
	mappedNormal =( float3(mappedNormal.x * pixel.tangent + mappedNormal.y * pixel.binormal + mappedNormal.z * pixel.normal));

	float4 texColor = mainTexture.Sample(mainSampler, pixel.uv);
	float4 shadowColor = texColor;
	shadowColor.xyz *= 0.5f;
	
	output.normal = float4((normalize(mul(mappedNormal, (float3x3) modelMatrix)) + float3(1.0f, 1.0f, 1.0f)) * 0.5, 1.0);
	output.baseColor = GetVSMColor(texColor, shadowColor, float2Texture_2, mainSampler, pixel.shadowPos);
	output.worldPosition = (pixel.modelPosition + float4(1.0, 1.0, 1.0, 1.0)) * 0.5;
	output.worldPosition.w = pixel.cameraPosition.z;
	output.materialInfo = float4(materialID, 1.0, 1.0, 1.0);

	return  output;
}