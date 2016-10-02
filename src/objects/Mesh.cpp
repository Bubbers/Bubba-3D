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
#include "Mesh.h"
#include <ResourceManager.h>
#include "Logger.h"
#include "Triangle.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>
#include "Chunk.h"
#include <string>
#include "Utils.h"
#include "GL/glew.h"
#include "BoneInfo.h"
#include "Utils.h"
#include "linmath/float3x3.h"

using namespace chag;

#define BONE_ID_LOCATION_GPU 6
#define BONE_WEIGHT_LOCATION_GPU 7

Mesh::Mesh() {
    numberOfBones = 0;
    numAnimations = 0;
}

Mesh::~Mesh() {

}

void Mesh::loadMesh(const std::string &fileName) {
    Logger::logInfo("Loading mesh " + fileName);

    importer.ReadFile(
            fileName.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace);
    assimpScene = importer.GetOrphanedScene();


    if (!assimpScene) {
        Logger::logError("Error loading mesh for " + fileName + ". Error message: " + importer.GetErrorString());
    } else {
        globalInverseTransform = convertAiMatrixToFloat4x4(assimpScene->mRootNode->mTransformation.Inverse());

        initMesh(assimpScene, fileName);
    }
}

void Mesh::initMesh(const aiScene *assimpScene, const std::string &fileNameOfMesh) {
    for (unsigned int i = 0; i < assimpScene->mNumMeshes; i++) {
        const aiMesh *paiMesh = assimpScene->mMeshes[i];
        initMeshFromAiMesh(i, paiMesh);
    }

    initMaterials(assimpScene, fileNameOfMesh);
    createTriangles();

    numAnimations = assimpScene->mNumAnimations;
}


void Mesh::initMeshFromAiMesh(unsigned int index, const aiMesh *paiMesh) {
    Chunk chunk;

    initChunkFromAiMesh(paiMesh, chunk);
    setupSphere(&chunk.m_positions);
}

void Mesh::initChunkFromAiMesh(const aiMesh *paiMesh, Chunk &chunk) {
    initVerticesFromAiMesh(paiMesh, chunk);
    initIndicesFromAiMesh(paiMesh, chunk);

    initBonesFromAiMesh(paiMesh, chunk);


    chunk.materialIndex = paiMesh->mMaterialIndex;

    setupChunkForRendering(chunk);
    m_chunks.push_back(chunk);
}


void Mesh::assertAllVertexWeightsSumToOne(Chunk &chunk) {
    for(unsigned int i = 0; i < chunk.bones.size(); i++) {
        BoneInfluenceOnVertex boneInfluenceOnVertex = chunk.bones[i];
        float sum = 0.0f;
        for (int boneIndex = 0; boneIndex < MAX_NUM_BONES; boneIndex++) {
            sum += boneInfluenceOnVertex.weights[boneIndex];
        }

        assert(fequals(sum, 1.0f));
    }
}


void Mesh::initVerticesFromAiMesh(const aiMesh *paiMesh, Chunk &chunk) {
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
        const aiVector3D pPos = paiMesh->mVertices[i];
        const aiVector3D pNormal = paiMesh->mNormals[i];
        const aiVector3D pTexCoord = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][i] : Zero3D;

        chunk.m_positions.push_back(make_vector(pPos.x, pPos.y, pPos.z));
        chunk.m_normals.push_back(make_vector(pNormal.x, pNormal.y, pNormal.z));
        chunk.m_uvs.push_back(make_vector(pTexCoord.x, pTexCoord.y));

        if (paiMesh->HasTangentsAndBitangents()) {
            const aiVector3D pBitTangents = paiMesh->mBitangents[i];
            const aiVector3D pTangents = paiMesh->mTangents[i];
            chunk.m_bittangents.push_back(make_vector(pBitTangents.x, pBitTangents.y, pBitTangents.z));
            chunk.m_tangents.push_back(make_vector(pTangents.x, pTangents.y, pTangents.z));
        }

        updateMinAndMax(pPos.x, pPos.y, pPos.z, &m_aabb.minV, &m_aabb.maxV);
    }
}

