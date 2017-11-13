#define GLOBAL_LIGHT_COLOR float3(0.1, 0.1, 0.3)

struct DirectionalLight
{
    float4 position;
    float3 color;
    float3 ambient;
};

struct Light
{
    float3 position;
    float  range;
    float3 color;
    float  intensity;
};
cbuffer LightBuffer : register(b2) {
    uint4 LightCount;
    Light Lights[128];
};

//Returns the shadow amount of a given position
float calcShadowFactor(SamplerComparisonState comparisonSampler, Texture2D shadowMap, DirectionalLight light, int sampleCount)
{
    light.position.x = (light.position.x * 0.5f) + 0.5f;
    light.position.y = (light.position.y * -0.5f) + 0.5f;

    float addedShadow = 0;

    [unroll]
    for (int y = -sampleCount; y <= sampleCount; y += 1)
    {
        [unroll]
        for (int x = -sampleCount; x <= sampleCount; x += 1)
        {
            addedShadow += shadowMap.SampleCmp(comparisonSampler, light.position.xy, light.position.z, int2(x, y)).r;
        }
    }

    float shadow = addedShadow / pow(sampleCount * 2 + 1, 2);

    return shadow;
}

//This is because i waas bored, it is not something we will use maybe i think
float3 toonify(float3 color, float intensity)
{
    if (intensity > 0.95)
        color = color; //Yes.
    else if (intensity > 0.5)
        color = 0.7 * color;
    else if (intensity > 0.05)
        color = 0.35 * color;
    else
        color = 0.1 * color;

    return color;
}

float3 calcNormal(float3 mappedNormal, float3 normal, float3 binormal, float3 tangent)
{
    // TODO Remove when everything is working
    if (mappedNormal.x == 0 && mappedNormal.y == 0 && mappedNormal.z == 0)
        return normal;
    if (mappedNormal.x == 1 && mappedNormal.y == 1 && mappedNormal.z == 1)
        return normal;
    ////

    //To make sure the tangent is perpendicular
    tangent = normalize(tangent - dot(tangent, mappedNormal) * mappedNormal);

    float3x3 tangentMatrix = float3x3(tangent, binormal, normal);
    mappedNormal = normalize(mappedNormal * 2.0 - 1);

    return normalize(mul(mappedNormal, tangentMatrix));
}

float3 calcLight(DirectionalLight light, float3 position, float3 normal, float3 viewDir, float specularExponent)
{
    float3 lightDir = normalize(light.position.xyz);
    float3 halfway = normalize(lightDir + viewDir);

    float diffuesFactor = saturate(dot(normal, lightDir));
    float specularFactor = saturate(pow(dot(normal, halfway), specularExponent));

    return diffuesFactor * light.color
        + specularFactor * light.color
        + light.ambient;
}

float3 calcLight(Light light, float3 position, float3 normal, float3 viewDir, float specularExponent)
{
    float3 lightDir = light.position.xyz - position.xyz;
    float distance = length(lightDir);
    lightDir = lightDir / distance;

    float3 halfway = normalize(lightDir + viewDir);
    float attenuation = 1.0f - smoothstep(0, light.range, distance);

    float diffuesFactor = saturate(dot(normal, lightDir));
    float specularFactor = saturate(pow(dot(normal, halfway), specularExponent));

    return diffuesFactor * light.color * attenuation * light.intensity
        + specularFactor * light.color * attenuation * light.intensity;
}

float3 calcAllLights(float4 ndcPosition, float3 position, float3 normal, float3 viewDir, float specularExponent)
{
    float3 lightSum = float3(0, 0, 0);

    for (uint i = 0; i < LightCount.x; i++) {
        lightSum += calcLight(Lights[i], position, normal, viewDir, specularExponent);
    }

/*
    uint2 tile = uint2(floor(ndcPosition.xy / BLOCK_SIZE));
    uint offset = lightGrid[tile].x;
    uint count = lightGrid[tile].y;
    for (uint i = 0; i < count; i++)
    {
        lightSum += calcLight(lights[lightIndexList[offset + i]], position, normal, viewDir, specularExponent);
    }
*/
    return lightSum;
}
