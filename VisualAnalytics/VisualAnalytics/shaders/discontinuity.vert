#version 450

void main() {
    
    gl_Position.x = (gl_VertexID == 2) ?  3.0 : -1.0;
    gl_Position.y = (gl_VertexID == 1) ? -3.0 :  1.0;
    gl_Position.zw = vec2(0.5,1.0);
}
