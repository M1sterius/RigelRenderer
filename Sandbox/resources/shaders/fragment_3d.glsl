#version 440

in vec2 v_TexCoords;

uniform vec3 u_Color;
uniform sampler2D u_Texture;

out vec4 outColor;

void main()
{   
    vec4 texColor = texture(u_Texture, v_TexCoords);
    outColor = texColor;
}
