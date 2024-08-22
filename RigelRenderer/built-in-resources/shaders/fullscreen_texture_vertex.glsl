#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 a_TexCoords;

out vec2 v_TexCoords;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    v_TexCoords = a_TexCoords;
}
