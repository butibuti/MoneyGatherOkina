#include"DefaultShader.hlsli"

Pixel_ShadowMapping VSMain(Vertex_UV_Normal_QuadBone vertex)
{
	Pixel_ShadowMapping output;
	matrix bm = vertex.weight_01 * bones[vertex.boneIndex_1] + vertex.weight_02 * bones[vertex.boneIndex_2] + vertex.weight_03 * bones[vertex.boneIndex_3] + vertex.weight_04 * bones[vertex.boneIndex_4];

	output.position = mul(bm, vertex.position);
	output.depth.x = length(mul(output.position, modelMatrix) - cameraPos) * 0.01f;
	output.depth.y = output.depth.x * output.depth.x;
	output.position = mul(output.position, mvpMatrix);
	return output;
}