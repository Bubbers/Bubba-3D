#version 330

in	vec3	position;
in	vec2	texCoordIn;

out	vec2	texCoord;

uniform mat4 modelMatrix;

void main() {
	texCoord = texCoordIn;
	gl_Position = modelMatrix * vec4(position, 1.0);
}