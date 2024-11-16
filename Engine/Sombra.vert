#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec3 inColor;


layout (binding = 0) uniform UBO 
{
	mat4 projection;
	mat4 view;
	mat4 model;
	mat4 lightSpace;
	vec4 lightPos;
	float zNear;
	float zFar;
} ubo1;



layout (set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 MVP;
    mat4 lightView;  // Añadido: Matriz de vista desde la luz
    mat4 lightProj;  // Añadido: Matriz de proyección desde la luz
    vec3 lightPos;
    vec3 lightColor;
    vec3 cameraPos;
    float lightIntensity;
} ubo;


layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec3 outViewVec;
layout (location = 3) out vec3 outLightVec;
layout (location = 4) out vec4 outShadowCoord;



const mat4 biasMat = mat4( 
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.5, 0.5, 0.0, 1.0 );

void main() 
{
	outColor = inColor;
	outNormal = inNormal;

	gl_Position = ubo.projection * ubo.view * ubo.model * vec4(inPos.xyz, 1.0);
	
    vec4 pos = ubo.model * vec4(inPos, 1.0);
    outNormal = mat3(ubo.model) * inNormal;
    outLightVec = normalize(ubo.lightPos.xyz - inPos);
    outViewVec = -pos.xyz;			
	mat4 lightSpace = ubo.lightProj * ubo.lightView;//ubo.lightSpace;
	outShadowCoord = ( biasMat * lightSpace * ubo.model ) * vec4(inPos, 1.0);	
}

