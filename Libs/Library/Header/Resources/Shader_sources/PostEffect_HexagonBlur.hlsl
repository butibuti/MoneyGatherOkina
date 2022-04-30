#include"DefaultShader.hlsli"

float4 PSMain(Pixel_UV pixel) :SV_TARGET
{
    float4 output;
    float2 uvOffset = float2(0.86602f * pixelScale.x, -0.5f * pixelScale.y) * ExInfo.x * 0.25f;
    output= mainTexture.Sample(mainSampler, pixel.uv+ uvOffset);
    output+= mainTexture.Sample(mainSampler, pixel.uv+ uvOffset*2);
    output+= mainTexture.Sample(mainSampler, pixel.uv+ uvOffset*3);
    output+= mainTexture.Sample(mainSampler, pixel.uv+ uvOffset*4);



    uvOffset = float2(-0.86602f * pixelScale.x, -0.5f * pixelScale.y) * ExInfo.x * 0.25f;
    output += subTexture_1.Sample(mainSampler, pixel.uv );
    output += subTexture_1.Sample(mainSampler, pixel.uv + uvOffset);
    output += subTexture_1.Sample(mainSampler, pixel.uv + uvOffset * 2);
    output += subTexture_1.Sample(mainSampler, pixel.uv + uvOffset * 3);
    output += subTexture_1.Sample(mainSampler, pixel.uv + uvOffset * 4);


    output/= 9.0;

    return output;
}