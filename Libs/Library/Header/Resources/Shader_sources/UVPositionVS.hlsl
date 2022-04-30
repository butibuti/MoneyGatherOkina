#include"DefaultShader.hlsli"

Pixel_UV_VertexPosition VSMain(Vertex_UV_Normal vertex)
{
	Pixel_UV_VertexPosition output;
	output.position = mul(vertex.position, mvpMatrix);
	output.vertexPosition = vertex.position;

	output.uv = vertex.uv;
	return output;
}