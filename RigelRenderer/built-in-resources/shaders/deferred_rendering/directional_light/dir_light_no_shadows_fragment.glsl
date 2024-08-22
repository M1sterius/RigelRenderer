#version 460

struct DirectionaLight
{
    vec3 color;
    float intensity;

    vec3 direction;
};

const float AMBIENT_INTENSITY = 0.15;

uniform DirectionaLight u_DirectionalLight;

uniform sampler2D g_Position;
uniform sampler2D g_Normal;
uniform sampler2D g_AlbedoSpec;

uniform vec3 u_ViewPos;
uniform vec2 u_ScreenSize;

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

void main()
{
    vec2 texCoords = gl_FragCoord.xy / u_ScreenSize;

    vec3 FragPos = texture(g_Position, texCoords).rgb;
    vec3 Normal = texture(g_Normal, texCoords).rgb;
    vec3 Diffuse = texture(g_AlbedoSpec, texCoords).rgb;
    float Specular = texture(g_AlbedoSpec, texCoords).a;

    vec3 viewDir = normalize(u_ViewPos - FragPos);

    vec3 light = CalcLight(Normal, viewDir, Diffuse, Specular);

    vec3 color = vec3(0.0);
    color += Diffuse * AMBIENT_INTENSITY;

    color += light;

    FragColor = vec4(color, 1.0);
}
