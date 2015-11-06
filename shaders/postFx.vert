#version 130
#extension GL_ARB_explicit_attrib_location : enable
// This vertex shader simply outputs the input coordinates to the rasterizer. It only uses 2D coordinates.
layout(location = 0) in vec2 position;

void main() 
{
	gl_Position = vec4(position, 0.0, 1.0);
}