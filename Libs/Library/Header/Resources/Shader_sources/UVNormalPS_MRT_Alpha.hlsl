#include"DefaultShader.hlsli"

static const int detherPattern[4][4] = {
	{0,32,8,40},
	{48,16,56,24},
	{12,44,4,36},
	{60,28,52,20},
};

PixelOutput_Default PSMain(Pixel_UV_Normal_Phong pixel)
{
	PixelOutput_Default output;

	float4 baseColor = (mainTexture.Sample(mainSampler, pixel.uv))* color;
	int x = (int)fmod(pixel.position.x, 4.0);
	int y = (int)fmod(pixel.position.y,4.0);

	int dither = detherPattern[y][x];

	clip(dither - 64 *(1.0- baseColor.a));

	output.normal = float4((pixel.normal + float3(1.0, 1.0, 1.0)) * 0.5, 1.0);
	output.baseColor =float4( baseColor.xyz,1.0);
	output.worldPosition = (pixel.modelPosition + float4(1.0, 1.0, 1.0, 1.0)) * 0.5;
	output.worldPosition.w = pixel.cameraPosition.z;
	output.materialInfo = float4(materialID, 1.0, 1.0, 1.0);
	return  output;
}