#include"DefaultShader.hlsli"

float2 PSMain(Pixel_UV pixel) : SV_TARGET
{
    float2 output = 0;

    for (float i = 0.0f; i <= 15.0f;i+=1.0f) {
        output += gausOffset[i].z * float2Texture.Sample(mainSampler, pixel.uv + gausOffset[i].xy  );
    }
    return output;
}