#version 450

layout(binding = 1) uniform sampler2D spriteTexture; // Asumiendo que usas una textura para el sprite

layout(location = 0) in vec2 fragTexCoords;
layout(location = 1) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

// Definir push constants
layout(push_constant) uniform PushConstants {
    vec2 size;
    vec4 backgroundColor;
    vec4 borderColor;
    float borderWidth;
	float borderRadius;
    int useTexture;
} push;


float sdRoundRect(vec2 p, vec2 b, float r) {
    vec2 q = abs(p) - b + r;
    return length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - r;
}


float sdRoundRect1(vec2 uv, vec2 size, float radius) {
    uv = uv * 2.0 - 1.0;          // Convierte UVs [0,1] a [-1,1]
    vec2 d = abs(uv) - (size - radius);
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0) - radius;
}
float sdRoundRect2(vec2 p, vec2 b, float r) {
    vec2 q = abs(p) - b;
    return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - r;
}
float sdRoundRect3(vec2 p, vec2 b, float r) {
    vec2 d = abs(p - b) - b;
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}
// Fragment Shader
float sdRoundRectAspect(vec2 uv, vec2 size, float radius, float aspect) {
    // Corrige la relación de aspecto
    uv.x *= aspect; // Escala X para "convertir" a espacio cuadrado
    vec2 center = vec2(0.5 * aspect, 0.5); // Centrado correcto
    vec2 q = abs(uv - center) - (size - radius) + vec2(radius);
    return length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - radius;
}


float roundedBoxSDF(vec2 centerPosition, vec2 size, float radius) {
    vec2 q = abs(centerPosition) - size + radius;
    return length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - radius;
}

float sdRoundRect4(vec2 uv, vec2 size, float radius) {
    uv = uv * size; // Convierte UVs a espacio de píxeles
    vec2 center = size / 2.0;
    vec2 d = abs(uv - center) - (size - radius) + radius;
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0) - radius;
}

float sdRoundedBoxDeep1(vec2 p, vec2 b, float r) {
    vec2 d = abs(p) - b + r;
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0) - r;
}

void mainDeep1() {
    vec2 rectSize = vec2(600.0, 300.0);
    float borderWidth = 20.0;
    vec4 borderColor = vec4(1.0, 0.0, 0.0, 1.0);
    vec4 backgroundColor = vec4(0.0, 0.0, 1.0, 1.0);
    float cornerRadius = 20.0;
    
    // Calcular posición relativa al centro (asumiendo coordenadas centradas)
    vec2 pos = (gl_FragCoord.xy - (rectSize/2.0));
    
    // Calcular SDF para el rectángulo exterior
    vec2 halfSize = rectSize/2.0;
    float d_outer = sdRoundedBoxDeep1(pos, halfSize, cornerRadius);
    
    // Calcular SDF para el rectángulo interior
    vec2 innerHalfSize = max(halfSize - borderWidth, vec2(0.0));
    float innerRadius = max(cornerRadius - borderWidth, 0.0);
    float d_inner = sdRoundedBoxDeep1(pos, innerHalfSize, innerRadius);
    
    // Anti-aliasing (1 píxel de suavizado)
    float aa = 1.0;
    
    // Calcular alphas
    float outerAlpha = 1.0 - smoothstep(-aa, aa, d_outer);
    float innerAlpha = 1.0 - smoothstep(-aa, aa, d_inner);
    float borderAlpha = outerAlpha - innerAlpha;
    
    // Mezclar colores
    vec4 finalColor = backgroundColor * innerAlpha + borderColor * borderAlpha;
    finalColor.a *= outerAlpha;  // Recortar área exterior
    
    outColor = finalColor;
}
void main6() {
vec2 rectSize = vec2(600.0, 300.0); // Tamaño real en píxeles
float borderWidth = 20.0;           // Grosor del borde en píxeles
 vec4 borderColor = vec4(1.0, 0.0, 0.0, 1.0);
 vec4 backgroundColor = vec4(0.0, 0.0, 1.0, 1.0);


    vec2 uv = fragTexCoords * rectSize; // UVs en píxeles
    float distance = sdRoundRect4(uv, rectSize, borderWidth);
    float borderMask = smoothstep(
        borderWidth - 1.0,
        borderWidth + 1.0,
        abs(distance)
    );
    outColor = mix(vec4(0.2, 0.5, 1.0, 1.0), vec4(1.0), borderMask);
}

