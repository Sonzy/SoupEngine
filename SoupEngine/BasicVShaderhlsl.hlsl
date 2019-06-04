struct VSOut
{
	float3 color : Color;
	float4 pos : SV_Position;
};

cbuffer CBuf //Dont need to do cbuf.tranform
{
	row_major matrix transform;
};

VSOut main( float2 pos : Position, float3 color : Color )
{
	VSOut vso;
	vso.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform); //Using mul, it right hand multiplies to matrix has to go on the right
	vso.color = color;
	return vso;
}