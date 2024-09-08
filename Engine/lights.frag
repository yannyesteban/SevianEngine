#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragPosition;

layout(location = 0) out vec4 outColor;

// Definir la luz dentro del shader
const vec3 lightPosition = vec3(2.0, 2.0, 2.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const vec3 viewPosition = vec3(0.0, 0.0, 5.0); // Posición de la cámara

// Parámetros de material
const vec3 ambientMaterial = vec3(0.1, 0.1, 0.1);
const vec3 diffuseMaterial = vec3(1.0, 1.0, 1.0);
const vec3 specularMaterial = vec3(0.5, 0.5, 0.5);
const float shininess = 32.0 / 2.0;

void main() {
    // Componente ambiente
    vec3 ambient = ambientMaterial * lightColor;

    // Componente difusa
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPosition - fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseMaterial * lightColor;

    // Componente especular
    vec3 viewDir = normalize(viewPosition - fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularMaterial * spec * lightColor;

    // Suma de las componentes
    vec3 result = ambient + diffuse + specular;

    outColor = vec4(result * fragColor, 1.0);
}
