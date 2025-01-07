#version 450

layout(location = 0) in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D textSampler;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

 float pxRange = 0.125; // set to distance field's pixel range

float screenPxRange() {
    vec2 unitRange = vec2(pxRange)/vec2(textureSize(textSampler, 0));
    vec2 screenTexSize = vec2(1.0)/fwidth(fragTexCoord);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

void main() {
    vec4 bgColor = vec4(1.0, 1.0, 1.0, 0.0); // Fondo blanco transparente
    vec4 fgColor = vec4(0.0, 0.0, 0.0, 1.0); // Texto negro opaco
    vec3 msd = texture(textSampler, fragTexCoord).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    outColor = mix(bgColor, fgColor, opacity);
}


