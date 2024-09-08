#version 450

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragPosition;
layout(location = 3) in vec3 fragColor;

layout(location = 0) out vec4 outColor;
layout(binding = 2) uniform sampler2D texSampler;
/*
layout(set = 0, binding = 2) uniform LightUBO {
    vec3 lightPosition;
    vec3 lightColor;
    float lightIntensity;
} lightUbo;
*/
/*
layout(set = 0, binding = 1) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;
*/
void main() {
    vec3 normal = normalize(fragNormal);
    //vec3 lightDir = normalize(lightUbo.lightPosition - fragPosition);

    // Ambient lighting
    //vec3 ambient = 0.1 * lightUbo.lightColor;

    // Diffuse lighting
    //float diff = max(dot(normal, lightDir), 0.0);
    //vec3 diffuse = diff * lightUbo.lightColor;

    // Specular lighting
    vec3 viewDir = normalize(-fragPosition); // Assuming the camera is at the origin
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // Shininess factor
    //vec3 specular = lightUbo.lightIntensity * spec * lightUbo.lightColor;
    vec3 luz = vec3(1.0,0.0,0.0);
    //vec3 result = (ambient + diffuse + specular) * lightUbo.lightIntensity;
    
    
    vec4 textureColor = texture(texSampler, fragTexCoord);
    
    
    //vec3 ambient = lightUbo.lightIntensity * luz;
    
    //outColor = /*vec4(result, 1.0) *  vec4(ambient, 1.0) * */ textureColor;
    outColor = vec4(fragColor, 1.0);
    //outColor = vec4(lightUbo.lightPosition, 1.0);
}
