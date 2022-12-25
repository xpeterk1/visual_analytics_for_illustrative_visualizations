#version 450

layout (location = 0) in float MinCurvature;
layout (location = 1) in float MaxCurvature;
layout (location = 2) in float ViewCurvature;

out vec3 FragColor;

void main() {
    FragColor = vec3(MinCurvature, MaxCurvature, ViewCurvature);
}