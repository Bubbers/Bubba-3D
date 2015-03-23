#version 130

// Note: this is core in OpenGL 3.1 (glsl 1.40) and later, we use OpenGL 3.0 for the tutorials
#extension GL_ARB_texture_rectangle : enable

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

uniform sampler2DRect frameBufferTexture;
uniform float time;
out vec4 fragmentColor;


float offsets[9] = float[9](-7.302940716, -5.35180578, -3.403984807, -1.458429517, 0.0, 1.458429517, 3.403984807, 5.35180578, 7.302940716);
//float weights[9] = float[9](0.011110875, 0.045328584, 0.119911134, 0.205817181, 0.117832226, 0.205817181, 0.119911134, 0.045328584, 0.011110875);
float weights[9] = float[9](0.0125949685786212, 0.0513831777608629, 0.1359278107392780, 0.2333084327472980, 0.1335712203478790, 0.2333084327472980, 0.1359278107392780, 0.0513831777608629, 0.0125949685786212);


/**
 * Implements the vertical part of a 17 tap separable gaussian blur, using standard deviation of 3.0, calculated
 * using the formluas and excel sheet from  this blog: http://theinstructionlimit.com/gaussian-blur-revisited-part-two
 * The weights have been normalized to make the kernel not brighten or darken the image.
 */
void main() 
{
	vec4 result = vec4(0.0);
	// do a 17 tap blur by sampling a biliniarly filtered texture.
	for (int i = 0; i < 9; ++i)
	{
		result += texture(frameBufferTexture, gl_FragCoord.xy + vec2(0.0, offsets[i])) * weights[i];
	}
	
	fragmentColor = result;
}
