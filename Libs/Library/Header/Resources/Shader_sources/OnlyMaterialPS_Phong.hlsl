#include"DefaultShader.hlsli"
float4 PSMain(Pixel_UV_Normal_Phong pixel) : SV_TARGET
{
    float3 l_lightdir = normalize((lightDir- pixel.modelPosition).xyz);
    float3  eyeDirection =normalize (cameraPos-pixel.modelPosition ).xyz;
    float3  halfLE = normalize(l_lightdir+eyeDirection );
    float l_specular =pow(max( dot( halfLE, pixel.normal),0.0f),20) * 500000;

    float3 N1 = (pixel.normal);
    float4 Light = saturate(dot(N1, l_lightdir) * diffuse + emissive+ l_specular );

    float4  destColor = Light ;
    return  float4(destColor.xyz,1.0f);
}