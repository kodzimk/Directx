
Texture2D objTexture : TEXTURE : register(t0);
SamplerState samplerState : SAMPLER : register(s0);


float4 main(float2 TexCoord : TEXCOORD,float Alpha : ALPHA) : SV_Target
{
    float3 pixelColor = objTexture.Sample(samplerState, TexCoord);
    return float4(1.0f,0.0f,0.0f, 1.0f);
}