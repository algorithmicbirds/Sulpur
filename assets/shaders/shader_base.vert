#version 460

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(push_constant) uniform Push{
    vec2 offset;
    vec3 color;
    mat2 transform;
} push;


void main() {
gl_Position = vec4(inPosition + push.offset ,0.0, 1.0);
}