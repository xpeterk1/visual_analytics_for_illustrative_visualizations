#version 450

layout (location = 0) in vec3 Normal;
layout (location = 1) in vec3 FragPosition;

out vec3 FragColor;

uniform sampler2D mask;

void main() {
    FragColor = normalize(Normal);
}