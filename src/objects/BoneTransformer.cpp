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
#include "linmath/float3x3.h"
#include <ResourceManager.h>
#include <Utils.h>
#include "Mesh.h"
#include "BoneTransformer.h"
#include "linmath/float4x4.h"

BoneTransformer::BoneTransformer(aiScene *aiScene){
    numberOfBones = 0;
    globalInverseTransform = convertAiMatrixToFloat4x4(aiScene->mRootNode->mTransformation.Inverse());
    this->assimpScene = aiScene;
}

void BoneTransformer::readNodeHierarchyAndUpdateBoneTransformations(float currentAnimationTick,
                                                                    aiNode *currentAssimpNode,
                                                                    chag::float4x4 parentMatrix) {
    std::__cxx11::string nodeName(currentAssimpNode->mName.data);

    chag::float4x4 nodeTransformationMatrix = getCurrentNodeTransformation(currentAnimationTick, currentAssimpNode, nodeName);
    chag::float4x4 globalTransformation = parentMatrix * nodeTransformationMatrix;

    // If the node isn't a bone we dont need to update any bones (duuuh)
    if(nodeIsABone(nodeName)) {
        updateBoneTransformation(nodeName, globalTransformation);
    }

    for (unsigned int i = 0; i < currentAssimpNode->mNumChildren; i++) {
        readNodeHierarchyAndUpdateBoneTransformations(currentAnimationTick, currentAssimpNode->mChildren[i],
                                                      globalTransformation);
    }
}

void BoneTransformer::updateBoneTransformation(const std::__cxx11::string &nodeName, const chag::float4x4 &globalTransformation) {
    uint boneIndex = boneNameToIndexMapping[nodeName];

    boneInfos[boneIndex]->finalTransformation = globalInverseTransform * globalTransformation * boneInfos[boneIndex]->boneOffset;
}

bool BoneTransformer::nodeIsABone(const std::__cxx11::string &nodeName) const {
    return boneNameToIndexMapping.find(nodeName) != boneNameToIndexMapping.end();
}

chag::float4x4 BoneTransformer::getCurrentNodeTransformation(float currentAnimationTick,
                                                             const aiNode *currentAssimpNode,
                                                             const std::__cxx11::string nodeName) {
    const aiNodeAnim *nodeAnimation = getAnimationNode(nodeName);
    if(nodeAnimation == nullptr) {
        /**
         * If we can't find a nodeAnimation, there is no corresponding bone.
         * Then the current node is only used for applying a transformation matrix to
         * the hierarchy. No interpolation needed.
         */
        return convertAiMatrixToFloat4x4(currentAssimpNode->mTransformation);
    } else {
        return getInterpolatedAnimationMatrix(currentAnimationTick, nodeAnimation);

    }
}

chag::float4x4 BoneTransformer::getInterpolatedAnimationMatrix(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
    chag::float4x4 scalingMatrix = getInterpolatedScalingMatrix(currentAnimationTick, nodeAnimation);
    chag::float4x4 rotationMatrix = getInterpolatedRotationMatrix(currentAnimationTick, nodeAnimation);
    chag::float4x4 translationMatrix = getInterpolatedTranslationMatrix(currentAnimationTick, nodeAnimation);

    chag::float4x4 interpolatedMatrix = translationMatrix * rotationMatrix * scalingMatrix;
    return interpolatedMatrix;
}

chag::float4x4 BoneTransformer::getInterpolatedTranslationMatrix(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
    aiVector3D translation = calculateTranslationInterpolation(currentAnimationTick, nodeAnimation);
    chag::float4x4 translationMatrix =
                chag::make_translation(chag::make_vector(
                        translation.x,
                        translation.y,
                        translation.z));
    return translationMatrix;
}

chag::float4x4 BoneTransformer::getInterpolatedRotationMatrix(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
    aiQuaternion rotationQuaternion = calculateRotationInterpolation(currentAnimationTick, nodeAnimation);
    chag::float4x4 rotationMatrix = chag::make_matrix(
                convertAiMatrixToFloat3x3(rotationQuaternion.GetMatrix()),
                chag::make_vector(0.0f, 0.0f, 0.0f)
        );
    return rotationMatrix;
}

