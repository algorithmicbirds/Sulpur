#version 460

vec2 positions[3] = vec2 [] (
vec2(1.0f, 1.0f),
vec2(2.0f, 3.0f),
vec2(3.0f,3.0f)
);

void main() {
gl_Position = vec4(positions[gl_VertexIndex], 0.0f, 1.0f);
}