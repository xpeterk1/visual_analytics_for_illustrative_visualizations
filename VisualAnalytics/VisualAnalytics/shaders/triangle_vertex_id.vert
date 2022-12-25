#version 450

layout (location = 0) in vec3 aPos;
layout (location = 0) out int triangleID;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    triangleID = gl_VertexID / 3 + 1; 
}
