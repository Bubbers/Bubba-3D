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

#include "IComponent.h"
#include "linmath/float3.h"
#include "GameObject.h"
#include "HudRenderer.h"
#include <linmath/Quaternion.h>

class MoveComponent : public IComponent {
public:
    MoveComponent();
    MoveComponent(chag::Quaternion rotationSpeed,
                  chag::float3 velocity, chag::float3 acceleration, chag::float3 scaleSpeed);

    virtual void update(float dt);

    chag::float3 getVelocity();
    chag::float3 getAcceleration();
    chag::Quaternion getRotationSpeed();
    chag::float3 getScaleSpeed();

    void setVelocity(chag::float3 v);
    void setAcceleration(chag::float3 a);
    void setRotationSpeed(chag::Quaternion rs);
    void setScaleSpeed(chag::float3 ss);

private:
    chag::float3 velocity = chag::make_vector(0.0f, 0.0f, 0.0f);
    chag::float3 acceleration = chag::make_vector(0.0f, 0.0f, 0.0f);
    chag::Quaternion rotationSpeed;
    chag::float3 scaleSpeed = chag::make_vector(0.0f,0.0f,0.0f);
};
