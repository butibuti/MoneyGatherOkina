#include"DefaultShader.hlsli"

float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
    float4 output = mainTexture.Sample(mainSampler, pixel.uv);
    float filter= subTexture_1.Sample(mainSampler, pixel.uv).a;
    float stepped = step(0.7f, output.r * 0.299f + output.g * 0.587f + output.b * 0.114f)*filter;
    clip(stepped-0.1f);


    return output;
}