chag::float4x4 BoneTransformer::getInterpolatedScalingMatrix(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
    aiVector3D scaling = calculateScalingInterpolation(currentAnimationTick, nodeAnimation);
    chag::float4x4 scalingMatrix = chag::make_scale<chag::float4x4>(chag::make_vector(scaling.x, scaling.y, scaling.z));
    return scalingMatrix;
}

const aiNodeAnim *BoneTransformer::getAnimationNode(const std::__cxx11::string &nodeName) {
    const aiAnimation* animation = assimpScene->mAnimations[0];

    const aiNodeAnim* nodeAnimation = findNodeAnim(animation, nodeName);
    return nodeAnimation;
}

const aiNodeAnim* BoneTransformer::findNodeAnim(const aiAnimation* animation, const std::__cxx11::string nodeName) {
    for (unsigned int i = 0; i < animation->mNumChannels; i++) {
        const aiNodeAnim* nodeAnimation = animation->mChannels[i];
        if(std::__cxx11::string(nodeAnimation->mNodeName.data) == nodeName) {
            return nodeAnimation;
        }
    }
    return nullptr;
}

aiVector3D BoneTransformer::calculateScalingInterpolation(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
    if(nodeAnimation->mNumScalingKeys <= 1) {
        return nodeAnimation->mScalingKeys[0].mValue;
    }

    unsigned int currentScalingIndex = findScalingIndexRightBeforeTick(currentAnimationTick, nodeAnimation);
    unsigned int nextScalingIndex = currentScalingIndex + 1;
    assert(nextScalingIndex < nodeAnimation->mNumScalingKeys);

    double deltaTimeBetweenAnimations = nodeAnimation->mScalingKeys[nextScalingIndex].mTime -
                                       nodeAnimation->mScalingKeys[currentScalingIndex].mTime;
    double deltaFactor = (currentAnimationTick - nodeAnimation->mScalingKeys[currentScalingIndex].mTime) /
                         deltaTimeBetweenAnimations;
    assert(deltaFactor >= 0.0f && deltaFactor <= 1.0f);
    const aiVector3D &currentScalingVector = nodeAnimation->mScalingKeys[currentScalingIndex].mValue;
    const aiVector3D &nextScalingVector = nodeAnimation->mScalingKeys[nextScalingIndex].mValue;

    return (nextScalingVector - currentScalingVector) * (float)deltaFactor;
}

aiQuaternion BoneTransformer::calculateRotationInterpolation(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
    if(nodeAnimation->mNumRotationKeys <= 1) {
        return nodeAnimation->mRotationKeys[0].mValue;
    }

    unsigned int currentRotationIndex = findRotationIndexRightBeforeTick(currentAnimationTick, nodeAnimation);
    unsigned int nextRotationIndex = currentRotationIndex + 1;
    assert(nextRotationIndex < nodeAnimation->mNumRotationKeys);

    double deltaTimeBetweenAnimations = nodeAnimation->mRotationKeys[nextRotationIndex].mTime -
                                       nodeAnimation->mRotationKeys[currentRotationIndex].mTime;
    double deltaFactor = (currentAnimationTick - nodeAnimation->mRotationKeys[currentRotationIndex].mTime) /
                         deltaTimeBetweenAnimations;
    assert(deltaFactor >= 0.0f && deltaFactor <= 1.0f);
    const aiQuaternion &currentRotationQuaternion = nodeAnimation->mRotationKeys[currentRotationIndex].mValue;
    const aiQuaternion &nextRotationQuaternion = nodeAnimation->mRotationKeys[nextRotationIndex].mValue;
    aiQuaternion interpolatedQuaternion;
    ::aiQuaterniont<float>::Interpolate(interpolatedQuaternion, currentRotationQuaternion, nextRotationQuaternion, deltaFactor);

    return interpolatedQuaternion;
}

