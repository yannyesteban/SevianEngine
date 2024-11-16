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
layout(location = 7) out vec3 lightColor;
layout(location = 8) out float lightIntensity;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    
    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 MVP;
    mat4 lightView;  // Añadido: Matriz de vista desde la luz
    mat4 lightProj;  // Añadido: Matriz de proyección desde la luz
    vec3 lightPos;
    vec3 lightColor;
    vec3 cameraPos;
    float lightIntensity;
    
} ubo;

void main() {
    // Transformación de vértices al espacio del mundo
    vec4 worldPosition = ubo.model * vec4(inPosition, 1.0);
    fragPosition = worldPosition.xyz;


   // fragPosition = (ubo.model * vec4(inPosition, 1.0)).xyz;


    // Transformación al espacio de la cámara
    gl_Position = ubo.proj * ubo.view * worldPosition;

    // Normal transformada
    fragNormal = transpose(inverse(mat3(ubo.model))) * inNormal;

    // Pasar otras variables
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    lightPosition = ubo.lightPos;
    camPosition = ubo.cameraPos;
    lightColor = ubo.lightColor;
    lightIntensity = ubo.lightIntensity;

    // Transformar al espacio de la luz para las sombras
    fragPosLightSpace = ubo.lightProj * ubo.lightView * worldPosition;
}

void main2() {
// Pasar las coordenadas de textura y color
    fragTexCoord = inTexCoord;
    fragColor = inColor;


    // Posición en el espacio del mundo
    fragPosition = vec3(ubo.model * vec4(inPosition, 1.0));
    
    // Posición en el espacio de la luz para calcular sombras
    fragPosLightSpace = ubo.lightProj * ubo.lightView * vec4(fragPosition, 1.0);
    
    // Normales en el espacio del mundo
    fragNormal = mat3(transpose(inverse(ubo.model))) * inNormal;
    //fragNormal = normalize(mat3(transpose(inverse(ubo.model))) * inNormal);
    // Pasar la posición de la luz y la cámara
    lightPosition = ubo.lightPos;
    camPosition = ubo.cameraPos;

    
    
    lightColor = ubo.lightColor;
    lightIntensity = ubo.lightIntensity;
    // Calculamos la posición final del vértice en el espacio de clip
    gl_Position = ubo.proj * ubo.view * vec4(fragPosition, 1.0);
}

void main1() {
    
    //gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    //fragColor = inColor;
    //fragTexCoord = inTexCoord;

    fragPosition = vec3(ubo.model * vec4(inPosition, 1.0));  // Posición en espacio mundial
    fragNormal = normalize(mat3(transpose(inverse(ubo.model))) * inNormal);  // Normal en espacio mundial
    fragTexCoord = inTexCoord;
    fragColor = inColor;
    lightPosition = ubo.lightPos;
    camPosition = ubo.cameraPos;

    // Calculamos la posición del vértice en el espacio de la luz
    fragPosLightSpace = ubo.lightProj * ubo.lightView * ubo.model * vec4(inPosition, 1.0);

    // Posición del vértice en espacio de cámara
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    
    
    
    
}