void Mesh::initIndicesFromAiMesh(const aiMesh *paiMesh, Chunk &chunk) {
    for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
        const aiFace face = paiMesh->mFaces[i];
        chunk.m_indices.push_back(face.mIndices[0]);
        chunk.m_indices.push_back(face.mIndices[1]);
        chunk.m_indices.push_back(face.mIndices[2]);
    }
}

void Mesh::initBonesFromAiMesh(const aiMesh *paiMesh, Chunk &chunk) {
    if(paiMesh->mNumBones == 0) {
        return;
    }

    chunk.bones.resize(paiMesh->mNumVertices);

    int boneIndex = 0;
    for(unsigned int i = 0; i < paiMesh->mNumBones; i++) {

        std::string boneName(paiMesh->mBones[i]->mName.data);

        if(boneNameToIndexMapping.find(boneName) == boneNameToIndexMapping.end()) {

            boneNameToIndexMapping.insert(std::pair<std::string, int>(boneName, numberOfBones));

            BoneInfo* boneInfo = new BoneInfo();
            boneInfos.push_back(boneInfo);
            boneInfo->boneOffset = convertAiMatrixToFloat4x4(paiMesh->mBones[i]->mOffsetMatrix);

            boneIndex = numberOfBones;
            numberOfBones++;
        } else {
            boneIndex = boneNameToIndexMapping[boneName];
        }

        for (unsigned int j = 0; j < paiMesh->mBones[i]->mNumWeights; j++) {
            auto aiWeight = paiMesh->mBones[i]->mWeights[j];
            uint vertexId = aiWeight.mVertexId;
            float weight = aiWeight.mWeight;

            chunk.bones[vertexId].addBoneData(boneIndex, weight);
        }
    }

    assertAllVertexWeightsSumToOne(chunk);
}

void Mesh::calculateBoneTransforms(float elapsedTimeInSeconds, std::vector<float4x4>& boneTransformMatrices) {
    float4x4 rootMatrix = make_identity<float4x4>();

    double ticksPerSecond = assimpScene->mAnimations[0]->mTicksPerSecond;
    ticksPerSecond = ticksPerSecond == 0 ? 25 : ticksPerSecond;

    double elapsedTimeInTicks = elapsedTimeInSeconds * ticksPerSecond;
    double animationDurationInTicks = assimpScene->mAnimations[0]->mDuration;
    double currentAnimationTick = fmod(elapsedTimeInTicks, animationDurationInTicks);

    readNodeHierarchyAndUpdateBoneTransformations((float)currentAnimationTick, assimpScene->mRootNode, rootMatrix);

    boneTransformMatrices.resize(numberOfBones);
    for (int bone = 0; bone < numberOfBones; bone++) {
        boneTransformMatrices[bone] = boneInfos[bone]->finalTransformation;
        //boneTransformMatrices[bone] = make_identity<float4x4>();
    }
}

/**
 * Reads the entire (animation) node hierarchy and updates
 * the bone transformations in {@code boneInfos} accordingly.
 */
void Mesh::readNodeHierarchyAndUpdateBoneTransformations(float currentAnimationTick,
                                                         aiNode *currentAssimpNode,
                                                         float4x4 parentMatrix) {
    std::string nodeName(currentAssimpNode->mName.data);

    float4x4 nodeTransformationMatrix = getCurrentNodeTransformation(currentAnimationTick, currentAssimpNode, nodeName);
    float4x4 globalTransformation = parentMatrix * nodeTransformationMatrix;

    // If the node isn't a bone we dont need to update any bones
    if(nodeIsABone(nodeName)) {
        updateBoneTransformation(nodeName, globalTransformation);
    }

    for (unsigned int i = 0; i < currentAssimpNode->mNumChildren; i++) {
        readNodeHierarchyAndUpdateBoneTransformations(currentAnimationTick, currentAssimpNode->mChildren[i],
                                                      globalTransformation);
    }
}

/**
 * Updates the transformation of the bone corresponding to @{code nodeName}
 */
void Mesh::updateBoneTransformation(const std::string &nodeName, const float4x4 &globalTransformation) {
    uint boneIndex = boneNameToIndexMapping[nodeName];

    boneInfos[boneIndex]->finalTransformation = globalInverseTransform * globalTransformation * boneInfos[boneIndex]->boneOffset;
}

/**
 * A node is a bone if there is a bone named exactly the same as the node
 */
