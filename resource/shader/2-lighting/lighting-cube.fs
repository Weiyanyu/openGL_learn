#version 460 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float     shininess;
};

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3  position;
    vec3  direction;
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

uniform Material   material;
uniform DirLight   dirLight;
uniform PointLight pointLight;
uniform SpotLight  spotLight;

uniform float matrixLight;
uniform float matrixMove;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3  lightDir = normalize(-light.direction);
    float diff     = max(dot(normal, lightDir), 0.0);
    vec3  diffuse  = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));

    vec3  refectDir = reflect(-lightDir, normal);
    float spec      = pow(max(dot(viewDir, refectDir), 0.0), material.shininess);
    vec3  specular  = light.specular * (spec * vec3(texture(material.specular, TexCoords)));

    vec3 result = (ambient + diffuse + specular);
    return result;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos)
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3  lightDir = normalize(light.position - FragPos);
    float diff     = max(dot(normal, lightDir), 0.0);
    vec3  diffuse  = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));

    vec3  refectDir = reflect(-lightDir, normal);
    float spec      = pow(max(dot(viewDir, refectDir), 0.0), material.shininess);
    vec3  specular  = light.specular * (spec * vec3(texture(material.specular, TexCoords)));

    float _distance   = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * _distance + light.quadratic * (_distance * _distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = (ambient + diffuse + specular);
    return result;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos)
{
    vec3 lightDir = normalize(light.position - FragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3  reflectDir = reflect(-lightDir, normal);
    float spec       = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 normal  = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result;
    // result += calcDirLight(dirLight, normal, viewDir);
    // result += calcPointLight(pointLight, normal, viewDir, FragPos);
    result += calcSpotLight(spotLight, normal, viewDir, FragPos);

    FragColor = vec4(result, 1.0);
}