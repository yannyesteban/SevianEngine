#version 450
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform sampler2D texSampler;

void main() {
    vec4 texColor = texture(texSampler, fragTexCoord);
    //outColor = vec4(1.0, 0.5, 0.1, 1.0);
    outColor = texColor * vec4(fragColor, 1.0);
    //outColor = texColor;
    //outColor = vec4(fragColor, 1.0);
}