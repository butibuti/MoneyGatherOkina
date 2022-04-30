#include"DefaultShader.hlsli"

Pixel_UV_Normal_Phong_Shadow VSMain(Vertex_UV_Normal vertex)
{
	Pixel_UV_Normal_Phong_Shadow output;
	output.position = mul(vertex.position, mvpMatrix);
	output.normal = normalize(mul(vertex.normal, (float3x3)modelMatrix));

	output.uv = vertex.uv;
	output.modelPosition = mul(vertex.position, modelMatrix);
	output.cameraPosition = mul(vertex.position, modelMatrix);
	output.cameraPosition = mul(output.cameraPosition, viewMatrix);


	float4 Pos = mul(vertex.position, modelMatrix);
	output.shadowPos.z = length(Pos.xyz - shadowCameraPos) * 0.01f;
	Pos = mul(Pos, shadowvpMatrix);
	Pos.xy = Pos.xy / Pos.w;
	output.shadowPos.x = (1.0f + Pos.x) *0.5f;
	output.shadowPos.y = (1.0f - Pos.y) *0.5f;
	return output;
}