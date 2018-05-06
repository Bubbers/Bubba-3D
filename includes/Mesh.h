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
#include "linmath/float3.h"
#include "Material.h"
#include "AABB2.h"
#include "assimp/material.h"
#include <Sphere.h>
#include <assimp/scene.h>
#include <map>
#include <assimp/Importer.hpp>
#include <memory>


class BoneTransformer;
class Triangle;
class Chunk;
class Texture;
struct BoneInfluenceOnVertex;

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

    ~Mesh() = default;

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

    bool hasAnimations();

    /**
    * Calculates the transform to be applied to each bone at the current time.
    *
    * @param totalElapsedTimeInSeconds The time since the application was started
    * @return A vector containing the transforms of each bone. The index in the vector corresponds to the bones index. The index of a bone can be found in boneNameToIndexMapping.
    *
    */
    std::vector<chag::float4x4> getBoneTransforms(float totalElapsedTimeInSeconds);

private:
    /**
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

    void initBonesFromAiMesh(const aiMesh *paiMesh, Chunk &bones);
    void assertAllVertexWeightsSumToOne(Chunk &chunk);


    /**
     * Loads all materials from the loaded aiScene.
     *
     * @param pScene The loaded aiScene containing the materials to be loaded
     * @param fileNameOfMesh The absolute/relative file path to the file containing the mesh
     */
    void initMaterials(const aiScene *pScene, const std::string &fileNameOfMesh);
    void initMaterialTextures(Material *material, std::string fileNameOfMesh,
                              const aiMaterial *loadedMaterial);
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
    chag::float3 getColorFromMaterial(const char* colorTypeString, unsigned int type,
                                      unsigned int index, const aiMaterial &material);


    void getVertexColors(const aiMesh *paiMesh, Chunk &chunk, unsigned int i) const;

    /**
     * Uses the file name of the mesh to calculate the absolute path to the specified texture
     *
     * @param fileNameOfMesh Absolute/relative path to the mesh file
     * @param textureName The path to the textureName relative to the mesh file
     * @return The path to the specified texture
     */
    std::string getPathOfTexture(const std::string &fileNameOfMesh,
                                 std::string textureName);

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
    std::shared_ptr<Texture> getTexture(const aiMaterial *material,
                                        const std::string &fileNameOfMesh,
                                        aiTextureType type);

    /**
     * Initiates OpenGL buffers and buffers the chunk data on to the graphics memory.
     * @param chunk The chunk to be initiated for rendering
     */
    void setupChunkForRendering(Chunk &chunk);

    void setupSphere(std::vector<chag::float3> *positions);

    void createTriangles();
    Triangle* createTriangleFromPositions(std::vector<chag::float3> positionBuffer,
                                          std::vector<unsigned int> indices,
                                          unsigned int startIndex);

    void resetMesh();

    std::vector<Triangle *> triangles;
    std::vector<Material> materials;
    std::vector<Chunk> chunks;

    std::shared_ptr<BoneTransformer> boneTransformer;
    Assimp::Importer importer;

    Sphere sphere;
    AABB aabb;

    unsigned int numAnimations;



};
