#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    
    mat4 model;
    mat4 view;
    mat4 proj;
    
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal; // Aunque no se use en este shader b�sico, est� en tu Vertex
layout(location = 2) in vec2 inTexCoords;
layout(location = 3) in vec3 inColor;

layout(location = 0) out vec2 fragTexCoords;
layout(location = 1) out vec3 fragColor;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);

    fragTexCoords = inTexCoords;
    fragColor = inColor;
}