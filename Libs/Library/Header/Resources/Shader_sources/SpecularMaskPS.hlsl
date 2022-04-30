#include"DefaultShader.hlsli"
float4 PSMain(Pixel_UV_Normal_Phong pixel) : SV_TARGET
{
    float3 l_lightdir = normalize((lightDir - pixel.modelPosition).xyz);
    float3 lightdir = normalize(lightDir.xyz);
    float3  eyeDirection = normalize(cameraPos - pixel.modelPosition).xyz;
    float3  halfLE = normalize(l_lightdir + eyeDirection);
    float l_specular = pow(max(dot(halfLE, pixel.normal),0.0f),5)* mainTexture.Sample(mainSampler, pixel.uv* Tiling).r;



    float3 n = normalize(pixel.normal);

    float light = dot(n, lightdir);



    float4  destColor = saturate(light * diffuse + emissive + specular * l_specular);
    return  float4(destColor.xyz,1.0f);
}