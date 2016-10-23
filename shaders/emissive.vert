#version 330

#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec3		position;
layout(location = 2) in	vec2	texCoordIn;	// incoming texcoord from the texcoord array

uniform mat4 modelMatrix;

uniform mat4 viewProjectionMatrix;


out	vec2	texCoord;	// outgoing interpolated texcoord to fragshader

void main(){

	mat4 modelViewProjectionMatrix = viewProjectionMatrix * modelMatrix;
	gl_Position = modelViewProjectionMatrix * vec4(position,1.0);
	texCoord = texCoordIn;

}