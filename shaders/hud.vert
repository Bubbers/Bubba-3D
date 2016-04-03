#version 330


layout(location=0) in	vec3	position;
layout(location=1) in	vec2	texCoordIn;
layout(location=2) in   vec2    distanceIn;

out	vec2	texCoord;
out vec2    distance;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main() {
	texCoord = texCoordIn;
	distance = distanceIn;
	gl_Position = projectionMatrix*modelMatrix * vec4(position, 1.0);
}