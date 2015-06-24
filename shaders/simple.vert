#version 150

in vec3		position;
in vec3		colorIn;
in	vec2	texCoordIn;	// incoming texcoord from the texcoord array
in  vec3	normalIn;

out vec4	viewSpacePosition; 
out vec3	worldSpaceNormal;
out vec4    worldSpacePosition;
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
	mat4 normalMatrix = inverse(transpose(modelMatrix));

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
