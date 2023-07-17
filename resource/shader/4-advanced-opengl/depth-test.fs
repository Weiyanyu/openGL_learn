#version 460 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture1;
uniform float     near;
uniform float     far;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    FragColor = texture(texture1, TexCoords);
    // FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    // float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
    // FragColor   = vec4(vec3(depth), 1.0);
}