bool Mesh::nodeIsABone(const std::string &nodeName) const {
    return boneNameToIndexMapping.find(nodeName) != boneNameToIndexMapping.end();
}

float4x4 Mesh::getCurrentNodeTransformation(float currentAnimationTick,
                                            const aiNode *currentAssimpNode,
                                            const std::string nodeName) {
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

/**
 * Interpolates the two animation matrices nearest the current tick
 */
float4x4 Mesh::getInterpolatedAnimationMatrix(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
    float4x4 scalingMatrix = getInterpolatedScalingMatrix(currentAnimationTick, nodeAnimation);
    float4x4 rotationMatrix = getInterpolatedRotationMatrix(currentAnimationTick, nodeAnimation);
    float4x4 translationMatrix = getInterpolatedTranslationMatrix(currentAnimationTick, nodeAnimation);

    float4x4 interpolatedMatrix = translationMatrix * rotationMatrix * scalingMatrix;
    return interpolatedMatrix;
}

float4x4 Mesh::getInterpolatedTranslationMatrix(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
    aiVector3D translation = calculateTranslationInterpolation(currentAnimationTick, nodeAnimation);
    float4x4 translationMatrix =
                make_translation(make_vector(
                        translation.x,
                        translation.y,
                        translation.z));
    return translationMatrix;
}

float4x4 Mesh::getInterpolatedRotationMatrix(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
    aiQuaternion rotationQuaternion = calculateRotationInterpolation(currentAnimationTick, nodeAnimation);
    float4x4 rotationMatrix = make_matrix(
                convertAiMatrixToFloat3x3(rotationQuaternion.GetMatrix()),
                make_vector(0.0f, 0.0f, 0.0f)
        );
    return rotationMatrix;
}

float4x4 Mesh::getInterpolatedScalingMatrix(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
    aiVector3D scaling = calculateScalingInterpolation(currentAnimationTick, nodeAnimation);
    float4x4 scalingMatrix = make_scale<float4x4>(make_vector(scaling.x, scaling.y, scaling.z));
    return scalingMatrix;
}

/**
 * Given the name of a node, fetches the corresponding animation node.
 *
 */
const aiNodeAnim *Mesh::getAnimationNode(const std::string &nodeName) {
    const aiAnimation* animation = assimpScene->mAnimations[0];

    const aiNodeAnim* nodeAnimation = findNodeAnim(animation, nodeName);
    return nodeAnimation;
}

/**
 * Searches through an animations channels for the animation node
 */
const aiNodeAnim* Mesh::findNodeAnim(const aiAnimation* animation, const std::string nodeName) {
    for (unsigned int i = 0; i < animation->mNumChannels; i++) {
        const aiNodeAnim* nodeAnimation = animation->mChannels[i];
        if(std::string(nodeAnimation->mNodeName.data) == nodeName) {
            return nodeAnimation;
        }
    }

    return nullptr;
}

void Mesh::initMaterials(const aiScene *pScene, const std::string &fileNameOfMesh) {
    for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
        const aiMaterial *material = pScene->mMaterials[i];
        Material m;

        initMaterialTextures(&m, fileNameOfMesh, material);
        initMaterialColors(&m, material);
        initMaterialShininess(&m, material);

        materials.push_back(m);
    }
}

void Mesh::initMaterialTextures(Material *material, std::string fileNameOfMesh, const aiMaterial *loadedMaterial) {
    if (loadedMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        material->diffuseTexture = getTexture(loadedMaterial, fileNameOfMesh, aiTextureType_DIFFUSE);
    }
    if (loadedMaterial->GetTextureCount(aiTextureType_HEIGHT) > 0) {
        material->bumpMapTexture = getTexture(loadedMaterial, fileNameOfMesh, aiTextureType_HEIGHT);
    }
}

void Mesh::initMaterialColors(Material *material, const aiMaterial *loadedMaterial) {
    material->ambientColor = getColorFromMaterial(AI_MATKEY_COLOR_AMBIENT, *loadedMaterial);
    material->diffuseColor = getColorFromMaterial(AI_MATKEY_COLOR_DIFFUSE, *loadedMaterial);
    material->specularColor = getColorFromMaterial(AI_MATKEY_COLOR_SPECULAR, *loadedMaterial);
    material->emissiveColor = getColorFromMaterial(AI_MATKEY_COLOR_EMISSIVE, *loadedMaterial);
}

