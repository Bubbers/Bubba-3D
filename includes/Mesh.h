#ifndef __MESH_H__
#define __MESH_H__


#include <string>
#include <vector>
#include "float2.h"
#include "float3.h"
#include "float4.h"
#include "glutil/glutil.h"
#include "GL/glew.h"
#include "GL/glut.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "AABB2.h"
#include "IDrawable.h"
#include "Texture.h"

struct Material {
    chag::float3 diffuseColor;
    chag::float3 ambientColor;
    chag::float3 specularColor;
    chag::float3 emissiveColor;
    float specularExponent;
    Texture *diffuseTexture = NULL;
    Texture *bumpMapTexture = NULL;
};

struct Chunk {
    Chunk(std::vector<chag::float3> &positions,
          std::vector<chag::float3> &normals,
          std::vector<chag::float2> &uvs,
          std::vector<unsigned int> &indices,
          std::vector<chag::float3> &tangents,
          std::vector<chag::float3> &bittangents,
          unsigned int textureIndex);

    ~Chunk() { };


    // Data on host
    std::vector<chag::float3> m_positions;
    std::vector<chag::float3> m_normals;
    std::vector<chag::float2> m_uvs;
    std::vector<unsigned int> m_indices;
    std::vector<chag::float3> m_tangents;
    std::vector<chag::float3> m_bittangents;

    // Data on GPU
    GLuint m_positions_bo;
    GLuint m_normals_bo;
    GLuint m_uvs_bo;
    GLuint m_ind_bo;
    GLuint m_tangents_bo;
    GLuint m_bittangents_bo;
    // Vertex Array Object
    GLuint m_vaob;

    unsigned int materialIndex;

    unsigned int m_numIndices;
};


class Mesh {
public:
    Mesh();

    ~Mesh();

    void loadMesh(const std::string &fileName);
    AABB* getAABB();

private:

    void initMeshFromScene(const aiScene *pScene, const std::string &fileName);
    void initMesh(unsigned int index, const aiMesh *paiMesh);
    void initMaterials(const aiScene *pScene, const std::string &fileName);
    void initMaterialTextures(Material *material, std::string fileName, const aiMaterial *loadedMaterial);
    void initMaterialColors(Material *material, const aiMaterial *loadedMaterial);
    std::string getAbsolutePath(const std::string &fileName, std::string textureName);
    std::string getDirectoryFromPath(const std::string &fileName);
    std::string cleanFileName(std::string filePath);
    float3 getColorFromMaterial(const char* pKey, unsigned int type, unsigned int idx, const aiMaterial &material);
    Texture* getTexture(const aiMaterial *material, const std::string &fileName, aiTextureType type);

public:
    std::vector<Material> materials;
    std::vector<Chunk> m_chunks;
    AABB m_aabb;
};

#endif // !__MESH_H__
