#include"DefaultShader.hlsli"

Pixel_UV_Normal_Phong_Tangent_Shadow VSMain(Vertex_UV_Normal vertex)
{
	Pixel_UV_Normal_Phong_Tangent_Shadow output;
	output.position = mul(vertex.position, mvpMatrix);
	output.normal = vertex.normal;
	output.tangent = normalize( cross(float3(0,1.0f,0), output.normal));
	output.binormal=- normalize( (cross(output.tangent, output.normal)));
	output.modelPosition = mul(vertex.position, modelMatrix);
	output.cameraPosition = mul(vertex.position, modelMatrix);
	output.cameraPosition= mul(output.cameraPosition, viewMatrix);
	output.uv = vertex.uv;
	float4 Pos = mul(vertex.position, modelMatrix);
	output.shadowPos.z = length(Pos.xyz - shadowCameraPos) * 0.01f;
	Pos = mul(Pos, shadowvpMatrix);
	Pos.xy = Pos.xy / Pos.w;
	output.shadowPos.x = (1.0f + Pos.x) * 0.5f;
	output.shadowPos.y = (1.0f - Pos.y) * 0.5f;
	return output;
}