#include"DefaultShader.hlsli"

[maxvertexcount(3)]
void GSMain(
    triangle Vertex_UV_Normal input[3],
    inout TriangleStream<Pixel_UV> stream)
{
        
        for (int j = 0; j<3; j++) {
            Pixel_UV outlinePixel;
            float dis = length(input[j].position.xyz - lightDir.xyz);

            int stepped = step(dis, 3.0);

            outlinePixel.position = input[j].position + float4(input[j].normal, 0)*(3.0-dis) * lightDir.w*stepped;
            outlinePixel.uv = input[j].uv;
            outlinePixel.position = mul(outlinePixel.position, viewMatrix);
            outlinePixel.position = mul(outlinePixel.position, projectionMatrix);
            stream.Append(outlinePixel);
        }
}