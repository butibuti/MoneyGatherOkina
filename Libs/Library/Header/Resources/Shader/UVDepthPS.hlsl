#include"DefaultShader.hlsli"

float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
	float Tex =step (1.0f,depthTexture.Sample(mainSampler,pixel.uv));
	return float4( Tex, Tex, Tex, 1.0f);
}
