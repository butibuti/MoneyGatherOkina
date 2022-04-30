#include"DefaultShader.hlsli"
float4 PSMain(Pixel_UV_VertexPosition pixel) : SV_TARGET
{
	float2 uv = mul(pixel.vertexPosition.xyz,QuaternionToMatrix(ExInfo)).xy;
	uv.y += Time;
	float4 Tex = (mainTexture.Sample(mainSampler,uv));
	return Tex;
}