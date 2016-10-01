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
        numUsedBones++;
        ids.push_back(boneId);
        weights.push_back(weight);
        return;
    }

    throw std::invalid_argument  ("Tried to add more than maxNumBones to a mesh");
}