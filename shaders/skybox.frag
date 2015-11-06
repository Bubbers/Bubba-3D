#version 130

in vec3 texCoord;

uniform samplerCube cubeMapSampler;

out vec4 fragmentColor;

void main() {
	fragmentColor = texture(cubeMapSampler, texCoord);
}