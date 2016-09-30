#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "linmath/float3.h"

class ShaderProgram;
class GameObject;

struct Fbo {
    GLuint id;
    GLuint texture;
    ShaderProgram* shaderProgram;
    GLuint depthbuffer;

    int width, height;
};

float degreeToRad(const float degree);

float radToDegree(const float rad);

float getRand(const float min, const float max);

template <typename T, unsigned S>
T getRandomElem(const T (&ts)[S]);

void updateMinAndMax(const float x, const float y, const float z, chag::float3* minV, chag::float3* maxV);

chag::float3 createRandomVector(const float minValue, const float maxValue);

typedef std::pair<GameObject*, GameObject*> CollisionPair;
typedef std::vector<CollisionPair> CollisionPairList;
