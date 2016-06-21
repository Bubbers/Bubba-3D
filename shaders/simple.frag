#version 130
// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

#define FOG_EQUATION_LINEAR 0
#define FOG_EQUATION_EXP    1
#define FOG_EQUATION_EXP2   2
#define FOG_EQUATION_NONE   3

uniform struct Fog {
	vec3 vColor;
	float fStart;
	float fEnd;
	float fDensity;

	int iEquation;
} fog;

struct Light
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};

struct Attenuation{
	float constant;
	float linear;
	float exp;
};

struct DirectionalLight{
	Light colors;
	vec3 direction;
};

struct SpotLight{
	Light colors;
	vec3 position;
	vec3 direction;
	float cutoff;
	float cutoffOuter;
	Attenuation attenuation;
};

struct PointLight{
	Light colors;
	vec3 position;
	Attenuation attenuation;
};

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS  4
uniform int nrPointLights = 0;
uniform int nrSpotLights  = 0;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight  spotLights [MAX_SPOT_LIGHTS];
uniform DirectionalLight directionalLight;


// inputs from vertex shader.
in vec4 color;
in vec2 texCoord;
in vec4 viewSpacePosition;
in vec3 worldSpaceNormal;
in vec4 worldSpacePosition;
in vec4 shadowTexCoord;
in mat3 TBN;

uniform mat4 inverseViewNormalMatrix;

uniform vec3 viewPosition;

// output to frame buffer.
out vec4 fragmentColor;

// global uniforms, that are the same for the whole scene
uniform sampler2DShadow shadowMap;

uniform int has_cube_map;
uniform samplerCube cubeMap;

// object specific uniforms, change once per object but are the same for all materials in object.
uniform float object_alpha;
uniform float object_reflectiveness = 0.0;

// matrial properties, changed when material changes.
uniform float material_shininess;
uniform vec3 material_diffuse_color;
uniform vec3 material_specular_color;
uniform vec3 material_emissive_color;
uniform int has_diffuse_texture;
uniform sampler2D diffuse_texture;
uniform int has_normal_texture;
uniform sampler2D normal_texture;


// FUNCTIONS DECLARATION
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 directionToEye);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 directionToEye);
vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 directionToEye);
Light calculateGeneralLight(Light colors, vec3 directionToLight, vec3 directionToEye, vec3 normal);

vec3 calculateAmbient(vec3 lightAmbient, vec3 materialAmbient);
vec3 calculateDiffuse(vec3 normal, vec3 directionToLight, vec3 materialLight, vec3 sceneLight);
vec3 calculateSpecular(vec3 normal, vec3 directionToLight, vec3 directionToEye, vec3 materialSpecular, vec3 sceneLight, float materialShininess);
vec3 calculateFresnel(vec3 directionToLight, vec3 directionToEye, vec3 materialSpecular);
vec3 calculateFog(vec3 color, float distance);
vec3 calculateCubeMapSample(vec3 directionToEye, vec3 normal);

void main()
{
	vec3 normal;
	if (has_normal_texture == 1) {
		normal = texture(normal_texture, texCoord.xy).xyz;
		normal = normalize(normal * 2.0 - 1.0);
		normal = normalize(TBN * normal);
	}
	else {
		normal = normalize(worldSpaceNormal);
	};

	vec3 directionToEye = normalize(viewPosition - worldSpacePosition.xyz);
	vec3 color = vec3(0.0);

	color += calculateDirectionalLight(directionalLight, normal, directionToEye);

	for (int i = 0; i < nrPointLights; i++) {
		color += calculatePointLight(pointLights[i], normal, directionToEye);
	}

	for (int i = 0; i < nrSpotLights; i++) {
		color += calculateSpotLight(spotLights[i], normal, directionToEye);
	}

	vec3 foggedColor = calculateFog(color, abs(viewSpacePosition.z / viewSpacePosition.w));
	vec3 emissive = material_emissive_color;

        vec3 cubeMapSample = calculateCubeMapSample(directionToEye, normal);

	fragmentColor = vec4(foggedColor + emissive + cubeMapSample, object_alpha);
}

