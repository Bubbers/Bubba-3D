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
layout(location = 8) in vec3 vertexColor;

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

uniform float time;

uniform vec3 windForce;

layout(std140) uniform Matrices {
    mat4 viewMatrix;
    mat4 projectionMatrix;
    mat4 viewProjectionMatrix;
};

vec3 applyMainBending(vec3 positionInWorldSpace, vec3 windForce);
vec3 applyDetailBending(vec3 positionInWorldSpace, vec3 objectPositionInWorldSpace, float time);

#define SIDE_TO_SIDE_FREQ1 1.975
#define SIDE_TO_SIDE_FREQ2 0.793
#define UP_AND_DOWN_FREQ1 0.375
#define UP_AND_DOWN_FREQ2 0.193

vec4 smoothCurve( vec4 x ) {
  return x * x * ( 3.0 - 2.0 * x );
}
vec4 triangleWave( vec4 x ) {
  return abs( fract( x + 0.5 ) * 2.0 - 1.0 );
}
vec4 smoothTriangleWave( vec4 x ) {
  return smoothCurve( triangleWave( x ) );
}


void main()
{
    vec3 positionInWorldSpace = position;
    vec3 normalVectorInWorldSpace = normalIn;

    if(has_animations == 1) {
        mat4 boneTransform = bones[boneIds.x] * boneWeights.x;
        boneTransform += bones[boneIds.y] * boneWeights.y;
        boneTransform += bones[boneIds.z] * boneWeights.z;
        boneTransform += bones[boneIds.w] * boneWeights.w;

        positionInWorldSpace = (boneTransform * vec4(position, 1.0)).xyz;
        normalVectorInWorldSpace = (boneTransform * vec4(normalIn, 0.0)).xyz;
    }

    vec3 objectPositionInWorldSpace = vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
    //positionInWorldSpace = applyMainBending(positionInWorldSpace - objectPositionInWorldSpace, windForce);
    //positionInWorldSpace += objectPositionInWorldSpace;

    positionInWorldSpace = applyDetailBending(positionInWorldSpace, objectPositionInWorldSpace, time);

	mat4 modelViewMatrix = viewMatrix * modelMatrix;
	mat4 modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;

	vec3 T = normalize(normalMatrix * vec4(tangent, 0.0)).xyz;
	vec3 B = normalize(normalMatrix * vec4(bittangent, 0.0)).xyz;
	vec3 N = normalize(normalMatrix * vec4(normalIn, 0.0)).xyz;
	TBN = mat3(T, B, N);

	color = vec4(colorIn, 1);
	texCoord = texCoordIn;

	viewSpacePosition = modelViewMatrix * vec4(positionInWorldSpace, 1.0);
	worldSpaceNormal = normalize( (normalMatrix * vec4(normalVectorInWorldSpace,0.0)).xyz );
	worldSpacePosition = modelMatrix * vec4(positionInWorldSpace, 1);

	shadowTexCoord = lightMatrix *vec4(viewSpacePosition.xyz, 1.0);
	gl_Position = modelViewProjectionMatrix * vec4(positionInWorldSpace,1.0);
}

vec3 applyMainBending(vec3 positionInWorldSpace, vec3 windForce) {
    float vertexHeight = positionInWorldSpace.y * 0.01; // TODO Extract 0.01 var to uniform and set for each gameobject
    vertexHeight += 1.0;
    vertexHeight *= vertexHeight;
    vertexHeight = vertexHeight * vertexHeight - vertexHeight;

    vec3 newPos = positionInWorldSpace;
    newPos.xy += windForce.xy * vertexHeight;

    float oldLength = length(positionInWorldSpace);

    return normalize(newPos) * oldLength;
}

vec3 applyDetailBending(vec3 positionInWorldSpace, vec3 objectPositionInWorldSpace, float time) {
    float branchAmplitude = 10.0;
    float branchStiffness = vertexColor.b;

    // Each object has its own phase to allow us to give different animations to different identical objects
    float objectPhase = dot(objectPositionInWorldSpace, vec3(1));

    float vertexPhase = dot(positionInWorldSpace, vec3(objectPhase));

    vec2 wavesIn = time + vec2(vertexPhase, 0.0);
    vec4 waves = (fract(wavesIn.xxyy * vec4(SIDE_TO_SIDE_FREQ1, SIDE_TO_SIDE_FREQ2, UP_AND_DOWN_FREQ1, UP_AND_DOWN_FREQ2) * 2.0 - 1.0)) * 2 * 1;
    waves = smoothTriangleWave(waves);
    vec2 waveSum = vec2(waves.x + waves.y, waves.z + waves.w);

    //positionInWorldSpace.xzy += waveSum.xxy * vec3(0.0, 0.0, vertexColor.b * 10.0);
    positionInWorldSpace.y += waveSum.y * branchStiffness * branchAmplitude;

    return positionInWorldSpace;
}
