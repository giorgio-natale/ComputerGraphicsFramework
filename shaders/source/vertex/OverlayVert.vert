#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform GlobalOverlayUniformBlock {
    mat4 overlayProjectionMatrix;
} goubo;
layout(set = 3, binding = 0) uniform SpriteUniformBlock {
    mat4 spriteTransform;
} subo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inUV;

layout(location = 0) out vec2 outUV;

void main() {
    gl_Position = vec4((mat3(goubo.overlayProjectionMatrix) * mat3(subo.spriteTransform) * vec3(inPosition, 1.0f)).xy, 0, 1.0f);
    //gl_Position = vec4((vec3(inPosition, 1.0f)).xy, 0.5f, 1.0f);

    outUV = inUV;
}