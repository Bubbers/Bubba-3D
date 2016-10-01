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
#include "Chunk.h"


Chunk::Chunk() {

}

Chunk::Chunk(std::vector<chag::float3> &positions, std::vector<chag::float3> &normals,
             std::vector<chag::float2> &uvs, std::vector<unsigned int> &indices,
             std::vector<chag::float3> &tangents, std::vector<chag::float3> &bittangents,
             unsigned int materialIndex)
           : m_positions(positions), m_normals(normals), m_uvs(uvs),
             m_indices(indices), m_tangents(tangents),
             m_bittangents(bittangents), materialIndex(materialIndex)
{
}
