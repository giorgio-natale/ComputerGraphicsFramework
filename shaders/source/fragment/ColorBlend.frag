#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform sampler2D tex;
layout(set = 2, binding = 0) uniform ColorBlendMaterialUniformBlock{
    vec3 color;
    float percentage;
}colorBlendMaterial;

void main() {
    outColor = (1-colorBlendMaterial.percentage) * vec4(texture(tex, fragUV).rgb, 1.0f) + colorBlendMaterial.percentage * vec4(colorBlendMaterial.color, 1.0f);		// main color
}