#version 130

precision highp float;

in vec2 texCoord;

out vec4 fragmentColor;

uniform sampler2D sprite;
uniform bool      textureElseColor;
uniform vec4      color;


void main() {

	fragmentColor = textureElseColor ? texture(sprite, texCoord) : color;
}