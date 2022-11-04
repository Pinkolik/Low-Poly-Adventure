#version 330 core
out vec4 fragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform sampler2D tex;
uniform vec3 lightPos;
uniform vec3 color;
uniform bool hasColor;

void main()
{
    if (hasColor) {
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(lightPos - fragPos);
        float diff = max(dot(norm, lightDir), 0.0);

        vec3 lightColor = vec3(1.0f);
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColor;
        vec3 diffuse = diff * lightColor;
        vec3 result = (ambient + diffuse) * color;

        fragColor = vec4(result, 1.0f);
    } else {
        fragColor = texture(tex, texCoord);
    }
}