#version 460
#extension GL_KHR_vulkan_glsl : enable

layout (set = 1, binding = 0) uniform sampler2D texSampler;   // Textura del objeto

layout (set = 1, binding = 1) uniform sampler2DShadow shadowMap;     // Mapa de sombras
layout (set = 1, binding = 1) uniform sampler2D shadowMap2;    // Mapa de sombras
layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inViewVec;
layout (location = 3) in vec3 inLightVec;
layout (location = 4) in vec4 inShadowCoord;
layout (location = 5) in vec2 inUV; // Coordenadas UV para la textura
layout (location = 6) in float lightIntensity;
layout( location = 7) in vec3 vert_light;

layout (constant_id = 0) const int enablePCF = 1;

layout (location = 0) out vec4 outColor;

#define ambient 0.5

// Función de textura proyectada (sin PCF)
float textureProj(vec4 shadowCoord, vec2 off)
{
    float shadow = 1.0;
    if (shadowCoord.z > -1.0 && shadowCoord.z < 1.0) {
        // Leer la profundidad del mapa de sombras usando las coordenadas + offset
        float dist = texture(shadowMap2, shadowCoord.st + off).r;
        // Si el valor de la profundidad del fragmento es mayor que la del mapa de sombras, está en sombra
        if (shadowCoord.w > 0.0 && dist < shadowCoord.z) {
            shadow = ambient;
        }
    }
    return shadow;
}

// Función para el filtrado PCF
float filterPCF(vec4 sc)
{
    ivec2 texDim = textureSize(shadowMap, 0); // Obtener las dimensiones de la textura de sombra
    float scale = 1.5;
    float dx = scale * 1.0 / float(texDim.x);  // Distancia en X
    float dy = scale * 1.0 / float(texDim.y);  // Distancia en Y

    float shadowFactor = 0.0;
    int count = 0;
    int range = 1;  // Rango de muestras para el filtrado

    // Iterar sobre el rango de muestras y calcular la sombra promedio
    for (int x = -range; x <= range; x++) {
        for (int y = -range; y <= range; y++) {
            shadowFactor += textureProj(sc, vec2(dx * x, dy * y));
            count++;
        }
    }
    return shadowFactor / float(count);
}

void main() 
{
    // Calcular la sombra según si se utiliza PCF o no
    float shadow = (enablePCF == 1) 
        ? filterPCF(inShadowCoord / inShadowCoord.w)  // Si PCF está habilitado, usar filtrado
        : textureProj(inShadowCoord / inShadowCoord.w, vec2(0.0));  // Sino, usar la comparación directa

    // Calcular iluminación difusa
    vec3 N = normalize(inNormal);    // Normal
    vec3 L = normalize(inLightVec);  // Vector de luz
    vec3 V = normalize(inViewVec);   // Vector de visión
    vec3 R = normalize(-reflect(L, N));  // Reflexión de la luz

    // Iluminación difusa
    vec3 diffuse = max(dot(N, L), ambient) * inColor;

    // Aplicar la sombra a la iluminación difusa y calcular el color final
    outColor = vec4(diffuse * shadow, 1.0);
}

void mainBetha() {
    // Convertir coordenadas de sombra a UV
    vec2 shadowCoord = inShadowCoord.xy / inShadowCoord.w;
    
    // Leer el valor de la textura de sombra
    float shadowDepth = texture(shadowMap2, shadowCoord).r;
    
    // Compara la profundidad
    float shadow = (shadowDepth < inShadowCoord.z) ? 1.0 : 0.0;
    
    outColor = vec4(vec3(1.0 - shadow), 1.0);
}

