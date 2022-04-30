#include"DefaultShader.hlsli"
float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
    
    float4 base = mainTexture.Sample(mainSampler, pixel.uv);
    float3 normal = subTexture_1.Sample(mainSampler, pixel.uv).xyz*2-float3(1.0,1.0,1.0);
    float3 worldPos = subTexture_2.Sample(mainSampler, pixel.uv).xyz*2 - float3(1.0, 1.0, 1.0);
    float4 materialInfo = subTexture_3.Sample(mainSampler, pixel.uv);
    

    float3  eyeDirection = normalize(cameraPos.xyz - worldPos);
    float3  halfLE = normalize(lightDir.xyz + eyeDirection);
    float t = max(0.0f, dot( lightDir.xyz, normal) );
    float specularPower = pow(max(dot(halfLE, normal), 0.0f), 20);

    float4 Light = saturate(t * Material_DeferredList[materialInfo.x].diffuse + Material_DeferredList[materialInfo.x].emissive);

    Light.rgb += Material_DeferredList[materialInfo.x].specular.rgb * specularPower;

    float4 destColor = base;
    destColor.xyz *= Light.rgb;
    return destColor;
}