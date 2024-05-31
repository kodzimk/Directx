struct VSOut
{
    float2 TexCoord : TEXCOORD;
    float4 position : SV_Position;
};

cbuffer CBuf
{
     matrix transform;
};

VSOut main(float3 pos : Pos , float2 TexCoord : TEXCOORD)
{
    VSOut vs;
    vs.TexCoord = TexCoord;
    vs.position = mul(float4(pos, 1.0f), transform);
    return vs;
}