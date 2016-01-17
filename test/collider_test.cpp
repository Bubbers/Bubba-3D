#ifndef BUBBA_3D_TEST_H
#define BUBBA_3D_TEST_H


#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <Triangle.h>
#include "catch.hpp"
#include "BFBroadPhase.h"
#include "float3.h"
#include "Collider.h"
#include "Utils.h"
#include "Octree.h"

using namespace chag;


TEST_CASE("AABBShouldIntersect", "[Collision]") {
    AABB a1;
    a1.maxV = make_vector(1.0f, 1.0f, 1.0f);
    a1.minV = make_vector(0.0f, 0.0f, 0.0f);

    AABB a2;
    a2.maxV = make_vector(0.5f, 0.5f, 0.5f);
    a2.minV = make_vector(0.0f, 0.0f, 0.0f);

    REQUIRE(AabbAabbintersection(&a1, &a2));
    REQUIRE(AabbAabbintersection(&a2, &a1));
}


TEST_CASE("AABBShouldIntersectNegative", "[Collision]") {
    AABB a1;
    a1.minV = make_vector(-1.0f, -1.0f, -1.0f);
    a1.maxV = make_vector(0.0f, 0.0f, 0.0f);

    AABB a2;
    a2.minV = make_vector(-0.5f, -0.5f, -0.5f);
    a2.maxV = make_vector(0.0f, 0.0f, 0.0f);

    REQUIRE(AabbAabbintersection(&a1, &a2));
    REQUIRE(AabbAabbintersection(&a2, &a1));
}


TEST_CASE("AABBShouldIntersectItself", "[Collision]") {
    AABB a1;
    a1.maxV = make_vector(1.0f, 1.0f, 1.0f);
    a1.minV = make_vector(0.0f, 0.0f, 0.0f);

    REQUIRE(AabbAabbintersection(&a1, &a1));
}


TEST_CASE("AABBShouldIntersectCornerCases", "[Collision]") {
    AABB a1;
    a1.maxV = make_vector(1.0f, 1.0f, 1.0f);
    a1.minV = make_vector(0.0f, 0.0f, 0.0f);

    AABB a2;
    a2.maxV = make_vector(2.0f, 2.0f, 2.0f);
    a2.minV = make_vector(1.0f, 1.0f, 1.0f);
    REQUIRE(AabbAabbintersection(&a1, &a2));
    REQUIRE(AabbAabbintersection(&a2, &a1));

    a2.maxV = make_vector(0.0f,   0.0f,  0.0f);
    a2.minV = make_vector(-1.0f, -1.0f, -1.0f);
    REQUIRE(AabbAabbintersection(&a1, &a2));
    REQUIRE(AabbAabbintersection(&a2, &a1));

    a2.maxV = make_vector(0.0f,  0.0f, 0.0f);
    a2.minV = make_vector(0.0f, -1.0f, 0.0f);
    REQUIRE(AabbAabbintersection(&a1, &a2));
    REQUIRE(AabbAabbintersection(&a2, &a1));
}

TEST_CASE("AABBShouldNotIntersect", "[Collision]") {
    AABB a1;
    a1.maxV = make_vector(1.0f, 1.0f, 1.0f);
    a1.minV = make_vector(0.0f, 0.0f, 0.0f);

    AABB a2;
    a2.maxV = make_vector(-0.5f, -0.5f, -0.5f);
    a2.minV = make_vector(-1.0f, -1.0f, -1.0f);
    REQUIRE(!AabbAabbintersection(&a1, &a2));
    REQUIRE(!AabbAabbintersection(&a2, &a1));
}

TEST_CASE("AABBShouldNotIntersectCornerCases", "[Collision]") {
    AABB a1;
    a1.maxV = make_vector(1.0f, 1.0f, 1.0f);
    a1.minV = make_vector(0.0f, 0.0f, 0.0f);

    AABB a2;
    a2.maxV = make_vector(2.0f, 2.0f, 2.0f);
    a2.minV = make_vector(1.01f, 1.01f, 1.01f);
    REQUIRE(!AabbAabbintersection(&a1, &a2));
    REQUIRE(!AabbAabbintersection(&a2, &a1));
}



