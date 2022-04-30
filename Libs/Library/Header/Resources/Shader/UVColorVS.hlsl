#include"DefaultShader.hlsli"

Pixel_UV_Color VSMain(Vertex_UV_Color vertex)
{
	Pixel_UV_Color output;
	output.position = mul(vertex.position, mvpMatrix);
	output.uv = vertex.uv;
	output.color = vertex.color;




	return output;
}
