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
#include <vector>
#include "stdint.h"
#include <stdexcept>

struct VertexBoneData {
    const unsigned int maxNumBones;
    unsigned int numUsedBones;
    std::vector<int> ids;
    std::vector<float> weights;

    VertexBoneData();
    VertexBoneData(int maxNumBones);

    /**
     * Adds bone data to the next free bone slot
     */
    void addBoneData(int boneId, float weight) ;
};
