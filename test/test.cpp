#ifndef BUBBA_3D_TEST_H
#define BUBBA_3D_TEST_H


#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../lib/catch.hpp"
#include "BFBroadPhase.h"
#include "float3.h"
#include "Collider.h"

using namespace chag;


TEST_CASE("AABB intersects", "BruteForceBroadPhase.intersect(a1,a2)") {


        AABB a1;
    a1.maxV = make_vector(1.0f, 1.0f, 1.0f);
    a1.minV = make_vector(0.0f, 0.0f, 0.0f);

    AABB a2;
    a2.maxV = make_vector(0.5f, 0.5f, 0.5f);
    a2.minV = make_vector(0.0f, 0.0f, 0.0f);
    REQUIRE(AabbAabbintersection(&a1, &a2));
    REQUIRE(AabbAabbintersection(&a2, &a1));

    a2.maxV = make_vector(-0.5f, -0.5f, -0.5f);
    a2.minV = make_vector(-1.0f, -1.0f, -1.0f);
    REQUIRE(AabbAabbintersection(&a1, &a2));
    REQUIRE(AabbAabbintersection(&a2, &a1));
}

TEST_CASE("","") {
    Triangle t1(make_vector(0.0f, 0.0f, 0.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 0.0f, 0.0f));
    Triangle t2(make_vector(5.0f, 5.0f, 5.0f), make_vector(6.0f, 6.0f, 5.0f), make_vector(7.0f, 5.0f, 5.0f));
    Triangle t3(make_vector(0.0f, 0.0f, 1.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 0.0f, -1.0f));
    Triangle t4(make_vector(0.0f, -1.0f, 1.0f), make_vector(1.0f, 1.0f, 0.0f), make_vector(2.0f, 1.0f, -1.0f));
    Triangle t5(make_vector(4.0f, 0.0f, 1.0f), make_vector(5.0f, 1.0f, 0.0f), make_vector(6.0f, 0.0f, -1.0f));
    Triangle t6(make_vector(0.5f, 0.0f, 1.0f), make_vector(1.1f, 1.0f, 0.0f), make_vector(2.1f, 0.0f, -1.0f));
    REQUIRE(!triangleTriangleIntersection(&t1, &t2));
    REQUIRE(!triangleTriangleIntersection(&t2, &t1));
    REQUIRE(triangleTriangleIntersection(&t1, &t3));
    REQUIRE(triangleTriangleIntersection(&t3, &t1));
    REQUIRE(triangleTriangleIntersection(&t4, &t1));
    REQUIRE(!triangleTriangleIntersection(&t1, &t5));
    REQUIRE(triangleTriangleIntersection(&t1, &t6));

}


#endif //BUBBA_3D_TEST_H
