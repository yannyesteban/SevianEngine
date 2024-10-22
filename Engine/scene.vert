#version 450

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

const mat4 biasMat = mat4( 
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.5, 0.5, 0.0, 1.0 );

void main() 
{
    outColor = inColor;
    outNormal = mat3(ubo.model) * inNormal;

    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition.xyz, 1.0);
    
    vec4 pos = ubo.model * vec4(inPosition, 1.0);
    outLightVec = normalize(ubo.lightPos - pos.xyz);
    outViewVec = -pos.xyz;

    // Calcula las coordenadas de sombra
    mat4 lightSpaceMatrix = ubo.lightProj * ubo.lightView;
    outShadowCoord = biasMat * lightSpaceMatrix * ubo.model * vec4(inPosition, 1.0);
    lightIntensity = ubo.lightIntensity;
    fragUV = inTexCoord;  
}
