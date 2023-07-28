#version 450
#extension GL_ARB_separate_shader_objects : enable
#define PI 3.1415926535897932384626433832795

#define DIRECT_LIGHTS_MAX 10
#define POINT_LIGHTS_MAX 10
#define SPOT_LIGHTS_MAX 10

struct DirectionalLightBlock{
    vec3 lightDir;
    vec4 lightColor;
};

struct PointLightBlock{
    vec3 lightPos;
    vec4 lightColor;
    float beta;
    float g;
};

struct SpotLightBlock{
    vec3 lightPos;
    vec3 lightDir;
    vec4 lightColor;
    float beta;
    float g;
    float cosOuter;
    float cosInner;
};

layout(set = 0, binding = 1) uniform GlobalLightUniformBlock{
    vec3 eyePosition;
    DirectionalLightBlock directLights[DIRECT_LIGHTS_MAX];
    PointLightBlock pointLights[POINT_LIGHTS_MAX];
    SpotLightBlock spotLights[SPOT_LIGHTS_MAX];
    int directLightsCount;
    int pointLightsCount;
    int spotLightsCount;
} lightsUniform;

layout(location = 0) in vec2 fragUV;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 fragNorm;
layout(location = 3) in vec4 fragTan;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform sampler2D tex;

layout(set = 2, binding = 0) uniform GlobalUniformBufferObject {
    vec3 eyePos;
} gubo;
layout(set = 2, binding = 1) uniform sampler2D normMap;
layout(set = 2, binding = 2) uniform sampler2D matMap;



float gHelper(float NdotA, float rho_2){
    return 2.0f / (1.0f + sqrt(1.0f + rho_2 * (1 - pow(NdotA, 2)) / (pow(NdotA, 2)) ));
}

vec3 BRDF(vec3 V, vec3 N, vec3 L, vec3 Md, float F0, float metallic, float roughness) {
    //vec3 V  - direction of the viewer
    //vec3 N  - normal vector to the surface
    //vec3 L  - light vector (from the light model)
    //vec3 Md - main color of the surface
    //float F0 - Base color for the Fresnel term
    //float metallic - parameter that mixes the diffuse with the specular term.
    //                 in particular, parmeter K seen in the slides is: float K = 1.0f - metallic;
    //float roughness - Material roughness (parmeter rho in the slides).
    //specular color Ms is not passed, and implicitely considered white: vec3 Ms = vec3(1.0f);
    vec3 Ms = vec3(1.0f);
    vec3 h = normalize(L + V);

    float LdotN = max(0.0000001f, dot(L, N));
    float VdotN = max(0.0000001f, dot(V, N));
    float HdotN = max(0.0000001f, dot(h, N));
    float HdotV = max(0.0000001f, dot(h, V));


    float rho_2 = roughness * roughness;
    float D = rho_2 / (PI * pow(( pow(HdotN, 2) * (rho_2 - 1.0f) + 1.0f), 2));
    float F = F0 + (1.0f - F0) * pow(1.0f - HdotV, 5);
    float G = gHelper(VdotN, rho_2) * gHelper(LdotN, rho_2);

    vec3 specularTerm = (D * F * G / (4.0f * VdotN)) * Ms;

    vec3 diffuseTerm = LdotN * Md;

    return (1.0f - metallic) * diffuseTerm + metallic * specularTerm;
}

void main() {
    vec3 Norm = normalize(fragNorm);
    vec3 Tan = normalize(fragTan.xyz - Norm * dot(fragTan.xyz, Norm));
    vec3 Bitan = cross(Norm, Tan) * fragTan.w;
    mat3 tbn = mat3(Tan, Bitan, Norm);
    vec4 nMap = texture(normMap, fragUV);
    vec3 N = normalize(tbn * (nMap.rgb * 2.0 - 1.0));

    vec3 albedo = texture(tex, fragUV).rgb;

    vec4 MRAO = texture(matMap, fragUV);
    float roughness = MRAO.g;
    float ao = MRAO.b;
    float metallic = MRAO.r;

    vec3 L = -lightsUniform.directLights[0].lightDir;
    vec3 lightColor = lightsUniform.directLights[0].lightColor.xyz;

    vec3 V = normalize(gubo.eyePos - fragPos);

    vec3 DiffSpec = BRDF(V, N, L, albedo, 0.3f, metallic, roughness);
    vec3 Ambient = albedo * 0.05f * ao;

    outColor = vec4(clamp(0.95 * DiffSpec * lightColor.rgb + Ambient,0.0,1.0), 1.0f);
}