#version 440

layout (location = 0) in vec3 a_Pos;

uniform mat4 u_MVP;

void main()
{
    gl_Position = vec4(a_Pos, 1.0);
}
