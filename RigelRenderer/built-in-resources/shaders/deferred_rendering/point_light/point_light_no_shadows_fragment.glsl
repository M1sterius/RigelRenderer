#version 460

struct PointLight {
    vec3 color;
    float intensity;

    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

uniform PointLight u_PointLight;

uniform sampler2D g_Position;
uniform sampler2D g_Normal;
uniform sampler2D g_AlbedoSpec;

uniform vec3 u_ViewPos;
uniform vec2 u_ScreenSize;

out vec4 FragColor;

vec3 CalcLight(vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuse, float specular)
{
    vec3 lightDir = normalize(u_PointLight.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);

    float dist = length(u_PointLight.position - fragPos);
    float attenuation = 1.0 / (u_PointLight.constant + u_PointLight.linear * dist + u_PointLight.quadratic * (dist * dist));

    vec3 diffuseRes = u_PointLight.color * u_PointLight.intensity * diff * diffuse;
    vec3 specularRes = u_PointLight.color * u_PointLight.intensity * spec * specular;
    diffuseRes *= attenuation;
    specularRes *= attenuation;

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

    vec3 light = CalcLight(Normal, FragPos, viewDir, Diffuse, Specular);

    vec3 color = vec3(0.0);

    color += light;

    FragColor = vec4(color, 1.0);
}
