#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inColor;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 fragPosition; // Pasar la posici�n del v�rtice al fragment shader

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

void main() {
    fragColor = inColor;
    fragNormal = mat3(transpose(inverse(ubo.model))) * inNormal; // Transformar la normal al espacio del mundo
    fragTexCoord = inTexCoord;
    fragPosition = vec3(ubo.model * vec4(inPosition, 1.0)); // Posici�n en espacio del mundo

    gl_Position = ubo.proj * ubo.view * vec4(fragPosition, 1.0);
}
