#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform GlobalUniformBlock {
	mat4 vpMat;
} gubo;
layout(set = 3, binding = 0) uniform EntityTransformUniformBlock {
	mat4 mMat;
} tubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;

layout(location = 0) out vec2 outUV;

void main() {
	gl_Position = gubo.vpMat * tubo.mMat * vec4(inPosition, 1.0);
	outUV = inUV;
}