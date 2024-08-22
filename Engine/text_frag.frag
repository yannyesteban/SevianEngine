#version 450

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D textSampler;

void main() {
    //vec4 sampled = texture(textSampler, textSampler);
    //outColor = vec4(1.0, 0.0, 1.0, sampled.r); // Ajusta el color según el canal alfa de la textura
    float edgeValue = 0.1;
    float thickness = 0.5;
    float softness = 0.0;
    vec4 outline_color = vec4(0.8, 0.6, 0.0, 1.0);
    float outline_thickness = 0.0;
    float outline_softness = 0.1;
    vec4 color = vec4(0.74, 0.2, 0.5, 1.0); 

    float distance = texture(textSampler, fragTexCoord).r;
    float outline = smoothstep(outline_thickness - outline_softness, outline_thickness + outline_softness, distance); 

    //float alpha = smoothstep(edgeValue - 0.01, edgeValue + 0.01, distance);
    //float alpha = smoothstep(1.0 - thickness, 1.0 - thickness, distance);
    float alpha = smoothstep(1.0 - thickness - softness, 1.0 - thickness + softness, distance);
    //outColor = vec4(vec3(0.45, 0.8, 0.500), alpha);
    //outColor = vec4(mix(outline_color.rgb, color.rgb, outline), alpha);


    //float distance = texture(sdfTexture, fragTexCoord).r;
    alpha = smoothstep(0.5 - fwidth(distance), 0.5 + fwidth(distance), distance);
    outColor = vec4(1.0, 1.0, 1.0, alpha); // Blanco con opacidad basada en SDF
}
