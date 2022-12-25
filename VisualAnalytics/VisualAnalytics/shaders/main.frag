#version 450

layout (location = 0) in vec3 Normal;
layout (location = 1) in vec3 FragPosition;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform vec4 lightPos;
uniform float shininess;

uniform sampler2D mask;

void main() {

    vec4 mask_texel = texelFetch(mask, ivec2(gl_FragCoord.xy), 0);
    vec3 light_vector = lightPos.xyz - FragPosition * lightPos.w;
    
    vec3 L = normalize(light_vector);
    vec3 N = normalize(Normal);
    vec3 E = normalize(cameraPos - FragPosition);
    vec3 H = normalize(L + E);

    float NdotL = max(dot(N, L), 0.0);
    float NdotH = max(dot(N, H), 0.0001);

    vec3 color = vec3(NdotL) + pow(NdotH, shininess);
    
    if (lightPos.w == 1.0) {
        color /= (dot(light_vector, light_vector));
    }

    color = color / (color + 1.0);
    color = pow(color, vec3(1.0 / 2.2));

    for (int u = -2; u < 2; u++)
    {
        for (int v = -2; v < 2; v++)
        {
            if (texelFetch(mask, ivec2(gl_FragCoord.x + u, gl_FragCoord.y + v), 0).x == 0.0f)
            {
                color = vec3(0.0f);
                break;
            }
        }
    }
    
    FragColor = vec4(color, 1.0);
}