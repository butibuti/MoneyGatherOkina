#include"DefaultShader.hlsli"
PixelOutput_Default PSMain(Pixel_UV_Normal_Phong pixel)
{
	PixelOutput_Default output;

	output.normal = float4((pixel.normal + float3(1.0, 1.0, 1.0)) * 0.5, 1.0);

	output.baseColor = color;
	output.worldPosition = (pixel.modelPosition + float4(1.0, 1.0, 1.0, 1.0)) * 0.5;
	output.worldPosition.w = pixel.cameraPosition.z;

	output.materialInfo = float4(materialID, 1.0, 1.0, 1.0);
	return  output;
}