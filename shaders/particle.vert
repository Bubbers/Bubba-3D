#version 150

#define scale 0.1
in	vec3	position;
in	vec2	texCoordIn;

uniform vec3 offset;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out	vec2	texCoord;



void main() {
	texCoord = texCoordIn;
	gl_Position = projectionMatrix * viewMatrix * vec4((position * scale) + offset, 1.0);
}