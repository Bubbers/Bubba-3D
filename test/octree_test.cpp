#ifndef BUBBA_3D_TEST_H__OCTREE
#define BUBBA_3D_TEST_H__OCTREE


#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <Triangle.h>
#include <Utils.h>
#include "catch.hpp"
#include "Octree.h"


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