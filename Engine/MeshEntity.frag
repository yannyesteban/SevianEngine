#version 450
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;  // Recibido del vertex shader
layout(location = 3) in vec3 fragPosition;  // Recibido del vertex shader
layout(location = 4) in vec3 lightPosition;
layout(location = 5) in vec3 camPosition;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform sampler2D texSampler;

const vec3 ambientMaterial = vec3(0.1, 0.1, 0.1);
const vec3 diffuseMaterial = vec3(1.0, 1.0, 1.0);
const vec3 specularMaterial = vec3(0.5, 0.5, 0.5);
//const float shininess = 32.0 / 2.0;
const vec3 lightColor = vec3(1.0, 1.0, 1.0);

const float ambientIntensity = 1.0;
const float diffuseIntensity = 1.0;
const float specularIntensity = 0.2;
const float shininess = 32.0f;

void main() {
    vec4 texColor = texture(texSampler, fragTexCoord);

    // Normalización de las normales
    vec3 normal = normalize(fragNormal);

    // Vector de la posición del fragmento hacia la luz
    vec3 lightDir = normalize(lightPosition - fragPosition);

    // Componente ambiente
    vec3 ambient = ambientIntensity * lightColor;

    // Componente difusa
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * diffuseIntensity * lightColor;

    // Componente especular
    vec3 viewDir = normalize(camPosition - fragPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * specularIntensity * lightColor;

    // Suma de las componentes difusa, especular y ambiente
    vec3 result = (ambient + diffuse + specular) * fragColor;

    // Aplicación de la textura y el color final
    outColor = vec4(result, 1.0) * texColor;
   
}