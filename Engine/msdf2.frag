#version 450


layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) flat  in int fragColor;


layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D textSampler;



float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}





void main1() {
    vec3 msdf = texture(textSampler, fragTexCoord).rgb;

    // Umbral de borde para determinar la cobertura
    float threshold = 0.5; 

    // Calcular el valor de distancia firmado usando el valor mínimo de los tres canales
    float signedDistance = min(min(msdf.r, msdf.g), msdf.b);

    // Suavizado alrededor del borde
    float alpha = smoothstep(threshold - 0.1, threshold + 0.1, signedDistance);

    // Salida final: texto blanco con fondo transparente
    outColor = vec4(vec3(1.0), alpha);
}
float pxRange = 0.125; // set to distance field's pixel range

float screenPxRange() {
    vec2 unitRange = vec2(pxRange)/vec2(textureSize(textSampler, 0));
    vec2 screenTexSize = vec2(1.0)/fwidth(fragTexCoord);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

void main2() {
    pxRange = 2.0;
    vec4 bgColor = vec4(1.0, 1.0, 1.0, 0.0);
    vec4 fgColor = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 msd = texture(textSampler, fragTexCoord);
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);


    outColor = mix(bgColor, fgColor, opacity);
}

void main3() {
    // Obtener el valor de la distancia desde la textura MSDF
    float distance = texture(textSampler, fragTexCoord).r;

    // Mostrar el valor de la distancia
    outColor = vec4(distance, distance, distance, 1.0);  // Gris para mostrar el valor de la distancia
   
}

vec4 bgColor = vec4(1.0, 1.0, 1.0, 1.0);
vec4 fgColor = vec4(1.0, 0.2, 0.1, 1.0);
void main4() {
    vec3 msd = texture(textSampler, fragTexCoord).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    outColor = mix(bgColor, fgColor, opacity);
}


void main5() {
    vec4 bgColor = vec4(1.0, 1.0, 1.0, 0.0); // Fondo blanco transparente
    vec4 fgColor = vec4(0.0, 0.0, 0.0, 1.0); // Texto negro opaco

    // Obtener el valor MSDF de la textura
    vec3 msdf = texture(textSampler, fragTexCoord).rgb;

    // Calcular la distancia firmada usando la mediana de los tres canales
    float signedDistance = median(msdf.r, msdf.g, msdf.b);

    // Convertir el valor normalizado de [0.0, 1.0] a un rango centrado en 0.0
    signedDistance = signedDistance * 2.0 - 1.0;

    // Escalar la distancia firmada según el rango de píxeles en pantalla
    float pxRange = screenPxRange();
    float screenPxDistance = pxRange * signedDistance;

    // Suavizado de bordes
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

    // Mezclar el color de fondo y el color del texto según la opacidad calculada
    outColor = mix(bgColor, fgColor, opacity);
}

void main6() 
{
    float distance = texture(textSampler, fragTexCoord).a;
    float smoothWidth = fwidth(distance);	
    float alpha = smoothstep(0.5 - smoothWidth, 0.5 + smoothWidth, distance);
	vec3 rgb = vec3(alpha);
									 
									 
									 
    outColor = vec4(rgb, alpha);	
	
}

void main7() {
    // Obtener el valor de la distancia desde la textura MSDF
    vec4 distance = texture(textSampler, fragTexCoord);

    // Mostrar el valor de la distancia
    outColor = vec4(distance);  // Gris para mostrar el valor de la distancia

}

void main8() {
    // Muestrear la textura flotante (R32G32B32_SFLOAT)
    vec3 msd = texture(textSampler, fragTexCoord).rgb;
    vec4 textColor = vec4(0.6f, 0.2f, 0.4f, 1.0f);
    // Calcular el valor de distancia firmado
    float distance = median(msd.r, msd.g, msd.b);
    
    // Calcular el suavizado de bordes usando el ancho del píxel en pantalla
    float screenPxDistance = fwidth(distance);
    float alpha = smoothstep(0.5 - screenPxDistance, 0.5 + screenPxDistance, distance);
    
    // Mezclar el color del texto con el alfa calculado
    outColor = vec4(textColor.rgb, textColor.a * alpha);
}


float median9(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main9() {
    // Muestrear la textura normalizada (R8G8B8A8_UNORM)
    vec3 msd = texture(textSampler, fragTexCoord).rgb;
    vec4 textColor = vec4(0.6f, 0.2f, 0.4f, 1.0f);
    // Calcular la distancia firmada
    float distance = median(msd.r, msd.g, msd.b);
    
    // Suavizar los bordes usando el ancho del píxel en pantalla
    float screenPxDistance = fwidth(distance);
    float alpha = smoothstep(0.5 - screenPxDistance, 0.5 + screenPxDistance, distance);
    
    // Mezclar el color del texto con el alfa calculado
    outColor = vec4(textColor.rgb, textColor.a * alpha);
}

                 // Rango de distancia en píxeles para el antialiasing

void main10() {
    const float pxRange = 4.0;
    // Obtener los valores multicanal de la textura MSDF
    vec3 msdf = texture(textSampler, fragTexCoord).rgb;

    // Calcular el valor de distancia usando la técnica de MSDF
    float sd = median(msdf.r, msdf.g, msdf.b);

    // Aplicar suavizado basado en el rango de píxeles
    float alpha = clamp(sd * pxRange + 0.5, 0.0, 1.0);
    vec4 textColor = vec4(0.6f, 0.2f, 0.4f, 1.0f);
    // Establecer el color de salida con el alpha calculado
    outColor = vec4(textColor.rgb, alpha);
}

void main11() 
{
    float distance = texture(textSampler, fragTexCoord).a; // Leer canal alfa como distancia normalizada
    float smoothWidth = fwidth(distance);	
    float alpha = smoothstep(0.5 - smoothWidth, 0.5 + smoothWidth, distance);
    outColor = vec4(vec3(alpha), alpha);
	
}

void main12() {
     pxRange = 8.0;
    vec4 bgColor = vec4(1.0, 1.0, 1.0, 0.0);
    vec4 fgColor = vec4(0.0, 0.0, 0.0, 1.0);

    if(fragColor == 1){
        bgColor = vec4(1.0, 1.0, 1.0, 0.0);

    }else{

        bgColor = vec4(1.0, 0.0, 1.0, 0.0);
    }
    vec4 msd = texture(textSampler, fragTexCoord);
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.4, 1.0);


    outColor = mix(bgColor, fgColor, opacity);
}

void main(){
    int x = 12;//12;//2;//7;

    if (x == 1) {
        main1();
    }
    if (x == 2) {
        main2();
    }
    if (x == 3) {
        main3();
    }
    if (x == 4) {
        main4();
    }
    if (x == 5) {
        main5();
    }
    if (x == 6) {
        main6();
    }
    if (x == 7) {
        main7();
    }
    if (x == 8) {
        main8();
    }
    if (x == 9) {
        main9();
    }
    if (x == 10) {
        main10();
    }
    if (x == 11) {
        main11();
    }
    if (x == 12) {
        main12();
    }

}