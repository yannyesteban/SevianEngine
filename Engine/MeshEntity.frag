#version 450
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragPosition;
layout(location = 4) in vec3 lightPosition;
layout(location = 5) in vec3 camPosition;
layout(location = 6) in vec4 fragPosLightSpace;

layout(location = 7) in vec3 lightColor;
layout(location = 8) in float lightIntensity;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform sampler2D texSampler;
layout(set = 1, binding = 1) uniform sampler2DShadow shadowMap;

const vec3 ambientMaterial = vec3(0.1, 0.1, 0.1);
const vec3 diffuseMaterial = vec3(1.0, 1.0, 1.0);
const vec3 specularMaterial = vec3(0.5, 0.5, 0.5);
//const vec3 lightColor = vec3(1.0, 1.0, 1.0);

const float ambientIntensity = 0.9;//1.0
const float diffuseIntensity = 1.0;//1.0
const float specularIntensity = 0.2;//0.2
const float shininess = 32.0f;

float calculateShadow(vec4 fragPosLightSpace) {
    // Coordenadas normalizadas de la luz
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; // Convierte de [-1, 1] a [0, 1]

    // Verificar si está fuera de los límites del shadow map
    if (projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0) {
        return 1.0;  // No hay sombra si está fuera del shadow map
    }

    // Calcular bias para evitar shadow acne
    float bias = max(0.05 * (1.0 - dot(normalize(fragNormal), normalize(lightPosition - fragPosition))), 0.005);
    
    // Realizar la consulta al shadow map con comparación de profundidad
    float shadow = texture(shadowMap, vec3(projCoords.xy, projCoords.z - bias));

    return shadow;
}

void main() {
    // Normalizar vectores
    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(lightPosition - fragPosition);
    vec3 viewDir = normalize(camPosition - fragPosition);
    vec3 reflectDir = reflect(-lightDir, normal);

    // Componentes de iluminación
    vec3 ambient = ambientIntensity * ambientMaterial * lightColor;

    // Difusa
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * diffuseIntensity * diffuseMaterial * lightColor;

    // Especular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * specularIntensity * specularMaterial * lightColor;

    // Calcular sombra
    float shadow = calculateShadow(fragPosLightSpace);

    // Combinamos luz ambiental, difusa, especular y aplicamos sombras
    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);

    // Aplicar textura si es necesario
    vec4 textureColor = texture(texSampler, fragTexCoord);

    outColor = vec4(lighting, 1.0) * textureColor;
}


float calculateShadowX(vec4 fragPosLightSpace) {
    // Convertir a coordenadas de textura de sombras
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; // Transformar de [-1, 1] a [0, 1]

    // Verificar si el fragmento está fuera de los límites del shadow map
    if (projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
        return 0.0;

    // Calcular bias para evitar el shadow acne
    float bias = max(0.05 * (1.0 - dot(fragNormal, normalize(lightPosition - fragPosition))), 0.005);

    // Comparar la profundidad de la sombra con el fragmento actual (con bias aplicado)
    float shadow = texture(shadowMap, vec3(projCoords.xy, projCoords.z - bias));

    // Retornar la sombra (1.0 = sin sombra, 0.0 = en sombra)
    return shadow;
}

void mainX() {
    // Normalizar vectores
    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(lightPosition - fragPosition);
    vec3 viewDir = normalize(camPosition - fragPosition);

    // Componente ambiente
    vec3 ambient = ambientIntensity * ambientMaterial * lightColor;

    // Componente difusa
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseIntensity * diff * diffuseMaterial * lightColor;

    // Componente especular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularIntensity * spec * specularMaterial * lightColor;

    // Cálculo de sombras
    float shadow = calculateShadow(fragPosLightSpace);
    
    // Mezclar los componentes de iluminación
    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular) * lightIntensity;
    vec3 color = texture(texSampler, fragTexCoord).rgb * lighting;
    //outColor = vec4(fragColor * lighting , 1.0) ;
    outColor = vec4(color, 1.0);
}

void main1() {
    // Convertir las coordenadas de luz en coordenadas de textura (espacio proyectado)
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;  // Convertir a rango [0, 1]

    // Asegurarse de que las coordenadas proyectadas están dentro de los límites
    if (projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0) {
        outColor = vec4(1.0, 0.0, 0.0, 1.0); // Color rojo para los fragmentos fuera de los límites
    } else {
        // Visualizar el valor de profundidad del shadow map
        float shadowDepth = texture(shadowMap, projCoords.xyz);
        
        // Convertir el valor de profundidad en un color en escala de grises
        outColor = vec4(vec3(shadowDepth), 1.0);

        vec3 color = texture(texSampler, fragTexCoord).rgb ;

        //outColor = vec4(color, 1.0);

        // Visualizar directamente la coordenada de profundidad
        float depth = projCoords.z;
        //outColor = vec4(vec3(depth), 1.0);
    }
}




float calculateShadow2(vec4 fragPosLightSpace) {
    // Convertir a espacio de textura de sombras
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // Verificar si el fragmento está fuera de los límites del shadow map
    if (projCoords.z > 1.0)
        return 0.0;

    // Aplicar un bias para evitar self-shadowing
    float bias = max(0.05 * (1.0 - dot(fragNormal, normalize(lightPosition - fragPosition))), 0.005);
    float shadow = texture(shadowMap, vec3(projCoords.xy, projCoords.z - bias));

    return shadow;
}

void main2() {
    // Normalizar vectores
    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(lightPosition - fragPosition);
    vec3 viewDir = normalize(camPosition - fragPosition);

    // Componente ambiente
    vec3 ambient = ambientIntensity * ambientMaterial * lightColor;

    // Componente difusa
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseIntensity * diff * diffuseMaterial * lightColor;

    // Componente especular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularIntensity * spec * specularMaterial * lightColor;

    // Cálculo de sombras
    float shadow = calculateShadow(fragPosLightSpace);
    
    // Mezclar los componentes de iluminación
    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);
    vec3 color = texture(texSampler, fragTexCoord).rgb * lighting;

    outColor = vec4(color, 1.0);

   
}