float sdfRect(vec2 p, vec2 size) {
    vec2 d = abs(p) - size / 2.0;
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

float sdfRect2(vec2 p, vec2 halfSize) {
    vec2 d = abs(p) - halfSize;
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

float sdfRoundedRect(vec2 p, vec2 halfSize, float radius) {
    vec2 q = abs(p) - halfSize + vec2(radius);
    return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - radius;
}
// SDF para rectángulo con esquinas redondeadas
float sdfRoundRect(vec2 p, vec2 halfSize, float radius) {
    // Limitamos el radio para que no sea mayor que la mitad del tamaño más pequeño
    radius = min(radius, min(halfSize.x, halfSize.y));
    vec2 d = abs(p) - halfSize + vec2(radius);
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0) - radius;
}
void mainBeautiful() {
    vec2 rectSize = vec2(600.0, 300.0); // Tamaño real en píxeles
    float borderWidth = 10.0;           // Grosor del borde en píxeles
    vec4 borderColor = vec4(1.0, 0.0, 0.0, 1.0); // Rojo
    vec4 backgroundColor = vec4(0.0, 0.0, 1.0, 1.0); // Azul
    float cornerRadius = 20.0;          // Radio de las esquinas redondeadas
    
    // Convertir UV a píxeles
    vec2 p_pixel = fragTexCoords * rectSize;
    
    // Centro en píxeles
    vec2 center = rectSize / 2.0;
    
    // Posición relativa al centro
    vec2 p_relative = p_pixel - center;
    
    // Calcular SDF para rectángulo con esquinas redondeadas
    float d = sdfRoundRect(p_relative, center, cornerRadius);
    
    // Determinar el color
    if (d < -borderWidth) {
        outColor = backgroundColor; // Dentro del rectángulo
    } else if (d < 0.0) {
        outColor = borderColor;     // Borde
    } else {
        discard;                    // Fuera
        // Alternativa: outColor = vec4(0.0, 0.0, 0.0, 0.0); // Transparente
    }
}

void mainBeautiful2() {
    vec2 rectSize = vec2(600.0, 300.0); // Tamaño real en píxeles
    float borderWidth = 10.0;           // Grosor del borde en píxeles
    vec4 borderColor = vec4(1.0, 0.0, 0.0, 1.0); // Rojo
    vec4 backgroundColor = vec4(0.0, 0.0, 1.0, 1.0); // Azul
    float cornerRadius = 20.0;          // Radio de las esquinas redondeadas
    
    // Convertir UV a píxeles
    vec2 p_pixel = fragTexCoords * rectSize;
    
    // Centro en píxeles
    vec2 center = rectSize / 2.0;
    
    // Posición relativa al centro
    vec2 p_relative = p_pixel - center;
    
    // Calcular SDF para rectángulo con esquinas redondeadas
    float d = sdfRoundRect(p_relative, center, cornerRadius);
    
    // Reemplazar el if-else con:
    float aa = -1.0; // 1.5 píxeles de suavizado
    float borderFactor = smoothstep(-borderWidth - aa, -borderWidth + aa, -d);
    outColor = mix(backgroundColor, borderColor, borderFactor);

    float alpha = 1.0 - smoothstep(0.0, aa, d);
    outColor.a *= alpha;
}

float sdfRoundedRectGrok1(vec2 p, vec2 halfSize, float radius) {
    vec2 d = abs(p) - halfSize + vec2(radius);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - radius;
}
void mainGrok1() {
    vec2 rectSize = vec2(600.0, 300.0); // Tamaño real en píxeles
    float borderWidth = 10.0;           // Grosor del borde en píxeles
    vec4 borderColor = vec4(1.0, 0.0, 0.0, 1.0);    // Rojo
    vec4 backgroundColor = vec4(0.0, 0.0, 1.0, 1.0); // Azul
    float cornerRadius = 10.0;          // Radio de las esquinas en píxeles

    // Convertir UV a píxeles
    vec2 p_pixel = fragTexCoords * rectSize;

    // Centro en píxeles
    vec2 center = rectSize / 2.0;

    // Posición relativa al centro
    vec2 p_relative = p_pixel - center;

    // Semi-tamaño ajustado para las esquinas redondeadas
    vec2 halfSize = center - vec2(cornerRadius); // vec2(280.0, 130.0)

    // Calcular SDF para el rectángulo con esquinas redondeadas
    float d = sdfRoundedRectGrok1(p_relative, halfSize, cornerRadius);

    // Determinar el color
    if (d < -borderWidth) {
        outColor = backgroundColor; // Dentro: azul
    } else if (d < borderWidth) {
        outColor = borderColor;     // Borde: rojo
    } else {
        outColor = vec4(0.0, 0.0, 0.0, 0.0); // Fuera: transparente
    }
}
void main9() {
    vec2 rectSize = vec2(600.0, 300.0);
    float borderWidth = 20.0;
    vec4 borderColor = vec4(1.0, 0.0, 0.0, 1.0); // Rojo
    vec4 backgroundColor = vec4(0.0, 0.0, 1.0, 1.0); // Azul
    float cornerRadius = 20.0;

    // Asumimos que fragCoord es una variable de entrada con la posición del fragmento
    // respecto al centro del rectángulo
    vec2 p = fragTexCoords;
    vec2 size = rectSize / 2.0;

    // Calcular la SDF para un rectángulo con esquinas redondeadas
    vec2 d = abs(p) - size + vec2(cornerRadius);
    float dist = min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - cornerRadius;

    // Determinar el color basado en la distancia
    if (dist > 0.0) {
        outColor = vec4(0.0, 0.0, 0.0, 0.0); // Fuera: transparente
    } else if (dist > -borderWidth) {
        outColor = borderColor; // En el borde: rojo
    } else {
        outColor = backgroundColor; // Dentro: azul
    }
}

void main7() {
 vec2 rectSize = vec2(600.0, 300.0); // Tamaño real en píxeles
 float borderWidth = 20.0;           // Grosor del borde en píxeles
 vec4 borderColor = vec4(1.0, 0.0, 0.0, 1.0);
 vec4 backgroundColor = vec4(0.0, 0.0, 1.0, 1.0);
 float cornerRadius = 20.0;
    // Convertir UV a píxeles
    vec2 p_pixel = fragTexCoords * rectSize;
    
    // Centro del rectángulo
    vec2 center = rectSize / 2.0;
    
    // Posición relativa al centro
    vec2 p_relative = p_pixel - center;
    
    // Calcular SDF con esquinas redondeadas
    //float d = sdfRoundedRect(p_relative, center, cornerRadius);
    float d = roundedBoxSDF(center, rectSize, cornerRadius);
    // Determinar el color según la distancia
    if (d < -borderWidth) {
        outColor = backgroundColor; // Dentro del rectángulo
    } else if (d < borderWidth) {
        outColor = borderColor;     // Borde
    } else {
        discard;                     // Fuera del rectángulo
    }
}

void mainCool() {
 vec2 rectSize = vec2(600.0, 300.0); // Tamaño real en píxeles
 float borderWidth = 20.0;           // Grosor del borde en píxeles
 vec4 borderColor = vec4(1.0, 0.0, 0.0, 1.0);
 vec4 backgroundColor = vec4(0.0, 0.0, 1.0, 1.0);
    // Convertir UV a píxeles
    vec2 p_pixel = fragTexCoords * rectSize;
    
    // Centro en píxeles
    vec2 center = rectSize / 2.0;
    
    // Posición relativa al centro
    vec2 p_relative = p_pixel - center;
    
    // Calcular SDF
    float d = sdfRect2(p_relative, center);
    
    // Determinar el color
    if (d < -borderWidth) {
        discard; 
        outColor = backgroundColor; // Dentro del rectángulo
    } else if (d < borderWidth) {
        outColor = borderColor;     // Borde
    } else {
        //discard;                     // Fuera

         outColor = vec4(0.6, 0.2, 1.0, 1.0);     // Borde
    }
}

void main5() {
    vec2 center = vec2(0.5); // Centro del rectángulo
     float borderWidth = 0.2;
    vec2 size = vec2(1.0,1.0);   // Tamaño (w, h)
    vec2 p = fragTexCoords.xy - center; // Posición relativa al centro
    float d = sdfRect(p, size);  // Distancia al borde
    vec4 borderColor = vec4(0.5, 0.0, 0.1, 1.0);
    vec4 backgroundColor = vec4(0.2, 0.2, 0.8, 1.0);
    if (d < 0) {
        outColor = backgroundColor; // Dentro: fondo
    } else if (d < borderWidth) {
        outColor = borderColor;     // Borde
    } else {
        discard;                     // Fuera: descarta el píxel
    }
}

void main4() {
    // Convertir coordenadas a espacio centrado para SDF
    vec2 centeredPos = fragTexCoords.xy - 0.5;
    
    float borderWidth = 0.5;
    float cornerRadius = 0.5;
    vec4 borderColor = vec4(0.2, 0.2, 0.8, 1.0);
    
   
        // Cálculo del SDF externo
        float outerSdf = roundedBoxSDF(centeredPos, vec2(0.5), cornerRadius /0.05);
        
        // Cálculo del SDF interno (reducido por el ancho del borde)
        float innerSdf = roundedBoxSDF(centeredPos, 
                          vec2(0.5) - vec2(borderWidth / 0.05), 
                          max(0.0, (cornerRadius - borderWidth) / 0.05));
        
        // Si estamos dentro del borde externo pero fuera del borde interno, es un borde
        if (outerSdf <= 0.0 && innerSdf > 0.0) {
            outColor = borderColor;
        } else {
            outColor = vec4(0.0);
        }
    
}

void main3() {
    vec2 uv = fragTexCoords.xy;
    vec2 size = vec2(600.0, 300.0);
    float aspect = size.x / size.y; // Relación de aspecto (ej: 600/300 = 2.0)
     float borderRadius = 10.5;
     float borderWidth = 10.5;
    // Normaliza el radio y grosor a UVs considerando el tamaño real
    float radiusUV = borderRadius / size.y; // Usamos height como referencia
    float borderWidthUV = borderWidth / size.y;
    
    // Calcula SDF corregido por aspect ratio
   
    float distance = sdRoundRectAspect(uv, vec2(0.5), radiusUV, aspect);
    
    // Suavizado y cálculo de bordes
    float smoothness = 0.005;
    float alpha = smoothstep(-smoothness, smoothness, -distance);
    float borderAlpha = smoothstep(
        borderWidthUV - smoothness, 
        borderWidthUV + smoothness, 
        abs(distance)
    );
    
    // Aplica colores
    outColor = mix(vec4(0.2, 0.2, 0.8, 1.0), vec4(1.0), borderAlpha);
    outColor.a *= alpha;
}

void main2() {

    vec2 uv = fragTexCoords.xy;
    // Tamaños relativos (normalizados)
    float borderSize = 0.1; // 10% del tamaño del widget
    float paddingSize = 0.0; // 20%

    // Calcula SDF para el fondo completo
    float bgDistance = sdRoundRect1(uv, vec2(1.0), 0.05);

    // Calcula SDF para el área de padding (fondo - padding)
    float paddingDistance = sdRoundRect1(uv, vec2(1.0 - paddingSize), 0.05);

    // Calcula SDF para el borde (fondo - contenido)
    float borderDistance = abs(bgDistance) - borderSize;

    // Colores
    vec4 bg = vec4(0.2, 0.2, 0.8, 1.0);
    vec4 padding = vec4(1.0);
    vec4 border = vec4(1.0, 0.0, 0.0, 1.0);

    // Mezcla
    float bgAlpha = smoothstep(0.01, -0.01, bgDistance);
    float paddingAlpha = smoothstep(0.01, -0.01, paddingDistance);
    float borderAlpha = smoothstep(0.01, -0.01, borderDistance);

    // Aplica
    vec4 finalColor = mix(bg, padding, paddingAlpha);
    finalColor = mix(finalColor, border, borderAlpha);
    outColor = finalColor;
}

void main1() {
    float inBorderRadius = 0.05;
    float inBorderWidth = 0.05;

// Tamaño normalizado (ajustado a la proporción del widget)
    vec2 size = vec2(1.0); // Usamos 1.0 porque el quad ya está normalizado
    
    // Calcula distancia al borde
    float distance = sdRoundRect3(fragTexCoords.xy, size, inBorderRadius);
    
    // Suavizado anti-aliasing
    float smoothness = 0.002;
    float alpha = smoothstep(-smoothness, smoothness, -distance);
    
    // Borde vs Fondo
    float borderMask = smoothstep(
        inBorderWidth - smoothness,
        inBorderWidth + smoothness,
        abs(distance)
    );
    
    // Colores
    vec4 backgroundColor = vec4(0.2, 0.0, 0.4, 1.0);;
    vec4 borderColor = vec4(0.5, 0.8, 0.2, 1.0); // Verde claro
    
    outColor = mix(backgroundColor, borderColor, borderMask);
    outColor.a *= alpha; // Aplica transparencia anti-aliasing


    vec4 textureColor = texture(spriteTexture, fragTexCoords);
    // Puedes multiplicar el color del vértice por el color de la textura si quieres
    // o usar solo uno de ellos, dependiendo de tu diseño.
    
    if(push.useTexture == 1){
        //outColor = textureColor * vec4(fragColor, 1.0); // Multiplicamos color de textura por color de vértice
    }else{
        //outColor = push.color; // Multiplicamos color de textura por color uniforme
    }
    
    
    //outColor = textureColor * vec4(fragColor, 1.0); // Multiplicamos color de textura por color de vértice
    
    
    
    //outColor =vec4(fragColor, 1.0);
    // Si solo quieres usar el color de la textura, usa:
    // outColor = textureColor;
    // Si solo quieres usar el color del vértice, usa:
    // outColor = vec4(fragColor, 1.0);
}



float sdRoundedBoxDeep2(vec2 p, vec2 b, float r) {
    vec2 d = abs(p) - b + r;
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0) - r;
}

