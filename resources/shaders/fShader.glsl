#version 330 core
out vec4 fragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform sampler2D tex;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 color;
uniform bool hasColor;

void main()
{
    vec4 aColor;
    if (hasColor) {
        aColor = vec4(color, 1.0);
    } else {
        aColor = texture(tex, texCoord);
    }

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    vec3 lightColor = vec3(1.0f);
    float ambientStrength = 0.1;
    float specularStrength = 0.5;

    vec3 ambient = ambientStrength * lightColor;
    vec3 diffuse = diff * lightColor;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * lightColor;  
    vec3 result = (ambient + diffuse + specular) * vec3(aColor.xyz);

    fragColor = vec4(result, 1.0f);
}