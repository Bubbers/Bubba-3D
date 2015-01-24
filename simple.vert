#version 150

in vec3		position;
in vec3		colorIn;
in	vec2	texCoordIn;	// incoming texcoord from the texcoord array
in  vec3	normalIn;

out vec4	viewSpacePosition; 
out vec3	viewSpaceNormal; 
out vec3	viewSpaceLightPosition; 
out vec4	color;
out	vec2	texCoord;	// outgoing interpolated texcoord to fragshader
out vec4    shadowTexCoord;

uniform mat4 modelMatrix; 
uniform mat4 viewMatrix; 
uniform mat4 projectionMatrix; 
uniform vec3 lightpos; 

uniform mat4 lightMatrix;


void main() 
{
	mat4 modelViewMatrix = viewMatrix * modelMatrix; 
	mat4 modelViewProjectionMatrix = projectionMatrix * modelViewMatrix; 
	///////////////////////////////////////////////////////////////////////////
	// The normal matrix should really be the inverse transpose of the 
	// modelViewMatrix, but that doesn't compile on current drivers.
	// Just using the modelView matrix works fine, as long as it does not
	// contain any nonuniform scaling. 
	mat4 normalMatrix = inverse(transpose(modelViewMatrix));
	///////////////////////////////////////////////////////////////////////////
	color = vec4(colorIn,1); 
	texCoord = texCoordIn; 

	viewSpacePosition = modelViewMatrix * vec4(position, 1.0); 
	viewSpaceNormal = normalize( (normalMatrix * vec4(normalIn,0.0)).xyz );
	viewSpaceLightPosition = (modelViewMatrix * vec4(lightpos, 1)).xyz; 
	vec4 worldSpacePosition = modelMatrix * vec4(position, 1); 

	shadowTexCoord = lightMatrix *vec4(viewSpacePosition.xyz, 1.0);
	//shadowTexCoord.xyz *= vec3(0.5, 0.5, 0.5);
	//shadowTexCoord.xyz += shadowTexCoord.w * vec3(0.5, 0.5, 0.5);

	gl_Position = modelViewProjectionMatrix * vec4(position,1.0);
}
