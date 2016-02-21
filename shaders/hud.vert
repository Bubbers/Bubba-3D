#version 330

in	vec3	position;
in	vec2	texCoordIn;

out	vec2	texCoord;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main() {
	texCoord = texCoordIn;
	gl_Position = projectionMatrix*modelMatrix * vec4(position, 1.0);
}