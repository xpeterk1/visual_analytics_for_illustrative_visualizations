#version 450

uniform usampler2D mask;
layout (location = 0) out uint FragColor;

void main() {   
    ivec2 fc = ivec2(gl_FragCoord.xy);
    uvec4 color = texelFetch(mask, fc, 0);

    FragColor = 0;
    if (color.x == 0.0)
    {
        uvec4 top = texelFetch(mask, fc - ivec2(0, 1), 0);
        uvec4 bottom = texelFetch(mask, fc + ivec2(0, 1), 0);
        uvec4 left = texelFetch(mask, fc + ivec2(1, 0), 0);
        uvec4 right = texelFetch(mask, fc - ivec2(1, 0), 0);
        
        if (top.x != 0 || bottom.x != 0 || left.x != 0 || right.x != 0)
            FragColor = 1;
    }
}
