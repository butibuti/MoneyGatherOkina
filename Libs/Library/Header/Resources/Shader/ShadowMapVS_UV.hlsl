#include"DefaultShader.hlsli"

Pixel_ShadowMapping_UV VSMain(Vertex_UV vertex)
{
	Pixel_ShadowMapping_UV output;
	output.position = mul(vertex.position, mvpMatrix);
	output.depth.x = length(mul(vertex.position, modelMatrix) - cameraPos) * 0.01f;
	output.depth.y = output.depth.x * output.depth.x;
	output.uv = vertex.uv;
	return output;
}