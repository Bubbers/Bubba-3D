#ifndef BUBBA_3D_TEST_H
#define BUBBA_3D_TEST_H


#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../lib/catch.hpp"
#include "BFBroadPhase.h"
#include "float3.h"

using namespace chag;


TEST_CASE("AABB intersects", "BruteForceBroadPhase.intersect(a1,a2)") {
    BFBroadPhase br;

    AABB a1;
    a1.maxV = make_vector(1.0f, 1.0f, 1.0f);
    a1.minV = make_vector(0.0f, 0.0f, 0.0f);

    AABB a2;
    a2.maxV = make_vector(0.5f, 0.5f, 0.5f);
    a2.minV = make_vector(0.0f, 0.0f, 0.0f);
    REQUIRE(br.intersects(&a1, &a2));
    REQUIRE(br.intersects(&a2, &a1));

    a2.maxV = make_vector(-0.5f, -0.5f, -0.5f);
    a2.minV = make_vector(-1.0f, -1.0f, -1.0f);
    REQUIRE(!br.intersects(&a1, &a2));
    REQUIRE(!br.intersects(&a2, &a1));
}

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}


    TEST_CASE("Factorials are computed", "lol") {
        REQUIRE(1== 1);
    }

    TEST_CASE("Factorials are compuated", "lol2") {
        REQUIRE(12 == 12);
    }



#endif //BUBBA_3D_TEST_H
