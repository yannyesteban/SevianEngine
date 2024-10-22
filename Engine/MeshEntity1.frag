#version 450
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;  // Recibido del vertex shader
layout(location = 3) in vec3 fragPosition;  // Recibido del vertex shader
layout(location = 4) in vec3 lightPosition;
layout(location = 5) in vec3 camPosition;
layout(location = 6) in vec4 fragPosLightSpace;  // Añadido: Coordenadas en el espacio de la luz

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform sampler2D texSampler;
layout(set = 1, binding = 1) uniform sampler2DShadow shadowMap;


const vec3 ambientMaterial = vec3(0.1, 0.1, 0.1);
const vec3 diffuseMaterial = vec3(1.0, 1.0, 1.0);
const vec3 specularMaterial = vec3(0.5, 0.5, 0.5);
//const float shininess = 32.0 / 2.0;
const vec3 lightColor = vec3(1.0, 1.0, 1.0);

const float ambientIntensity = 1.0;
const float diffuseIntensity = 1.0;
const float specularIntensity = 0.2;
const float shininess = 32.0f;

float calculateShadow2(vec4 fragPosLightSpace) {
    // Convertir las coordenadas de clip [-1, 1] a [0, 1] para la consulta del shadow map
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // Consultar el shadow map: si la profundidad actual es mayor, significa que está en sombra
    float closestDepth = texture(shadowMap, projCoords);

    // Verificar si el fragmento está en sombra
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth + 0.005 ? 0.5 : 1.0;  // 0.005 es un valor para bias
    return shadow;
}

float calculateShadow(vec4 fragPosLightSpace, sampler2DShadow shadowMap) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; // Llevar a coordenadas de textura
    
    float closestDepth = texture(shadowMap, projCoords.xyz);
    float currentDepth = projCoords.z;
    
    float bias = 0.005; // Bias para evitar self-shadowing
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    
    return shadow;
}

// Función antigua (referencia)
float calculateShadowOld(vec4 fragPosLightSpace, sampler2DShadow shadowMap) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; // Llevar a coordenadas de textura
    
    float closestDepth = texture(shadowMap, projCoords.xyz);
    float currentDepth = projCoords.z;
    
    float bias = 0.005; // Bias para evitar self-shadowing
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    
    return shadow;
}

// Función mejorada con bias dinámico
float calculateShadowImproved(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;  // Convertir a coordenadas de textura [0, 1]

    // Cálculo del bias dinámico
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // Consultar el shadow map y comparar la profundidad automáticamente
    float shadow = texture(shadowMap, projCoords.xyz - vec3(0.0, 0.0, bias));

    return shadow;
}

// Función antigua para PCF (referencia)
float calculateShadowPCFOld(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords);
    float currentDepth = projCoords.z;
    
    float bias = 0.005;
    float shadow = currentDepth > closestDepth + bias ? 0.5 : 1.0; // Sombra básica sin suavizado
    
    return shadow;
}

// Función mejorada con PCF
float calculateShadowPCFImproved(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; // Llevar a coordenadas [0, 1]

    // Verifica si estamos dentro del rango de la textura
    if (projCoords.z > 1.0) return 1.0;

    float shadow = 0.0;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); // Bias dinámico
    int samples = 4; // Número de muestras para suavizado (PCF)

    // Tomar varias muestras alrededor de la coordenada del shadow map (PCF)
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            vec2 offset = vec2(float(x), float(y)) * 0.002; // Ajusta para suavidad
            shadow += texture(shadowMap, vec3(projCoords.xy + offset, projCoords.z - bias));
        }
    }

    shadow /= 9.0;  // Normalizar por el número de muestras (3x3)
    return shadow;
}


void main() {
    // Luz ambiental
    vec3 ambient = ambientIntensity * ambientMaterial * lightColor;
    
    // Dirección de la luz y normalización
    vec3 lightDir = normalize(lightPosition - fragPosition);
    vec3 normal = normalize(fragNormal);
    
    // Difusa
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseIntensity * diff * diffuseMaterial * lightColor;
    
    // Especular
    vec3 viewDir = normalize(camPosition - fragPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularIntensity * spec * specularMaterial * lightColor;
    
    // Cálculo de sombras con función mejorada
    float shadow = calculateShadowImproved(fragPosLightSpace, normal, lightDir);
   
    // Color final
    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);
    vec4 textureColor = texture(texSampler, fragTexCoord);
    outColor = vec4(lighting, 1.0) * textureColor;
}

void main1() {
    // Luz ambiental
    vec3 ambient = ambientIntensity * ambientMaterial * lightColor;
    
    // Dirección de la luz y normalización
    vec3 lightDir = normalize(lightPosition - fragPosition);
    vec3 normal = normalize(fragNormal);
    
    // Difusa
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseIntensity * diff * diffuseMaterial * lightColor;
    
    // Especular
    vec3 viewDir = normalize(camPosition - fragPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularIntensity * spec * specularMaterial * lightColor;
    
    // Cálculo de sombras
    float shadow = calculateShadow(fragPosLightSpace, shadowMap);
   
    // Color final
    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);
    vec4 textureColor = texture(texSampler, fragTexCoord);
    outColor = vec4(lighting, 1.0) * textureColor;

    //outColor = texture(shadowMap, vec3(1.0, 0.0, 0.0)) * vec4(1.0, 1.0, 0.0, 0.0);
}

void main3() {
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

    vec3 result = (ambient + diffuse + specular) ;//* fragColor;

     // Calcular la sombra usando el shadow map
    float shadow = calculateShadow2(fragPosLightSpace);

    // Atenuar la iluminación si está en sombra
    result *= shadow;


    // Aplicación de la textura y el color final
    outColor = vec4(result, 1.0) * texColor;


   
   
}