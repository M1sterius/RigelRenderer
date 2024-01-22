#version 440

in vec2 v_TexCoords;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

out vec4 outColor;

void main()
{
    outColor = texture(u_Texture, v_TexCoords) + u_Color;
}
