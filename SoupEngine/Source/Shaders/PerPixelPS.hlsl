cbuffer LightCBuf
{
    float3 lightPos;
    float3 materialColor;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity = 1.0f;
    float attConst = 1.0f;
    float attLin = 0.0045f;
    float attQuad = 0.0075f;
}

float4 main(float3 worldPos : Position, float3 normal : Normal) : SV_TARGET
{
	//fragment to light vector data
    const float3 vectorToLight = lightPos - worldPos;
    const float distanceToLight = length(vectorToLight);
    const float3 directionToLight = vectorToLight / distanceToLight;

	//diffuse attenuation
    const float attenuation = 1.0f / (attConst + attLin * distanceToLight + attQuad * (distanceToLight * distanceToLight));

	//diffuse intensity
    const float diffuse = diffuseColor * diffuseIntensity * attenuation * max(0.0f, dot(directionToLight, normal));

	//final colour
    return float4(saturate((diffuse + ambient) * materialColor), 1.0f);
}