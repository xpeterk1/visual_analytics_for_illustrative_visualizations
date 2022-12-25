#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aMinCurvature;
layout (location = 2) in float aMaxCurvature;
layout (location = 3) in float aViewCurvature;

layout (location = 0) out float fs_min_curvature;
layout (location = 1) out float fs_max_curvature;
layout (location = 2) out float fs_view_curvature;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    fs_min_curvature = aMinCurvature;
    fs_max_curvature = aMaxCurvature;
    fs_view_curvature = aViewCurvature;
}