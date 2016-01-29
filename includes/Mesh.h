#ifndef __MESH_H__
#define __MESH_H__

#include <string>
#include <vector>
#include "float3.h"
#include "Material.h"
#include "Chunk.h"
#include "AABB2.h"
#include "assimp/material.h"
#include <Sphere.h>


using namespace chag;

class Triangle;
class aiScene;
class aiMesh;
class aiMaterial;


class Mesh {
public:
    Mesh();

    ~Mesh();

    void loadMesh(const std::string &fileName);
    void createTriangles();
    std::vector<Triangle *> getTriangles();
    AABB* getAABB();
    Sphere getSphere();

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

    std::vector<Triangle *> triangles;
    Sphere sphere;

public:
    std::vector<Material> materials;
    std::vector<Chunk> m_chunks;
    AABB m_aabb;
};

#endif // !__MESH_H__
