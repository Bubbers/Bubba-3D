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
#include <GameObject.h>
#include "MoveComponent.h"
#include "linmath/float3x3.h"

MoveComponent::MoveComponent(){

}

MoveComponent::MoveComponent(std::shared_ptr<GameObject> meshObject) {
    this->meshObject = meshObject;
}

MoveComponent::MoveComponent(std::shared_ptr<GameObject> meshObject, chag::Quaternion rotationSpeed,
                             chag::float3 velocity, chag::float3 acceleration,
                             chag::float3 scaleSpeed) : MoveComponent (meshObject)
{
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->rotationSpeed = rotationSpeed;
    this->scaleSpeed = scaleSpeed;
}

void MoveComponent::update(float dt){

    velocity += acceleration*dt;
    meshObject->setLocation(meshObject->getRelativeLocation() + velocity*dt);

    chag::Quaternion q = slerp(chag::Quaternion(), rotationSpeed, dt);
    meshObject->updateRotation(q);
    meshObject->setScale(meshObject->getRelativeScale()+scaleSpeed*dt);
}

chag::float3 MoveComponent::getVelocity(){ return velocity; }
chag::float3 MoveComponent::getAcceleration(){ return acceleration; }
chag::Quaternion MoveComponent::getRotationSpeed(){ return rotationSpeed; }
chag::float3 MoveComponent::getScaleSpeed(){ return scaleSpeed; }


void MoveComponent::setVelocity(chag::float3 v){velocity = v;}
void MoveComponent::setAcceleration(chag::float3 a){acceleration = a;}
void MoveComponent::setRotationSpeed(chag::Quaternion rs){rotationSpeed = rs;}
void MoveComponent::setScaleSpeed(chag::float3 ss){scaleSpeed = ss;}
