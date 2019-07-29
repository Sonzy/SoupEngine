//Per frame
cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity = 1.0f;
    float attConst = 1.0f;
    float attLin = 0.0045f;
    float attQuad = 0.0075f;
}

//Per Object
cbuffer ObjectCBuf
{
    float3 materialColour;
    float specularIntensity;
    float specularPower;
};

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

    //reflected light vector
    const float3 w = normal * dot(vectorToLight, normal);
    const float3 r = w * 2.0f - vectorToLight;

    //Calculate the intensity based on angle between viewing and normal
    const float3 specular = attenuation * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), specularPower);

	//final colour
    return float4(saturate((diffuse + ambient + specular) * materialColour), 1.0f);
}