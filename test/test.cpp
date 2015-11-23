#ifndef BUBBA_3D_TEST_H
#define BUBBA_3D_TEST_H


#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../lib/catch.hpp"

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
