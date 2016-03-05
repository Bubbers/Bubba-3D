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

using namespace chag;

Mesh::Mesh() {

}

Mesh::~Mesh() {

}

void Mesh::loadMesh(const std::string &fileName) {
    Logger::logInfo("Loading mesh " + fileName);
    Assimp::Importer importer;

    const aiScene *pScene = importer.ReadFile(
            fileName.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace);

    if (!pScene) {
        Logger::logError("Error loading mesh for " + fileName);
    } else {
        initMeshFromScene(pScene, fileName);
    }
}

void Mesh::initMeshFromScene(const aiScene *pScene, const std::string &fileName) {
    for (unsigned int i = 0; i < pScene->mNumMeshes; i++) {
        const aiMesh *paiMesh = pScene->mMeshes[i];
        initMesh(i, paiMesh);
    }

    initMaterials(pScene, fileName);
    createTriangles();
}

void Mesh::initMaterials(const aiScene *pScene, const std::string &fileName) {
    for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
        const aiMaterial *material = pScene->mMaterials[i];
        Material m;

        initMaterialTextures(&m, fileName, material);
        initMaterialColors(&m, material);
        initMaterialShininess(&m, material);

        materials.push_back(m);
    }
}

void Mesh::initMaterialTextures(Material *material, std::string fileName, const aiMaterial *loadedMaterial) {
    if (loadedMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        material->diffuseTexture = getTexture(loadedMaterial, fileName, aiTextureType_DIFFUSE);
    }
    if (loadedMaterial->GetTextureCount(aiTextureType_HEIGHT) > 0) {
        material->bumpMapTexture = getTexture(loadedMaterial, fileName, aiTextureType_HEIGHT);
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

float3 Mesh::getColorFromMaterial(const char* pKey, unsigned int type, unsigned int idx, const aiMaterial &material) {
    aiColor3D color;
    material.Get(pKey, type, idx, color);
    return make_vector(color.r, color.g, color.b);
}

Texture *Mesh::getTexture(const aiMaterial *material, const std::string &fileName, aiTextureType type) {
    aiString texturePath;
    if (material->GetTexture(type, 0, &texturePath, NULL, NULL, NULL, NULL, NULL) != AI_SUCCESS) {
        return NULL;
    }

    std::string absolutePath = getAbsolutePath(fileName, string(texturePath.data));
    return ResourceManager::loadAndFetchTexture(absolutePath);
}

std::string Mesh::getAbsolutePath(const std::string &fileName, std::string textureName) {
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

void Mesh::initMesh(unsigned int index, const aiMesh *paiMesh) {
    Chunk chunk;

    initChunkFromAiMesh(paiMesh, chunk);
    setupSphere(&chunk.m_positions);
}

void Mesh::initChunkFromAiMesh(const aiMesh *paiMesh, Chunk &chunk) {
    initVerticesFromAiMesh(paiMesh, chunk);
    initIndicesFromAiMesh(paiMesh, chunk);

    chunk.materialIndex = paiMesh->mMaterialIndex;

    setupChunkForRendering(chunk);
    m_chunks.push_back(chunk);
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

        checkMinMax(pPos.x, pPos.y, pPos.z, &m_aabb.minV, &m_aabb.maxV);
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

// TODO(Bubbad) Remove all GL dependencies directly in mesh
template <typename T>
void setupGlBuffer(std::vector<T> buffer, GLuint *bufferGLObject, GLuint vertexAttibute, int numbersPerObject,
                   const void* firstObject, GLenum type) {
    glGenBuffers(1, bufferGLObject);
    glBindBuffer(type, *bufferGLObject);
    glBufferData(type, buffer.size() * sizeof(buffer[0]), firstObject, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexAttibute, numbersPerObject, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexAttibute);
}

void Mesh::setupChunkForRendering(Chunk &chunk) {
    glGenVertexArrays(1, &chunk.m_vaob);
    glBindVertexArray(chunk.m_vaob);

    setupGlBuffer(chunk.m_positions, &chunk.m_positions_bo, 0, 3 , &chunk.m_positions[0].x, GL_ARRAY_BUFFER_ARB);
    setupGlBuffer(chunk.m_normals, &chunk.m_normals_bo, 1, 3, &chunk.m_normals[0].x, GL_ARRAY_BUFFER_ARB);

    if (chunk.m_uvs.size() > 0) {
        setupGlBuffer(chunk.m_uvs, &chunk.m_uvs_bo, 2, 2, &chunk.m_uvs[0].x, GL_ARRAY_BUFFER_ARB);
    }

    setupGlBuffer(chunk.m_indices, &chunk.m_ind_bo, 3, 3, &chunk.m_indices[0], GL_ELEMENT_ARRAY_BUFFER_ARB);

    if (chunk.m_bittangents.size() > 0) {
        setupGlBuffer(chunk.m_tangents, &chunk.m_tangents_bo, 4, 3, &chunk.m_tangents[0].x, GL_ARRAY_BUFFER_ARB);
        setupGlBuffer(chunk.m_bittangents, &chunk.m_bittangents_bo, 5, 3,
                      &chunk.m_bittangents[0].x, GL_ARRAY_BUFFER_ARB);
    }
}

AABB* Mesh::getAABB() {
    return &m_aabb;
}

void Mesh::createTriangles() {
    for (unsigned int i = 0; i < m_chunks.size(); i++) {
        for (unsigned int j = 0; j + 2 < m_chunks[i].m_positions.size(); j += 3) {
            triangles.push_back(createTriangleFromPositions(m_chunks[i].m_positions, j));
        }
    }
}

Triangle* Mesh::createTriangleFromPositions(std::vector<chag::float3> positionBuffer, unsigned int startIndex) {
    return new Triangle(make_vector(positionBuffer[startIndex + 0].x,
                                    positionBuffer[startIndex + 0].y,
                                    positionBuffer[startIndex + 0].z),
                        make_vector(positionBuffer[startIndex + 1].x,
                                    positionBuffer[startIndex + 1].y,
                                    positionBuffer[startIndex + 1].z),
                        make_vector(positionBuffer[startIndex + 2].x,
                                    positionBuffer[startIndex + 2].y,
                                    positionBuffer[startIndex + 2].z));
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
