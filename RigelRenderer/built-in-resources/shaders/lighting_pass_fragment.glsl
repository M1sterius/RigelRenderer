#version 440

struct DirectionalLight
{
    vec3 color;
    float intensity;

    vec3 direction;

    bool smoothShadows;
    mat4 lightSpaceViewProj;
};

struct SpotLight {
    vec3 color;
    float intensity;

    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    bool smoothShadows;
    mat4 lightSpaceViewProj;
};

struct PointLight {
    vec3 color;
    float intensity;

    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

in vec2 v_TexCoords;

uniform vec3 u_ViewPos;

uniform sampler2D g_Position;
uniform sampler2D g_Normal;
uniform sampler2D g_AlbedoSpec;

uniform sampler2D u_DirLightsShadowAtlas;
uniform sampler2D u_SpotLightsShadowAtlas;

uniform DirectionalLight u_DirectionalLights[16];
uniform uint u_DirLightsCount;

uniform SpotLight u_SpotLights[32];
uniform uint u_SpotLightsCount;

uniform PointLight u_PointLights[64];
uniform uint u_PointLightsCount;

out vec4 FragColor;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 diffuse, float specular)
{
    vec3 lightDir = -normalize(light.direction);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuseRes = diff * light.color * light.intensity * diffuse;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specularRes = spec * light.color * light.intensity * specular;

    return diffuseRes + specularRes;
}

float CalcDirLightShadow(vec4 fragPosLightSpace, uint index, vec3 normal)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0)
        return 1.0;

    const uint MAPS_PER_AXIS = 3;
    const float MAP_OFFSET_UV = 0.33333;

    vec2 atlasOffset = vec2((index % MAPS_PER_AXIS) * MAP_OFFSET_UV, (index / MAPS_PER_AXIS) * MAP_OFFSET_UV);

    vec2 shadowMapUV = projCoords.xy * MAP_OFFSET_UV + atlasOffset;

    float currentDepth = projCoords.z;
    float bias = max(0.005 * (1.0 - dot(normal, normalize(u_DirectionalLights[index].direction))), 0.001);

    // Naive approach to switching bewteen smooth and hard shadows, requires rewrite later
    if (u_DirectionalLights[index].smoothShadows)
    {
        vec2 shadowMapSize = textureSize(u_DirLightsShadowAtlas, 0);
        vec2 texelSize = 1 / shadowMapSize;

        float shadow = 0.0;
        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                vec2 sampleOffset = vec2(x, y) * texelSize;
                float closestDepth = texture(u_DirLightsShadowAtlas, shadowMapUV + sampleOffset).r;
                shadow += currentDepth - bias > closestDepth ? 0.0 : 1.0;
            }
        }
        shadow /= 9;

        return shadow;
    }
    else
    {
        float closestDepth = texture(u_DirLightsShadowAtlas, shadowMapUV).r;
        float shadow = currentDepth - bias > closestDepth ? 0.0 : 1.0;
        return shadow;
    }
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuse, float specular)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
    float dist = length(light.position - fragPos);

    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 diffuseRes = light.color * light.intensity * diff * diffuse;
    vec3 specularRes = light.color * light.intensity * spec * specular;

    diffuseRes *= attenuation * intensity;
    specularRes *= attenuation * intensity;

    return diffuseRes + specularRes;
}

float CalcSpotLightShadow(vec4 fragPosLightSpace, uint index, vec3 normal)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0)
        return 1.0;

    const uint MAPS_PER_AXIS = 8;
    const float MAP_OFFSET_UV = 0.125;

    vec2 atlasOffset = vec2((index % MAPS_PER_AXIS) * MAP_OFFSET_UV, (index / MAPS_PER_AXIS) * MAP_OFFSET_UV);

    vec2 shadowMapUV = projCoords.xy * MAP_OFFSET_UV + atlasOffset;

    float currentDepth = projCoords.z;
    float bias = max(0.00025 * (1.0 - dot(normal, u_SpotLights[index].direction)), 0.000005);

    // Naive approach to switching bewteen smooth and hard shadows, requires rewrite later
    if (u_SpotLights[index].smoothShadows)
    {
        vec2 shadowMapSize = textureSize(u_SpotLightsShadowAtlas, 0);
        vec2 texelSize = 1 / shadowMapSize;

        float shadow = 0.0;
        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                vec2 sampleOffset = vec2(x, y) * texelSize;
                float closestDepth = texture(u_SpotLightsShadowAtlas, shadowMapUV + sampleOffset).r;
                shadow += currentDepth - bias > closestDepth ? 0.0 : 1.0;
            }
        }
        shadow /= 9;

        return shadow;
    }
    else
    {
        float closestDepth = texture(u_SpotLightsShadowAtlas, shadowMapUV).r;
        float shadow = currentDepth - bias > closestDepth ? 0.0 : 1.0;
        return shadow;
    }
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuse, float specular)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    vec3 diffuseRes = light.color * light.intensity * diff * diffuse;
    vec3 specularRes = light.color * light.intensity * spec * specular;
    diffuseRes *= attenuation;
    specularRes *= attenuation;

    return diffuseRes + specularRes;
}

float CalcPointLightShadow(vec4 fragPosLightSpace, uint index, vec3 normal)
{
    // To be implemented :)
    return 1.0f;
}

void main()
{
    vec3 FragPos = texture(g_Position, v_TexCoords).rgb;
    vec3 Normal = texture(g_Normal, v_TexCoords).rgb;
    vec3 Diffuse = texture(g_AlbedoSpec, v_TexCoords).rgb;
    float Specular = texture(g_AlbedoSpec, v_TexCoords).a;

    vec3 viewDir = normalize(u_ViewPos - FragPos);

    vec3 color = vec3(0.0);
    color += Diffuse * 0.15;

    for (uint i = 0; i < u_DirLightsCount; i++)
    {
        DirectionalLight dirLight = u_DirectionalLights[i];

        vec4 fragPosLightSpace = dirLight.lightSpaceViewProj * vec4(FragPos, 1.0);
        float shadow = CalcDirLightShadow(fragPosLightSpace, i, Normal);

        color += CalcDirLight(dirLight, Normal, viewDir, Diffuse, Specular) * shadow;
    }

    for (uint i = 0; i < u_SpotLightsCount; i++)
    {
        SpotLight spotLight = u_SpotLights[i];

        vec4 fragPosLightSpace = spotLight.lightSpaceViewProj * vec4(FragPos, 1.0);
        float shadow = CalcSpotLightShadow(fragPosLightSpace, i, Normal);

        color += CalcSpotLight(spotLight, Normal, FragPos, viewDir, Diffuse, Specular) * shadow;
    }

    for (uint i = 0; i < u_PointLightsCount; i++)
    {
        PointLight pointLight = u_PointLights[i];

        color += CalcPointLight(pointLight, Normal, FragPos, viewDir, Diffuse, Specular);
    }

    FragColor = vec4(color, 1.0);
}
