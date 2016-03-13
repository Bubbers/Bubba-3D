#version 330

#define scale 0.1
in	vec3	position;
in	vec2	texCoordIn;

uniform mat4 modelMatrix;

layout(std140) uniform Matrices {
    mat4 viewMatrix;
    mat4 projectionMatrix;
    mat4 viewProjectionMatrix;
};

out	vec2	texCoord;

void main() {
	texCoord = texCoordIn;
	gl_Position = viewProjectionMatrix * modelMatrix * vec4(position, 1.0);
}