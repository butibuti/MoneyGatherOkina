#include"DefaultShader.hlsli"

float2 PSMain(Pixel_ShadowMapping_UV pixel) : SV_TARGET{
	clip(mainTexture.Sample(mainSampler, pixel.uv).a * color.a-0.01);
	return float2(pixel.depth.x, pixel.depth.y);
}