#version 440

in vec2 v_TexCoords;

uniform sampler2D u_Texture;

out vec4 FragColor;

void main()
{
    float depth = texture(u_Texture, v_TexCoords).r;
    FragColor = vec4(vec3(depth), 1.0);
}
