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


#include <string>
#include <vector>
#include <assimp/scene.h>
#include <map>
#include <BoneMatrices.h>
#include <assimp/Importer.hpp>
#include "linmath/float3.h"
#include "AABB2.h"
#include "assimp/material.h"

/**
 * Class for calculating the transform of all bones at a certain tick in a mesh.
 *
 * Usage:
 * \code{.cpp}
 * aiScene assimpScene;
 * BoneTransformer boneTransformer(assimpScene->mMeshes[0]);
 *
 * for(unsigned int i = 0; i < assimpScene->mMeshes[0]->mNumBones; i++) {
 *     int boneIndex = boneTransformer->createBoneIndexIfAbsent(paiMesh->mBones[i]);
 * }
 *
 * std::vector<chag::float4x4> boneTransforms = boneTransformer.calculateBoneTransforms(elapsedTimeInSeconds);
 * //boneTransforms will now contain the transformation of each bone. Each index corresponds the boneIndex calculated in the for loop.
 * \endcode
 */
class BoneTransformer {

public:
    BoneTransformer(aiScene *aiScene);

    /**
    * Calculates the transform to be applied to each bone at the current time.
    *
    * @param totalElapsedTimeInSeconds The time since the application was started
    * @return A vector containing the transforms of each bone. The index in the vector corresponds to the bones index. The index of a bone can be found in boneNameToIndexMapping.
    *
    */
    std::vector<chag::float4x4> calculateBoneTransforms(float totalElapsedTimeInSeconds);

    /**
     * Updates the bonetransformer to include the bone if not already present.
     *
     * @param bone The bone to add
     * @return The index of the bone
     */
    int createBoneIndexIfAbsent(const aiBone *bone);


private:
    double getCurrentAnimationTick(float totalElapsedTimeInSeconds) const;

    /**
     * Reads the entire (animation) node hierarchy and updates
     * the bone transformations in {@code boneInfos} accordingly.
     */
    void readNodeHierarchyAndUpdateBoneTransformations(float currentAnimationTick, aiNode *currentAssimpNode,
                                                       chag::float4x4 parentMatrix);

    /**
     * Updates the transformation of the bone corresponding to @{code nodeName}
     */
    void updateBoneTransformation(const std::string &nodeName, const chag::float4x4 &globalTransformation);

    /**
     * A node is a bone if there is a bone named exactly the same as the node
     */
    bool nodeIsABone(const std::string &nodeName) const;

    chag::float4x4 getCurrentNodeTransformation(float currentAnimationTick, const aiNode *currentAssimpNode,
                                                const std::string nodeName);


    /**
     * Interpolates the two animation matrices nearest the current tick
     */
    chag::float4x4 getInterpolatedAnimationMatrix(float currentAnimationTick, const aiNodeAnim *nodeAnimation);
    chag::float4x4 getInterpolatedTranslationMatrix(float currentAnimationTick, const aiNodeAnim *nodeAnimation);
    chag::float4x4 getInterpolatedRotationMatrix(float currentAnimationTick, const aiNodeAnim *nodeAnimation);
    chag::float4x4 getInterpolatedScalingMatrix(float currentAnimationTick, const aiNodeAnim *nodeAnimation);

    /**
     * Given the name of a node, fetches the corresponding animation node.
     */
    const aiNodeAnim *getAnimationNode(const std::string &nodeName);

    /**
     * Searches through an animations channels for the animation node
     */
    const aiNodeAnim* findNodeAnim(const aiAnimation* animation, const std::string nodeName);

    aiVector3D calculateScalingInterpolation(float currentAnimationTick, const aiNodeAnim *nodeAnimation);
    aiQuaternion calculateRotationInterpolation(float currentAnimationTick, const aiNodeAnim *nodeAnimation);
    aiVector3D calculateTranslationInterpolation(float currentAnimationTick, const aiNodeAnim *nodeAnimation);

    unsigned int findScalingIndexRightBeforeTick(float currentAnimationTick, const aiNodeAnim *nodeAnimation);
    unsigned int findRotationIndexRightBeforeTick(float currentAnimationTick, const aiNodeAnim *nodeAnimation);
    unsigned int findTranslationIndexRightBeforeTick(float currentAnimationTick, const aiNodeAnim *nodeAnimation);

    //Matrix for transforming FROM bone space TO world space
    chag::float4x4 globalInverseTransform;
    std::map<std::string, int> boneNameToIndexMapping;
    std::vector<BoneMatrices*> boneInfos;
    const aiScene *assimpScene;
    int numberOfBones;
};


