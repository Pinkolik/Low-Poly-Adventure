#version 330 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D texture_diffuse1;
uniform vec3 debugColor;

void main()
{   
    if (debugColor == vec3(0)) {
    fragColor = texture(texture_diffuse1, texCoord);
    } else {
        fragColor = mix(vec4(debugColor, 1.0), texture(texture_diffuse1, texCoord), 0.5);
    }
}