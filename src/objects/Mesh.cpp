#include "Mesh.h"
#include <ResourceManager.h>
#include "Logger.h"
#include "Triangle.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace chag;

Mesh::Mesh() {
};

Mesh::~Mesh() {
};

Chunk::Chunk(std::vector<chag::float3> &positions,
             std::vector<chag::float3> &normals,
             std::vector<chag::float2> &uvs,
             std::vector<unsigned int> &indices,
             std::vector<chag::float3> &tangents,
             std::vector<chag::float3> &bittangents,
             unsigned int materialIndex) :
        m_positions(positions), m_normals(normals), m_uvs(uvs), m_indices(indices), materialIndex(materialIndex),
        m_tangents(tangents), m_bittangents(bittangents) {
    m_numIndices = indices.size();
}

void Mesh::loadMesh(const std::string &fileName) {
    Logger::logInfo("Loading mesh " + fileName);
    Assimp::Importer importer;

    const aiScene *pScene = importer.ReadFile(
            fileName.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace);

    if (!pScene) {
        Logger::logSevere("Error loading mesh for " + fileName);
    }
    else {
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

        float specExp;
        material->Get(AI_MATKEY_SHININESS, specExp);
        m.specularExponent = specExp > 0.0f ? specExp : 0.0f;

        materials.push_back(m);
    }
}

void Mesh::initMaterialTextures(Material *material, std::string fileName, const aiMaterial *loadedMaterial) {
    if(loadedMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        material->diffuseTexture = getTexture(loadedMaterial, fileName, aiTextureType_DIFFUSE);
    }
    if(loadedMaterial->GetTextureCount(aiTextureType_HEIGHT) > 0) {
        material->bumpMapTexture = getTexture(loadedMaterial, fileName, aiTextureType_HEIGHT);
    }
}

void Mesh::initMaterialColors(Material *material, const aiMaterial *loadedMaterial) {
    material->ambientColor = getColorFromMaterial(AI_MATKEY_COLOR_AMBIENT, *loadedMaterial);
    material->diffuseColor = getColorFromMaterial(AI_MATKEY_COLOR_DIFFUSE, *loadedMaterial);
    material->specularColor = getColorFromMaterial(AI_MATKEY_COLOR_SPECULAR, *loadedMaterial);
    material->emissiveColor = getColorFromMaterial(AI_MATKEY_COLOR_EMISSIVE, *loadedMaterial);
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
    }
    else if (index == 0) {
        return "/";
    }
    else {
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

void Mesh::initMesh(unsigned int index, const aiMesh *paiMesh) {
    std::vector<float3> positions;
    std::vector<float3> normals;
    std::vector<float3> tangents;
    std::vector<float3> bittangents;
    std::vector<float2> uvs;
    std::vector<unsigned int> indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    float3 minV = make_vector(FLT_MAX, FLT_MAX, FLT_MAX);
    float3 maxV = make_vector(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
        const aiVector3D pPos = paiMesh->mVertices[i];
        const aiVector3D pNormal = paiMesh->mNormals[i];
        const aiVector3D pTexCoord = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][i] : Zero3D;


        positions.push_back(make_vector(pPos.x, pPos.y, pPos.z));
        normals.push_back(make_vector(pNormal.x, pNormal.y, pNormal.z));
        uvs.push_back(make_vector(pTexCoord.x, pTexCoord.y));
        if (paiMesh->HasTangentsAndBitangents()) {
            const aiVector3D pBitTangents = paiMesh->mBitangents[i];
            const aiVector3D pTangents = paiMesh->mTangents[i];
            bittangents.push_back(make_vector(pBitTangents.x, pBitTangents.y, pBitTangents.z));
            tangents.push_back(make_vector(pTangents.x, pTangents.y, pTangents.z));
        }

        checkMinMax(pPos.x, pPos.y, pPos.z, &minV, &maxV);
    }

    checkMinMax(minV.x, minV.y, minV.z, &m_aabb.minV, &m_aabb.maxV);
    checkMinMax(maxV.x, maxV.y, maxV.z, &m_aabb.minV, &m_aabb.maxV);

    for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
        const aiFace face = paiMesh->mFaces[i];
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }


    Chunk c(positions, normals, uvs, indices, tangents, bittangents, paiMesh->mMaterialIndex);
    glGenVertexArrays(1, &c.m_vaob);
    glBindVertexArray(c.m_vaob);

    glGenBuffers(1, &c.m_positions_bo);
    glBindBuffer(GL_ARRAY_BUFFER_ARB, c.m_positions_bo);
    glBufferData(GL_ARRAY_BUFFER_ARB, c.m_positions.size() * sizeof(float3), &c.m_positions[0].x, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &c.m_normals_bo);
    glBindBuffer(GL_ARRAY_BUFFER_ARB, c.m_normals_bo);
    glBufferData(GL_ARRAY_BUFFER_ARB, c.m_normals.size() * sizeof(float3),
                 &c.m_normals[0].x, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(1);

    if (c.m_uvs.size() > 0) {
        glGenBuffers(1, &c.m_uvs_bo);
        glBindBuffer(GL_ARRAY_BUFFER_ARB, c.m_uvs_bo);
        glBufferData(GL_ARRAY_BUFFER_ARB, c.m_uvs.size() * sizeof(float2),
                     &c.m_uvs[0].x, GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, 0);
    }
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &c.m_ind_bo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, c.m_ind_bo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, c.m_numIndices * sizeof(unsigned int), &c.m_indices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(3);

    if (c.m_bittangents.size() > 0) {
        glGenBuffers(1, &c.m_tangents_bo);
        glBindBuffer(GL_ARRAY_BUFFER_ARB, c.m_tangents_bo);
        glBufferData(GL_ARRAY_BUFFER_ARB, c.m_tangents.size() * sizeof(float3), &c.m_tangents[0].x, GL_STATIC_DRAW);
        glVertexAttribPointer(4, 3, GL_FLOAT, false, 0, 0);
        glEnableVertexAttribArray(4);

        glGenBuffers(1, &c.m_bittangents_bo);
        glBindBuffer(GL_ARRAY_BUFFER_ARB, c.m_bittangents_bo);
        glBufferData(GL_ARRAY_BUFFER_ARB, c.m_bittangents.size() * sizeof(float3), &c.m_bittangents[0].x,
                     GL_STATIC_DRAW);
        glVertexAttribPointer(5, 3, GL_FLOAT, false, 0, 0);
        glEnableVertexAttribArray(5);
    }

    m_chunks.push_back(c);
}

AABB* Mesh::getAABB() {
    return &m_aabb;
}

void Mesh::createTriangles() {
    for (unsigned int i = 0; i < m_chunks.size(); i++) {

        for (unsigned int j = 0; j + 2 < m_chunks[i].m_positions.size(); j += 3) {


            Triangle *t = new Triangle(make_vector(m_chunks[i].m_positions[j + 0].x,
                                                   m_chunks[i].m_positions[j + 0].y,
                                                   m_chunks[i].m_positions[j + 0].z),
                                       make_vector(m_chunks[i].m_positions[j + 1].x,
                                                   m_chunks[i].m_positions[j + 1].y,
                                                   m_chunks[i].m_positions[j + 1].z),
                                       make_vector(m_chunks[i].m_positions[j + 2].x,
                                                   m_chunks[i].m_positions[j + 2].y,
                                                   m_chunks[i].m_positions[j + 2].z));


            triangles.push_back(t);
        }
    }
}

std::vector<Triangle*> Mesh::getTriangles() {
    return triangles;
}