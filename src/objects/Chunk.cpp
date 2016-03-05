#include "Chunk.h"


Chunk::Chunk() {

}

Chunk::Chunk(std::vector<chag::float3> &positions, std::vector<chag::float3> &normals,
             std::vector<chag::float2> &uvs, std::vector<unsigned int> &indices,
             std::vector<chag::float3> &tangents, std::vector<chag::float3> &bittangents,
             unsigned int materialIndex) :
        m_positions(positions), m_normals(normals), m_uvs(uvs), m_indices(indices), materialIndex(materialIndex),
        m_tangents(tangents), m_bittangents(bittangents) {

}