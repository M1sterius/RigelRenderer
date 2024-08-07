#version 460

struct DirectionaLight
{
    vec3 color;
    float intensity;

    vec3 direction;

    bool smoothShadows;
    mat4 lightSpaceViewProj;
    vec2 atlasOffset;
};

uniform sampler2D g_Position;
uniform sampler2D g_Normal;
uniform sampler2D g_AlbedoSpec;
uniform sampler2D u_DepthMapAtlas;

uniform DirectionaLight u_DirectionalLight;

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0);
}
