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
#ifndef BUBBA_3D_KEYBOARDMOVECOMPONENT_H
#define BUBBA_3D_KEYBOARDMOVECOMPONENT_H

#include "IComponent.h"
#include "linmath/float3.h"
#include "GameObject.h"
#include "HudRenderer.h"
#include <linmath/Quaternion.h>

using namespace chag;




class MoveComponent : public IComponent {
public:
    MoveComponent();
    MoveComponent(GameObject* meshObject);
    MoveComponent(GameObject* meshObject,Quaternion rotationSpeed, float3 velocity,
                  float3 acceleration, float3 scaleSpeed);
    virtual void update(float dt);
    float3 getVelocity();
    float3 getAcceleration();
    Quaternion getRotationSpeed();
    float3 getScaleSpeed();
    void setVelocity(float3 v);
    void setAcceleration(float3 a);
    void setRotationSpeed(Quaternion rs);
    void setScaleSpeed(float3 ss);
protected:

    GameObject* meshObject;

private:
    float3 velocity = make_vector(0.0f, 0.0f, 0.0f);
    float3 acceleration = make_vector(0.0f, 0.0f, 0.0f);
    Quaternion rotationSpeed = Quaternion();
    float3 scaleSpeed = make_vector(0.0f,0.0f,0.0f);
};


#endif //BUBBA_3D_KEYBOARDMOVECOMPONENT_H
