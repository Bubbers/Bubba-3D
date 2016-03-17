#version 330


precision highp float;

in vec2 texCoord;

out vec4 fragmentColor;

uniform sampler2D sprite;
uniform bool isTexture;
uniform bool isColor;
uniform bool isFont;
uniform vec4 color;

void main() {
    if(isTexture){
	    fragmentColor = texture(sprite, texCoord);
	}else if(isColor){
	    fragmentColor = color;
	}else if(isFont){
	    fragmentColor = vec4(1,1,1,texture(sprite,texCoord).x);
	}
	
}