#include"DefaultShader.hlsli"
float4 PSMain(Pixel_UV_Normal pixel) : SV_TARGET
{

    float3 lightdir = normalize(lightDir.xyz);
    


    float3 n = normalize(pixel.normal);

    float light = dot(n, lightdir);

    float s = step((light), lightDir.a);
    



    float4 Light = saturate(s * diffuse + (1-s)* emissive  );


    return  float4(Light.rgb,1.0f);
}