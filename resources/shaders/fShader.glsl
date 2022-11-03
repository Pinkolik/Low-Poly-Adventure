#version 330 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D tex;
uniform vec3 color;
uniform bool hasColor;

void main()
{
    if (hasColor) {
        fragColor = vec4(color, 1.0f);
    } else {
        fragColor = texture(tex, texCoord);
    }
}