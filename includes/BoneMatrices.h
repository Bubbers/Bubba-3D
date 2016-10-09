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

#include "linmath/float4x4.h"

/**
 * Struct for containing different matrices related to a bone.
 * Used for transforming vertices connected to a bone.
 */
struct BoneMatrices
{
    //BoneOffset transforms a vertex from local space to bone space
    chag::float4x4 boneOffset = chag::make_identity<chag::float4x4>();
    //finalTransformation transforms a vertex connected to the bone in bone space to its new position relative the bone.
    chag::float4x4 finalTransformation = chag::make_identity<chag::float4x4>();
};

