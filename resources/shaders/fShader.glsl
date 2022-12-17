#version 330 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D tex;
uniform bool debug;

vec4 limitColors(vec4 color) {
    vec3 color3 = floor(color.xyz * 255 / 16) / 16;
    return vec4(color3, color.w);
}

void main() {
    if (!debug) {
        vec4 color = texture(tex, texCoord);
        fragColor = limitColors(color);
        //        fragColor = texture(tex, texCoord);
    } else {
        fragColor = vec4(0, 1, 0.5, 0.2);
    }
}

