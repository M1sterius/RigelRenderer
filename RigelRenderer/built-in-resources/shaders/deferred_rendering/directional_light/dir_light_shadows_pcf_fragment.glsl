#version 460

struct DirectionaLight
{
    vec3 color;
    float intensity;

    vec3 direction;

    mat4 lightSpaceViewProj;
    vec2 atlasOffset;
};

const float AMBIENT_INTENSITY = 0.15;

uniform DirectionaLight u_DirectionalLight;

uniform sampler2D g_Position;
uniform sampler2D g_Normal;
uniform sampler2D g_AlbedoSpec;
uniform sampler2D u_DepthMapAtlas;

uniform vec3 u_ViewPos;
uniform vec2 u_ScreenSize;
uniform float u_OneShadowMapOffset;

out vec4 FragColor;

vec3 CalcLight(vec3 normal, vec3 viewDir, vec3 diffuse, float specular)
{
    vec3 lightDir = -normalize(u_DirectionalLight.direction);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuseRes = diff * u_DirectionalLight.color * u_DirectionalLight.intensity * diffuse;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specularRes = spec * u_DirectionalLight.color * u_DirectionalLight.intensity * specular;

    return diffuseRes + specularRes;
}

float CalcShadow(vec4 fragPosLightSpace, vec3 normal)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0)
    return 1.0;

    vec2 shadowMapUV = projCoords.xy * u_OneShadowMapOffset + u_DirectionalLight.atlasOffset;

    float currentDepth = projCoords.z;
    float bias = max(0.005 * (1.0 - dot(normal, normalize(u_DirectionalLight.direction))), 0.001);

    vec2 texelSize = 1 / vec2(textureSize(u_DepthMapAtlas, 0));

    float shadow = 0.0;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            vec2 sampleOffset = vec2(x, y) * texelSize;
            float closestDepth = texture(u_DepthMapAtlas, shadowMapUV + sampleOffset).r;
            shadow += currentDepth - bias > closestDepth ? 0.0 : 1.0;
        }
    }
    shadow /= 9;

    return shadow;
}

void main()
{
    vec2 texCoords = gl_FragCoord.xy / u_ScreenSize;

    vec3 FragPos = texture(g_Position, texCoords).rgb;
    vec3 Normal = texture(g_Normal, texCoords).rgb;
    vec3 Diffuse = texture(g_AlbedoSpec, texCoords).rgb;
    float Specular = texture(g_AlbedoSpec, texCoords).a;

    vec3 viewDir = normalize(u_ViewPos - FragPos);

    vec3 light = CalcLight(Normal, viewDir, Diffuse, Specular);

    vec4 fragPosLightSpace = u_DirectionalLight.lightSpaceViewProj * vec4(FragPos, 1.0);

    float shadow = CalcShadow(fragPosLightSpace, Normal);

    vec3 color = vec3(0.0);
    color += Diffuse * AMBIENT_INTENSITY;

    color += (light * shadow);

    FragColor = vec4(color, 1.0);
}
