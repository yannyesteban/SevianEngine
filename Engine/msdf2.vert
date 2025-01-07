#version 450

layout(location = 0) in vec3 inPos;    // Posición del vértice
layout(location = 1) in vec2 inTexCoord; // Coordenadas UV

layout(location = 0) out vec2 fragTexCoord; // Salida de UV hacia el fragment shader

layout(push_constant) uniform PushConstants {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

void main() {
    fragTexCoord = inTexCoord;
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPos, 1.0);
}
