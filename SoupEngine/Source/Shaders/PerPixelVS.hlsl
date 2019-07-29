cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProjection;
};

 struct VSOut
 {
     float3 PosRelCam : Position; //Position relative to the camera
     float3 normal : Normal;
     float4 pos : SV_Position;
 };

VSOut main(  float3 pos : POSITION, float3 normal : Normal)
{
    VSOut vso;
    vso.PosRelCam = (float3) mul(float4(pos, 1.0f), modelView);
    vso.normal = mul(normal, (float3x3) modelView);
    vso.pos = mul(float4(pos, 1.0f), modelViewProjection);
    return vso;
}