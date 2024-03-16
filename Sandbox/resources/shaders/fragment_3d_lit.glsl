#version 440

// Light structures ------------------------------
struct DirectionalLight {
    vec3 color;
    float intensity;

    vec3 direction;
};
struct PointLight {
    vec3 color;
    float intensity;

    vec3 position;

    float constant;
    float linear;
    float quadratic;
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
};
// -----------------------------------------------

// Custom material struct that must match with similar material class on the CPU side
struct Material {
    vec3 diffuse;
    float shininess;
    sampler2D specular;
};

in vec2 v_TexCoords;
in vec3 v_Normal;
in vec3 v_FragPos;

// Uniforms set in material class on the CPU side
uniform Material u_Material;
uniform sampler2D u_Texture;
// -----------------------------------------------

uniform vec3 u_Ambient;
uniform vec3 u_ViewPos;

uniform DirectionalLight u_DirectionalLights[32];
uniform PointLight u_PointLights[32];
uniform SpotLight u_SpotLights[32];
uniform uint u_DirectionalLightsCount;
uniform uint u_PointLightsCount;
uniform uint u_SpotLightsCount;

const float ambient = 0.1f;

out vec4 outColor;

// Phong directional light calculations
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) 
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), u_Material.shininess);

    vec3 diffuse = light.color * light.intensity * diff * u_Material.diffuse;
    vec3 specular = light.color * light.intensity * spec * vec3(texture(u_Material.specular, v_TexCoords));

    return (diffuse + specular);
}

// Phong point light calculations
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) 
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), u_Material.shininess);

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    vec3 diffuse = light.color * light.intensity * diff * u_Material.diffuse;
    vec3 specular = light.color * light.intensity * spec * vec3(texture(u_Material.specular, v_TexCoords));
    diffuse *= attenuation;
    specular *= attenuation;

    return (diffuse + specular);
}

// Phong spot light calculations
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) 
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), u_Material.shininess);
    float dist = length(light.position - fragPos);

    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 diffuse = light.color * light.intensity * diff * u_Material.diffuse;
    vec3 specular = light.color * light.intensity * spec * vec3(texture(u_Material.specular, v_TexCoords));

    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (diffuse + specular);
}

void main()
{   
    vec3 normal = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);

    vec3 color = vec3(0.0);

    color += ambient * u_Material.diffuse;

    for (uint i = 0; i < u_DirectionalLightsCount; i++) {
        color += CalcDirLight(u_DirectionalLights[i], normal, viewDir);
    }

    for (uint i = 0; i < u_PointLightsCount; i++) {
        color += CalcPointLight(u_PointLights[i], normal, v_FragPos, viewDir);
    }

    for (uint i = 0; i < u_SpotLightsCount; i++) {
        color += CalcSpotLight(u_SpotLights[i], normal, v_FragPos, viewDir);
    }

    outColor = texture(u_Texture, v_TexCoords) * vec4(color, 1.0);
}