void mainDeep2() {
    // Parámetros (ajusta estos según necesites)
    vec2 rectSize = vec2(600.0, 300.0);
    float borderWidth = 20.0;
    vec4 borderColor = vec4(1.0, 0.0, 0.0, 1.0);
    vec4 backgroundColor = vec4(0.0, 0.0, 1.0, 1.0);
    float cornerRadius = 20.0;
    
    // 1. Calcular posición relativa al centro del rectángulo
    vec2 screenCenter = rectSize / 2; // Ajusta según tu resolución
    vec2 rectPosition = screenCenter; // Centro de la pantalla
    vec2 pos = gl_FragCoord.xy - rectPosition;
    
    // 2. Asegurar relación de aspecto correcta
    vec2 halfSize = rectSize/2.0;
    
    // 3. Cálculos SDF
    float d_outer = sdRoundedBoxDeep2(pos, halfSize, cornerRadius);
    
    // 4. Cálculo del borde interno
    vec2 innerHalfSize = halfSize - borderWidth;
    float innerRadius = max(cornerRadius - borderWidth, 0.0);
    float d_inner = sdRoundedBoxDeep2(pos, innerHalfSize, innerRadius);
    
    // Anti-aliasing (1.5 píxeles de suavizado)
    float aa = 1.5;
    
    // 5. Mezcla de colores corregida
    float outer = smoothstep(-aa, aa, -d_outer);
    float inner = smoothstep(-aa, aa, -d_inner);
    float border = outer - inner;
    
    vec4 finalColor = mix(backgroundColor, borderColor, border);
    finalColor.a *= outer; // Recortar transparencia
    
    outColor = finalColor;
}

// Función SDF para rectángulo con esquinas redondeadas
float sdfRoundRectGrok2(vec2 p, vec2 halfSize, float radius) {
    vec2 d = abs(p) - halfSize + vec2(radius);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - radius;
}

void mainGrok2() {
    // Tamaño del rectángulo
    vec2 rectSize = vec2(600.0, 300.0); // Tamaño real en píxeles
    vec2 halfSize = rectSize / 2.0;

    // Grosores de los bordes
    float borderWidthLeft = 10.0;    // Grosor del borde izquierdo
    float borderWidthRight = 15.0;   // Grosor del borde derecho
    float borderWidthTop = 20.0;     // Grosor del borde superior
    float borderWidthBottom = 25.0;  // Grosor del borde inferior

    // Colores de los bordes
    vec4 borderColorLeft = vec4(1.0, 0.0, 0.0, 1.0);   // Rojo (izquierdo)
    vec4 borderColorRight = vec4(0.0, 1.0, 0.0, 1.0);  // Verde (derecho)
    vec4 borderColorTop = vec4(0.0, 0.0, 1.0, 1.0);    // Azul (superior)
    vec4 borderColorBottom = vec4(1.0, 1.0, 0.0, 1.0); // Amarillo (inferior)

    // Color de fondo
    vec4 backgroundColor = vec4(0.5, 0.5, 0.5, 1.0);   // Gris

    // Radio de las esquinas
    float cornerRadius = 20.0;

    // Convertir UV a píxeles y calcular posición relativa al centro
    vec2 p_pixel = fragTexCoords * rectSize;
    vec2 center = rectSize / 2.0;
    vec2 p_relative = p_pixel - center;

    // Calcular SDF
    float d = sdfRoundRectGrok2(p_relative, halfSize - vec2(cornerRadius), cornerRadius);

    // Determinar la región del fragmento
    vec2 ap = abs(p_relative);
    bool isLeft = ap.x < halfSize.x - cornerRadius && p_relative.x < 0.0;
    bool isRight = ap.x < halfSize.x - cornerRadius && p_relative.x > 0.0;
    bool isTop = ap.y < halfSize.y - cornerRadius && p_relative.y > 0.0;
    bool isBottom = ap.y < halfSize.y - cornerRadius && p_relative.y < 0.0;

    // Asignar colores y grosores según la región
    if (isTop || (ap.x >= halfSize.x - cornerRadius && ap.y >= halfSize.y - cornerRadius && p_relative.y > 0.0)) {
        // Región superior (incluye esquinas superiores)
        if (d < -borderWidthTop) {
            outColor = backgroundColor; // Dentro
        } else if (d < 0.0) {
            outColor = borderColorTop;  // Borde superior
        } else {
            discard;                    // Fuera
        }
    } else if (isBottom || (ap.x >= halfSize.x - cornerRadius && ap.y >= halfSize.y - cornerRadius && p_relative.y < 0.0)) {
        // Región inferior (incluye esquinas inferiores)
        if (d < -borderWidthBottom) {
            outColor = backgroundColor; // Dentro
        } else if (d < 0.0) {
            outColor = borderColorBottom; // Borde inferior
        } else {
            discard;                    // Fuera
        }
    } else if (isLeft) {
        // Borde izquierdo
        if (d < -borderWidthLeft) {
            outColor = backgroundColor; // Dentro
        } else if (d < 0.0) {
            outColor = borderColorLeft; // Borde izquierdo
        } else {
            discard;                    // Fuera
        }
    } else if (isRight) {
        // Borde derecho
        if (d < -borderWidthRight) {
            outColor = backgroundColor; // Dentro
        } else if (d < 0.0) {
            outColor = borderColorRight; // Borde derecho
        } else {
            discard;                    // Fuera
        }
    } else {
        discard; // Fuera del rectángulo o en una región no definida
    }
}


// Función SDF para rectángulo con esquinas redondeadas (Inigo Quilez)
float sdfRoundRectGPT1(vec2 p, vec2 rectHalfSize, float radius) {
    vec2 d = abs(p) - rectHalfSize + vec2(radius);
    return length(max(d, vec2(0.0))) + min(max(d.x, d.y), 0.0) - radius;
}

