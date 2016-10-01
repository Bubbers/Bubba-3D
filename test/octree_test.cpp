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
#ifndef BUBBA_3D_TEST_H__OCTREE
#define BUBBA_3D_TEST_H__OCTREE


#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <Triangle.h>
#include <Utils.h>
#include "catch.hpp"
#include "Octree.h"

using namespace chag;

TEST_CASE("OctreeTriangleLargerThanOctantCount", "[Collision]") {
    Octree octree = Octree();
    Triangle triangle = Triangle(make_vector(-0.1f, 0.0f, 0.0f),make_vector(0.5f, 1.0f, 0.0f),make_vector(1.0f, 0.0f, 0.0f));
    octree.insertTriangle(&triangle);

    REQUIRE(octree.getTriangleCount() == 1);
    REQUIRE(octree.getTriangleCountRecursively() == 1);
    REQUIRE(octree.getTriangles()->size() == 1);
    REQUIRE(octree.getNumberOfSubTrees() == 1);

    REQUIRE(!octree.hasChildren());

    octree.clearChildren();
    REQUIRE(octree.getTriangleCount() == 1);
    REQUIRE(octree.getTriangleCountRecursively() == 1);
    REQUIRE(octree.getTriangles()->size() == 1);
}


TEST_CASE("OctreeTriangleFitInOneSubOctree", "[Collision]") {
    Octree octree = Octree();
    Triangle triangle = Triangle(make_vector(0.0f, 0.0f, 0.0f),make_vector(0.5f, 1.0f, 0.0f),make_vector(1.0f, 0.0f, 0.0f));
    octree.insertTriangle(&triangle);

    REQUIRE(octree.getTriangleCount() == 0);
    REQUIRE(octree.getTriangleCountRecursively() == 1);
    REQUIRE(octree.getTriangles()->size() == 0);
    REQUIRE(octree.getNumberOfSubTrees() == 1 + 8);

    REQUIRE(octree.hasChildren());

    octree.clearChildren();
    REQUIRE(octree.getTriangleCount() == 0);
    REQUIRE(octree.getTriangleCountRecursively() == 0);
    REQUIRE(octree.getTriangles()->size() == 0);
    REQUIRE(octree.getNumberOfSubTrees() == 1);
}

TEST_CASE("OctreeManyTriangles", "[Collision]") {
    Octree octree = Octree();

    for(unsigned int i = 0; i < 100; i++) {
        Triangle triangle = Triangle(createRandomVector(-1.0f, 1.0f), createRandomVector(-1.0f, 1.0f), createRandomVector(-1.0f, 1.0f) );
        octree.insertTriangle(&triangle);
        REQUIRE(octree.getTriangleCountRecursively() == i + 1);
    }
}


#endif
