#ifndef __UTILS_H__
#define __UTILS_H__

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ShaderProgram.h"
#include "GameObject.h"

//*****************************************************************************
//	Frame buffer objects
//*****************************************************************************
struct Fbo{
	GLuint id;
	GLuint texture;
	ShaderProgram* shaderProgram;
	GLuint depthbuffer;

	int width, height;
};



static float degreeToRad(float degree) {
	return (float) (degree * M_PI / 180);
}

static float radToDegree(float rad) {
	return (float) (rad * 180 / M_PI);
}

static float getRand(float min, float max) {
	//srand(time(NULL));
	float range = max - min;
	return (((float) rand() / (float) RAND_MAX) * range) + min;
}

template <typename T, unsigned S>
static T getRandomElem(const T (&ts)[S])
{
	if (S > 0) {
		int ix = (int) getRand(0.0f, S - 0.1f);
		return ts[ix];
	} else {
		return NULL;
	}
}
 static void updateMinAndMax(float x, float y, float z, chag::float3* minV, chag::float3* maxV) {
	if (x < minV->x) { minV->x = x; };
	if (y < minV->y) { minV->y = y; };
	if (z < minV->z) { minV->z = z; };
	if (x > maxV->x) { maxV->x = x; };
	if (y > maxV->y) { maxV->y = y; };
	if (z > maxV->z) { maxV->z = z; };
}

static float3 createRandomVector(float minValue, float maxValue){
	return make_vector(getRand(minValue,maxValue),getRand(minValue,maxValue),getRand(minValue,maxValue));
}

typedef std::pair<GameObject*, GameObject*> CollisionPair;
typedef std::vector<CollisionPair> CollisionPairList;

#endif