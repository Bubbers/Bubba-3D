//
// Created by simon on 2016-06-19.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <userInterface/InsideSquareChecker.h>
#include <userInterface/InsideRoundedSquareChecker.h>
#include "catch.hpp"

TEST_CASE("IsInsideSquare","[isInsideSquare]"){
    InsideSquareChecker* squareChecker = new InsideSquareChecker(10,10,20,20);
    REQUIRE(squareChecker->isInside(10,10));
    REQUIRE(squareChecker->isInside(29,29));
    REQUIRE(squareChecker->isInside(15,23));
    REQUIRE_FALSE(squareChecker->isInside(9,11));
    REQUIRE_FALSE(squareChecker->isInside(11,9));
    REQUIRE_FALSE(squareChecker->isInside(9,9));
    REQUIRE_FALSE(squareChecker->isInside(32,25));
    REQUIRE_FALSE(squareChecker->isInside(25,32));
};

TEST_CASE("IsInsideRoundedSquare","[isInsideRoundedSquare]"){
    InsideSquareChecker* squareChecker = new InsideRoundedSquareChecker(10,10,90,90,0,10,20,30);
    double sin45 = sin(M_PI/4.0f) + 0.1f;
    REQUIRE(squareChecker->isInside(10,10));
    REQUIRE(squareChecker->isInside(95,15));
    REQUIRE_FALSE(squareChecker->isInside(90+sin45*10,10+10-(sin45*10)));
    REQUIRE(squareChecker->isInside(90,90));
    REQUIRE_FALSE(squareChecker->isInside(80+sin45*20,80+sin45*20));
    REQUIRE(squareChecker->isInside(25,75));
    REQUIRE_FALSE(squareChecker->isInside(10+30-(sin45*30),70+sin45*30));
}