#version 450
#extension GL_ARB_separate_shader_objects : enable
#define PI 3.1415926535897932384626433832795

#define DIRECT_LIGHTS_MAX 10
#define POINT_LIGHTS_MAX 10
#define SPOT_LIGHTS_MAX 10

struct DirectLightBlock{
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
    DirectLightBlock directLights[DIRECT_LIGHTS_MAX];
    PointLightBlock pointLights[POINT_LIGHTS_MAX];
    SpotLightBlock spotLights[SPOT_LIGHTS_MAX];
    int directLightsCount;
    int pointLightsCount;
    int spotLightsCount;
} lightsUniform;

layout(location = 0) in vec2 fragUV;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 fragNorm;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform sampler2D tex;

layout(set = 2, binding = 0) uniform GlobalUniformBufferObject {
    vec3 eyePos;
} gubo;
layout(set = 2, binding = 1) uniform sampler2D matMap;



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
    vec3 N = Norm;
    vec3 albedo = texture(tex, fragUV).rgb;

    vec4 MRAO = texture(matMap, fragUV);
    float roughness = MRAO.g;
    float ao = MRAO.b;
    float metallic = MRAO.r;
    vec3 V = normalize(gubo.eyePos - fragPos);
    vec3 Ambient = albedo * 0.3f * ao;

    vec3 DiffSpec = vec3(0,0,0);

    for(int i = 0; i < lightsUniform.directLightsCount; i++){
        vec3 L = -lightsUniform.directLights[i].lightDir;
        vec3 lightColor = lightsUniform.directLights[i].lightColor.xyz;
        DiffSpec += BRDF(V, N, L, albedo, 0.3f, metallic, roughness) * lightColor;
    }

    for(int i = 0; i < lightsUniform.pointLightsCount; i++){
        vec3 L = normalize(lightsUniform.pointLights[i].lightPos - fragPos);
        vec3 lightColor = (lightsUniform.pointLights[i].lightColor * pow(lightsUniform.pointLights[i].g / length(lightsUniform.pointLights[i].lightPos - fragPos), lightsUniform.pointLights[i].beta)).xyz;
        DiffSpec += BRDF(V, N, L, albedo, 0.3f, metallic, roughness) * lightColor;
    }

    for(int i = 0; i < lightsUniform.spotLightsCount; i++){
        vec3 L = normalize(lightsUniform.spotLights[i].lightPos - fragPos);

        float cosAlpha = dot(L, -lightsUniform.spotLights[i].lightDir);
        float decay = clamp( (cosAlpha - lightsUniform.spotLights[i].cosOuter) / (lightsUniform.spotLights[i].cosInner - lightsUniform.spotLights[i].cosOuter), 0, 1);
        float pointLightDimming = pow(lightsUniform.spotLights[i].g / length(lightsUniform.spotLights[i].lightPos - fragPos), lightsUniform.spotLights[i].beta);
        vec3 lightColor = lightsUniform.spotLights[i].lightColor.xyz * decay * pointLightDimming;

        DiffSpec += BRDF(V, N, L, albedo, 0.3f, metallic, roughness) * lightColor;
    }
    outColor = vec4(clamp(0.95 * DiffSpec + Ambient,0.0,1.0), 1.0f);



}