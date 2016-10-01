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

#include <functional>
#include <linmath/float3.h>
#include <vector>
#include <set>
#include <ctime>
#include <math.h>
#include "AABB2.h"

class Triangle;

const int MAX_DEPTH = 8;
const int MAX_CHILDREN = 16;

class Octree {
public:
    /**
     * Initiates the Octree with default values for origin and halfVector.
     * Should NOT be used unless you really want a small Octree.
     * // TODO(Bubbad) Make Octree resize after insertions to make this constructor useful
     */
    Octree();

    /**
     * Initiates a Octree that can contain triangles that
     * are at within the cube of size 2*halfVector that
     * is centered around the specified origin.
     *
     * @param origin The center position of the Octree
     * @param halfVector Half the length of the surrounding cube
     */
    Octree(chag::float3 origin, chag::float3 halfVector);

    ~Octree();

    void insertTriangle(Triangle* t);
    void insertAll(std::vector<Triangle*> &triangles);

    /**
     * return True if the Octree is recursed and have triangles in deeper levels, false otherwise
     */
    bool hasChildren();
    /**
     * Fills the specified list with all the Octrees child Octrees
     *
     * @param octs List to be filled
     */
    void getChildren(std::vector<Octree*>* octs);
    /**
     * Removes all the Octrees children and their triangles
     */
    void clearChildren();

    int getOctantContainingPoint(const chag::float3& point);
    int getTriangleCount();
    /**
     * @return The triangle count of this Octree + the triangle count of all its child Octrees
     */
    int getTriangleCountRecursively();
    /**
     * @return A list of the triangles contained in this level of the Octree
     */
    std::vector<Triangle*> *getTriangles();

    /**
     * @return Recursivly searches the Octrees child Octrees and count how many sub trees there are.
     */
    int getNumberOfSubTrees();


    /**
     * Recursively searches the Octree for triangles hit by the ray cast
     *
     * @param triangleList A list to be filled with all intersected triangles
     */
    void getTrianglesInsersectedByRayCast(chag::float3 rayOrigin, chag::float3 rayVector,
                                          std::vector<Triangle*> *triangleList);

    /**
     * NOTE: The AABB is not transformed.
     */
    AABB* getAABB();

private:
    Octree(chag::float3 origin, chag::float3 halfVector, int depth);

    void setupAABB(chag::float3 origin, chag::float3 halfVector);

    /**
     * Allocates memory for 8 children.
     */
    void createChildren();

    /**
     * Combines both vectors into one. If the comparator is true for an element of the vector,
     * it takes the element from p1, else p2.
     * Useful for eg creating a vector containing the max or min elements of two vectors.
     */
    chag::float3 combineTwoPointsByComparator(chag::float3 p1, chag::float3 p2,
                                              std::function<bool(float, float)> comparator);

    bool rayCastIntersectsAABB(chag::float3 rayOrigin, chag::float3 rayVector);
    void putTrianglesToList(std::vector<Triangle *> *triangleList);

    chag::float3 origin;
    chag::float3 halfVector;

    Octree *children[8];
    std::vector<Triangle*> ts;
    int depth;


    AABB aabb;
};
