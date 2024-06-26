#version 440

struct DirectionalLight
{
    vec3 color;
    float intensity;

    vec3 direction;

    mat4 lightSpaceViewProj;
};

in vec2 v_TexCoords;

uniform vec3 u_ViewPos;

uniform sampler2D g_Position;
uniform sampler2D g_Normal;
uniform sampler2D g_AlbedoSpec;

uniform sampler2D u_DirLightsShadowAtlas;

uniform DirectionalLight u_DirectionalLights[16];
uniform uint u_DirLightsCount;

out vec4 FragColor;

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

float CalcDirLightShadow(vec4 fragPosLightSpace, uint index, float bias)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    uint pos_x = (index % 4);
    uint pos_y = (index / 4);

    vec2 atlasOffset = vec2(pos_x * 0.25, pos_y * 0.25);

    vec2 shadowMapUV = projCoords.xy * 0.25 + atlasOffset;

    float currentDepth = projCoords.z;
    float closestDepth = texture(u_DirLightsShadowAtlas, shadowMapUV).r;

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
        float bias = max(0.05 * (1.0 - dot(Normal, dirLight.direction)), 0.005);
        float shadow = CalcDirLightShadow(fragPosLightSpace, i, bias);

        color += CalcDirLight(dirLight, Normal, viewDir, Diffuse, Specular) * shadow;
    }

    FragColor = vec4(color, 1.0);
}
