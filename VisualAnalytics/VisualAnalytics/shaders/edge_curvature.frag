#version 450

uniform usampler2D mask;
layout (location = 0) out ivec2 FragColor;

void main() {    
    uvec4 color = texelFetch(mask, ivec2(gl_FragCoord.xy), 0);
    ivec2 outputColor = ivec2(0);

    if (color.x != 0)
    {
        uvec4 top = texelFetch(mask, ivec2(gl_FragCoord.x, gl_FragCoord.y + 1), 0);
        if (top.x != 0)
        {
            outputColor.r += 2; // => 2/4 pi
            outputColor.g += 1;
        }

        uvec4 topright = texelFetch(mask, ivec2(gl_FragCoord.x + 1, gl_FragCoord.y + 1), 0);
        if (topright.x != 0)
        {
            outputColor.r += 1; // => 1/4 pi
            outputColor.g += 1;
        }

        uvec4 right = texelFetch(mask, ivec2(gl_FragCoord.x + 1, gl_FragCoord.y), 0);
        if (right. x != 0)
        {
            outputColor.g += 1; // +0 to the red channel
        }

        uvec4 bottomRight = texelFetch(mask, ivec2(gl_FragCoord.x + 1, gl_FragCoord.y - 1), 0);
        if (bottomRight.x != 0)
        {
            outputColor.r += 1; // => -1/4 pi
            outputColor.g += 1;
        }
        
        uvec4 bottom = texelFetch(mask, ivec2(gl_FragCoord.x, gl_FragCoord.y - 1), 0);
        if (bottom.x != 0)
        {
            outputColor.r += 2; // => -2/4 pi
            outputColor.g += 1;
        }
    }

    FragColor = outputColor;
}