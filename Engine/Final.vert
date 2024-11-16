#version 460

//layout (location = 0) in vec3 inPos;
//layout (location = 1) in vec2 inUV;
//layout (location = 2) in vec3 inColor;
//layout (location = 3) in vec3 inNormal;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inColor;

layout (set = 0, binding = 0) uniform UniformBufferObject {
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

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec3 outViewVec;
layout (location = 3) out vec3 outLightVec;
layout (location = 4) out vec4 outShadowCoord;
layout (location = 5) out vec2 fragUV;  // Coordenadas UV a pasar al fragment shader
layout (location = 6) out float lightIntensity;
layout( location = 7) out vec3 vert_light;


const mat4 bias = mat4( 
  0.5, 0.0, 0.0, 0.0,
  0.0, 0.5, 0.0, 0.0,
  0.0, 0.0, 1.0, 0.0,
  0.5, 0.5, 0.0, 1.0 );


void main1() 
{
    outNormal = mat3(ubo.model) * inNormal;
    outColor = inColor;
    

    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition.xyz, 1.0);
    
    vec4 pos = ubo.model * vec4(inPosition, 1.0);
    outLightVec = normalize(ubo.lightPos - pos.xyz);
    outViewVec = -pos.xyz;

    // Calcula las coordenadas de sombra
    mat4 lightSpaceMatrix = ubo.lightProj * ubo.lightView;
    outShadowCoord = bias * lightSpaceMatrix * ubo.model * vec4(inPosition, 1.0);
    lightIntensity = ubo.lightIntensity;
    fragUV = inTexCoord;  
}




void main() {
    // Transforma la normal al espacio del modelo
    outNormal = mat3(ubo.model) * inNormal;

    // Color del vértice
    outColor = inColor;

    // Calcula el vector hacia la cámara
    vec4 worldPosition = ubo.model * vec4(inPosition, 1.0);
    outViewVec = normalize(ubo.cameraPos - worldPosition.xyz);

    // Calcula el vector hacia la luz
    outLightVec = normalize(ubo.lightPos - worldPosition.xyz);

    // Calcula las coordenadas de sombra
    outShadowCoord = bias * ubo.lightProj * ubo.lightView * worldPosition;

    // Pasa las coordenadas UV
    fragUV = inTexCoord;

    // Pasa la intensidad de la luz
    lightIntensity = ubo.lightIntensity;

    // Calcula la posición de la luz en el espacio de la vista
    vert_light = (ubo.view * vec4(ubo.lightPos, 1.0)).xyz;

    // Posición final en el espacio de recorte
    gl_Position = ubo.proj * ubo.view * worldPosition;


}