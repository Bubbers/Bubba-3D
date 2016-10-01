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
#ifndef BUBBA_3D_TEST_H
#define BUBBA_3D_TEST_H


#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <vector>
#include "Triangle.h"
#include "catch.hpp"
#include "linmath/float3.h"
#include "Collider.h"
#include "Utils.h"
#include "Octree.h"

using namespace chag;

#define MAX_ITERATIONS 100
#define MAX_TRIANGLES 50
#define MAX_SPACE 10
#define MIN_SPACE -10
#define ORIGIN_POS make_vector(0.0f, 0.0f, 0.0f)
#define HALF_VECTOR make_vector(10.0f, 10.0f, 10.0f)




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



TEST_CASE("TriangleTriangleShouldIntersect", "[Collision]") {
    Triangle t1(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 0.0f, 0.0f));
    Triangle t3(make_vector(0.0f, 0.0f, 1.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 0.0f, -1.0f));
    Triangle t4(make_vector(0.0f, -1.0f, 1.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 1.0f, -1.0f));
    Triangle t6(make_vector(0.5f, 0.0f, 1.0f), make_vector(1.1f, 1.0f, 0.0f), make_vector(2.1f, 0.0f, -1.0f));
    REQUIRE(triangleTriangleIntersection(&t1, &t3));
    REQUIRE(triangleTriangleIntersection(&t3, &t1));
    REQUIRE(triangleTriangleIntersection(&t4, &t1));
    REQUIRE(triangleTriangleIntersection(&t1, &t6));
}

TEST_CASE("TriangleTriangleShouldNotIntersect", "[Collision]") {
    Triangle t1(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 0.0f, 0.0f));
    Triangle t2(make_vector(5.0f, 5.0f, 5.0f), make_vector(6.0f, 6.0f, 5.0f), make_vector(7.0f, 5.0f, 5.0f));
    Triangle t5(make_vector(4.0f, 0.0f, 1.0f), make_vector(5.0f, 1.0f, 0.0f), make_vector(6.0f, 0.0f, -1.0f));
    REQUIRE(!triangleTriangleIntersection(&t1, &t2));
    REQUIRE(!triangleTriangleIntersection(&t2, &t1));
    REQUIRE(!triangleTriangleIntersection(&t1, &t5));
}


TEST_CASE("OctreeOctreeShouldIntersect", "[Collision]") {
    Octree tree1 = Octree(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 1.0f));
    Triangle t1(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 0.0f, 0.0f));
    tree1.insertTriangle(&t1);

    Octree tree2 = Octree(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 1.0f));
    Triangle t2(make_vector(0.1f, 0.0f, 0.0f), make_vector(1.1f, 1.0f, 0.0f), make_vector(2.1f, 0.0f, 0.0f));
    tree2.insertTriangle(&t2);

    float4x4 model1 = make_identity<float4x4>();
    float4x4 model2 = make_identity<float4x4>();

    REQUIRE(octreeOctreeIntersection(&tree1, &model1, &tree1, &model1));
    REQUIRE(octreeOctreeIntersection(&tree1, &model1, &tree2, &model2));
}

TEST_CASE("OctreeOctreeShouldNotIntersect", "[Collision]") {
    Octree tree1 = Octree(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 1.0f));
    Triangle t1(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 0.0f, 0.0f));
    tree1.insertTriangle(&t1);

    Octree tree2 = Octree(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 1.0f));
    Triangle t2(make_vector(0.0f, 0.0f, 1.0f), make_vector(1.0f, 1.0f, 1.0f), make_vector(2.0f, 0.0f, 1.0f));
    tree2.insertTriangle(&t2);

    float4x4 model1 = make_identity<float4x4>();
    float4x4 model2 = make_identity<float4x4>();

    REQUIRE(!octreeOctreeIntersection(&tree1, &model1, &tree2, &model2));
}

TEST_CASE("OctreeOctreeShouldIntersectRotated", "[Collision]") {
    Octree tree1 = Octree(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 1.0f) );
    Triangle t1(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 0.0f, 0.0f));
    tree1.insertTriangle(&t1);

    Octree tree2 = Octree(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 1.0f));
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

TEST_CASE("TriangleOctreeShouldIntersect", "[Collision, Random]") {
    unsigned int iteration = 0;

    while (iteration++ < MAX_ITERATIONS) {
        std::vector<Triangle*> triangles;
        Octree octreeAllTriangles(ORIGIN_POS, HALF_VECTOR);

        unsigned int maxTriangles = getRand(1.0, MAX_TRIANGLES);

        for (unsigned int i = 0; i < maxTriangles; i++) {
            Triangle *triangle = new Triangle(createRandomVector(MIN_SPACE, MAX_SPACE),
                                              createRandomVector(MIN_SPACE, MAX_SPACE),
                                              createRandomVector(MIN_SPACE, MAX_SPACE));
            triangles.push_back(triangle);
            octreeAllTriangles.insertTriangle(triangle);
        }

        for (unsigned int i = 0; i < triangles.size(); i++) {
            Triangle *triangle1 = triangles[i];
            for (unsigned int j = i + 1; j < triangles.size(); j++) {
                Triangle *triangle2 = triangles[j];

                float3 minV = make_vector(FLT_MAX, FLT_MAX, FLT_MAX);
                float3 maxV = make_vector(-FLT_MAX, -FLT_MAX, -FLT_MAX);

                for (float3 point : triangle2->getBoundingBox()->points) {
                    updateMinAndMax(point.x, point.y, point.z, &minV, &maxV);
                }

                float3 halfVector = (maxV - minV) / 2;
                float3 origin = maxV - halfVector;

                Octree octreeSingleTriangle(origin, halfVector);
                octreeSingleTriangle.insertTriangle(triangle1);
                float4x4 modelMatrix = make_identity<float4x4>();

                bool intersectingOctrees = octreeOctreeIntersection(&octreeSingleTriangle, &modelMatrix,
                                                                    &octreeAllTriangles, &modelMatrix);

                if (triangleTriangleIntersection(triangle1, triangle2)) {
                    REQUIRE(intersectingOctrees);
                }
            }
        }
    }
}




#endif //BUBBA_3D_TEST_H
