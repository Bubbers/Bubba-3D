#version 330
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec3 position;
uniform mat4 modelMatrix;
out vec3 texCoord;

layout(std140) uniform Matrices {
    mat4 viewMatrix;
    mat4 projectionMatrix;
    mat4 viewProjectionMatrix;
};

void main() {
	vec4 wvp_pos = viewProjectionMatrix * modelMatrix * vec4(position, 1.0);
	texCoord = position;
	gl_Position = wvp_pos.xyww;
}