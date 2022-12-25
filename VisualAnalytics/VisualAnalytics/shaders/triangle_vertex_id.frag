#version 450

layout (location = 0) flat in int triangleID;
out uvec4 FragColor;

uniform mat4 projection;

vec3 screenSpaceToWorldSpace(vec3 coord, mat4 invProjection) {
    vec4 clip = vec4(coord * 2.0 - 1.0, 1.0);
    vec4 view = invProjection * clip;
    return view.xyz / view.w;
}

float getPixelViewArea(vec3 coords, vec2 invTexSize) {
    float viewX1 = screenSpaceToWorldSpace(coords - vec3(invTexSize.x * 0.5, 0.0, 0.0), inverse(projection)).x;
    float viewX2 = screenSpaceToWorldSpace(coords + vec3(invTexSize.x * 0.5, 0.0, 0.0), inverse(projection)).x;

    float viewY1 = screenSpaceToWorldSpace(coords - vec3(0.0, invTexSize.y * 0.5, 0.0), inverse(projection)).y;
    float viewY2 = screenSpaceToWorldSpace(coords + vec3(0.0, invTexSize.y * 0.5, 0.0), inverse(projection)).y;

    return abs(viewX2 - viewX1) * abs(viewY2 - viewY1);
}

void main() {
    FragColor = uvec4(triangleID, 0, 0, 0);

    vec2 invTexSize = vec2(1.0 / 800.0, 1.0 / 600.0);
    vec3 coords = vec3(gl_FragCoord.xy * invTexSize, gl_FragCoord.z);

    gl_FragDepth = getPixelViewArea(coords, invTexSize);
}

