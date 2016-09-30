#version 330


layout(location=0) in	vec3	position;
layout(location=1) in	vec2	texCoordIn;
layout(location=2) in   vec2    locationIn;

out	vec2	texCoord;
out vec2    location;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main() {
	texCoord = texCoordIn;
	location = locationIn;
	gl_Position = projectionMatrix*modelMatrix * vec4(position, 1.0);
}