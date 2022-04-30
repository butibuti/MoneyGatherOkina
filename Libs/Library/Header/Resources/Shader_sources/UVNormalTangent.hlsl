#include"DefaultShader.hlsli"

Pixel_UV_Normal_Phong_Tangent VSMain(Vertex_UV_Normal vertex)
{
	Pixel_UV_Normal_Phong_Tangent output;
	output.position = mul(vertex.position, mvpMatrix);
	output.normal = vertex.normal;
	output.tangent = normalize( cross(float3(0,1.0,0), output.normal));
	output.binormal=- normalize( (cross(output.tangent, output.normal)));
	output.modelPosition = mul(vertex.position, modelMatrix);
	output.cameraPosition = mul(vertex.position, modelMatrix);
	output.cameraPosition= mul(output.cameraPosition, viewMatrix);
	output.uv = vertex.uv;
	return output;
}