vec3 calculateCubeMapSample(vec3 directionToEye, vec3 normal) {
    if(has_cube_map == 1) {
        vec3 reflectionVector = normalize((inverseViewNormalMatrix * vec4(reflect(-directionToEye, normal), 0.0)).xyz);
        vec3 cubeMapSample = texture(cubeMap, reflectionVector).rgb * object_reflectiveness;
    } else {
        return vec3(0.0);
    }
}

Light calculateGeneralLight(Light colors, vec3 directionToLight, vec3 directionToEye, vec3 normal) {
	vec3 ambi_color = colors.ambientColor;
	vec3 diff_color = colors.diffuseColor;
	vec3 spec_color = colors.specularColor;

	//Ambient tem
	vec3 ambient = calculateAmbient(ambi_color, material_diffuse_color);

	//Diffuse term
	vec3 diffuse = calculateDiffuse(normal, directionToLight, material_diffuse_color, diff_color);

	//Specular term
	vec3 fresnelTerm = calculateFresnel(normal, directionToEye, material_specular_color);
	vec3 specular = calculateSpecular(normal, directionToLight, directionToEye, fresnelTerm, spec_color, material_shininess);


	// if we have a texture we modulate all of the color properties
	if (has_diffuse_texture == 1)
	{
		vec3 tex_color = texture(diffuse_texture, texCoord.xy).xyz;
		diffuse *= tex_color;
		ambient *= tex_color;
	}

	Light light;
	light.ambientColor  = ambient;
	light.diffuseColor  = diffuse;
	light.specularColor = specular;

	return light;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 directionToEye){
	vec3 directionToLight = normalize(light.position - worldSpacePosition.xyz);
	float theta = dot(directionToLight, normalize(-light.direction));
	float epsilon = light.cutoff - light.cutoffOuter;
	float intensity = clamp(((theta - light.cutoffOuter) / epsilon ), 0.0, 1.0);

	Light lt = calculateGeneralLight(light.colors, directionToLight, directionToEye, normal);

	float distance = length(light.position - worldSpacePosition.xyz);
	float attenuation = 1.0f / (light.attenuation.constant + light.attenuation.linear * distance + light.attenuation.exp * (distance * distance));
	float multiple = attenuation * intensity;

	lt.diffuseColor  *= multiple;
	lt.specularColor *= multiple;
	lt.ambientColor  *= multiple;

	return lt.ambientColor + lt.diffuseColor + lt.specularColor;
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 directionToEye) {
	Light lt =  calculateGeneralLight(light.colors, normalize(-light.direction), directionToEye, normal);

	float visibility = textureProj(shadowMap, shadowTexCoord);
	lt.specularColor *= visibility;
	lt.diffuseColor *= visibility;

	return lt.ambientColor +lt.diffuseColor + lt.specularColor;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 directionToEye) {

	vec3 directionToLight = normalize(light.position - worldSpacePosition.xyz);

	Light lt = calculateGeneralLight(light.colors, directionToLight, directionToEye, normal);

	float distance = length(light.position - worldSpacePosition.xyz);
	float attenuation = 1.0f / (light.attenuation.constant + light.attenuation.linear * distance + light.attenuation.exp * (distance * distance));

	lt.diffuseColor  *= attenuation;
	lt.specularColor *= attenuation;
	lt.ambientColor  *= attenuation;

	return lt.ambientColor + lt.diffuseColor + lt.specularColor;
}

vec3 calculateFog(vec3 color, float dist) {

    if(fog.iEquation == FOG_EQUATION_NONE) {
         return color;
    }

	float f = 1.0;
	if (fog.iEquation == FOG_EQUATION_LINEAR) {
		f = (fog.fEnd - dist) / (fog.fEnd - fog.fStart);
	}
	else if (fog.iEquation == FOG_EQUATION_EXP) {
		f = 1.0 / exp(dist * fog.fDensity);
	}
	else if (fog.iEquation == FOG_EQUATION_EXP2) {
		f = 1.0 / exp(pow(dist * fog.fDensity, 2.0));
	}

	f = clamp(f, 0.0, 1.0);
	return f*color + (1.0 - f)*fog.vColor;
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
