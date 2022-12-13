#version 330 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D texture_diffuse1;
uniform bool debug;
uniform bool intersectionDetected;

void main()
{   if (!debug) {
        if (!intersectionDetected) {
            fragColor = texture(texture_diffuse1, texCoord);
        } else {
            fragColor = mix(texture(texture_diffuse1, texCoord), vec4(1.0, 0.0, 0.0, 1.0), 0.5f);
        }
    } else {
        fragColor = vec4(0, 1, 0.5, 0.2);
    }
}