#version 460

uniform sampler2D u_Texture;

in vec2 v_TexCoords;

out vec4 FragColor;

void main()
{
    vec3 color = texture(u_Texture, v_TexCoords).rgb;
    FragColor = vec4(color, 1.0);
}
