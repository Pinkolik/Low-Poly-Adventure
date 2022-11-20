#version 330 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D texture_diffuse1;
uniform bool debug;

void main()
{   if (!debug) {
        fragColor = texture(texture_diffuse1, texCoord);
    } else {
        fragColor = vec4(0, 1, 0.5, 0.2);
    }
}