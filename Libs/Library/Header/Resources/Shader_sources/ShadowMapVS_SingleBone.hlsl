#include"DefaultShader.hlsli"

Pixel_ShadowMapping VSMain(Vertex_UV_Normal_SingleBone vertex)
{
	Pixel_ShadowMapping output;
	matrix bm = bones[vertex.bone];
	output.position = mul(bm, vertex.position);
	output.depth.x = length(mul(output.position, modelMatrix) - cameraPos) * 0.01f;
	output.depth.y = output.depth.x * output.depth.x;
	output.position = mul(output.position, mvpMatrix);
	return output;
}