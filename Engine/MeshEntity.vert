#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inColor;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;  // Añadido: Normal para el fragment shader
layout(location = 3) out vec3 fragPosition;  // Añadido: Posición en espacio del mundo
layout(location = 4) out vec3 lightPosition;
layout(location = 5) out vec3 camPosition;
layout(set = 0, binding = 0) uniform UniformBufferObject {
    
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 lightPosition;
    vec3 cameraPos;
    
} ubo;

void main() {
    
    //gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    //fragColor = inColor;
    //fragTexCoord = inTexCoord;

    fragPosition = vec3(ubo.model * vec4(inPosition, 1.0));  // Posición en espacio mundial
    fragNormal = normalize(mat3(transpose(inverse(ubo.model))) * inNormal);  // Normal en espacio mundial
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    lightPosition = ubo.lightPosition;
    camPosition = ubo.cameraPos;
}