float calculateShadowPCF(vec3 shadow_coords) {
    float shadowFactor = 0.0;
    int samples = 4;  // Número de muestras (aumenta para sombras más suaves)
    float offset = 1.0 / 1024.0;  // Tamaño del offset, ajusta según la resolución del mapa de sombras

    // Realiza una media de varias muestras alrededor de las coordenadas de sombra
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            vec3 sampleCoord = shadow_coords;
            sampleCoord.xy += vec2(x, y) * offset;
            shadowFactor += texture(shadowMap, sampleCoord);
        }
    }
    
    // Promedia los valores de sombra
    shadowFactor /= float((samples + 1) * (samples + 1));
    return shadowFactor;
}
/*
void main() {
    // Coordenadas normalizadas de sombra con un pequeño bias
    vec3 shadow_coords = inShadowCoord.xyz / inShadowCoord.w;
    shadow_coords.z -= 0.005;  // Ajuste para evitar "shadow acne"

    // Calcula el factor de sombra usando PCF
    float shadowFactor = calculateShadowPCF(shadow_coords);

    // Iluminación difusa
    vec3 normal_vector = normalize(inNormal);
    vec3 light_vector = normalize(inLightVec);
    float diffuse_term = max(dot(normal_vector, light_vector), 0.0);

    // Color base de la textura
    vec4 textureColor = texture(texSampler, inUV);

    // Color final combinado con sombra y luz difusa
    vec3 finalColor = (ambient + lightIntensity * diffuse_term * shadowFactor) * textureColor.rgb;

    // Color de salida
    outColor = vec4(finalColor, textureColor.a);
}
*/

void mainAlpha() {
    // Calcula las coordenadas normalizadas de sombra con bias
    vec3 shadow_coords = inShadowCoord.xyz / inShadowCoord.w;
    shadow_coords.z -= 0.005;  // Bias para evitar "shadow acne"

    // Calcula el factor de sombra usando el mapa de sombras
    float shadowFactor = texture(shadowMap, shadow_coords);

    // Calcula el término difuso de iluminación
    vec3 normal_vector = normalize(inNormal);
    vec3 light_vector = normalize(inLightVec);
    float diffuse_term = max(dot(normal_vector, light_vector), 0.0);

    // Obtén el color base de la textura en las coordenadas UV
    vec4 textureColor = texture(texSampler, inUV);

    // Calcula la combinación final de color con sombra, textura y luz
    vec3 finalColor = (ambient + lightIntensity * diffuse_term * shadowFactor) * textureColor.rgb;

    // Color de salida con opacidad completa
    

     if (shadowFactor < 1.0) {
      //finalColor = (ambient + lightIntensity * diffuse_term * shadowFactor) * textureColor.rgb;
    }

    if (shadowFactor < 1.0) {
        // Ajusta el color en las áreas sombreadas
        finalColor = (ambient * lightIntensity * diffuse_term * 0.5) * textureColor.rgb;
    } else {
        // Color normal sin sombra
        finalColor = (ambient + lightIntensity * diffuse_term) * textureColor.rgb;
    }
    
    outColor = vec4(finalColor, textureColor.a);
}

/*void main() {
    // Calcula las coordenadas normalizadas de sombra
    vec3 shadow_coords = inShadowCoord.xyz / inShadowCoord.w;

    // Muestreo del shadow map con comparación de profundidad
    float shadowFactor = texture(shadowMap, shadow_coords);

    // Usa el shadowFactor directamente para la intensidad de la sombra
    // 1.0 significa luz completa, y valores más bajos representan sombra
    float shadowIntensity = mix(ambient, 1.0, shadowFactor);

    // Color de salida solo basado en el factor de sombra
    outColor = vec4(vec3(shadowIntensity), 1.0);
}*/

void mainSombraTextura() {
    // Calcula las coordenadas normalizadas de sombra
    vec3 shadow_coords = inShadowCoord.xyz / inShadowCoord.w;
    
    // Muestreo del shadow map con comparación de profundidad
    float shadowFactor = texture(shadowMap, shadow_coords);
    
    // Factor difuso de iluminación
    vec3 normal_vector = normalize(inNormal);
    vec3 light_vector = normalize(inLightVec);
    float diffuse_term = max(dot(normal_vector, light_vector), 0.0);
    
    // Obtén el color de la textura en las coordenadas UV
    vec4 textureColor = texture(texSampler, inUV);
    
    // Combinación final de color, aplicando sombra y textura
    vec3 finalColor = (ambient + lightIntensity * diffuse_term * shadowFactor) * textureColor.rgb;
    
    //outColor = vec4(finalColor, textureColor.a);
    outColor = vec4(finalColor, 1.0);
}

