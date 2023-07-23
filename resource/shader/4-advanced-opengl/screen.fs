#version 460 core

in vec2  TexCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

vec2 offsets[9] = vec2[](vec2(-offset, offset), // 左上
                         vec2(0.0f, offset), // 正上
                         vec2(offset, offset), // 右上
                         vec2(-offset, 0.0f), // 左
                         vec2(0.0f, 0.0f), // 中
                         vec2(offset, 0.0f), // 右
                         vec2(-offset, -offset), // 左下
                         vec2(0.0f, -offset), // 正下
                         vec2(offset, -offset) // 右下
);

// clang-format off
float sharpenkernel[9] = float[](
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
);
float blurkernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);

float edgeDetectionKernal[9] = float[](
    1, 1, 1,
    1, -8, 1,
    1, 1, 1
);
// clang-format on

vec3 doKernal(vec2 inTexCoords, float kernal[9])
{
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, inTexCoords.st + offsets[i]));
    }
    vec3 color = vec3(0.0);
    for(int i = 0; i < 9; i++)
    {
        color += sampleTex[i] * kernal[i];
    }
    return color;
}

vec3 inversion(vec3 inColor)
{
    return (1.0 - inColor);
}

vec3 grayscale(vec3 inColor)
{
    float average = (inColor.x + inColor.y + inColor.z) / 3.0;
    return vec3(average, average, average);
}

vec3 sharpen(vec2 inTexCoords)
{
    return doKernal(inTexCoords, sharpenkernel);
}

vec3 blur(vec2 inTexCoords)
{
    return doKernal(inTexCoords, blurkernel);
}

vec3 edgeDetection(vec2 inTexCoords)
{
    return doKernal(inTexCoords, edgeDetectionKernal);
}

void main()
{
    vec3 color = texture(screenTexture, TexCoords).rgb;
    // color      = inversion(color);
    // color = grayscale(color);
    // color = sharpen(TexCoords);
    // color = blur(TexCoords);
    color = edgeDetection(TexCoords);

    FragColor = vec4(color, 1.0);
}