void Mesh::initMaterialShininess(Material *material, const aiMaterial *loadedMaterial) {
    float specExp;
    loadedMaterial->Get(AI_MATKEY_SHININESS, specExp);
    material->specularExponent = specExp > 0.0f ? specExp : 0.0f;
}

float3 Mesh::getColorFromMaterial(const char* colorTypeString, unsigned int type, unsigned int index, const aiMaterial &material) {
    aiColor3D color;
    material.Get(colorTypeString, type, index, color);
    return make_vector(color.r, color.g, color.b);
}

Texture *Mesh::getTexture(const aiMaterial *material, const std::string &fileNameOfMesh, aiTextureType type) {
    aiString texturePath;
    if (material->GetTexture(type, 0, &texturePath, NULL, NULL, NULL, NULL, NULL) != AI_SUCCESS) {
        return NULL;
    }

    std::string absolutePath = getPathOfTexture(fileNameOfMesh, std::string(texturePath.data));
    return ResourceManager::loadAndFetchTexture(absolutePath);
}

std::string Mesh::getPathOfTexture(const std::string &fileName, std::string textureName) {
    std::string dir = getDirectoryFromPath(fileName);
    std::string filePath = cleanFileName(textureName);
    return dir + "/" + filePath;
}

std::string Mesh::getDirectoryFromPath(const std::string &fileName) {
    std::string::size_type index = fileName.find_last_of("/");
    if (index == std::string::npos) {
        return ".";
    } else if (index == 0) {
        return "/";
    } else {
        return fileName.substr(0, index);
    }
}

std::string Mesh::cleanFileName(std::string fileName) {
    if (fileName.substr(0, 2) == ".\\") {
        return fileName.substr(2, fileName.size() - 2);
    } else {
        return fileName;
    }
}

Sphere Mesh::getSphere() {
    return sphere;
}

void Mesh::setupSphere(std::vector<float3> *positions) {
    sphere.setPosition(m_aabb.getCenterPosition());
    sphere.setRadius(0.0f);
    for (float3 posIt : *positions) {
        float rad = length(sphere.getPosition() - posIt);
        if (rad > sphere.getRadius()) {
            sphere.setRadius(rad);
        }
    }
}


// TODO(Bubbad) Remove all GL dependencies directly in mesh
template <typename T>
void setupGlBuffer(std::vector<T> buffer, GLuint *bufferGLObject, GLuint vertexAttibute, int numbersPerObject,
                   const void* firstObject, GLenum bufferType, GLuint dataType) {
    glGenBuffers(1, bufferGLObject);
    glBindBuffer(bufferType, *bufferGLObject);
    glBufferData(bufferType, buffer.size() * sizeof(buffer[0]), firstObject, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexAttibute, numbersPerObject, dataType, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexAttibute);
}

void Mesh::setupChunkForRendering(Chunk &chunk) {
    glGenVertexArrays(1, &chunk.m_vaob);
    glBindVertexArray(chunk.m_vaob);

    setupGlBuffer(chunk.m_positions, &chunk.m_positions_bo, 0, 3 , &chunk.m_positions[0].x, GL_ARRAY_BUFFER_ARB, GL_FLOAT);
    setupGlBuffer(chunk.m_normals, &chunk.m_normals_bo, 1, 3, &chunk.m_normals[0].x, GL_ARRAY_BUFFER_ARB, GL_FLOAT);

    if (chunk.m_uvs.size() > 0) {
        setupGlBuffer(chunk.m_uvs, &chunk.m_uvs_bo, 2, 2, &chunk.m_uvs[0].x, GL_ARRAY_BUFFER_ARB, GL_FLOAT);
    }

    setupGlBuffer(chunk.m_indices, &chunk.m_ind_bo, 3, 3, &chunk.m_indices[0], GL_ELEMENT_ARRAY_BUFFER_ARB, GL_FLOAT);

    if (chunk.m_bittangents.size() > 0) {
        setupGlBuffer(chunk.m_tangents, &chunk.m_tangents_bo, 4, 3, &chunk.m_tangents[0].x, GL_ARRAY_BUFFER_ARB, GL_FLOAT);
        setupGlBuffer(chunk.m_bittangents, &chunk.m_bittangents_bo, 5, 3,
                      &chunk.m_bittangents[0].x, GL_ARRAY_BUFFER_ARB, GL_FLOAT);
    }

    if(chunk.bones.size() > 0 ) {
        glGenBuffers(1, &chunk.bonesBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, chunk.bonesBufferObject);
        glBufferData(GL_ARRAY_BUFFER, chunk.bones.size() * sizeof(chunk.bones[0]), &chunk.bones[0], GL_STATIC_DRAW);

        const int boneIdsPerObject = 4;
        glVertexAttribIPointer(BONE_ID_LOCATION_GPU, boneIdsPerObject, GL_INT, sizeof(BoneInfluenceOnVertex), 0);
        glEnableVertexAttribArray(BONE_ID_LOCATION_GPU);

        const int boneWeightsPerObject = 4;
        glVertexAttribPointer(BONE_WEIGHT_LOCATION_GPU, boneWeightsPerObject, GL_FLOAT, GL_FALSE, sizeof(BoneInfluenceOnVertex), (const GLvoid*)16);
        glEnableVertexAttribArray(BONE_WEIGHT_LOCATION_GPU);
    }
}