void main02() {
    // Calcula las coordenadas normalizadas de sombra
    vec3 shadow_coords = inShadowCoord.xyz / inShadowCoord.w;
    
    // Muestreo del shadow map con comparación de profundidad
    float shadowFactor = texture(shadowMap, shadow_coords);
    
    // Factor difuso de iluminación
    vec3 normal_vector = normalize(inNormal);
    vec3 light_vector = normalize(inLightVec);
    float diffuse_term = max(dot(normal_vector, light_vector), 0.0);
    
    // Obtén el color de la textura en las coordenadas UV
    vec4 textureColor = texture(texSampler, inUV);
    
    // Combinación final de color, aplicando sombra y textura
    vec3 finalColor = (ambient + lightIntensity * diffuse_term * shadowFactor) * textureColor.rgb * inColor;
    
    outColor = vec4(finalColor, textureColor.a);
}

float textureProj_cero(vec4 shadowCoord, vec2 off)
{
	float shadow = 1.0;
	if ( shadowCoord.z > -1.0 && shadowCoord.z < 1.0 ) 
	{
		float dist = texture( shadowMap2, shadowCoord.st + off ).r;
		dist =  texture ( shadowMap, shadowCoord.xyz);
		if ( shadowCoord.w > 0.0 && dist < shadowCoord.z ) 
		{
			shadow = ambient;
		}
	}
	return shadow;
}

float filterPCF_cero(vec4 sc)
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
			shadowFactor += textureProj(sc, vec2(dx*x, dy*y));
			count++;
		}
	
	}
	return shadowFactor / count;
}

void main00() {
    // Coordenadas de sombra normalizadas
    vec3 shadow_coords = inShadowCoord.xyz / inShadowCoord.w;

    // Realiza la comparación de profundidad con sampler2DShadow (retorna 0 o 1)
    float shadow = texture(shadowMap, shadow_coords);

    // Factor difuso de iluminación
    vec3 normal_vector = normalize(inNormal);
    vec3 light_vector = normalize(vert_light);
    float diffuse_term = max(0.0, dot(normal_vector, light_vector));

    // Muestra el color de la textura con texSampler usando las coordenadas UV
    vec4 textureColor = texture(texSampler, inUV);

    // Color final con sombra, textura y luz
    outColor = shadow * vec4((diffuse_term * lightIntensity * textureColor.rgb) + ambient, 1.0);

    // Opción de color diferente si la sombra está presente
    if (shadow < 0.5) {
        //outColor = vec4(0.6, 0.2, 0.1, 1.0);
    }
}

void main0() {
  // Coordenadas de sombra normalizadas para la comparación
  vec3 shadow_coords = inShadowCoord.xyz / inShadowCoord.w;

  // Realiza la comparación de profundidad con sampler2DShadow (retorna 0 o 1)
  float shadow = texture(shadowMap, shadow_coords);
  
  if (shadow == 1.0) {
    shadow = 0.5;

  }



  // Factor de sombra aplicado a la iluminación difusa
  vec3 normal_vector = normalize(inNormal);
  vec3 light_vector = normalize(vert_light);
  float diffuse_term = max(0.0, dot(normal_vector, light_vector));

  outColor = shadow * vec4(diffuse_term) + vec4(ambient);
  /*
  // Opción de color diferente si la sombra está presente
  if (shadow < 0.5) {
    outColor = vec4(0.6, 0.2, 0.1, 1.0);
  }

  shadow = (enablePCF == 1) ? filterPCF(inShadowCoord / inShadowCoord.w) : textureProj(inShadowCoord / inShadowCoord.w, vec2(0.0));

	vec3 N = normalize(inNormal);
	vec3 L = normalize(inLightVec);
	vec3 V = normalize(inViewVec);
	vec3 R = normalize(-reflect(L, N));
	vec3 diffuse = max(dot(N, L), ambient) * inColor;

	outColor = vec4(diffuse * shadow, 1.0);

	outColor = vec4(vec3(0.2, 0.0, 0.6) * inColor , 1.0) ;
	*/
}

void main1() {
  float shadow = 1.0;
  vec4 shadow_coords = inShadowCoord / inShadowCoord.w;
  
  if( texture( shadowMap2, inShadowCoord.xy ).r < inShadowCoord.z - 0.005 ) {
    shadow = 0.5;
  }

  vec3 normal_vector = normalize( inNormal );
  vec3 light_vector = normalize( vert_light );
  float diffuse_term = max( 0.0, dot( normal_vector, light_vector ) );

  outColor = shadow * vec4( diffuse_term ) + 0.1;

  if (shadow <= 0.5) {
    outColor = vec4(0.6f, 0.2f, 0.1f, 1.0f);
    }
}

