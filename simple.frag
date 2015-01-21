#version 150
// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

// inputs from vertex shader.
in vec4 color;
in vec2 texCoord;
in vec3 viewSpacePosition; 
in vec3 viewSpaceNormal; 
in vec3 viewSpaceLightPosition; 
in vec4 shadowTexCoord;

uniform mat4 inverseViewNormalMatrix;

// output to frame buffer.
out vec4 fragmentColor;

// global uniforms, that are the same for the whole scene
uniform sampler2DShadow shadowMap;
uniform samplerCube cubeMap; 

//Suns ambient light
uniform vec3 scene_ambient_light = vec3(0.05, 0.05, 0.05);

//Suns real light
uniform vec3 scene_light = vec3(0.6, 0.6, 0.6);

// object specific uniforms, change once per object but are the same for all materials in object.
uniform float object_alpha; 
uniform float object_reflectiveness = 0.0;

// matrial properties, changed when material changes.
uniform float material_shininess;
uniform vec3 material_diffuse_color; 
uniform vec3 material_specular_color; 
//uniform vec3 material_ambient_color;
uniform vec3 material_emissive_color; 
uniform int has_diffuse_texture; 
uniform sampler2D diffuse_texture;

vec3 calculateAmbient(vec3 lightAmbient, vec3 materialAmbient);

vec3 calculateDiffuse(vec3 normal, vec3 directionToLight, vec3 materialLight, vec3 sceneLight);

vec3 calculateSpecular(vec3 normal, vec3 directionToLight, vec3 directionToEye, vec3 materialSpecular, vec3 sceneLight, float materialShininess);

vec3 calculateFresnel(vec3 directionToLight, vec3 directionToEye, vec3 materialSpecular);

void main() 
{
	vec3 directionToLight = normalize(viewSpaceLightPosition - viewSpacePosition);
	vec3 directionToEye = normalize(vec3(0.0) -viewSpacePosition);

	vec3 normal = normalize(viewSpaceNormal);

	float visibility = textureProj(shadowMap, shadowTexCoord);

	vec3 reflectionVector = normalize((inverseViewNormalMatrix * vec4(reflect(-directionToEye, normal),0.0)).xyz);


	vec3 diffuse = calculateDiffuse(normal, directionToLight, material_diffuse_color, scene_light);
	
	vec3 fresnelTerm = calculateFresnel(normal, directionToEye, material_specular_color);
	vec3 specular = calculateSpecular(normal, directionToLight, directionToEye, fresnelTerm, scene_light, material_shininess);
	
	vec3 emissive = material_emissive_color;
	vec3 ambient = calculateAmbient(scene_ambient_light, material_diffuse_color);

	vec3 cubeMapSample = texture(cubeMap, reflectionVector).rgb * object_reflectiveness;
	vec3 reflection = fresnelTerm * cubeMapSample;
	
	// if we have a texture we modulate all of the color properties
	if (has_diffuse_texture == 1)
	{
		diffuse *= texture(diffuse_texture, texCoord.xy).xyz;
		ambient *= texture(diffuse_texture, texCoord.xy).xyz;
		emissive *= texture(diffuse_texture, texCoord.xy).xyz;
	}

	specular *= visibility;
	diffuse *= visibility;
	reflection *= max(visibility, 0.2);
	
	fragmentColor = vec4(ambient + diffuse + emissive + specular + reflection, object_alpha);

	/*if (object_reflectiveness > 0.0) {
		fragmentColor = vec4(texture(cubeMap, reflectionVector).rgb, object_alpha);
	}*/
}

vec3 calculateDiffuse(vec3 normal, vec3 directionToLight, vec3 materialLight, vec3 sceneLight) {
	float angle = max(dot(normal, directionToLight), 0.0);
	return angle * materialLight * sceneLight;
}

vec3 calculateAmbient(vec3 lightAmbient, vec3 materialAmbient) {
	return materialAmbient * lightAmbient;
}

vec3 calculateSpecular(vec3 normal, vec3 directionToLight, vec3 directionToEye, vec3 materialSpecular, vec3 sceneLight, float materialShininess) {
	vec3 h = normalize(directionToLight + directionToEye);
	float normalizationFactor = ((materialShininess + 2.0) / 8.0);
	return normalizationFactor * (dot(normal, directionToLight) < 0.0 ? vec3(0.0) : pow(max(0.0, dot(h, normal)), 4.0 * materialShininess) * materialSpecular * sceneLight);
} 

vec3 calculateFresnel(vec3 normal, vec3 directionToEye, vec3 materialSpecular) {
	float angle = dot(directionToEye, normal);
	return materialSpecular + (1.0 - materialSpecular) * pow(clamp((1.0 - angle), 0.0, 1.0), 5.0);
}