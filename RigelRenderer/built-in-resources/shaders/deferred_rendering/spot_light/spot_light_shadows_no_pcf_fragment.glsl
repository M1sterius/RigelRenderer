#version 460

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

    mat4 lightSpaceViewProj;
    vec2 atlasOffset;
};

uniform SpotLight u_SpotLight;

uniform sampler2D g_Position;
uniform sampler2D g_Normal;
uniform sampler2D g_AlbedoSpec;
uniform sampler2D u_DepthMapAtlas;

uniform vec3 u_ViewPos;
uniform vec2 u_ScreenSize;
uniform float u_OneShadowMapOffset;

out vec4 FragColor;

vec3 CalcLight(vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuse, float specular)
{
    vec3 lightDir = normalize(u_SpotLight.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
    float dist = length(u_SpotLight.position - fragPos);

    float attenuation = 1.0 / (u_SpotLight.constant + u_SpotLight.linear * dist + u_SpotLight.quadratic * (dist * dist));

    float theta = dot(lightDir, normalize(-u_SpotLight.direction));
    float epsilon = u_SpotLight.cutOff - u_SpotLight.outerCutOff;
    float intensity = clamp((theta - u_SpotLight.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 diffuseRes = u_SpotLight.color * u_SpotLight.intensity * diff * diffuse;
    vec3 specularRes = u_SpotLight.color * u_SpotLight.intensity * spec * specular;

    diffuseRes *= attenuation * intensity;
    specularRes *= attenuation * intensity;

    return diffuseRes + specularRes;
}

float CalcShadow(vec4 fragPosLightSpace, vec3 normal)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0)
    return 1.0;

    vec2 shadowMapUV = projCoords.xy * u_OneShadowMapOffset + u_SpotLight.atlasOffset;

    float currentDepth = projCoords.z;
    float bias = max(0.005 * (1.0 - dot(normal, normalize(u_SpotLight.direction))), 0.001);

    float closestDepth = texture(u_DepthMapAtlas, shadowMapUV).r;
    float shadow = currentDepth - bias > closestDepth ? 0.0 : 1.0;

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

    vec3 light = CalcLight(Normal, FragPos, viewDir, Diffuse, Specular);

    vec4 fragPosLightSpace = u_SpotLight.lightSpaceViewProj * vec4(FragPos, 1.0);

    float shadow = CalcShadow(fragPosLightSpace, Normal);

    vec3 color = vec3(0.0);

    color += (light * shadow);

    FragColor = vec4(color, 1.0);
}
