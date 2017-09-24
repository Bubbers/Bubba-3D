#version 130

// Note: this is core in OpenGL 3.1 (glsl 1.40) and later, we use OpenGL 3.0 for the tutorials
#extension GL_ARB_texture_rectangle : enable

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

uniform sampler2DRect frameBufferTexture;
uniform float time;
out vec4 fragmentColor;


void main() 
{
	vec3 color = texture(frameBufferTexture, gl_FragCoord.xy).xyz;

	fragmentColor = vec4(color, 1.0);
}
