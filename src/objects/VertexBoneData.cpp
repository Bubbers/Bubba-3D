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

#include "VertexBoneData.h"

VertexBoneData::VertexBoneData() : VertexBoneData(4){

}

VertexBoneData::VertexBoneData(int maxNumBones) : maxNumBones(maxNumBones){
    numUsedBones = 0;

    // Init default values. Neccessary as the vertex shader must iterate through all maxNumBones weights
    for (int i = 0; i < maxNumBones; ++i) {
        ids.push_back(0);
        weights.push_back(0);
    }
}
void VertexBoneData::addBoneData(int boneId, float weight) {
    if(numUsedBones < maxNumBones) {
        ids[numUsedBones] = boneId;
        weights[numUsedBones] = weight;
        numUsedBones++;
        return;
    }

    throw std::invalid_argument  ("Tried to add more than maxNumBones to a mesh");
}