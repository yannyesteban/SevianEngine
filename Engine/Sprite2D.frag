#version 450

layout(binding = 1) uniform sampler2D spriteTexture; // Asumiendo que usas una textura para el sprite

layout(location = 0) in vec2 fragTexCoords;
layout(location = 1) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 textureColor = texture(spriteTexture, fragTexCoords);
    // Puedes multiplicar el color del vértice por el color de la textura si quieres
    // o usar solo uno de ellos, dependiendo de tu diseño.
    outColor = textureColor * vec4(fragColor, 1.0); // Multiplicamos color de textura por color de vértice
    //outColor =vec4(fragColor, 1.0);
    // Si solo quieres usar el color de la textura, usa:
    // outColor = textureColor;
    // Si solo quieres usar el color del vértice, usa:
    // outColor = vec4(fragColor, 1.0);
}