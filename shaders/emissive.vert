#version 330

#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec3		position;
layout(location = 2) in	vec2	texCoordIn;	// incoming texcoord from the texcoord array
layout(location = 6) in ivec4 boneIds;
layout(location = 7) in vec4 boneWeights;

uniform mat4 modelMatrix;

uniform mat4 viewProjectionMatrix;

uniform int has_animations;
const int MAX_NUM_BONES = 100;
uniform mat4 bones[MAX_NUM_BONES];

out	vec2	texCoord;	// outgoing interpolated texcoord to fragshader

void main(){

    vec3 positionInWorldSpace = position;

    if(has_animations == 1) {
        mat4 boneTransform = bones[boneIds.x] * boneWeights.x;
        boneTransform += bones[boneIds.y] * boneWeights.y;
        boneTransform += bones[boneIds.z] * boneWeights.z;
        boneTransform += bones[boneIds.w] * boneWeights.w;

        positionInWorldSpace = (boneTransform * vec4(position, 1.0)).xyz;
    }

	mat4 modelViewProjectionMatrix = viewProjectionMatrix * modelMatrix;
	gl_Position = modelViewProjectionMatrix * vec4(positionInWorldSpace,1.0);
	texCoord = texCoordIn;

}