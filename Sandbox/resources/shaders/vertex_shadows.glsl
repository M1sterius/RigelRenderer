#version 440

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoords;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat3 u_NormalMatrix;
uniform mat4 u_LightSpaceMatrix;

out vec2 v_TexCoords;
out vec3 v_Normal;
out vec3 v_FragPos;
out vec4 v_FragPosLightSpace;

void main()
{
    v_TexCoords = a_TexCoords;
    v_FragPos = vec3(u_Model * vec4(a_Pos, 1.0));
    v_Normal = a_Normal * u_NormalMatrix;

    v_FragPosLightSpace = vec4(v_FragPos, 1.0) * u_LightSpaceMatrix;

    gl_Position = vec4(a_Pos, 1.0) * u_MVP;
}