#include"DefaultShader.hlsli"

Pixel_UV_Normal_Shadow VSMain(Vertex_UV_Normal_QuadBone vertex)
{
	Pixel_UV_Normal_Shadow output;
	matrix bm = vertex.weight_01 * bones[vertex.boneIndex_1] + vertex.weight_02 * bones[vertex.boneIndex_2] + vertex.weight_03 * bones[vertex.boneIndex_3] + vertex.weight_04 * bones[vertex.boneIndex_4];

	output.position = mul(bm, vertex.position);

	output.position = mul(output.position, mvpMatrix);
	output.normal = mul(vertex.normal, (float3x3)bm);
	output.normal = mul(output.normal, (float3x3)modelMatrix);
	output.uv = vertex.uv;


	float4 Pos = mul(vertex.position, modelMatrix);
	Pos = mul(Pos, shadowvpMatrix);
	Pos.xyz = Pos.xyz / Pos.w;
	output.shadowPos.x = (1.0f + Pos.x) / 2.0f;
	output.shadowPos.y = (1.0f - Pos.y) / 2.0f;
	output.shadowPos.z = Pos.z;
	return output;
}