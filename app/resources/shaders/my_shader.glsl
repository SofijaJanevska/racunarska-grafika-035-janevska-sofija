//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;

uniform vec3 viewPos;

uniform vec3 dirLightDir;
uniform vec3 dirLightColor;

uniform vec3 pointLightPos;
uniform vec3 pointLightColor;

uniform vec3 objectColor;

void main() {

    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(-dirLightDir);
    float diffDir = max(dot(norm, lightDir), 0.0);
    vec3 diffuseDir = diffDir * dirLightColor;
    vec3 ambientDir = 0.1 * dirLightColor;

    vec3 lightDirPoint = normalize(pointLightPos - FragPos);
    float diffPoint = max(dot(norm, lightDirPoint), 0.0);
    vec3 diffusePoint = diffPoint * pointLightColor;
    vec3 ambientPoint = 0.1 * pointLightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirPoint, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.5 * spec * (dirLightColor + pointLightColor);

    vec3 result = (ambientDir + diffuseDir +  ambientPoint + diffusePoint + specular) * texture(texture_diffuse1, TexCoords).rgb * objectColor;
    FragColor = vec4(result, 1.0);
}
