#version 450

layout(location = 0) in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2DArray textSampler;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
    vec3 sample1 = texture(textSampler, vec3(fragTexCoord, 0.0)).rgb; // Asumiendo que estamos usando la primera capa
    float dx = dFdx(fragTexCoord.x);
    float dy = dFdy(fragTexCoord.y);
    float toPixels = inversesqrt(dx * dx + dy * dy);
    float sigDist = median(sample1.r, sample1.g, sample1.b);
    float w = fwidth(sigDist) * toPixels;
    float alpha = smoothstep(0.5 - w, 0.5 + w, sigDist);
    outColor = vec4(1.0, 1.0, 1.0, alpha); // Blanco con opacidad basada en SDF
}
