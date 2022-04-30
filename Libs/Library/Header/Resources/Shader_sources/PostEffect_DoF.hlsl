#include"DefaultShader.hlsli"

float4 PSMain(Pixel_UV pixel) :SV_TARGET
{
    float4 source = mainTexture.Sample(mainSampler, pixel.uv);
    float4 blured = subTexture_1.Sample(mainSampler, pixel.uv);
    float4 depthSource = subTexture_2.Sample(mainSampler, pixel.uv);
    float d =  depthSource.w;
    float depthRate = step(d, ExInfo.x) * -(d - ExInfo.x) / (ExInfo.x) + step(ExInfo.y, d) * (d - ExInfo.y) / (ExInfo.z - ExInfo.y);
    float4 output = source * (1.0-depthRate)+blured*(depthRate);

    return output;
}