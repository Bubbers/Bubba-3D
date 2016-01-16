#ifndef BUBBA_3D_CHUNK_H
#define BUBBA_3D_CHUNK_H

#include "float2.h"

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



#endif //BUBBA_3D_CHUNK_H
