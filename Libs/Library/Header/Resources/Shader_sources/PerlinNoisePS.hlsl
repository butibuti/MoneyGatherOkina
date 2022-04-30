#include"DefaultShader.hlsli"

float2 WhiteNoiseVec(float2 coord) {
    float2 angle =float2( dot(coord, float2(1.9898, 8.233)), dot(coord, float2(0.2838, 4.3993)));
    return frac(sin(angle) * 438777.425547998) * 2 - 1;
}

float PerlinNoise(float density,float2 coord) {
    float2 floored = floor(coord * density * float2(1920, 1080));
    float2 fracted = frac(coord * density * float2(1920, 1080));

    float2 v_lt = WhiteNoiseVec(floored + float2(0, 0));
    float2 v_lb = WhiteNoiseVec(floored + float2(0, 1));
    float2 v_rt = WhiteNoiseVec(floored + float2(1, 0));
    float2 v_rb = WhiteNoiseVec(floored + float2(1, 1));

    float c_lt = dot(v_lt, fracted - float2(0, 0));
    float c_lb = dot(v_lb, fracted - float2(0, 1));
    float c_rt = dot(v_rt, fracted - float2(1, 0));
    float c_rb = dot(v_rb, fracted - float2(1, 1));


    float2 u = fracted * fracted * (3 - 2 * fracted);

    float l_top = lerp(c_lt, c_rt, u.x);
    float l_bottom = lerp(c_lb, c_rb,u.x);

    return lerp(l_top, l_bottom, u.y)*0.5+0.5;

}

float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
    float output = PerlinNoise(ExInfo.x, pixel.uv)*0.5;
    output += PerlinNoise(ExInfo.x*2, pixel.uv) * 0.25; 
    output += PerlinNoise(ExInfo.x*4, pixel.uv) * 0.125; 
    output += PerlinNoise(ExInfo.x*8, pixel.uv) * 0.0625;

    return float4(output,output,output,1.0f);
}