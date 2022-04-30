#include"DefaultShader.hlsli"
PixelOutput_BloomSource PSMain(Pixel_UV_Normal_Phong pixel)
{ 
	PixelOutput_BloomSource output;
	output.baseColor = emissive;
	output.worldPosition = (pixel.modelPosition + float4(1.0, 1.0, 1.0, 1.0)) * 0.5;
	output.worldPosition.w = pixel.cameraPosition.z;
	return  output;

}