#version 130
#extension GL_ARB_explicit_attrib_location : enable

in vec3 texCoord;

uniform samplerCube cubeMapSampler;

layout(location = 0) out vec4 fragmentColor;

void main() {
	fragmentColor = texture(cubeMapSampler, texCoord);
}