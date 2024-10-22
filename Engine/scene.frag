#version 450
#extension GL_KHR_vulkan_glsl : enable

layout (set = 1, binding = 0) uniform sampler2D texSampler;   // Textura del objeto
layout (set = 1, binding = 1) uniform sampler2DShadow shadowMap;    // Mapa de sombras

layout (location = 0) in vec3 outNormal;
layout (location = 1) in vec3 outColor;
layout (location = 2) in vec3 outViewVec;
layout (location = 3) in vec3 outLightVec;
layout (location = 4) in vec4 outShadowCoord;
layout (location = 5) in vec2 inUV; // Coordenadas UV para la textura
layout (location = 6) in float lightIntensity;

layout (constant_id = 0) const int enablePCF = 1;

layout (location = 0) out vec4 outFragColor;

#define ambient 0.1

// Función que realiza la comparación de profundidad con el shadow map
float textureProj(vec4 shadowCoord, vec2 off)
{
    float shadow = 1.0;
    if (shadowCoord.z > 0.0 && shadowCoord.z < 1.0)  // Sólo calculamos sombra si estamos en el rango de profundidad válido
    {
        // Se pasa vec3 con UV (st) y coordenada de profundidad (z)
        shadow = texture(shadowMap, vec3(shadowCoord.st + off, shadowCoord.z));
    }
    return shadow;
}

// Filtro PCF (Percentage Closer Filtering)
float filterPCF(vec4 sc)
{
    ivec2 texDim = textureSize(shadowMap, 0);
    float scale = 1.5;
    float dx = scale * 1.0 / float(texDim.x);
    float dy = scale * 1.0 / float(texDim.y);

    float shadowFactor = 0.0;
    int count = 0;
    int range = 1;

    for (int x = -range; x <= range; x++)
    {
        for (int y = -range; y <= range; y++)
        {
            shadowFactor += textureProj(sc, vec2(dx * x, dy * y));
            count++;
        }
    }
    return shadowFactor / count;
}

void main() 
{
    // Aplicar el filtro PCF si está habilitado, o solo proyectar sombra
    float shadow = (enablePCF == 1) ? filterPCF(outShadowCoord / outShadowCoord.w) : textureProj(outShadowCoord / outShadowCoord.w, vec2(0.0));

    vec3 N = normalize(outNormal);
    vec3 L = normalize(outLightVec);

    // Calcula la intensidad de la luz sin utilizar el color del objeto
    float lightFactor = max(dot(N, L), ambient) * lightIntensity;

    // Aplicar la textura al resultado de la iluminación
    vec4 texColor = texture(texSampler, inUV);

    // Calcular el color final, multiplicando la intensidad de la luz y las sombras por el color de la textura
    outFragColor = vec4(texColor.rgb * lightFactor * shadow, texColor.a);
}

void main2() 
{
    // Aplicar el filtro PCF si está habilitado, o solo proyectar sombra
    float shadow = (enablePCF == 1) ? filterPCF(outShadowCoord / outShadowCoord.w) : textureProj(outShadowCoord / outShadowCoord.w, vec2(0.0));

    // Aplicar la textura directamente sin usar el color difuso
    vec4 texColor = texture(texSampler, inUV);

    // Calcular el color final, multiplicando por el factor de sombra
    outFragColor = vec4(texColor.rgb * shadow, texColor.a);
}

void main1() 
{
    // Aplicar el filtro PCF si está habilitado, o solo proyectar sombra
    float shadow = (enablePCF == 1) ? filterPCF(outShadowCoord / outShadowCoord.w) : textureProj(outShadowCoord / outShadowCoord.w, vec2(0.0));

    vec3 N = normalize(outNormal);
    vec3 L = normalize(outLightVec);
    vec3 V = normalize(outViewVec);
    //vec3 diffuse = max(dot(N, L), ambient) * outColor * lightIntensity;
    vec3 diffuse = max(dot(N, L), ambient) * outColor * lightIntensity;
    // Aplicar la textura al color difuso
    vec4 texColor = texture(texSampler, inUV);

    // Calcular el color final, multiplicando por el factor de sombra
    outFragColor = vec4(diffuse * shadow, 1.0) * texColor;
    // outFragColor = vec4(diffuse, 1.0) * texColor;
}
