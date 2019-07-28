cbuffer LightCBuf
{
    float3 lightPos;
}

static const float3 materialColor = { 0.7f, 0.7f, 0.9f};
static const float3 ambient = { 0.05f, 0.05f, 0.05f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float diffuseIntensity = 1.0f;
static const float attConst = 1.0f;
static const float attLin = 0.0045f;
static const float attQuad = 0.0075f;

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
    return float4(saturate(diffuse + ambient), 1.0f);
}