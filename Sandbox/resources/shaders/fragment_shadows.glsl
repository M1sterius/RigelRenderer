#version 440

// Light structures ------------------------------
struct DirectionalLight 
{
    vec3 color;
    float intensity;

    vec3 direction;
};

in vec2 v_TexCoords;
in vec3 v_Normal;
in vec3 v_FragPos;
in vec4 v_FragPosLightSpace;

uniform sampler2D u_Texture;

uniform sampler2D u_DepthMap;
uniform vec3 u_LightPos;

uniform vec3 u_ViewPos;

uniform DirectionalLight u_DirectionalLight;

out vec4 outColor;

const vec3 AMBIENT = vec3(0.1f);

float CalculateShadow(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = v_FragPosLightSpace.xyz / v_FragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_DepthMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main()
{   
    DirectionalLight dirLight = u_DirectionalLight;
    
    vec3 color = texture(u_Texture, v_TexCoords).rgb;
    vec3 normal = normalize(v_Normal);
    vec3 lightColor = vec3(1.0);

    vec3 ambient = 0.15 * dirLight.color;

    vec3 lightDir = normalize(u_LightPos - v_FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * dirLight.color;

    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;  

    float shadow = CalculateShadow(v_FragPosLightSpace);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color; 

    outColor = vec4(lighting, 1.0);
}
