#version 450

uniform usampler2D mask;
layout (location = 0) out int FragColor;

void main() {    
    
    vec4 color = texelFetch(mask, ivec2(gl_FragCoord.xy), 0);
    int num_neighbours = 0;

    if (color.x != 0)
    {
        vec4 top = texelFetch(mask, ivec2(gl_FragCoord.x - 1, gl_FragCoord.y), 0);
        vec4 bottom = texelFetch(mask, ivec2(gl_FragCoord.x + 1, gl_FragCoord.y), 0);
        vec4 left = texelFetch(mask, ivec2(gl_FragCoord.x, gl_FragCoord.y + 1), 0);
        vec4 right = texelFetch(mask, ivec2(gl_FragCoord.x, gl_FragCoord.y - 1), 0);

        if (top.x != 0) num_neighbours += 1;
        if (bottom.x != 0) num_neighbours += 1;
        if (left.x != 0) num_neighbours += 1;
        if (right.x != 0) num_neighbours += 1;
    }
    
    FragColor = num_neighbours;
}