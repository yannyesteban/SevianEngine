#version 450

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in int color;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out int fragColor;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

void main() {
    // Transforma las posiciones si usas matrices (descomenta según sea necesario)
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPos, 0.0, 1.0);
    //gl_Position = ubo.proj * vec4(inPos, 0.0, 1.0);
    //gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0); // Directo para 2D
    fragTexCoord = inTexCoord;
    fragColor = color;
}
