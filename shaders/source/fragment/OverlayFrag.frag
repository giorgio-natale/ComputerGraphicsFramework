#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform sampler2D tex;
layout(set = 2, binding = 0) uniform DefaultSpriteUniformBlock{
    float brightness;
}spriteUbo;

void main() {
    outColor = texture(tex, fragUV);		// main color
}