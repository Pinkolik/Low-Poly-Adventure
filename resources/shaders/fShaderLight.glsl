#version 330 core
out vec4 fragColor;

uniform vec3 aColor;

void main()
{
    fragColor = vec4(aColor, 1.0f);
}