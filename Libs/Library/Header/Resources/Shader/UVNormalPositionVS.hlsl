#include"DefaultShader.hlsli"

Pixel_UV_Normal_VertexPosition VSMain(Vertex_UV_Normal vertex)
{
	Pixel_UV_Normal_VertexPosition output;
	output.position = mul(vertex.position, mvpMatrix);
	output.vertexPosition = mul(vertex.position, modelMatrix);
	output.normal = vertex.normal;
	output.uv = output.vertexPosition.xz;
	return output;
}