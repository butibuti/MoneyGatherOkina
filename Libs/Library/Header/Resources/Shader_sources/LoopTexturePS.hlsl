#include"DefaultShader.hlsli"


float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
    pixel.uv.x *= modelMatrix[0][0];

    float4 Tex = (mainTexture.Sample(mainSampler, pixel.uv).rgba);


    return Tex;
}