#version 450

layout(location = 0) in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D textSampler;

void main1() {
    // Obtén los valores MSDF de la textura
    vec3 msdf = texture(textSampler, fragTexCoord).rgb;

    // Calcula la distancia mínima combinando los tres canales
    float signedDistance = msdf.r - 0.5 + msdf.g - 0.5 + msdf.b - 0.5;

    // Define parámetros para bordes suaves
    float edgeValue = 0.5;        // Valor central del borde
    float softness = 0.02;       // Ajusta la suavidad de los bordes

    // Suaviza el borde usando smoothstep
    float alpha = smoothstep(edgeValue - softness, edgeValue + softness, signedDistance);

    // Color del texto (blanco) con el alpha calculado
    vec4 textColor = vec4(1.0, 1.0, 1.0, alpha);

    // Asigna el color de salida
    outColor = textColor;

   

    // MSDF interpretación (suavizado con antialiasing)
    float sdf = min(min(msdf.r, msdf.g), msdf.b);
    alpha = smoothstep(0.5 - fwidth(sdf), 0.5 + fwidth(sdf), sdf);

    outColor = vec4(vec3(1.0), alpha); // Texto blanco con transparencia
}

void main2() {
    vec3 msdf = texture(textSampler, fragTexCoord).rgb;
    float sd = min(min(msdf.r, msdf.g), msdf.b); // Combinar canales
    float alpha = clamp(0.5 - sd, 0.0, 1.0);    // Binarización suave
    outColor = vec4(vec3(1.0), alpha);         // Texto blanco con transparencia
}
const float smoothing = 1.0/64.0;
float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}
void main3() {
    vec3 fontSample = texture(textSampler, fragTexCoord).rgb;
    float sigDist = median(fontSample.r, fontSample.g, fontSample.b);
    float opacity = smoothstep(0.5-smoothing, 0.5 + smoothing, sigDist);
    if(opacity < 0.05) {
    discard;
    }
    outColor = vec4(outColor.rgb, opacity);
}

void main4() {
    vec3 msdf = texture(textSampler, fragTexCoord).rgb;

    // Calcular la cobertura basada en MSDF
    float signedDistance = median(msdf.r, msdf.g, msdf.b);
    float alpha = clamp(0.5 - signedDistance, 0.0, 1.0);

    // Output color con alpha
    outColor = vec4(vec3(1.0), alpha); // Blanco con transparencia basada en el MSDF
}


void main5() {
    vec3 msdf = texture(textSampler, fragTexCoord).rgb;

    // Calcular la distancia con el MSDF
    float signedDistance = median(msdf.r, msdf.g, msdf.b);

    // Controlar el borde mediante un umbral ajustable
    float smoothing = fwidth(signedDistance) * 0.5; // Smoothing basado en derivadas de pantalla
    float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, signedDistance);

    // Color blanco con transparencia basada en MSDF
    outColor = vec4(1.0, 0.0, 1.0, alpha);
}


void mainMSDF() {
    // Obtener el valor de la distancia desde la textura MSDF
    float distance = texture(textSampler, fragTexCoord).r;

    // Mostrar el valor de la distancia
    outColor = vec4(distance, distance, distance, 1.0);  // Gris para mostrar el valor de la distancia
}

void main7() {
    // Obtener el color de la textura MSDF (en formato R8G8B8A8_UNORM)
    vec4 msdf = texture(textSampler, fragTexCoord);
    
    // Normalizar los valores de la textura (de 0-255 a 0.0-1.0)
    vec3 msdfNormalized = msdf.rgb / 255.0;

    // Calcular el valor de MSDF (esto depende de cómo estés usando el MSDF en tu shader)
    // Si msdfNormalized está en el rango [0.0, 1.0], puedes usarlo directamente para el cálculo.
    float msdfValue = msdfNormalized.r; // O el canal que estés usando (R, G, B)

    // El valor MSDF representa la distancia, por lo que lo puedes usar para hacer
    // un cálculo de borde, suavizado, o cualquier otro efecto que necesites.
    // Como ejemplo, podemos simplemente usarlo para mostrar el borde:
    
    // Si el valor MSDF es cercano a 0, está en el borde, de lo contrario, está lejos.
    if (msdfValue < 0.1) {
        outColor = vec4(1.0, 0.0, 0.0, 1.0); // Color rojo si está cerca del borde
    } else {
        outColor = vec4(0.0, 0.0, 0.0, 1.0); // Negro si está lejos del borde
    }
}

void main8() 
{
    float distance = texture(textSampler, fragTexCoord).r;
    float smoothWidth = fwidth(distance);	
    float alpha = smoothstep(0.5 - smoothWidth, 0.5 + smoothWidth, distance);
	vec3 rgb = vec3(alpha);
	/*								 
	if (ubo.outline > 0.0) 
	{
		float w = 1.0 - ubo.outlineWidth;
		alpha = smoothstep(w - smoothWidth, w + smoothWidth, distance);
        rgb += mix(vec3(alpha), ubo.outlineColor.rgb, alpha);
    }									 
		*/							 
    outColor = vec4(rgb, alpha);	
	
}

const float distanceThreshold = 0.5f;

void main9() {
    // Obtener el valor de la distancia desde la textura MSDF
    vec3 msdfValue = texture(textSampler, fragTexCoord).rgb;

    // Separamos los valores de distancia en R, G y B para cada canal
    float distanceR = msdfValue.r;
    float distanceG = msdfValue.g;
    float distanceB = msdfValue.b;

    // Usamos el canal rojo (R) para el cálculo de la distancia
    // Se puede mezclar estos valores según lo necesites para obtener una representación del borde
    float dist = distanceR;

    // Si el valor de la distancia está por encima del umbral, el fragmento es parte del texto
    if (dist >= distanceThreshold) {
        outColor = vec4(1.0, 1.0, 1.0, 1.0); // Blanco para el texto
    } else {
        outColor = vec4(0.0, 0.0, 0.0, 0.0); // Transparente para el fondo
    }
}
 // set to distance field's pixel range

float screenPxRange() {
    float pxRange = 5.0;
    vec2 unitRange = vec2(pxRange)/vec2(textureSize(textSampler, 0));
    vec2 screenTexSize = vec2(1.0)/fwidth(fragTexCoord);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

void main() {
    vec4 bgColor = vec4(1.0, 1.0, 1.0, 0.0);
    vec4 fgColor = vec4(0.0, 0.0, 0.0, 1.0);
    vec3 msd = texture(textSampler, fragTexCoord).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);


    outColor = mix(bgColor, fgColor, opacity);
}