#version 130

#extension GL_ARB_gpu_shader5 : enable
#extension GL_ARB_explicit_attrib_location : enable


layout(location = 0) in vec3		position;
in vec3		colorIn;
layout(location = 2) in	vec2	texCoordIn;	// incoming texcoord from the texcoord array
layout(location = 1) in  vec3	normalIn;
in	vec3	tangent;
in  vec3	bittangent;

out vec4	viewSpacePosition; 
out vec3	worldSpaceNormal;
out vec4    worldSpacePosition;
out vec4	color;
out	vec2	texCoord;	// outgoing interpolated texcoord to fragshader
out vec4    shadowTexCoord;
out mat3    TBN;

uniform mat4 modelMatrix; 
uniform mat4 viewMatrix; 
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

uniform mat4 lightMatrix;


void main() 
{
	mat4 modelViewMatrix = viewMatrix * modelMatrix; 
	mat4 modelViewProjectionMatrix = projectionMatrix * modelViewMatrix; 
	//mat4 normalMatrix = inverse(transpose(modelMatrix));

	vec3 T = normalize(normalMatrix * vec4(tangent, 0.0)).xyz;
	vec3 B = normalize(normalMatrix * vec4(bittangent, 0.0)).xyz;
	vec3 N = normalize(normalMatrix * vec4(normalIn, 0.0)).xyz;
	TBN = mat3(T, B, N);

	color = vec4(colorIn,1); 
	texCoord = texCoordIn; 

	viewSpacePosition = modelViewMatrix * vec4(position, 1.0); 
	worldSpaceNormal = normalize( (normalMatrix * vec4(normalIn,0.0)).xyz );
	worldSpacePosition = modelMatrix * vec4(position, 1); 

	shadowTexCoord = lightMatrix *vec4(viewSpacePosition.xyz, 1.0);
	//shadowTexCoord.xyz *= vec3(0.5, 0.5, 0.5);
	//shadowTexCoord.xyz += shadowTexCoord.w * vec3(0.5, 0.5, 0.5);

	gl_Position = modelViewProjectionMatrix * vec4(position,1.0);
}
