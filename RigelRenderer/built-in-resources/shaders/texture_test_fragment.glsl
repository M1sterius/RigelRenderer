#version 440

in vec2 v_TexCoords;

uniform sampler2D u_Texture;

out vec4 FragColor;

const float near = 0.1f;
const float far = 20.0f;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    float depth = texture(u_Texture, v_TexCoords).r;
    //FragColor = vec4(vec3(depth), 1.0); // orthographic
    FragColor = vec4(vec3(LinearizeDepth(depth) / far), 1.0); // perspective
}
