#version 130

#define scale 0.1
in	vec3	position;
in	vec2	texCoordIn;

uniform mat4 mvpMatrix;

out	vec2	texCoord;

void main() {
	texCoord = texCoordIn;
	gl_Position = mvpMatrix * vec4(position, 1.0);
}