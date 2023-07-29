#version 460 core

in vec2 TexCoords;
in vec3 Position;
in vec3 Normal;

out vec4 FragColor;

uniform samplerCube skybox;
uniform float       near;
uniform float       far;
uniform vec3        cameraPos;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));

    vec4 texColor = vec4(texture(skybox, R).rgb, 1.0);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
    // FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    // float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
    // FragColor   = vec4(vec3(depth), 1.0);
}