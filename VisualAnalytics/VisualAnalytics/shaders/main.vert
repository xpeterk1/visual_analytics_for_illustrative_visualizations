#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

layout (location = 0) out vec3 fs_normal;
layout (location = 1) out vec3 fs_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    fs_position = vec3(model * vec4(aPos, 1.0));
    fs_normal = aNormal;
}