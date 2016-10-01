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
