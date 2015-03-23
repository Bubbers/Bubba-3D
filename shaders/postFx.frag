#version 130

// Note: this is core in OpenGL 3.1 (glsl 1.40) and later, we use OpenGL 3.0 for the tutorials
#extension GL_ARB_texture_rectangle : enable

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

uniform sampler2DRect frameBufferTexture;
uniform sampler2DRect blurredFrameBufferTexture;
uniform float time;
out vec4 fragmentColor;

/**
 * Perturps the sampling coordinates of the pixel and returns the new coordinates
 * these can then be used to sample the frame buffer. The effect uses a sine wave to make us
 * feel woozy.
 */ 
vec2 mushrooms(vec2 inCoord);

/**
 * Samples a region of the frame buffer using gaussian filter weights to blur the image
 * as the kernel width is not that large, it doesnt produce a very large effect. Making it larger
 * is both tedious and expensive, for real time purposes a separable blur is preferable, but this 
 * requires several passes. 
 * takes as input the centre coordinate to sample around.
 */
vec3 blur(vec2 coord);

/**
 * Simply returns the luminance of the input sample color.
 */
vec3 grayscale(vec3 sample);

/**
 * Converts the color sample to sepia tone (by transformation to the yiq color space).
 */
vec3 toSepiaTone(vec3 rgbSample);

vec3 mosaic(vec2 coords);
vec2 mushrooms2(vec2 inCoord);



void main() 
{
	vec3 color = (texture(frameBufferTexture, gl_FragCoord.xy) + texture(blurredFrameBufferTexture, gl_FragCoord.xy) * 4).xyz;

	fragmentColor = vec4(color, 1.0);
	//fragmentColor = vec4(toSepiaTone(color), 1.0);

	//fragmentColor = vec4(mosaic(gl_FragCoord.xy), 1.0);

	//fragmentColor = texture(frameBufferTexture, mushrooms(gl_FragCoord.xy));
	
	//fragmentColor = vec4(blur(gl_FragCoord.xy), 1.0);
	//fragmentColor = vec4(grayscale(texture(frameBufferTexture, gl_FragCoord.xy).xyz), 1.0);

	//fragmentColor = vec4(mosaic(mushrooms2(gl_FragCoord.xy)), 1.0);
	

	// all at once
	//fragmentColor = vec4(toSepiaTone(blur(mushrooms(gl_FragCoord.xy))), 1.0);
}


vec3 toSepiaTone(vec3 rgbSample)
{
	//-----------------------------------------------------------------
	// Variables used for YIQ/RGB color space conversion.
	//-----------------------------------------------------------------
	vec3 yiqTransform0 = vec3(0.299, 0.587, 0.144);
	vec3 yiqTransform1 = vec3(0.596,-0.275,-0.321);
	vec3 yiqTransform2 = vec3(0.212,-0.523, 0.311);

	//vec3 yiqInverseTransform0 = vec3(1, 0.956, 0.621);
	//vec3 yiqInverseTransform1 = vec3(1,-0.272,-0.647);
	//vec3 yiqInverseTransform2 = vec3(1,-1.105, 1.702);

	vec3 yiqInverseTransform0 = vec3(1, 1.0, 0.0);
	vec3 yiqInverseTransform1 = vec3(0,0, 0);
	vec3 yiqInverseTransform2 = vec3(0,0, 0);
	
	// transform to YIQ color space and set color information to sepia tone
	vec3 yiq = vec3(dot(yiqTransform0, rgbSample), 0.2, 0.0);
	
	// inverse transform to RGB color space
	vec3 result = vec3(dot(yiqInverseTransform0, yiq), dot(yiqInverseTransform1, yiq), dot(yiqInverseTransform2, yiq));
	return result;
}

vec2 mushrooms(vec2 inCoord)
{
	return inCoord + vec2(mod(pow(sin(time * 4.3127 + inCoord.y / 9.0) * 15.0, time), 10), 0.0);
}

vec2 mushrooms2(vec2 inCoord)
{
	return inCoord + vec2(sin(time * 4.3127 + inCoord.y / 9.0) * 15.0, 0.0);
}

vec3 blur(vec2 coord)
{
	// sample framebuffer texture at current texel
	vec3 result = texture(blurredFrameBufferTexture, coord).xyz;

	// sample framebuffer texture at neighbouring texels
	result += 0.125 * texture(blurredFrameBufferTexture, coord + vec2(1, 0)).xyz;
	result += 0.125 * texture(blurredFrameBufferTexture, coord + vec2(0, 1)).xyz;
	result += 0.125 * texture(blurredFrameBufferTexture, coord + vec2(-1, 0)).xyz;
	result += 0.125 * texture(blurredFrameBufferTexture, coord + vec2(0, -1)).xyz;

	return result;
}

vec3 mosaic(vec2 coords) {
	vec2 xy = vec2(coords.x - mod(coords.x, -15), coords.y - mod(coords.y, -15));

	return texture(frameBufferTexture, xy).xyz;
}

vec3 grayscale(vec3 sample)
{
	return vec3(sample.r * 0.2126 + sample.g * 0.7152 + sample.b * 0.0722);
}
