#version 450
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inColor;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragPosition;
layout(location = 3) out vec3 fragColor;


layout(std140, set = 0, binding = 0) uniform UniformBufferObject {
    vec3 color;
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 color2;
    float zoom;
    vec3 position;
} ubo;


layout(std140, set = 0, binding = 3) uniform MeUBO {
    
    float zoom1;
    vec3 color;
    
    vec3 position;
   
    
} me2;

/*
layout(set = 0, binding = 1) uniform UniformBufferObject2 {
    mat4 model;

    mat4 view;
    mat4 proj;
} ubo2;
*/

layout(std140, set = 0, binding = 1) uniform LightUBO {
    
   
    vec3 color;
    float zoom1;
    vec3 position;
} me;

void main() {
    fragNormal = mat3(ubo.model) * inNormal; // Transform normal
    fragTexCoord = inTexCoord;
    fragPosition = vec3(ubo.model * vec4(inPosition, 1.0)); // Transform position to world space
    fragColor = me.color;//light.position;

    if (me.zoom1 <= 2.0) {

        fragColor = me2.color;
    }
    //fragColor = ubo.position;
    //gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, light.intensity);
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, me.zoom1);
}