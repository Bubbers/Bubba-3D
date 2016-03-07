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


/**
 * A class for containing all triangle and material of a mesh.
 *
 * \code
 * Mesh mesh;
 * mesh.loadMesh("url/to/meshfile");
 * \endcode
 */
class Mesh {
public:
    Mesh();

    ~Mesh();

    void loadMesh(const std::string &fileName);

    /**
     * NOTE: The triangles have not been transformed.
     *
     * @return A list of all the triangles of the mesh.
     */
    std::vector<Triangle *> getTriangles();

    /**
     * NOTE: The AABB has not been transformed.
     *
     * @return The AABB of the mesh.
     */
    AABB* getAABB();

    /**
     * NOTE: The sphere has not been transformed.
     *
     * @return The sphere surrounding the object.
     */
    Sphere getSphere();

    std::vector<Chunk>* getChunks();
    std::vector<Material>* getMaterials();

 private:
    /**
     * NOTE: This will only load the first mesh in the scene. If you have
     * several meshes in a single loaded object please implement support for it!
     *
     * Loads all the chunks, materials, triangles and collision details of the mesh
     * in the loaded aiScene.
     *
     * @param pScene The loaded aiScene containing the mesh to be loaded
     * @param fileNameOfMesh The absolute/relative file path to the file containing the mesh
     */
    void initMesh(const aiScene *pScene, const std::string &fileNameOfMesh);
    void initMeshFromAiMesh(unsigned int index, const aiMesh *paiMesh);
    void initChunkFromAiMesh(const aiMesh *paiMesh, Chunk &chunk);
    void initVerticesFromAiMesh(const aiMesh *paiMesh, Chunk &chunk);
    void initIndicesFromAiMesh(const aiMesh *paiMesh, Chunk &chunk);

    /**
     * Loads all materials from the loaded aiScene.
     *
     * @param pScene The loaded aiScene containing the materials to be loaded
     * @param fileNameOfMesh The absolute/relative file path to the file containing the mesh
     */
    void initMaterials(const aiScene *pScene, const std::string &fileNameOfMesh);
    void initMaterialTextures(Material *material, std::string fileNameOfMesh, const aiMaterial *loadedMaterial);
    void initMaterialColors(Material *material, const aiMaterial *loadedMaterial);
    void initMaterialShininess(Material *material, const aiMaterial *loadedMaterial);

    /**
     * Fetches the color of specified type in the aiMaterial.
     * Use the predefined macros for the first three parameters (eg AI_MATKEY_COLOR_AMBIENT)
     *
     * @param colorTypeString
     * @param type
     * @param index
     * @param loadedMaterial
     * @return A float3 containing the colors rgb
     */
    float3 getColorFromMaterial(const char* colorTypeString, unsigned int type, unsigned int index, const aiMaterial &material);

    /**
     * Uses the file name of the mesh to calculate the absolute path to the specified texture
     *
     * @param fileNameOfMesh Absolute/relative path to the mesh file
     * @param textureName The path to the textureName relative to the mesh file
     * @return The path to the specified texture
     */
    std::string getPathOfTexture(const std::string &fileNameOfMesh, std::string textureName);

    /**
     * Removes the file part from the path
     */
    std::string getDirectoryFromPath(const std::string &fileName);

    /**
     * Removes possible ending '.\\' from a path
     * @param filePath
     * @return  A cleaned file
     */
    std::string cleanFileName(std::string filePath);

    /**
     * Returns the texture that was specified in the material.
     * Loads the texture into the resourcemanager if it was not previously loaded, or
     * fetches it from the resourcemanager if it was.
     *
     * @param material The material containing a texture
     * @param fileNameOfMesh
     * @param type A enum saying which texture type to load from the material
     * @return A pointer to the texture loaded
     */
    Texture* getTexture(const aiMaterial *material, const std::string &fileNameOfMesh, aiTextureType type);

    /**
     * Initiates OpenGL buffers and buffers the chunk data on to the graphics memory.
     * @param chunk The chunk to be initiated for rendering
     */
    void setupChunkForRendering(Chunk &chunk);

    void setupSphere(std::vector<float3> *positions);

    void createTriangles();
    Triangle* createTriangleFromPositions(std::vector<chag::float3> positionBuffer, unsigned int startIndex);

    std::vector<Triangle *> triangles;
    std::vector<Material> materials;
    std::vector<Chunk> m_chunks;

    Sphere sphere;
    AABB m_aabb;
};

#endif // !__MESH_H__
