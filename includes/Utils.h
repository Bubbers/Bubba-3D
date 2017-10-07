/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */
#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "linmath/float3.h"
#include "linmath/float4x4.h"
#include "assimp/material.h"
#include <memory>
#include <vector>

class ShaderProgram;
class GameObject;


struct Fbo {
    GLuint id;
    GLuint texture;
    std::shared_ptr<ShaderProgram> shaderProgram;
    GLuint depthbuffer;

    int width, height;
};


bool fequals(double a, double b);

float degreeToRad(const float degree);

float radToDegree(const float rad);

float getRand(const float min, const float max);

template <typename T, unsigned S>
T getRandomElem(const T (&ts)[S]);

void updateMinAndMax(const float x, const float y, const float z, chag::float3* minV, chag::float3* maxV);

chag::float3 createRandomVector(const float minValue, const float maxValue);

/**
 * Used to convert assimp matrices to interal format
 */
chag::float4x4 convertAiMatrixToFloat4x4(aiMatrix4x4 fromMatrix) ;
/**
 * Used to convert assimp matrices to interal format
 */
chag::float3x3 convertAiMatrixToFloat3x3(aiMatrix3x3 fromMatrix) ;

typedef std::pair<std::shared_ptr<GameObject>, std::shared_ptr<GameObject>> CollisionPair;
typedef std::vector<CollisionPair> CollisionPairList;

chag::float3 linearSmoothStep(chag::float3 v1, chag::float3 v2, float time);
