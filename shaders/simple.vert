#version 330

#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec3 position;
in vec3 colorIn;
layout(location = 2) in vec2 texCoordIn;
layout(location = 1) in vec3 normalIn;
in vec3 tangent;
in vec3 bittangent;
layout(location = 6) in ivec4 boneIds;
layout(location = 7) in vec4 boneWeights;

out vec4 viewSpacePosition;
out vec3 worldSpaceNormal;
out vec4 worldSpacePosition;
out vec4 color;
out	vec2 texCoord; // outgoing interpolated texcoord to fragshader
out vec4 shadowTexCoord;
out mat3 TBN;

uniform mat4 modelMatrix;
uniform mat4 normalMatrix;
uniform mat4 lightMatrix;

uniform int has_animations;
const int MAX_NUM_BONES = 100;
uniform mat4 bones[MAX_NUM_BONES];

layout(std140) uniform Matrices {
    mat4 viewMatrix;
    mat4 projectionMatrix;
    mat4 viewProjectionMatrix;
};

bool fequals(float a, float b) {
    return abs(a - b) < 0.01f;
}

float getBoneWeightSum() {
    return boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w;
}

void main()
{
    vec3 positionInWorldSpace = position;
    vec3 normalVectorInWorldSpace = normalIn;

    float boneWeightSum = getBoneWeightSum();

    if(has_animations == 1 && fequals(boneWeightSum, 1.0f)) {
        mat4 boneTransform = bones[boneIds.x] * boneWeights.x;
        boneTransform += bones[boneIds.y] * boneWeights.y;
        boneTransform += bones[boneIds.z] * boneWeights.z;
        boneTransform += bones[boneIds.w] * boneWeights.w;

        positionInWorldSpace = (boneTransform * vec4(position, 1.0)).xyz;
        normalVectorInWorldSpace = (boneTransform * vec4(normalIn, 1.0)).xyz;
    }

	mat4 modelViewMatrix = viewMatrix * modelMatrix;
	mat4 modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;

	vec3 T = normalize(normalMatrix * vec4(tangent, 0.0)).xyz;
	vec3 B = normalize(normalMatrix * vec4(bittangent, 0.0)).xyz;
	vec3 N = normalize(normalMatrix * vec4(normalIn, 0.0)).xyz;
	TBN = mat3(T, B, N);

	color = vec4(colorIn,1);
	texCoord = texCoordIn;

	viewSpacePosition = modelViewMatrix * vec4(positionInWorldSpace, 1.0);
	worldSpaceNormal = normalize( (normalMatrix * vec4(normalVectorInWorldSpace,0.0)).xyz );
	worldSpacePosition = modelMatrix * vec4(positionInWorldSpace, 1);

	shadowTexCoord = lightMatrix *vec4(viewSpacePosition.xyz, 1.0);
	gl_Position = modelViewProjectionMatrix * vec4(positionInWorldSpace,1.0);
}