void mainGPT1() {
    // Tamaño del rectángulo en píxeles
    vec2 rectSize = vec2(600.0, 300.0);
    vec2 halfSize = rectSize * 0.5;
    float cornerRadius = 20.0;

    // Grosor y color de cada borde
    float borderLeft   = 20.0;
    float borderRight  = 20.0;
    float borderTop    = 20.0;
    float borderBottom = 20.0;
    
    vec4 borderColorLeft   = vec4(1.0, 0.0, 0.0, 1.0); // Rojo
    vec4 borderColorRight  = vec4(0.0, 1.0, 0.0, 1.0); // Verde
    vec4 borderColorTop    = vec4(0.0, 0.0, 1.0, 1.0); // Azul
    vec4 borderColorBottom = vec4(1.0, 1.0, 0.0, 1.0); // Amarillo
    
    vec4 backgroundColor = vec4(0.0, 0.0, 0.0, 1.0); // Color interior

    // Convertir UV a coordenadas en píxeles
    vec2 p_pixel = fragTexCoords * rectSize;
    
    // Para el SDF, trasladamos el origen al centro del rectángulo
    vec2 p_centered = p_pixel - halfSize;
    
    // Para que el SDF considere el área "interior" sin borde, se define un rectángulo interior
    // Restamos en cada lado el grosor del borde (se utiliza el máximo en horizontal y vertical)
    vec2 innerHalfSize = halfSize - vec2(max(borderLeft, borderRight), max(borderTop, borderBottom));
    
    // Calculamos la distancia SDF al rectángulo redondeado interior
    float d = sdfRoundRectGPT1(p_centered, innerHalfSize, cornerRadius);
    
    // Factor anti-aliasing (se puede ajustar o calcular con fwidth)
    float aa = 1.0;

    // Si estamos dentro del área interior, pintamos el fondo
    if(d < -aa) {
        outColor = backgroundColor;
        return;
    }
    
    // Si no es interior, determinamos a qué borde pertenece el fragmento.
    // Calculamos la distancia en píxeles a cada lado:
    float dL = p_pixel.x;
    float dR = rectSize.x - p_pixel.x;
    float dT = p_pixel.y;
    float dB = rectSize.y - p_pixel.y;
    
    // Se determina el borde más cercano (se usa <= para evitar ambigüedades)
    vec4 selectedBorderColor = vec4(0.0);
    float selectedBorderWidth = 0.0;
    float edgeDistance = 0.0;
    
    if(dL <= dR && dL <= dT && dL <= dB) {
        selectedBorderColor = borderColorLeft;
        selectedBorderWidth  = borderLeft;
        edgeDistance = dL;
    } else if(dR <= dL && dR <= dT && dR <= dB) {
        selectedBorderColor = borderColorRight;
        selectedBorderWidth  = borderRight;
        edgeDistance = dR;
    } else if(dT <= dL && dT <= dR && dT <= dB) {
        selectedBorderColor = borderColorTop;
        selectedBorderWidth  = borderTop;
        edgeDistance = dT;
    } else { // dB es el mínimo
        selectedBorderColor = borderColorBottom;
        selectedBorderWidth  = borderBottom;
        edgeDistance = dB;
    }
    
    // Con smoothstep se logra una transición suave en el borde.
    // Si el fragmento se encuentra a una distancia menor al grosor del borde, se usa el color del borde.
    float borderMask = smoothstep(selectedBorderWidth + aa, selectedBorderWidth - aa, edgeDistance);
    
    // En zonas de transición (por ejemplo, en las esquinas) se mezclará con el color interior.
    outColor = mix(selectedBorderColor, backgroundColor, borderMask);
}

float sdfRoundRectDeep3(vec2 p, vec2 halfSize, float radius) {
    radius = min(radius, min(halfSize.x, halfSize.y));
    vec2 d = abs(p) - halfSize + vec2(radius);
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0) - radius;
}

void maindeep3() {
    vec2 rectSize = vec2(600.0, 300.0);
    vec4 backgroundColor = vec4(0.4, 0.3, 0.88, 1.0);
    float cornerRadius = 20.0;
    
    // Parámetros individuales para cada borde
    float borderWidthLeft = 20.0;
    float borderWidthRight = 20.0;
    float borderWidthTop = 20.0;
    float borderWidthBottom = 20.0;
    
    vec4 borderColorLeft = vec4(1.0, 0.0, 0.0, 1.0);    // Rojo
    vec4 borderColorRight = vec4(0.0, 1.0, 0.0, 1.0);   // Verde
    vec4 borderColorTop = vec4(0.0, 0.0, 1.0, 1.0);     // Azul
    vec4 borderColorBottom = vec4(1.0, 1.0, 0.0, 1.0);  // Amarillo

    vec2 p_pixel = fragTexCoords * rectSize;
    vec2 center = rectSize / 2.0;
    vec2 p_relative = p_pixel - center;
    
    float d = sdfRoundRectDeep3(p_relative, center, cornerRadius);
    
    if (d < 0.0) {
        // Calcular distancias a cada borde
        vec2 edgeDist = center - abs(p_relative);
        vec2 borderWeights = vec2(
            (p_relative.x < 0.0) ? borderWidthLeft : borderWidthRight,
            (p_relative.y < 0.0) ? borderWidthBottom : borderWidthTop
        );
        
        // Determinar en qué región estamos
        vec2 borderFactor = smoothstep(vec2(0.0), vec2(1.0), edgeDist / borderWeights);
        
        // Calcular contribución de cada borde
        vec4 horizontalColor = mix(
            (p_relative.x < 0.0) ? borderColorLeft : borderColorRight, 
            backgroundColor, 
            borderFactor.x
        );
        
        vec4 verticalColor = mix(
            (p_relative.y < 0.0) ? borderColorBottom : borderColorTop, 
            backgroundColor, 
            borderFactor.y
        );
        
        // Mezclar colores y manejar esquinas
        float cornerBlend = min(borderFactor.x, borderFactor.y);
        outColor = mix(mix(horizontalColor, verticalColor, 0.5), backgroundColor, cornerBlend);
        
        // Manejar transparencia en bordes
        float alpha = 1.0 - smoothstep(0.0, 1.0, d + max(max(borderWidthLeft, borderWidthRight), max(borderWidthTop, borderWidthBottom)));
        outColor.a *= alpha;


    } else {
        discard;
    }
}

void mainGemini1() {

    vec2 rectSize = vec2(600.0, 300.0);
    vec4 backgroundColor = vec4(0.0, 0.0, 1.0, 1.0);
    float cornerRadius = 20.0;
    
    // Parámetros individuales para cada borde
    float borderWidthLeft = 20.0;
    float borderWidthRight = 10.0;
    float borderWidthTop = 15.0;
    float borderWidthBottom = 5.0;
    
    vec4 borderColorLeft = vec4(1.0, 0.0, 0.0, 1.0);    // Rojo
    vec4 borderColorRight = vec4(0.0, 1.0, 0.0, 1.0);   // Verde
    vec4 borderColorTop = vec4(0.0, 0.0, 1.0, 1.0);     // Azul
    vec4 borderColorBottom = vec4(1.0, 1.0, 0.0, 1.0);  // Amarillo

    // Convert UV to pixels
    vec2 p = fragTexCoords * rectSize;

    // Inner rectangle boundaries
    vec2 innerMin = vec2(borderWidthLeft, borderWidthBottom);
    vec2 innerMax = rectSize - vec2(borderWidthRight, borderWidthTop);

    // Check if inside the inner rectangle
    if (p.x >= innerMin.x && p.x <= innerMax.x && p.y >= innerMin.y && p.y <= innerMax.y) {
        outColor = backgroundColor;
        return;
    }

    outColor = vec4(0.0, 0.0, 0.0, 0.0); // Initialize as transparent

    // Left border
    if (p.x >= 0.0 && p.x < borderWidthLeft && p.y >= 0.0 && p.y <= rectSize.y) {
        outColor = borderColorLeft;
    }
    // Right border
    if (p.x > rectSize.x - borderWidthRight && p.x <= rectSize.x && p.y >= 0.0 && p.y <= rectSize.y) {
        outColor = borderColorRight;
    }
    // Bottom border
    if (p.y >= 0.0 && p.y < borderWidthBottom && p.x >= 0.0 && p.x <= rectSize.x) {
        outColor = borderColorBottom;
    }
    // Top border
    if (p.y > rectSize.y - borderWidthTop && p.y <= rectSize.y && p.x >= 0.0 && p.x <= rectSize.x) {
        outColor = borderColorTop;
    }

    // Rounded Corners - Overwrite if within the rounded border area
    float radiusSq = cornerRadius * cornerRadius;

    // Bottom-Left
    vec2 blCenter = innerMin + vec2(cornerRadius, cornerRadius);
    if (distance(p, blCenter) <= cornerRadius) {
        vec2 fromCenter = p - blCenter;
        if (fromCenter.x < 0.0 && abs(fromCenter.x) > borderWidthLeft) discard;
        if (fromCenter.y < 0.0 && abs(fromCenter.y) > borderWidthBottom) discard;
        if (outColor == vec4(0.0)) {
            if (p.x < borderWidthLeft) outColor = borderColorLeft;
            else if (p.y < borderWidthBottom) outColor = borderColorBottom;
        }
    }

    // Bottom-Right
    vec2 brCenter = vec2(innerMax.x - cornerRadius, innerMin.y + cornerRadius);
    if (distance(p, brCenter) <= cornerRadius) {
        vec2 fromCenter = p - brCenter;
        if (fromCenter.x > 0.0 && fromCenter.x > borderWidthRight) discard;
        if (fromCenter.y < 0.0 && abs(fromCenter.y) > borderWidthBottom) discard;
        if (outColor == vec4(0.0)) {
            if (p.x > rectSize.x - borderWidthRight) outColor = borderColorRight;
            else if (p.y < borderWidthBottom) outColor = borderColorBottom;
        }
    }

    // Top-Left
    vec2 tlCenter = vec2(innerMin.x + cornerRadius, innerMax.y - cornerRadius);
    if (distance(p, tlCenter) <= cornerRadius) {
        vec2 fromCenter = p - tlCenter;
        if (fromCenter.x < 0.0 && abs(fromCenter.x) > borderWidthLeft) discard;
        if (fromCenter.y > 0.0 && fromCenter.y > borderWidthTop) discard;
        if (outColor == vec4(0.0)) {
            if (p.x < borderWidthLeft) outColor = borderColorLeft;
            else if (p.y > rectSize.y - borderWidthTop) outColor = borderColorTop;
        }
    }

    // Top-Right
    vec2 trCenter = innerMax - vec2(cornerRadius, cornerRadius);
    if (distance(p, trCenter) <= cornerRadius) {
        vec2 fromCenter = p - trCenter;
        if (fromCenter.x > 0.0 && fromCenter.x > borderWidthRight) discard;
        if (fromCenter.y > 0.0 && fromCenter.y > borderWidthTop) discard;
        if (outColor == vec4(0.0)) {
            if (p.x > rectSize.x - borderWidthRight) outColor = borderColorRight;
            else if (p.y > rectSize.y - borderWidthTop) outColor = borderColorTop;
        }
    }

    if (outColor.a == 0.0) discard;
}

