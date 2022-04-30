#include"DefaultShader.hlsli"

PixelOutput_HexaBlur PSMain(Pixel_UV pixel)
{
    PixelOutput_HexaBlur output;
    float2 uvOffset = float2(0, 1.0f * pixelScale.y) * ExInfo.x * 0.25f;
    output.verticalColor = mainTexture.Sample(mainSampler, pixel.uv+ uvOffset);
    output.verticalColor += mainTexture.Sample(mainSampler, pixel.uv+ uvOffset*2);
    output.verticalColor += mainTexture.Sample(mainSampler, pixel.uv+ uvOffset*3);
    output.verticalColor += mainTexture.Sample(mainSampler, pixel.uv+ uvOffset*4);

    output.verticalColor*= 0.25f;

    uvOffset = float2(0.86602f* pixelScale.x, -0.5f * pixelScale.y) * ExInfo.x * 0.25f;
    output.diagonalColor = mainTexture.Sample(mainSampler, pixel.uv );
    output.diagonalColor += mainTexture.Sample(mainSampler, pixel.uv + uvOffset);
    output.diagonalColor += mainTexture.Sample(mainSampler, pixel.uv + uvOffset * 2);
    output.diagonalColor += mainTexture.Sample(mainSampler, pixel.uv + uvOffset * 3);
    output.diagonalColor += mainTexture.Sample(mainSampler, pixel.uv + uvOffset * 4);

    output.diagonalColor*= 0.2f;

    output.diagonalColor += output.verticalColor;
    output.diagonalColor *= 0.5f;

    return output;
}