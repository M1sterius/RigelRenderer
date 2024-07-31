#version 440

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_SpecularTexture;

in vec2 v_TexCoords;
in vec3 v_Normal;
in vec3 v_FragPos;

void main()
{
    gPosition = v_FragPos;
    gNormal = normalize(v_Normal);
    gAlbedoSpec.rgb = texture(u_DiffuseTexture, v_TexCoords).rgb;
    gAlbedoSpec.a = texture(u_SpecularTexture, v_TexCoords).r;
}