void mainGrok3() {
    vec2 rectSize = vec2(600.0, 300.0); // Tamaño real en píxeles
    float borderWidth = 10.0;           // Grosor del borde en píxeles
    vec4 borderColor = vec4(1.0, 0.0, 0.0, 1.0); // Rojo
    vec4 backgroundColor = vec4(0.0, 0.0, 1.0, 1.0); // Azul
    float cornerRadius = 10.0;          // Radio de las esquinas redondeadas
    float aaWidth = 2.0;                // Ancho del suavizado en píxeles

    // Convertir UV a píxeles
    vec2 p_pixel = fragTexCoords * rectSize;
    
    // Centro en píxeles
    vec2 center = rectSize / 2.0;
    
    // Posición relativa al centro
    vec2 p_relative = p_pixel - center;
    
    // Calcular SDF para rectángulo con esquinas redondeadas
    float d = sdfRoundRect(p_relative, center, cornerRadius);
    
    // Suavizado del borde exterior
    float outerEdge = smoothstep(-aaWidth, aaWidth, d);
    
    // Suavizado entre el borde y el interior
    float innerEdge = smoothstep(-borderWidth - aaWidth, -borderWidth + aaWidth, d);
    
    // Determinar el color
    if (d < -borderWidth) {
        // Dentro del rectángulo: backgroundColor
        outColor = backgroundColor;
    } else if (d < 0.0) {
        // Borde: transición suave hacia el interior
        outColor = mix(borderColor, backgroundColor, innerEdge);
    } else {
        // Fuera: transición suave hacia transparente
        outColor = mix(borderColor, vec4(0.0, 0.0, 0.0, 0.0), outerEdge);
    }
}


void mainClaude1() {
    vec2 rectSize = vec2(600.0, 300.0); // Tamaño real en píxeles
    float borderWidth = 30.0;           // Grosor del borde en píxeles
    vec4 borderColor = vec4(1.0, 0.0, 0.0, 1.0); // Rojo
    vec4 backgroundColor = vec4(0.0, 0.0, 1.0, 1.0); // Azul
    float cornerRadius = 30.0;          // Radio de las esquinas redondeadas
    
    // Factor de anti-aliasing (ajusta según la resolución de tu pantalla)
    float aa = 0.0;
    
    // Convertir UV a píxeles
    vec2 p_pixel = fragTexCoords * rectSize;
    
    // Centro en píxeles
    vec2 center = rectSize / 2.0;
    
    // Posición relativa al centro
    vec2 p_relative = p_pixel - center;
    
    // Calcular SDF para rectángulo con esquinas redondeadas
    float d = sdfRoundRect(p_relative, center, cornerRadius);
    
    // Suavizado para el borde exterior
    float outerEdge = smoothstep(0.0, aa, -d);
    
    // Suavizado para el borde interior
    float innerEdge = smoothstep(0.0, aa, -(d + borderWidth));
    
    // Mezclar colores con anti-aliasing
    vec4 finalColor = mix(vec4(0.0, 0.0, 0.0, 0.0), borderColor, outerEdge);
    finalColor = mix(finalColor, backgroundColor, innerEdge);
    
    // Asignar color final
    outColor = finalColor;
}

void mainClaude2() {
    vec2 rectSize = vec2(600.0, 300.0); // Tamaño real en píxeles
    float borderWidth = 10.0;           // Grosor del borde en píxeles
    vec4 borderColor = vec4(1.0, 0.0, 0.0, 1.0); // Rojo
    vec4 backgroundColor = vec4(0.0, 0.0, 1.0, 1.0); // Azul
    float cornerRadius = 20.0;          // Radio de las esquinas redondeadas
    
    // Convertir UV a píxeles
    vec2 p_pixel = fragTexCoords * rectSize;
    
    // Centro en píxeles
    vec2 center = rectSize / 2.0;
    
    // Posición relativa al centro
    vec2 p_relative = p_pixel - center;
    
    // Calcular SDF para rectángulo con esquinas redondeadas
    float d = sdfRoundRect(p_relative, center, cornerRadius);
    
    // Calcular factor de anti-aliasing basado en la derivada
    vec2 grad = vec2(dFdx(d), dFdy(d));
    float aa = length(grad) * 0.5; // Factor de anti-aliasing adaptativo
    
    // Evitar división por cero o valores muy pequeños
    aa = max(aa, 0.001);
    
    // Suavizado para el borde exterior
    float outerEdge = smoothstep(0.0, aa, -d);
    
    // Suavizado para el borde interior
    float innerEdge = smoothstep(0.0, aa, -(d + borderWidth));
    
    // Mezclar colores con anti-aliasing
    vec4 finalColor = mix(vec4(0.0, 0.0, 0.0, 0.0), borderColor, outerEdge);
    finalColor = mix(finalColor, backgroundColor, innerEdge);
    
    // Asignar color final
    outColor = finalColor;
}


// Suponiendo que tienes implementada la función sdfRoundRect, por ejemplo:
float sdfRoundRectGPT2(vec2 p, vec2 rectHalfSize, float radius) {
    vec2 d = abs(p) - rectHalfSize + vec2(radius);
    return length(max(d, vec2(0.0))) + min(max(d.x, d.y), 0.0) - radius;
}

void mainGPT2() {
    vec2 rectSize = vec2(600.0, 300.0);           // Tamaño real en píxeles
    float borderWidth = 10.0;                     // Grosor del borde en píxeles
    vec4 borderColor = vec4(1.0, 0.0, 0.0, 1.0);    // Rojo
    vec4 backgroundColor = vec4(0.0, 0.0, 1.0, 1.0); // Azul
    float cornerRadius = 20.0;                    // Radio de las esquinas redondeadas
    
    // Convertir UV a coordenadas en píxeles
    vec2 p_pixel = fragTexCoords * rectSize;
    
    // Centro del rectángulo en píxeles
    vec2 center = rectSize / 2.0;
    
    // Posición relativa al centro
    vec2 p_relative = p_pixel - center;
    
    // Se considera "center" como halfSize para el SDF
    float d = sdfRoundRectGPT2(p_relative, center, cornerRadius);
    
    // Calcular el ancho del anti-aliasing dinámicamente
    float aa = fwidth(d);
    
    // Usar smoothstep para suavizar la transición
    float tBackground = smoothstep(-borderWidth - aa, -borderWidth + aa, d);
    float tEdge       = smoothstep(0.0 + aa, 0.0 - aa, d);
    
    // Mezclar el color del borde y el del fondo de acuerdo a la posición
    // tEdge: 1 cuando está claramente dentro del borde y 0 cuando está fuera.
    vec4 color = mix(borderColor, backgroundColor, tBackground);
    
    // Aplicar el anti-aliasing al alpha (opcional)
    float alpha = tEdge;
    
    // Si estamos fuera de la forma, descartamos el fragmento
    if(d > aa) {
        discard;
    }
    
    outColor = vec4(color.rgb, color.a * alpha);
}

// SDF para rectángulo con esquinas redondeadas
float sdfRoundRectClaude3(vec2 p, vec2 halfSize, float radius) {
    // Limitamos el radio para que no sea mayor que la mitad del tamaño más pequeño
    radius = min(radius, min(halfSize.x, halfSize.y));
    vec2 d = abs(p) - halfSize + vec2(radius);
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0) - radius;
}

