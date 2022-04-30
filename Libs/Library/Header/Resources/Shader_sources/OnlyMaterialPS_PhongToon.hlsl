#include"DefaultShader.hlsli"
float4 PSMain(Pixel_UV_Normal_Phong pixel) : SV_TARGET
{
    float3 l_lightdir = normalize((lightDir- pixel.modelPosition).xyz);
    float3 lightdir = normalize(lightDir.xyz);
    float3  eyeDirection =normalize (cameraPos-pixel.modelPosition ).xyz;
    float3  halfLE = normalize(l_lightdir+eyeDirection );
    float l_specular =pow(max( dot( halfLE, pixel.normal),0.0f),2) ;


    float3 n = normalize(pixel.normal);

    float light = dot(n, lightdir);

    float s =  smoothstep(lightDir.a-0.1f, lightDir.a,(light));
    s += 0.3 * (1.0 - s);

    float specularStep = smoothstep(specular.a-0.0125f, specular.a,l_specular);

    float4 Light = saturate(s * diffuse + emissive+ specular*specularStep );

    float4  destColor = Light ;
    return  float4(destColor.xyz,1.0f);
}