#version 130

precision highp float;

in vec2 texCoord;

out vec4 fragmentColor;

uniform sampler2D sprite;
uniform vec3 color;

void main() {
	fragmentColor = texture(sprite, texCoord) * vec4(color, 1.0);
}