// Función para determinar qué borde se debe colorear
vec4 getBorderColor(vec2 p, vec2 halfSize, float cornerRadius, vec4 topColor, vec4 rightColor, vec4 bottomColor, vec4 leftColor) {
    // Normalizar la posición para trabajar con el rango [-1, 1]
    vec2 normPos = p / (halfSize - vec2(cornerRadius));
    
    // Determinar en cuál de los cuatro cuadrantes estamos
    bool topHalf = normPos.y < 0.0;
    bool rightHalf = normPos.x > 0.0;
    
    // Valores absolutos para comparación
    float absX = abs(normPos.x);
    float absY = abs(normPos.y);
    
    // Determinar qué borde está más cerca
    if (absX > absY) {
        // Borde horizontal (izquierdo o derecho)
        return rightHalf ? rightColor : leftColor;
    } else {
        // Borde vertical (superior o inferior)
        return topHalf ? bottomColor : topColor;
    }
}

void mainClaude3() {
    vec2 rectSize = push.size;  // Tamaño real en píxeles
    float borderWidth = push.borderWidth;            // Grosor del borde en píxeles
    float cornerRadius = push.borderRadius;           // Radio de las esquinas redondeadas
    
    // Definir colores para cada borde
    //vec4 topColor = vec4(1.0, 0.0, 0.0, 1.0);     // Rojo
    //vec4 rightColor = vec4(0.0, 1.0, 0.0, 1.0);   // Verde
    //vec4 bottomColor = vec4(0.0, 0.0, 1.0, 1.0);  // Azul
    //vec4 leftColor = vec4(1.0, 1.0, 0.0, 1.0);    // Amarillo
    
    vec4 topColor = push.borderColor;     // Rojo
    vec4 rightColor = push.borderColor;   // Verde
    vec4 bottomColor = push.borderColor;  // Azul
    vec4 leftColor = push.borderColor;    // Amarillo

    // Color del fondo (interior del rectángulo)
    vec4 backgroundColor = push.backgroundColor; // Gris oscuro
    
    // Convertir UV a píxeles
    vec2 p_pixel = fragTexCoords * rectSize;
    
    // Centro en píxeles
    vec2 center = rectSize / 2.0;
    
    // Posición relativa al centro
    vec2 p_relative = p_pixel - center;
    
    // Calcular SDF para rectángulo con esquinas redondeadas
    float d = sdfRoundRectClaude3(p_relative, center, cornerRadius);
    
    // Calcular factor de anti-aliasing basado en la derivada
    vec2 grad = vec2(dFdx(d), dFdy(d));
    float aa = length(grad) * 0.5; // Factor de anti-aliasing adaptativo
    aa = max(aa, 0.001);
    
    // Suavizado para el borde exterior
    float outerEdge = smoothstep(0.0, aa, -d);
    
    // Suavizado para el borde interior
    float innerEdge = smoothstep(0.0, aa, -(d + borderWidth));
    
    // Determinar el color del borde basado en la posición
    vec4 borderColor = getBorderColor(p_relative, center, cornerRadius, topColor, rightColor, bottomColor, leftColor);
    
    // Mezclar colores con anti-aliasing
    vec4 finalColor = mix(vec4(0.0, 0.0, 0.0, 0.0), borderColor, outerEdge);
    finalColor = mix(finalColor, backgroundColor, innerEdge);
    
    // Asignar color final
    outColor = finalColor;
}

float sdfRoundRectGPT4(vec2 p, vec2 halfSize, float radius) {
    vec2 d = abs(p) - halfSize + vec2(radius);
    return length(max(d, vec2(0.0))) + min(max(d.x, d.y), 0.0) - radius;
}
void mainGPT4() {
    // Tamaño del rectángulo en píxeles
    vec2 rectSize = vec2(600.0, 300.0);
    vec2 halfSize = rectSize * 0.5;
    float cornerRadius = 20.0;
    
    // Definición de grosor y color para cada borde
    float borderLeft   = 10.0;
    float borderRight  = 15.0;
    float borderTop    = 20.0;
    float borderBottom = 8.0;
    
    vec4 borderColorLeft   = vec4(1.0, 0.0, 0.0, 1.0);   // Rojo
    vec4 borderColorRight  = vec4(0.0, 1.0, 0.0, 1.0);   // Verde
    vec4 borderColorTop    = vec4(0.0, 0.0, 1.0, 1.0);   // Azul
    vec4 borderColorBottom = vec4(1.0, 1.0, 0.0, 1.0);   // Amarillo
    
    vec4 backgroundColor = vec4(0.0, 0.0, 0.0, 1.0);        // Interior
    
    // Convertir UV a coordenadas en píxeles
    vec2 p_pixel = fragTexCoords * rectSize;
    
    // Calcular la posición relativa al centro
    vec2 center = halfSize;
    vec2 p_relative = p_pixel - center;
    
    // Calculamos el SDF para el rectángulo interior (donde no se pinta borde)
    // Se define el rectángulo interior restando, en cada dirección, el grosor respectivo
    vec2 innerHalfSize = vec2(halfSize.x - max(borderLeft, borderRight),
                                halfSize.y - max(borderTop, borderBottom));
    float d = sdfRoundRectGPT4(p_relative, innerHalfSize, cornerRadius);
    
    // Obtenemos un ancho de anti-aliasing en función de la derivada de d
    float aa = fwidth(d);
    
    // Si el fragmento está claramente dentro del área interior, pinta el fondo
    if(d < -aa) {
        outColor = backgroundColor;
        return;
    }
    
    // Calcular distancias a cada borde (en coordenadas de píxel)
    float dL = p_pixel.x;
    float dR = rectSize.x - p_pixel.x;
    float dT = p_pixel.y;
    float dB = rectSize.y - p_pixel.y;
    
    // Para cada borde, determinamos un factor de influencia (entre 0 y 1)
    // Usamos smoothstep para suavizar la transición del borde con anti-aliasing
    float wL = 1.0 - smoothstep(borderLeft - aa, borderLeft + aa, dL);
    float wR = 1.0 - smoothstep(borderRight - aa, borderRight + aa, dR);
    float wT = 1.0 - smoothstep(borderTop - aa, borderTop + aa, dT);
    float wB = 1.0 - smoothstep(borderBottom - aa, borderBottom + aa, dB);
    
    // Sumar los pesos (en las zonas centrales de cada borde, uno de ellos predominará;
    // en las esquinas se mezclarán en función de la cercanía a cada borde)
    float totalWeight = wL + wR + wT + wB;
    
    // Si no hay influencia de ningún borde, estamos en el interior
    if(totalWeight < 0.01) {
        outColor = backgroundColor;
        return;
    }
    
    // Mezclar los colores de los bordes en función de sus pesos
    vec4 mixedBorderColor = (wL * borderColorLeft +
                             wR * borderColorRight +
                             wT * borderColorTop +
                             wB * borderColorBottom) / totalWeight;
    
    // Finalmente, usar smoothstep sobre d para definir la opacidad del borde
    float borderAlpha = smoothstep(aa, -aa, d);
    
    outColor = vec4(mixedBorderColor.rgb, mixedBorderColor.a * borderAlpha);
}

// Función SDF para un rectángulo con esquinas redondeadas
float sdfRoundRectGrok4(vec2 p, vec2 halfSize, float radius) {
    vec2 d = abs(p) - halfSize + vec2(radius);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - radius;
}

