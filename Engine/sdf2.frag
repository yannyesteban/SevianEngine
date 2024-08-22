#version 450

layout(location = 0) in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D textSampler;

void main() {
    float edgeValue = 0.0; // Valor central del borde
    float thickness = 0.0; // Grosor del borde
    float softness = 0.0; // Suavidad del borde

    vec4 outlineColor = vec4(0.4, 0.66, 0.45, 1.0); // Color del borde
    float outlineThickness = 0.0; // Grosor del borde
    float outlineSoftness = 0.0; // Suavidad del borde

    vec4 fillColor = vec4(0.4, 0.0, 0.3, 1.0); // Color del relleno

    float distance = texture(textSampler, fragTexCoord).r;
    // distance = 1.0 - distance; // Invertir si los valores están al revés
    // Calcular el valor del borde
    float outlineAlpha = smoothstep(edgeValue - outlineThickness - outlineSoftness, edgeValue - outlineThickness + outlineSoftness, distance);

    // Calcular el valor del relleno
    float fillAlpha = smoothstep(edgeValue - softness, edgeValue + softness, distance);

    // Combinar los colores del borde y del relleno
    vec4 color = mix(outlineColor, fillColor, fillAlpha);

    // Calcular la opacidad final
    float alpha = max(outlineAlpha, fillAlpha);

    outColor = vec4(color.rgb, alpha);
}
