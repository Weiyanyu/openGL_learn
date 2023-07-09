#version 460 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 viewPos;

struct Material
{
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light    light;

void main()
{
    float ambientStrength = 0.1;
    vec3  ambient         = material.ambient * light.ambient;

    vec3  norm     = normalize(Normal);
    vec3  lightDir = normalize(light.position - FragPos);
    float diff     = max(dot(norm, lightDir), 0.0);
    vec3  diffuse  = light.diffuse * (diff * material.diffuse);

    vec3  viewDir   = normalize(viewPos - FragPos);
    vec3  refectDir = reflect(-lightDir, norm);
    float spec      = pow(max(dot(viewDir, refectDir), 0.0), material.shininess);
    vec3  specular  = light.specular * (spec * material.specular);

    vec3 result = (ambient + diffuse + specular);

    FragColor = vec4(result, 1.0);
}