void mainGrok4() {
    // Tamaño del rectángulo en píxeles
    vec2 rectSize = vec2(600.0, 300.0);
    float borderWidth = 10.0;    // Grosor del borde
    float cornerRadius = 20.0;   // Radio de las esquinas
    float aaWidth = 1.0;         // Ancho del suavizado

    // Colores de los bordes
    vec4 borderColorTop = vec4(1.0, 0.0, 0.0, 1.0);    // Rojo (superior)
    vec4 borderColorBottom = vec4(0.0, 1.0, 0.0, 1.0); // Verde (inferior)
    vec4 borderColorLeft = vec4(0.0, 0.0, 1.0, 1.0);   // Azul (izquierdo)
    vec4 borderColorRight = vec4(1.0, 1.0, 0.0, 1.0);  // Amarillo (derecho)

    // Color del fondo del rectángulo
    vec4 backgroundColor = vec4(0.5, 0.5, 0.5, 1.0);   // Gris

    // Coordenadas en píxeles (asumiendo fragTexCoords está en [0,1])
    vec2 p_pixel = gl_FragCoord.xy * rectSize / rectSize; // Ajusta según tu sistema de coordenadas
    vec2 center = rectSize / 2.0;
    vec2 p_relative = p_pixel - center;

    // Calcular la distancia SDF
    float d = sdfRoundRectGrok4(p_relative, center - vec2(cornerRadius), cornerRadius);

    // Determinar la región del fragmento
    vec2 ap = abs(p_relative);
    bool isTop = ap.y > center.y - cornerRadius && p_relative.y > 0.0;
    bool isBottom = ap.y > center.y - cornerRadius && p_relative.y < 0.0;
    bool isLeft = ap.x > center.x - cornerRadius && p_relative.x < 0.0;
    bool isRight = ap.x > center.x - cornerRadius && p_relative.x > 0.0;

    // Asignar el color del borde según la región
    vec4 borderColor;
    if (isTop) {
        borderColor = borderColorTop;
    } else if (isBottom) {
        borderColor = borderColorBottom;
    } else if (isLeft) {
        borderColor = borderColorLeft;
    } else if (isRight) {
        borderColor = borderColorRight;
    } else {
        // En las esquinas, mezclar colores de bordes adyacentes
        float mixFactor = 0.5; // Proporción de mezcla (ajustable)
        if (p_relative.x > 0.0 && p_relative.y > 0.0) {
            // Esquina superior derecha
            borderColor = mix(borderColorTop, borderColorRight, mixFactor);
        } else if (p_relative.x < 0.0 && p_relative.y > 0.0) {
            // Esquina superior izquierda
            borderColor = mix(borderColorTop, borderColorLeft, mixFactor);
        } else if (p_relative.x > 0.0 && p_relative.y < 0.0) {
            // Esquina inferior derecha
            borderColor = mix(borderColorBottom, borderColorRight, mixFactor);
        } else {
            // Esquina inferior izquierda
            borderColor = mix(borderColorBottom, borderColorLeft, mixFactor);
        }
    }

    // Suavizado del borde exterior
    float outerEdge = smoothstep(-aaWidth, aaWidth, d);

    // Suavizado entre el borde y el interior
    float innerEdge = smoothstep(-borderWidth - aaWidth, -borderWidth + aaWidth, d);

    // Asignar el color final
    if (d < -borderWidth) {
        // Dentro del rectángulo
        outColor = backgroundColor;
    } else if (d < 0.0) {
        // En el borde, transición suave hacia el interior
        outColor = mix(borderColor, backgroundColor, innerEdge);
    } else {
        // Fuera del rectángulo, transición hacia transparente
        outColor = mix(borderColor, vec4(0.0, 0.0, 0.0, 0.0), outerEdge);
    }
}


// SDF para rectángulo con esquinas redondeadas
float sdfRoundRectClaude4(vec2 p, vec2 halfSize, float radius) {
    // Limitamos el radio para que no sea mayor que la mitad del tamaño más pequeño
    radius = min(radius, min(halfSize.x, halfSize.y));
    vec2 d = abs(p) - halfSize + vec2(radius);
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0) - radius;
}

// Función para determinar qué borde se debe colorear con transición proporcional en esquinas
vec4 getBorderColorClaude4(vec2 p, vec2 halfSize, float cornerRadius, vec4 topColor, vec4 rightColor, vec4 bottomColor, vec4 leftColor) {
    // Determinar si estamos dentro de la región de una esquina
    vec2 cornerTest = abs(p) - (halfSize - vec2(cornerRadius));
    bool inCorner = cornerTest.x > 0.0 && cornerTest.y > 0.0;
    
    // Determinar en qué cuadrante estamos
    bool topHalf = p.y < 0.0;
    bool rightHalf = p.x > 0.0;
    
    if (inCorner) {
        // Estamos en una esquina, usar ángulo para una transición proporcional
        float angle = atan(p.y, p.x); // Rango de -PI a PI
        
        // Normalizar ángulo al rango [0, 2*PI]
        if (angle < 0.0) angle += 2.0 * 3.14159265359;
        
        // Calculamos en qué esquina estamos
        if (p.x > 0.0 && p.y < 0.0) {
            // Esquina superior derecha: mezcla entre top y right
            float t = (angle) / (3.14159265359 * 0.5); // 0 = right, 1 = top
            return mix(rightColor, topColor, t);
        } else if (p.x > 0.0 && p.y > 0.0) {
            // Esquina inferior derecha: mezcla entre right y bottom
            float t = (angle - 3.14159265359 * 0.5) / (3.14159265359 * 0.5); // 0 = right, 1 = bottom
            return mix(bottomColor, rightColor, t);
        } else if (p.x < 0.0 && p.y > 0.0) {
            // Esquina inferior izquierda: mezcla entre bottom y left
            float t = (angle - 3.14159265359) / (3.14159265359 * 0.5); // 0 = bottom, 1 = left
            return mix(leftColor, bottomColor, t);
        } else {
            // Esquina superior izquierda: mezcla entre left y top
            float t = (angle - 3.14159265359 * 1.5) / (3.14159265359 * 0.5); // 0 = left, 1 = top
            return mix(topColor, leftColor, t);
        }
    } else {
        // No estamos en una esquina, usar simple comparación x vs y
        if (abs(p.x) > abs(p.y)) {
            // Borde horizontal (izquierdo o derecho)
            return p.x > 0.0 ? rightColor : leftColor;
        } else {
            // Borde vertical (superior o inferior)
            return p.y > 0.0 ? bottomColor : topColor;
        }
    }
}

void mainClaude4() {
    vec2 rectSize = vec2(600.0, 300.0);  // Tamaño real en píxeles
    float borderWidth = 10.0;            // Grosor del borde en píxeles
    float cornerRadius = 40.0;           // Radio de las esquinas redondeadas
    
    // Definir colores para cada borde
    vec4 topColor = vec4(1.0, 0.0, 0.0, 1.0);     // Rojo
    vec4 rightColor = vec4(0.0, 1.0, 0.0, 1.0);   // Verde
    vec4 bottomColor = vec4(0.0, 0.0, 1.0, 1.0);  // Azul
    vec4 leftColor = vec4(1.0, 1.0, 0.0, 1.0);    // Amarillo
    
    // Color del fondo (interior del rectángulo)
    vec4 backgroundColor = vec4(0.2, 0.2, 0.2, 1.0); // Gris oscuro
    
    // Convertir UV a píxeles
    vec2 p_pixel = fragTexCoords * rectSize;
    
    // Centro en píxeles
    vec2 center = rectSize / 2.0;
    
    // Posición relativa al centro
    vec2 p_relative = p_pixel - center;
    
    // Calcular SDF para rectángulo con esquinas redondeadas
    float d = sdfRoundRectClaude4(p_relative, center, cornerRadius);
    
    // Calcular factor de anti-aliasing basado en la derivada
    vec2 grad = vec2(dFdx(d), dFdy(d));
    float aa = length(grad) * 0.5; // Factor de anti-aliasing adaptativo
    aa = max(aa, 0.001);
    
    // Suavizado para el borde exterior
    float outerEdge = smoothstep(0.0, aa, -d);
    
    // Suavizado para el borde interior
    float innerEdge = smoothstep(0.0, aa, -(d + borderWidth));
    
    // Determinar el color del borde basado en la posición
    vec4 borderColor = getBorderColorClaude4(p_relative, center, cornerRadius, topColor, rightColor, bottomColor, leftColor);
    
    // Mezclar colores con anti-aliasing
    vec4 finalColor = mix(vec4(0.0, 0.0, 0.0, 0.0), borderColor, outerEdge);
    finalColor = mix(finalColor, backgroundColor, innerEdge);
    
    // Asignar color final
    outColor = finalColor;
}

vec4 getBorderColorGPT5(vec2 p_relative, float cornerRadius,
                      vec4 topColor, vec4 rightColor, vec4 bottomColor, vec4 leftColor)
{
    // Calcula los valores absolutos para determinar la proporción
    float absX = abs(p_relative.x);
    float absY = abs(p_relative.y);
    
    // Inicializamos el color
    vec4 color = vec4(0.0);
    
    // Si la diferencia entre las componentes es considerable, se está lejos de la zona de mezcla (es decir, en el centro de un lado)
    if(absX > absY + cornerRadius) {
        // Se está en la zona derecha o izquierda
        color = (p_relative.x > 0.0) ? rightColor : leftColor;
    } else if(absY > absX + cornerRadius) {
        // Se está en la zona superior o inferior
        color = (p_relative.y > 0.0) ? topColor : bottomColor;
    } else {
        // En la zona de esquina se mezcla proporcionalmente:
        // Calculamos un factor de mezcla t: cuanto mayor sea la componente horizontal,
        // mayor la influencia del color lateral.
        float t = absX / (absX + absY);
        if(p_relative.x < 0.0 && p_relative.y > 0.0) {
            // Esquina superior izquierda: mezcla entre top y left
            color = mix(topColor, leftColor, t);
        } else if(p_relative.x > 0.0 && p_relative.y > 0.0) {
            // Esquina superior derecha: mezcla entre top y right
            color = mix(topColor, rightColor, t);
        } else if(p_relative.x > 0.0 && p_relative.y < 0.0) {
            // Esquina inferior derecha: mezcla entre bottom y right
            color = mix(bottomColor, rightColor, t);
        } else {
            // Esquina inferior izquierda: mezcla entre bottom y left
            color = mix(bottomColor, leftColor, t);
        }
    }
    return color;
}

