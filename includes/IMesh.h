
#include "Triangle.h"
#include <vector>

#ifndef HALLSOFBUBBA_IMESH_H
#define HALLSOFBUBBA_IMESH_H

#endif //HALLSOFBUBBA_IMESH_H

class Chunk;

class IMesh {
public:
    /**
     * NOTE: The triangles have not been transformed.
     *
     * @return A list of all the triangles of the mesh.
     */
    virtual std::vector<Triangle *> getTriangles() = 0;

    /**
     * NOTE: The AABB has not been transformed.
     *
     * @return The AABB of the mesh.
     */
    virtual AABB* getAABB() = 0;

    /**
     * NOTE: The sphere has not been transformed.
     *
     * @return The sphere surrounding the object.
     */
    virtual Sphere getSphere() = 0;

    virtual std::vector<Chunk>* getChunks() = 0;
    virtual std::vector<Material>* getMaterials() = 0;

    virtual bool hasAnimations() = 0;

    /**
    * Calculates the transform to be applied to each bone at the current time.
    *
    * @param totalElapsedTimeInSeconds The time since the application was started
    * @return A vector containing the transforms of each bone. The index in the vector corresponds to the bones index. The index of a bone can be found in boneNameToIndexMapping.
    *
    */
    virtual std::vector<chag::float4x4> getBoneTransforms(float totalElapsedTimeInSeconds) = 0;
};