#version 450

layout (location = 0) flat in int triangleID;
out vec3 FragColor;

uniform mat4 projection;

vec3 screenSpaceToWorldSpace(vec3 coord, mat4 invProjection) {
    vec4 clip = vec4(coord * 2.0 - 1.0, 1.0);
    vec4 view = invProjection * clip;
    return view.xyz / view.w;
}

void main() {
    vec2 invTexSize = vec2(1.0 / 800.0, 1.0 / 600.0);
    vec3 coords = vec3(gl_FragCoord.xy * invTexSize, gl_FragCoord.z);

    FragColor = screenSpaceToWorldSpace(coords, inverse(projection));
}