// Shader completo con anti-aliasing y bordes con colores distintos
void mainGPT5() {
    vec2 rectSize = vec2(600.0, 300.0);  // Tamaño real en píxeles
    float borderWidth = 10.0;            // Grosor del borde en píxeles
    float cornerRadius = 10.0;           // Radio de las esquinas redondeadas
    
    // Definir colores para cada borde
    vec4 topColor = vec4(1.0, 0.0, 0.0, 1.0);     // Rojo
    vec4 rightColor = vec4(0.0, 1.0, 0.0, 1.0);   // Verde
    vec4 bottomColor = vec4(0.0, 0.0, 1.0, 1.0);  // Azul
    vec4 leftColor = vec4(1.0, 1.0, 0.0, 1.0);    // Amarillo
    
    // Color del fondo (interior del rectángulo)
    vec4 backgroundColor = vec4(0.2, 0.2, 0.2, 1.0); // Gris oscuro
    
    // Convertir UV a píxeles
    vec2 p_pixel = fragTexCoords * rectSize;
    
    // Centro en píxeles
    vec2 center = rectSize / 2.0;
    
    // Posición relativa al centro
    vec2 p_relative = p_pixel - center;
    
    // Calcular SDF para rectángulo con esquinas redondeadas
    float d = sdfRoundRectClaude3(p_relative, center, cornerRadius);
    
    // Calcular factor de anti-aliasing basado en la derivada
    vec2 grad = vec2(dFdx(d), dFdy(d));
    float aa = length(grad) * 0.5; // Factor de anti-aliasing adaptativo
    aa = max(aa, 0.001);
    
    // Suavizado para el borde exterior
    float outerEdge = smoothstep(0.0, aa, -d);
    
    // Suavizado para el borde interior
    float innerEdge = smoothstep(0.0, aa, -(d + borderWidth));
    
    // Determinar el color del borde basado en la posición relativa
    vec4 borderColor = getBorderColorGPT5(p_relative, cornerRadius, topColor, rightColor, bottomColor, leftColor);
    
    // Mezclar colores con anti-aliasing:
    // outerEdge determina la opacidad del borde y innerEdge el recorte hacia el fondo.
    vec4 finalColor = mix(vec4(0.0), borderColor, outerEdge);
    finalColor = mix(finalColor, backgroundColor, innerEdge);
    
    // Asignar color final
    outColor = finalColor;
}


// SDF para rectángulo con esquinas redondeadas
float sdfRoundRectClaude5(vec2 p, vec2 halfSize, float radius) {
    // Limitamos el radio para que no sea mayor que la mitad del tamaño más pequeño
    radius = min(radius, min(halfSize.x, halfSize.y));
    vec2 d = abs(p) - halfSize + vec2(radius);
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0) - radius;
}

// Función para determinar el color del borde basado en la posición
vec4 getBorderColorClaude5(vec2 p, vec2 halfSize, float cornerRadius, vec4 topColor, vec4 rightColor, vec4 bottomColor, vec4 leftColor) {
    // Calculamos qué tan lejos estamos de cada borde
    vec2 distToBorder = halfSize - abs(p);
    
    // Identificamos si estamos en una región de esquina
    vec2 cornerDist = abs(p) - (halfSize - vec2(cornerRadius));
    bool inCorner = cornerDist.x > 0.0 && cornerDist.y > 0.0;
    
    if (inCorner) {
        // Estamos en una región de esquina
        // Calculamos la posición normalizada en la esquina
        vec2 cornerCenter;
        cornerCenter.x = sign(p.x) * (halfSize.x - cornerRadius);
        cornerCenter.y = sign(p.y) * (halfSize.y - cornerRadius);
        
        // Vector desde el centro de la esquina hasta el punto actual
        vec2 cornerVector = p - cornerCenter;
        
        // Ángulo del vector (en radianes)
        float angle = atan(cornerVector.y, cornerVector.x);
        
        // Determinamos qué esquina es
        if (p.x > 0.0 && p.y < 0.0) {
            // Esquina superior derecha: transición desde right (0°) a top (-90°)
            float t = clamp((angle + 1.5708) / 1.5708, 0.0, 1.0); // 0 = right, 1 = top
            return mix(rightColor, topColor, t);
        } else if (p.x > 0.0 && p.y > 0.0) {
            // Esquina inferior derecha: transición desde bottom (90°) a right (0°)
            float t = clamp((angle) / 1.5708, 0.0, 1.0); // 0 = right, 1 = bottom
            return mix(rightColor, bottomColor, t);
        } else if (p.x < 0.0 && p.y > 0.0) {
            // Esquina inferior izquierda: transición desde left (180°) a bottom (90°)
            float t = clamp((angle - 1.5708) / 1.5708, 0.0, 1.0); // 0 = bottom, 1 = left
            return mix(bottomColor, leftColor, t);
        } else {
            // Esquina superior izquierda: transición desde top (-90° o 270°) a left (180°)
            float t = clamp((angle + 4.7124) / 1.5708, 0.0, 1.0); // 0 = left, 1 = top
            if (t > 1.0) t = t - 1.0; // Corregir el desbordamiento
            return mix(leftColor, topColor, t);
        }
    } else {
        // Estamos en un borde recto
        if (distToBorder.x < distToBorder.y) {
            // Borde vertical (izquierdo o derecho)
            return p.x > 0.0 ? rightColor : leftColor;
        } else {
            // Borde horizontal (superior o inferior)
            return p.y > 0.0 ? bottomColor : topColor;
        }
    }
}

void mainClaude5() {
    vec2 rectSize = vec2(600.0, 300.0);  // Tamaño real en píxeles
    float borderWidth = 20.0;            // Grosor del borde en píxeles
    float cornerRadius = 20.0;           // Radio de las esquinas redondeadas
    
    // Definir colores para cada borde
    vec4 topColor = vec4(1.0, 0.0, 0.0, 1.0);     // Rojo
    vec4 rightColor = vec4(0.0, 1.0, 0.0, 1.0);   // Verde
    vec4 bottomColor = vec4(0.0, 0.0, 1.0, 1.0);  // Azul
    vec4 leftColor = vec4(1.0, 1.0, 0.0, 1.0);    // Amarillo
    
    // Color del fondo (interior del rectángulo)
    vec4 backgroundColor = vec4(0.2, 0.2, 0.2, 1.0); // Gris oscuro
    
    // Convertir UV a píxeles
    vec2 p_pixel = fragTexCoords * rectSize;
    
    // Centro en píxeles
    vec2 center = rectSize / 2.0;
    
    // Posición relativa al centro
    vec2 p_relative = p_pixel - center;
    
    // Calcular SDF para rectángulo con esquinas redondeadas
    float d = sdfRoundRectClaude5(p_relative, center, cornerRadius);
    
    // Calcular factor de anti-aliasing basado en la derivada
    vec2 grad = vec2(dFdx(d), dFdy(d));
    float aa = length(grad) * 0.5; // Factor de anti-aliasing adaptativo
    aa = max(aa, 0.001);
    aa = 0.0f;
    // Suavizado para el borde exterior
    float outerEdge = smoothstep(0.0, aa, -d);
    
    // Suavizado para el borde interior
    float innerEdge = smoothstep(0.0, aa, -(d + borderWidth));
    
    // Determinar el color del borde basado en la posición
    vec4 borderColor = getBorderColorClaude5(p_relative, center, cornerRadius, topColor, rightColor, bottomColor, leftColor);
    
    // Mezclar colores con anti-aliasing
    vec4 finalColor = mix(vec4(0.0, 0.0, 0.0, 0.0), borderColor, outerEdge);
    finalColor = mix(finalColor, backgroundColor, innerEdge);
    
    // Asignar color final
    outColor = finalColor;
}
void main() {
    int n = 5;

    if(n == 0) {
        
        mainBeautiful();
    } else if(n== 1) {
        mainGrok1();
    } else if (n==2){
        mainDeep1();
    }else if (n==3){
        mainDeep2();
    }else if (n==4){
        mainBeautiful2();
    } else if(n == 5) {
        if(push.useTexture == 1){
            vec4 textureColor = texture(spriteTexture, fragTexCoords);
           outColor = textureColor * vec4(fragColor, 1.0); // Multiplicamos color de textura por color de vértice
        }else{
            mainClaude3();
            //outColor = push.color; // Multiplicamos color de textura por color uniforme
        }
       
    }


}