aiVector3D BoneTransformer::calculateTranslationInterpolation(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
    if(nodeAnimation->mNumPositionKeys <= 1) {
        return nodeAnimation->mPositionKeys[0].mValue;
    }

    unsigned int currentTranslationIndex = findTranslationIndexRightBeforeTick(currentAnimationTick, nodeAnimation);
    unsigned int nextTranslationIndex = currentTranslationIndex + 1;
    assert(nextTranslationIndex < nodeAnimation->mNumPositionKeys);

    double deltaTimeBetweenAnimations = nodeAnimation->mPositionKeys[nextTranslationIndex].mTime -
                                       nodeAnimation->mPositionKeys[currentTranslationIndex].mTime;
    double deltaFactor = (currentAnimationTick - nodeAnimation->mPositionKeys[currentTranslationIndex].mTime) /
                         deltaTimeBetweenAnimations;
    assert(deltaFactor >= 0.0f && deltaFactor <= 1.0f);
    const aiVector3D &currentTranslationVector = nodeAnimation->mPositionKeys[currentTranslationIndex].mValue;
    const aiVector3D &nextTranslationVector = nodeAnimation->mPositionKeys[nextTranslationIndex].mValue;

    return currentTranslationVector + ((nextTranslationVector - currentTranslationVector) * (float)deltaFactor);

}

unsigned int BoneTransformer::findScalingIndexRightBeforeTick(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
    for (unsigned int i = 0; i < nodeAnimation->mNumScalingKeys - 1; i++) {
        if(currentAnimationTick < nodeAnimation->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }

    throw std::invalid_argument("Tried to find a scaling index in an animation, but couldn't find it");
}

unsigned int BoneTransformer::findRotationIndexRightBeforeTick(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
    for (unsigned int i = 0; i < nodeAnimation->mNumRotationKeys - 1; i++) {
        if(currentAnimationTick < nodeAnimation->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }

    throw std::invalid_argument("Tried to find a rotation index in an animation, but couldn't find it");
}

unsigned int BoneTransformer::findTranslationIndexRightBeforeTick(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
    for (unsigned int i = 0; i < nodeAnimation->mNumPositionKeys - 1; i++) {
        if(currentAnimationTick < nodeAnimation->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }

    throw std::invalid_argument("Tried to find a translation index in an animation, but couldn't find it");
}

std::vector<chag::float4x4> BoneTransformer::calculateBoneTransforms(float totalElapsedTimeInSeconds) {

    chag::float4x4 rootMatrix = chag::make_identity<chag::float4x4>();

    double currentAnimationTick = getCurrentAnimationTick(totalElapsedTimeInSeconds);

    readNodeHierarchyAndUpdateBoneTransformations((float)currentAnimationTick, assimpScene->mRootNode, rootMatrix);

    std::vector<chag::float4x4> boneTransformMatrices;
    boneTransformMatrices.resize(numberOfBones);
    for (int bone = 0; bone < numberOfBones; bone++) {
        boneTransformMatrices[bone] = boneInfos[bone]->finalTransformation;
    }
    return boneTransformMatrices;
}

int BoneTransformer::createOrGetBoneIndex(const aiMesh *paiMesh, unsigned int i) {
    int boneIndex;
    std::string boneName(paiMesh->mBones[i]->mName.data);

    if(this->boneNameToIndexMapping.find(boneName) == this->boneNameToIndexMapping.end()) {

            this->boneNameToIndexMapping.insert(std::pair<std::string, int>(boneName, this->numberOfBones));

            BoneInfo* boneInfo = new BoneInfo();
            this->boneInfos.push_back(boneInfo);
            boneInfo->boneOffset = convertAiMatrixToFloat4x4(paiMesh->mBones[i]->mOffsetMatrix);

            boneIndex = this->numberOfBones;
            this->numberOfBones++;
        } else {
            boneIndex = this->boneNameToIndexMapping[boneName];
        }
    return boneIndex;
}

double BoneTransformer::getCurrentAnimationTick(float totalElapsedTimeInSeconds) const {
    double ticksPerSecond = assimpScene->mAnimations[0]->mTicksPerSecond;
    ticksPerSecond = ticksPerSecond == 0 ? 25 : ticksPerSecond;

    double elapsedTimeInTicks = totalElapsedTimeInSeconds * ticksPerSecond;
    double animationDurationInTicks = assimpScene->mAnimations[0]->mDuration;
    double currentAnimationTick = fmod(elapsedTimeInTicks, animationDurationInTicks);
    return currentAnimationTick;
}