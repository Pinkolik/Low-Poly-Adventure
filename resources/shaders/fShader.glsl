#version 330 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D tex;
uniform vec3 color;
uniform bool hasColor;

void main()
{
    if (hasColor) {
        vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColor;
        vec3 result = ambient * color;

        fragColor = vec4(result, 1.0f);
    } else {
        fragColor = texture(tex, texCoord);
    }
}