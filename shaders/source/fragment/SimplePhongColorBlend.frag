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

layout(set = 2, binding = 0) uniform SimplePhongColorBlendMaterialUniformBlock {
    vec3 color;
    float percentage;
} phongColorBlendBlinkMaterial;

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
    vec3 EyeDir = normalize(lightsUniform.eyePosition - fragPos);
    vec3 mainColor = texture(tex, fragUV).rgb;

    vec3 DiffSpec = vec3(0,0,0);
    for(int i = 0; i < lightsUniform.directLightsCount; i++){
        vec3 L = -lightsUniform.directLights[i].lightDir;
        vec3 lightColor = lightsUniform.directLights[i].lightColor.xyz;
        DiffSpec += BRDF(EyeDir, Norm, L, mainColor , vec3(1.0f), 160.0f) * lightColor;
    }

    for(int i = 0; i < lightsUniform.pointLightsCount; i++){
        vec3 L = normalize(lightsUniform.pointLights[i].lightPos - fragPos);
        vec3 lightColor = (lightsUniform.pointLights[i].lightColor * pow(lightsUniform.pointLights[i].g / length(lightsUniform.pointLights[i].lightPos - fragPos), lightsUniform.pointLights[i].beta)).xyz;
        DiffSpec += BRDF(EyeDir, Norm, L, mainColor, vec3(1.0f), 160.0f) * lightColor;
    }

    for(int i = 0; i < lightsUniform.spotLightsCount; i++){
        vec3 L = normalize(lightsUniform.spotLights[i].lightPos - fragPos);

        float cosAlpha = dot(L, -lightsUniform.spotLights[i].lightDir);
        float decay = clamp( (cosAlpha - lightsUniform.spotLights[i].cosOuter) / (lightsUniform.spotLights[i].cosInner - lightsUniform.spotLights[i].cosOuter), 0, 1);
        float pointLightDimming = pow(lightsUniform.spotLights[i].g / length(lightsUniform.spotLights[i].lightPos - fragPos), lightsUniform.spotLights[i].beta);
        vec3 lightColor = lightsUniform.spotLights[i].lightColor.xyz * decay * pointLightDimming;

        DiffSpec += BRDF(EyeDir, Norm, L, mainColor, vec3(1.0f), 160.0f) * lightColor;
    }

    vec3 Ambient = texture(tex, fragUV).rgb * 0.15f;
    vec3 phongColor = clamp(0.95 * (DiffSpec) + Ambient,0.0,1.0);
    vec3 blendedColor = (1-phongColorBlendBlinkMaterial.percentage) * phongColor + phongColorBlendBlinkMaterial.percentage * phongColorBlendBlinkMaterial.color;
    outColor = vec4(blendedColor, 1.0f);
}