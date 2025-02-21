#!/bin/bash
echo [pwd]

echo $(dirname "$0")


glslangValidator -V $(dirname "$0")/shader_base.vert -o $(dirname "$0")/vertex_shader.spv
glslangValidator -V $(dirname "$0")/shader_base.frag -o $(dirname "$0")/fragment_shader.spv


