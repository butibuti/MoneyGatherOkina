#include"DefaultShader.hlsli"

Pixel_UV_Normal_Color_Shadow VSMain(Vertex_UV_Normal_Color vertex)
{
	Pixel_UV_Normal_Color_Shadow output;
	output.position = mul(vertex.position, modelMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.uv = vertex.uv;
	output.normal = mul(vertex.normal, (float3x3)modelMatrix);
	output.color = vertex.color;
	output.color.a = 0;


	float4 Pos = mul(vertex.position, modelMatrix);
	Pos = mul(Pos, shadowvpMatrix);
	Pos.xyz = Pos.xyz / Pos.w;
	output.shadowPos.x = (1.0f + Pos.x) / 2.0f;
	output.shadowPos.y = (1.0f - Pos.y) / 2.0f;
	output.shadowPos.z = Pos.z;
	return output;
}
