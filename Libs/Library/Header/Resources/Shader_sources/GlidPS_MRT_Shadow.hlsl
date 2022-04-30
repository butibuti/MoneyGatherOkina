#include"DefaultShader.hlsli"


PixelOutput_Default PSMain(Pixel_UV_Normal_Phong_Shadow pixel)
{

    PixelOutput_Default output;

    float divisions = 1*modelMatrix[0][0];
    float thickness = 0.8/modelMatrix[0][0];
	float glidColor = (0.2f);


	pixel.uv*=divisions;
   
    pixel.uv.x = frac(pixel.uv.x );
    pixel.uv.x =min(pixel.uv.x, 1.0 - pixel.uv.x);
    float delta = fwidth(pixel.uv.x);


	float glidX= smoothstep(pixel.uv.x- delta, pixel.uv.x+ delta,thickness);

    pixel.uv.y = frac(pixel.uv.y);
    pixel.uv.y = min(pixel.uv.y, 1.0 - pixel.uv.y);
    delta = fwidth(pixel.uv.y);


	float glidY= smoothstep(pixel.uv.y- delta, pixel.uv.y+ delta,thickness);




	glidColor +=0.5*clamp( glidX+glidY,0.0f,1.0f);

	output.normal = float4((pixel.normal + float3(1.0, 1.0, 1.0)) * 0.5, 1.0);
	float4 c = color*float4(glidColor, glidColor, glidColor, 1.0f);

	float4 shadowColor = c;
	shadowColor.xyz *= ExInfo.x;
	output.baseColor = GetVSMColor(c, shadowColor, float2Texture_1, mainSampler, pixel.shadowPos) ;
	output.worldPosition = (pixel.modelPosition + float4(1.0, 1.0, 1.0, 1.0)) * 0.5;
	output.worldPosition.w = pixel.cameraPosition.z;

	output.materialInfo = float4(materialID, 1.0, 1.0, 1.0);


	return output;
}