#version 460

layout (location = 0 ) out vec4 outColor;


layout(push_constant) uniform Push{
    vec2 offset;
    vec3 color;
    mat2 transform;
} push;


void main() {
outColor = vec4(push.color, 1.0f);
}