AABB* Mesh::getAABB() {
    return &m_aabb;
}

void Mesh::createTriangles() {
    for (unsigned int i = 0; i < m_chunks.size(); i++) {

        for (unsigned int j = 0; j + 2 < m_chunks[i].m_indices.size(); j += 3) {
            triangles.push_back(createTriangleFromPositions(m_chunks[i].m_positions, m_chunks[i].m_indices, j));
        }
    }
}

Triangle* Mesh::createTriangleFromPositions(std::vector<chag::float3> positionBuffer, std::vector<unsigned int> indices, unsigned int startIndex) {

    return new Triangle(make_vector(positionBuffer[indices[startIndex + 0]].x,
                                    positionBuffer[indices[startIndex + 0]].y,
                                    positionBuffer[indices[startIndex + 0]].z),
                        make_vector(positionBuffer[indices[startIndex + 1]].x,
                                    positionBuffer[indices[startIndex + 1]].y,
                                    positionBuffer[indices[startIndex + 1]].z),
                        make_vector(positionBuffer[indices[startIndex + 2]].x,
                                    positionBuffer[indices[startIndex + 2]].y,
                                    positionBuffer[indices[startIndex + 2]].z));
}

std::vector<Triangle*> Mesh::getTriangles() {
    return triangles;
}

std::vector<Chunk>* Mesh::getChunks() {
    return &m_chunks;
}

std::vector<Material>* Mesh::getMaterials() {
    return &materials;
}

aiVector3D Mesh::calculateScalingInterpolation(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
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

aiQuaternion Mesh::calculateRotationInterpolation(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
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
    aiQuaternion::Interpolate(interpolatedQuaternion, currentRotationQuaternion, nextRotationQuaternion, deltaFactor);

    return interpolatedQuaternion;
}

aiVector3D Mesh::calculateTranslationInterpolation(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
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

unsigned int Mesh::findScalingIndexRightBeforeTick(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
    for (unsigned int i = 0; i < nodeAnimation->mNumScalingKeys - 1; i++) {
        if(currentAnimationTick < nodeAnimation->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }

    throw std::invalid_argument("Tried to find a scaling index in an animation, but couldn't find it");
}

unsigned int Mesh::findRotationIndexRightBeforeTick(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
    for (unsigned int i = 0; i < nodeAnimation->mNumRotationKeys - 1; i++) {
        if(currentAnimationTick < nodeAnimation->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }

    throw std::invalid_argument("Tried to find a rotation index in an animation, but couldn't find it");
}

unsigned int Mesh::findTranslationIndexRightBeforeTick(float currentAnimationTick, const aiNodeAnim *nodeAnimation) {
    for (unsigned int i = 0; i < nodeAnimation->mNumPositionKeys - 1; i++) {
        if(currentAnimationTick < nodeAnimation->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }

    throw std::invalid_argument("Tried to find a translation index in an animation, but couldn't find it");
}
 bool Mesh::hasAnimations() {
     return numAnimations != 0;
 }




