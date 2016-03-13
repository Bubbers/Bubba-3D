#version 130

precision highp float;

in vec2 texCoord;

out vec4 fragmentColor;

uniform sampler2D sprite;


void main() {
	fragmentColor = texture(sprite, texCoord);
}