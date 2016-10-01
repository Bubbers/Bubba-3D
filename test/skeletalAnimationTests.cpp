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

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <objects/VertexBoneData.h>
#include "catch.hpp"

TEST_CASE("SmokeTestAddingBones", "[Skeletal]") {
    VertexBoneData boneData;
    boneData.addBoneData(1, 3);
}

TEST_CASE("AddingMoreThanMaxBones", "[Skeletal]") {
    const int maxNumBones = 4;
    VertexBoneData boneData(maxNumBones);
    try {
        for (int i = 0; i < maxNumBones + 1; ++i) {
            boneData.addBoneData(i, i);
        }
    } catch (const std::exception& exception) {
        REQUIRE(true);
        return;
    }
    REQUIRE(false);
}


TEST_CASE("AddingManyBones", "[Skeletal]") {
    const int maxNumBones = 4;
    VertexBoneData boneData(maxNumBones);
    for (int i = 0; i < maxNumBones; ++i) {
        boneData.addBoneData(i, i);
        REQUIRE(boneData.ids.size() == i + 1);
        REQUIRE(boneData.weights.size() == i + 1);
    }
}
