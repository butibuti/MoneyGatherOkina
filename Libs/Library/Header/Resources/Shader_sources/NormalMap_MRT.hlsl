#include"DefaultShader.hlsli"
PixelOutput_Default PSMain(Pixel_UV_Normal_Phong_Tangent pixel)
{
	PixelOutput_Default output;
	float3 mappedNormal = subTexture_1.Sample(mainSampler,pixel.uv).rgb*2.0- float3(1.0,1.0,1.0);

	mappedNormal =( float3(mappedNormal.x * pixel.tangent + mappedNormal.y * pixel.binormal + mappedNormal.z * pixel.normal));


	output.normal = float4((normalize(mul(mappedNormal, (float3x3) modelMatrix)) + float3(1.0, 1.0, 1.0)) * 0.5, 1.0);
	output.baseColor = mainTexture.Sample(mainSampler, pixel.uv);
	output.worldPosition = (pixel.modelPosition + float4(1.0, 1.0, 1.0, 1.0)) * 0.5;
	output.worldPosition.w = pixel.cameraPosition.z;
	output.materialInfo = float4(materialID, 1.0, 1.0, 1.0);

	return  output;
}