#include"DefaultShader.hlsli"


float4 PSMain(Pixel_UV_Normal_VertexPosition pixel) : SV_TARGET
{
	float Color = (0.2f);
    float timeWave = abs(sin(Time));

    float3 lightdir = normalize(lightDir.xyz);
    float3 N1 = normalize(pixel.normal);
    float4 Light = saturate(dot(N1, -lightdir) * diffuse + emissive);
    Light.rgb += specular.rgb * specular.a;

    pixel.uv.x = frac(pixel.uv.x );
    pixel.uv.x =min(pixel.uv.x, 1.0 - pixel.uv.x);
    float delta = fwidth(pixel.uv.x);


    float gridX = smoothstep(pixel.uv.x - delta, pixel.uv.x + delta, ExInfo.w);

    pixel.uv.y = frac(pixel.uv.y);
    pixel.uv.y = min(pixel.uv.y, 1.0 - pixel.uv.y);
    delta = fwidth(pixel.uv.y);


    float gridY = gridX + smoothstep(pixel.uv.y - delta, pixel.uv.y + delta, ExInfo.w);

    pixel.vertexPosition.y = frac(pixel.vertexPosition.y);
    pixel.vertexPosition.y = min(pixel.vertexPosition.y, 1.0 - pixel.vertexPosition.y);
    delta = fwidth(pixel.vertexPosition.y);


    float grid = step(1.0,gridY+ smoothstep(pixel.vertexPosition.y - delta, pixel.vertexPosition.y + delta, ExInfo.w));



	Color+= (0.2+timeWave*0.6)*clamp(grid,0.0f,1.0f);
    
	return Light*float4(ExInfo.xyz,1.0)+color* Color*float4(grid * (frac(Time + pixel.vertexPosition.z + pixel.vertexPosition.y)), grid *(frac(Time + pixel.vertexPosition.x + pixel.vertexPosition.z)),grid * ( frac(Time + pixel.vertexPosition.x+ pixel.vertexPosition.y) ),1.0f);
}