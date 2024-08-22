#version 450

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

void main() {
//ubo.proj * ubo.view * ubo.model * 
    //gl_Position = vec4(inPos, 0.0, 3.0);
    gl_Position = vec4(inPos.x, inPos.y , 0.0, 1.0);
    fragTexCoord = inTexCoord;
}
