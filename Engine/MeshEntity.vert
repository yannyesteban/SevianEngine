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
layout(location = 6) out vec4 fragPosLightSpace;  // Añadido: Coordenadas en el espacio de la luz

layout(set = 0, binding = 0) uniform UniformBufferObject {
    
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 lightPosition;
    vec3 cameraPos;

    mat4 lightView;  // Añadido: Matriz de vista desde la luz
    mat4 lightProj;  // Añadido: Matriz de proyección desde la luz
    
} ubo;

void main() {
    
    //gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    //fragColor = inColor;
    //fragTexCoord = inTexCoord;

    fragPosition = vec3(ubo.model * vec4(inPosition, 1.0));  // Posición en espacio mundial
    fragNormal = normalize(mat3(transpose(inverse(ubo.model))) * inNormal);  // Normal en espacio mundial
    fragTexCoord = inTexCoord;
    fragColor = inColor;
    lightPosition = ubo.lightPosition;
    camPosition = ubo.cameraPos;

    // Calculamos la posición del vértice en el espacio de la luz
    fragPosLightSpace = ubo.lightProj * ubo.lightView * ubo.model * vec4(inPosition, 1.0);

    // Posición del vértice en espacio de cámara
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    
    
    
    
}