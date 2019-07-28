cbuffer CBuf
{
    matrix model;
    matrix modelViewProjection;
};

 struct VSOut
 {
     float3 worldPos : Position;
     float3 normal : Normal;
     float4 pos : SV_Position;
 };

VSOut main(  float3 pos : POSITION, float3 normal : Normal)
{
    VSOut vso;
    vso.worldPos = (float3) mul(float4(pos, 1.0f), model);
    vso.normal = mul(normal, (float3x3) model);
    vso.pos = mul(float4(pos, 1.0f), modelViewProjection);
    return vso;
}