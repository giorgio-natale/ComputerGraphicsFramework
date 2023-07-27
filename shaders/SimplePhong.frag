#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 fragUV;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 fragNorm;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform sampler2D tex;

layout(set = 2, binding = 0) uniform SimplePhongUniformBlock {
    vec3 eyePos;
} phongMaterial;

vec3 BRDF(vec3 V, vec3 N, vec3 L, vec3 Md, vec3 Ms, float gamma) {
    //vec3 V  - direction of the viewer
    //vec3 N  - normal vector to the surface
    //vec3 L  - light vector (from the light model)
    //vec3 Md - main color of the surface
    //vec3 Ms - specular color of the surface
    //float gamma - Exponent for power specular term

    //Lambert diffusion
    float lambertDimmer = clamp(dot(L, N), 0, 1);
    vec3 diffusionTerm = lambertDimmer * Md;
    //Phong reflection
    vec3 r = -reflect(L, N);
    float phongDimmer = lambertDimmer * pow(clamp(dot(V, r), 0, 1), gamma);
    vec3 specularTerm = phongDimmer * Ms;
    return diffusionTerm + specularTerm;
}

void main() {
    vec3 Norm = normalize(fragNorm);
    vec3 EyeDir = normalize(phongMaterial.eyePos - fragPos);

    vec3 lightDir = -normalize(vec3(-1, -1, 0));
    vec3 lightColor = vec3(1, 1, 1);

    vec3 DiffSpec = BRDF(EyeDir, Norm, lightDir, texture(tex, fragUV).rgb, vec3(1.0f), 25.0f);
    vec3 Ambient = texture(tex, fragUV).rgb * 0.15f;

    outColor = vec4(clamp(0.95 * (DiffSpec) * lightColor.rgb + Ambient,0.0,1.0), 1.0f);
}