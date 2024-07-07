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

out vec4 FragColor;

const float near = 0.1f;
const float far = 20.0f;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 diffuse, float specular)
{
    vec3 lightDir = normalize(-light.direction);

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

    const uint MAPS_PER_ROW = 4;
    const uint MAPS_PER_COL = 4;
    const float MAP_OFFSET_UV = 0.25;

    vec2 atlasOffset = vec2((index % MAPS_PER_ROW) * MAP_OFFSET_UV, (index / MAPS_PER_COL) * MAP_OFFSET_UV);

    vec2 shadowMapUV = projCoords.xy * MAP_OFFSET_UV + atlasOffset;

    float currentDepth = projCoords.z;
    float closestDepth = texture(u_DirLightsShadowAtlas, shadowMapUV).r;

    float bias = max(0.05 * (1.0 - dot(normal, u_DirectionalLights[index].direction)), 0.005);

    float shadow = currentDepth - bias > closestDepth ? 0.0 : 1.0;

    return shadow;
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

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

float CalcSpotLightShadow(vec4 fragPosLightSpace, uint index, vec3 normal)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0)
        return 1.0;

    const uint MAPS_PER_ROW = 8;
    const uint MAPS_PER_COL = 8;
    const float MAP_OFFSET_UV = 0.125;

    vec2 atlasOffset = vec2((index % MAPS_PER_ROW) * MAP_OFFSET_UV, (index / MAPS_PER_COL) * MAP_OFFSET_UV);

    vec2 shadowMapUV = projCoords.xy * MAP_OFFSET_UV + atlasOffset;

    float currentDepth = projCoords.z;
    float closestDepth = texture(u_SpotLightsShadowAtlas, shadowMapUV).r;

    float bias = max(0.00025 * (1.0 - dot(normal, u_SpotLights[index].direction)), 0.000005);

    float shadow = currentDepth - bias > closestDepth ? 0.0 : 1.0;

    return shadow;
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

    FragColor = vec4(color, 1.0);
}