TEST_CASE("TriangleTriangleShouldIntersect","[Collision]") {
    Triangle t1(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 0.0f, 0.0f));
    Triangle t3(make_vector(0.0f, 0.0f, 1.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 0.0f, -1.0f));
    Triangle t4(make_vector(0.0f, -1.0f, 1.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 1.0f, -1.0f));
    Triangle t6(make_vector(0.5f, 0.0f, 1.0f), make_vector(1.1f, 1.0f, 0.0f), make_vector(2.1f, 0.0f, -1.0f));
    REQUIRE(triangleTriangleIntersection(&t1, &t3));
    REQUIRE(triangleTriangleIntersection(&t3, &t1));
    REQUIRE(triangleTriangleIntersection(&t4, &t1));
    REQUIRE(triangleTriangleIntersection(&t1, &t6));
}

TEST_CASE("TriangleTriangleShouldNotIntersect","[Collision]") {
    Triangle t1(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 0.0f, 0.0f));
    Triangle t2(make_vector(5.0f, 5.0f, 5.0f), make_vector(6.0f, 6.0f, 5.0f), make_vector(7.0f, 5.0f, 5.0f));
    Triangle t5(make_vector(4.0f, 0.0f, 1.0f), make_vector(5.0f, 1.0f, 0.0f), make_vector(6.0f, 0.0f, -1.0f));
    REQUIRE(!triangleTriangleIntersection(&t1, &t2));
    REQUIRE(!triangleTriangleIntersection(&t2, &t1));
    REQUIRE(!triangleTriangleIntersection(&t1, &t5));
}


TEST_CASE("OctreeOctreeShouldIntersect","[Collision]"){
    Octree tree1 = Octree(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 1.0f) , 0);
    Triangle t1(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 0.0f, 0.0f));
    tree1.insertTriangle(&t1);

    Octree tree2 = Octree(make_vector(0.0f, 0.0f, 0.0f),make_vector(1.0f, 1.0f, 1.0f), 0);
    Triangle t2(make_vector(0.1f, 0.0f, 0.0f), make_vector(1.1f, 1.0f, 0.0f), make_vector(2.1f, 0.0f, 0.0f));
    tree2.insertTriangle(&t2);

    float4x4 model1 = make_identity<float4x4>();
    float4x4 model2 = make_identity<float4x4>();

    REQUIRE(octreeOctreeIntersection(&tree1, &model1, &tree1, &model1));
    REQUIRE(octreeOctreeIntersection(&tree1, &model1, &tree2, &model2));
}

TEST_CASE("OctreeOctreeShouldNotIntersect","[Collision]"){
    Octree tree1 = Octree(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 1.0f) , 0);
    Triangle t1(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 0.0f, 0.0f));
    tree1.insertTriangle(&t1);

    Octree tree2 = Octree(make_vector(0.0f, 0.0f, 0.0f),make_vector(1.0f, 1.0f, 1.0f), 0);
    Triangle t2(make_vector(0.0f, 0.0f, 1.0f), make_vector(1.0f, 1.0f, 1.0f), make_vector(2.0f, 0.0f, 1.0f));
    tree2.insertTriangle(&t2);

    float4x4 model1 = make_identity<float4x4>();
    float4x4 model2 = make_identity<float4x4>();

    REQUIRE(!octreeOctreeIntersection(&tree1, &model1, &tree2, &model2));
}

TEST_CASE("OctreeOctreeShouldIntersectRotated","[Collision]"){
    Octree tree1 = Octree(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 1.0f) , 0);
    Triangle t1(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 0.0f, 0.0f));
    tree1.insertTriangle(&t1);

    Octree tree2 = Octree(make_vector(0.0f, 0.0f, 0.0f),make_vector(1.0f, 1.0f, 1.0f), 0);
    Triangle t2(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 0.0f, 0.0f));
    tree2.insertTriangle(&t2);

    float4x4 model1 = make_identity<float4x4>();
    float4x4 model2;

    model2 = make_rotation_y<float4x4>(degreeToRad(90)) * make_identity<float4x4>();
    REQUIRE(octreeOctreeIntersection(&tree1, &model1, &tree2, &model2));

    model2 = make_rotation_y<float4x4>(degreeToRad(180)) * make_identity<float4x4>();
    REQUIRE(octreeOctreeIntersection(&tree1, &model1, &tree2, &model2));

    model2 = make_rotation_y<float4x4>(degreeToRad(360)) * make_identity<float4x4>();
    REQUIRE(octreeOctreeIntersection(&tree1, &model1, &tree2, &model2));
}




#endif //BUBBA_3D_TEST_H
