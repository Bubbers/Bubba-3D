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

#include <vector>
#include "linmath/float2.h"
#include "linmath/float3.h"
#include <GL/glew.h>
#include <map>
#include <BoneMatrices.h>
#include "BoneInfluenceOnVertex.h"

class Chunk {
public:
    Chunk();

    ~Chunk() {
    }

    // Data on host
    std::vector<chag::float3> m_positions;
    std::vector<chag::float3> m_normals;
    std::vector<chag::float2> m_uvs;
    std::vector<unsigned int> m_indices;
    std::vector<chag::float3> m_tangents;
    std::vector<chag::float3> m_bittangents;
    std::vector<BoneInfluenceOnVertex> bones;

    // Data on GPU
    GLuint m_positions_bo;
    GLuint m_normals_bo;
    GLuint m_uvs_bo;
    GLuint m_ind_bo;
    GLuint m_tangents_bo;
    GLuint m_bittangents_bo;
    GLuint bonesBufferObject;
    // Vertex Array Object
    GLuint m_vaob;

    unsigned int materialIndex;
};
