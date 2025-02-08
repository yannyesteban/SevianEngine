#version 450
layout(location = 0) in vec2 fragUV;
layout(location = 0) out vec4 outColor;

uniform vec2 size;           // Tamaño del botón
uniform float borderRadius;  // Radio de las esquinas redondeadas
uniform float borderWidth;   // Grosor del borde
uniform vec4 backgroundColor;// Color de fondo
uniform vec4 borderColor;    // Color del borde

float roundedBox(vec2 p, vec2 b, float r) {
    return length(max(abs(p) - b + r, 0.0)) - r;
}

void main() {
    vec2 center = size * 0.5;
    vec2 pos = fragUV * size - center;

    // Distancia al borde redondeado
    float distance = roundedBox(pos, center - borderWidth, borderRadius);

    // Dibuja el fondo y el borde
    if (distance < 0.0) {
        outColor = backgroundColor; // Fondo
    } else if (distance < borderWidth) {
        outColor = borderColor; // Borde
    } else {
        discard; // Fuera del botón
    }
}