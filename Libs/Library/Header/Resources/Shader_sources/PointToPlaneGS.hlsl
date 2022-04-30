#include"DefaultShader.hlsli"

static const float4 array_Offset[6] = {
    float4(0.09f,0.16f,0.0,0),
    float4(0.09f,-0.16f,0.0,0),
    float4(-0.09f,-0.16f,0.0,0),

    float4(0.09f,0.16f,0.0,0),
    float4(-0.09f,-0.16f,0.0,0),
    float4(-0.09f,0.16f,0.0,0),
};
static const float2 array_UV[6]={
    float2(1.0,0.0),
    float2(1.0,1.0),
    float2(0.0,1.0),
    float2(1.0,0.0),
    float2(0.0,1.0),
    float2(0.0,0.0),
};

[maxvertexcount(18)]
void GSMain(
    triangle Vertex_UV_Normal_Color input[3],
    inout TriangleStream<Pixel_UV_Color> stream)
{


   

    Pixel_UV_Color pixel;
    for (uint j = 0; j < 3; j++) {
        float4 position = input[j].position;
        pixel.color = input[j].color;
        float scale = input[j].uv.x;
        for (uint i = 0; i < 6; i += 3) {

            pixel.position = position + array_Offset[i] * scale;
            pixel.uv = array_UV[i];
            stream.Append(pixel);
            pixel.position = position + array_Offset[i + 1] * scale;
            pixel.uv = array_UV[i + 1];
            stream.Append(pixel);
            pixel.position = position + array_Offset[i + 2] * scale;
            pixel.uv = array_UV[i + 2];
            stream.Append(pixel);

            stream.RestartStrip();
        }
    }



}