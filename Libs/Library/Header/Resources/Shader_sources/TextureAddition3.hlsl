#include"DefaultShader.hlsli"

float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
    float4 output = mainTexture.Sample(mainSampler, pixel.uv);
    float4 bloomSource = subTexture_2.Sample(mainSampler, pixel.uv);
    output = output  + bloomSource+subTexture_1.Sample(mainSampler, pixel.uv);
    return output;
}