#version 450
layout(set = 0, binding = 0) uniform UniformBufferObject {
    
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 lightPosition;
    vec3 cameraPos;

    mat4 lightView;  // Añadido: Matriz de vista desde la luz
    mat4 lightProj;  // Añadido: Matriz de proyección desde la luz
    
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inColor;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
}

// Random function to generate pseudo-random values
float random(vec3 scale, float seed) {
    return fract(sin(dot(gl_Position.xyz + seed, scale)) * 43758.5453);
}

void main1() {
    // Calculate the final vertex position as usual
    vec4 worldPosition = ubo.model * vec4(inPosition, 1.0);
    
    // Random depth based on vertex position (for debugging purposes)
    float randomDepth = random(vec3(12.9898, 78.233, 151.7182), 0.0);
    gl_Position = ubo.lightProj * ubo.lightView * worldPosition;

    // Overwrite depth value with random depth for debug purposes
    gl_Position.z = randomDepth;  // Overwrite with random depth
}