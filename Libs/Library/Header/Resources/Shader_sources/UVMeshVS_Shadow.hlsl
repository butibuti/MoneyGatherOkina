#include"DefaultShader.hlsli"

Pixel_UV_Shadow VSMain(Vertex_UV vertex)
{
	Pixel_UV_Shadow output;
	output.position = mul(vertex.position, mvpMatrix);
	output.uv = vertex.uv;


	float4 Pos = mul(vertex.position, modelMatrix);
	Pos = mul(Pos, shadowvpMatrix);
	Pos.xyz = Pos.xyz / Pos.w;
	output.shadowPos.x = (1.0f + Pos.x) / 2.0f;
	output.shadowPos.y = (1.0f - Pos.y) / 2.0f;
	output.shadowPos.z = Pos.z;
	return output;
}
