cbuffer CBuf
{
	float4 face_colors[6];
};

float4 main( uint tid : SV_PrimitiveID) : SV_TARGET
{
	return face_colors[tid/2];
	//return float4(1.0f,1.0f,0.0f,1.0f);
}