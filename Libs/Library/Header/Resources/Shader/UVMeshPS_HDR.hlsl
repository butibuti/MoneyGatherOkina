#include"DefaultShader.hlsli"

PixelOutput_HDR PSMain(Pixel_UV pixel) 
{
	PixelOutput_HDR output;
	output.color = (mainTexture.Sample(mainSampler,pixel.uv))* color;

	return output;
}
