#version 450

#define MAX_BONES 64  // Número máximo de huesos

// Atributos de entrada del vértice
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec3 inColor;
layout (location = 4) in ivec4 inBoneIDs; // Índices de huesos
layout (location = 5) in vec4 inWeights; // Pesos de huesos

// Uniformes
layout (std140, set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 MVP;
    mat4 lightView;
    mat4 lightProj;
   
    mat4 boneTransforms[MAX_BONES]; // Transformaciones de los huesos
     vec3 outColor;
    vec3 lightPos;
     float lightIntensity;
    
    vec3 lightColor;
    vec3 cameraPos;
    
   
    
    
    
} ubo;

// Salidas del shader
layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec3 outViewVec;
layout (location = 3) out vec3 outLightVec;
layout (location = 4) out vec4 outShadowCoord;

// Matriz de sesgo (bias) para la proyección de sombras
const mat4 biasMat = mat4( 
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.5, 0.5, 0.0, 1.0 );

// Función para aplicar la transformación de huesos al vértice

 vec3 outColor2 = vec3(0.9, 0.4, 0.4);
vec4 applyBoneTransformations(vec3 position) {
    
    vec4 transformedPosition = vec4(0.0);
    for (int i = 0; i < 4; ++i) {

        if (inWeights[i] > 0.0) { // Solo aplica huesos relevantes
            transformedPosition += inWeights[i] * (ubo.boneTransforms[inBoneIDs[i]] * vec4(position, 1.0));
          
        } else {
        
            outColor2 = vec3(0.1, 0.0, 0.9);
        }
    }
    return transformedPosition;
}

vec3 applyBoneTransformationsToNormal(vec3 normal) {
    vec3 transformedNormal = vec3(0.0);
    for (int i = 0; i < 4; ++i) {
        if (inWeights[i] > 0.0) {
            transformedNormal += inWeights[i] * mat3(ubo.boneTransforms[inBoneIDs[i]]) * normal;
        }
    }
    return transformedNormal;
}


void mainBone() {
    // Calcular la transformación por huesos
    mat4 boneTransform = 
        inWeights.x * ubo.boneTransforms[inBoneIDs.x] +
        inWeights.y * ubo.boneTransforms[inBoneIDs.y] +
        inWeights.z * ubo.boneTransforms[inBoneIDs.z] +
        inWeights.w * ubo.boneTransforms[inBoneIDs.w];

    // Aplicar transformación al vértice
    vec4 worldPos = ubo.model * boneTransform * vec4(inPos, 1.0);



    outNormal = mat3(transpose(inverse(ubo.model))) * inNormal; // Transformar normal al espacio mundial

    // Calcular la posición final en clip space
    gl_Position = ubo.projection * ubo.view * worldPos;
    gl_Position = ubo.projection * ubo.view * ubo.model * boneTransform * vec4(inPos, 1.0);
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(inPos.xyz, 1.0);

    // Salida de colores y vectores
    outColor = inColor;
    outViewVec = normalize(ubo.cameraPos - vec3(worldPos)); // Vector hacia la cámara
    outLightVec = normalize(ubo.lightPos - vec3(worldPos)); // Vector hacia la luz

    // Coordenadas para sombras (transformación al espacio de la luz)
    outShadowCoord = biasMat * ubo.lightProj * ubo.lightView * worldPos;
}


void mainBasic() 
{
	
     vec4 animatedPos = applyBoneTransformations(inPos);
    
    outColor = inColor;
    outColor = outColor2;
	outNormal = inNormal;
    
	gl_Position = ubo.projection * ubo.view * ubo.model * vec4(inPos.xyz, 1.0);
	
    vec4 pos = ubo.model * vec4(inPos, 1.0);
    outNormal = mat3(ubo.model) * inNormal;
    outLightVec = normalize(ubo.lightPos.xyz - inPos);
    outViewVec = -pos.xyz;			
	mat4 lightSpace = ubo.lightProj * ubo.lightView;//ubo.lightSpace;
	outShadowCoord = ( biasMat * lightSpace * ubo.model ) * vec4(inPos, 1.0);
    
    //outColor = ubo.boneTransforms[0][0].xyz; // Solo rojo
    //outColor = vec3(ubo.boneTransforms[0][0].x, ubo.boneTransforms[0][0].y, ubo.boneTransforms[0][0].z);

    //outColor = vec3(ubo.boneTransforms[0][0].x, ubo.boneTransforms[0][0].y, ubo.boneTransforms[0][0].z);

    //outColor = ubo.outColor;
    //outColor = vec3(0.1, 0.6, 0.6);
    //outColor = ubo.lightPos;
}


void main2() 
{
    
    // Aplica la transformación de huesos a la posición y al normal
    vec4 animatedPos = applyBoneTransformations(inPos);
    vec3 animatedNormal = applyBoneTransformationsToNormal(inNormal);

    // Calcula la posición transformada por el modelo
    vec4 pos = ubo.model * animatedPos;

    // Calcula los vectores de iluminación y vista
    outNormal = mat3(ubo.model) * animatedNormal;
    outLightVec = normalize(ubo.lightPos.xyz - pos.xyz);
    outViewVec = -pos.xyz;

    // Proyección de la sombra y cálculo de coordenadas de sombra
    mat4 lightSpace = ubo.lightProj * ubo.lightView;
    outShadowCoord = biasMat * lightSpace * pos;

    // Proyección final del vértice
    gl_Position = ubo.projection * ubo.view * pos;

    // Asigna el color del vértice
    outColor = inColor;
    
}


void main() {
    int band = 0;


    if (band == 0) {
        //mainBone();
        main2();

    }
    else {
        mainBasic();
    }

}