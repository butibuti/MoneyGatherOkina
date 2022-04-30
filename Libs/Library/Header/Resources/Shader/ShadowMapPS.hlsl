#include"DefaultShader.hlsli"

float2 PSMain(Pixel_ShadowMapping pixel) : SV_TARGET{	
	return float2(pixel.depth.x, pixel.depth.y);
}