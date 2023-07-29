#version 460 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    sampler2D ambient;

    float shininess;
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

uniform DirLight    dirLight;
uniform PointLight  pointLight;
uniform SpotLight   spotLight;
uniform Material    material1;
uniform vec3        cameraPos;
uniform vec3        Position;
uniform samplerCube skybox;
uniform float       refectTextureShitness;

uniform vec3 viewPos;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 ambient = light.ambient * vec3(texture(material1.diffuse, TexCoords));

    vec3  lightDir = normalize(-light.direction);
    float diff     = max(dot(normal, lightDir), 0.0);
    vec3  diffuse  = light.diffuse * (diff * vec3(texture(material1.diffuse, TexCoords)));

    vec3  refectDir = reflect(-lightDir, normal);
    float spec      = pow(max(dot(viewDir, refectDir), 0.0), material1.shininess);
    vec3  specular  = light.specular * (spec * vec3(texture(material1.specular, TexCoords)));

    vec3 result = (ambient + diffuse + specular);
    return result;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos)
{
    vec3 ambient = light.ambient * vec3(texture(material1.diffuse, TexCoords));

    vec3  lightDir = normalize(light.position - FragPos);
    float diff     = max(dot(normal, lightDir), 0.0);
    vec3  diffuse  = light.diffuse * (diff * vec3(texture(material1.diffuse, TexCoords)));

    vec3  refectDir = reflect(-lightDir, normal);
    float spec      = pow(max(dot(viewDir, refectDir), 0.0), material1.shininess);
    vec3  specular  = light.specular * (spec * vec3(texture(material1.specular, TexCoords)));

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
    float spec       = pow(max(dot(viewDir, reflectDir), 0.0), material1.shininess);
    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient  = light.ambient * vec3(texture(material1.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(material1.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material1.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main()
{
    // vec3 normal  = normalize(Normal);
    // vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 result;
    // result += calcDirLight(dirLight, normal, viewDir);
    // result += calcPointLight(pointLight, normal, viewDir, FragPos);
    // result += calcSpotLight(spotLight, normal, viewDir, FragPos);

    // FragColor = vec4(result, 1.0);

    vec3 viewDir = normalize(cameraPos - Position);
    vec3 normal  = normalize(Normal);

    vec3 R          = reflect(-viewDir, normal);
    vec3 reflectMap = vec3(texture(material1.ambient, TexCoords));
    vec3 reflection = vec3(texture(skybox, R).rgb) * reflectMap;

    float diff    = max(normalize(dot(normal, viewDir)), 0.0f);
    vec3  diffuse = diff * vec3(texture(material1.diffuse, TexCoords));

    FragColor = vec4(diffuse + (refectTextureShitness * reflection), 1.0f);

    // FragColor = vec4(10 * reflection, 1.0);
    // FragColor = texture(material1.specular, TexCoords);
}