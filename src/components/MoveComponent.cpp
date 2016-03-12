//
// Created by johan on 2015-11-29.
//

#include <GameObject.h>
#include <Logger.h>
#include "MoveComponent.h"
#include "linmath/float3x3.h"

MoveComponent::MoveComponent(){

}

MoveComponent::MoveComponent(GameObject* meshObject) {
    this->meshObject = meshObject;
}

MoveComponent::MoveComponent(GameObject* meshObject, Quaternion rotationSpeed, float3 velocity, float3 acceleration,
                             float3 scaleSpeed) : MoveComponent (meshObject){
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->rotationSpeed = rotationSpeed;
    this->scaleSpeed = scaleSpeed;
}

void MoveComponent::afterCollision() {
}

void MoveComponent::duringCollision() {
}

void MoveComponent::beforeCollision() {
}


void MoveComponent::update(float dt){

    velocity += acceleration*dt;
    meshObject->setLocation(meshObject->getLocation() + velocity*dt);
	Quaternion q = slerp(Quaternion(), rotationSpeed, dt);
	meshObject->updateRotation(q);
    meshObject->setScale(meshObject->getScale()+scaleSpeed*dt);


}

float3 MoveComponent::getVelocity(){ return velocity; }
float3 MoveComponent::getAcceleration(){ return acceleration; }
Quaternion MoveComponent::getRotationSpeed(){ return rotationSpeed; }
float3 MoveComponent::getScaleSpeed(){ return scaleSpeed; }


void MoveComponent::setVelocity(float3 v){velocity = v;}
void MoveComponent::setAcceleration(float3 a){acceleration = a;}
void MoveComponent::setRotationSpeed(Quaternion rs){rotationSpeed = rs;}
void MoveComponent::setScaleSpeed(float3 ss){scaleSpeed = ss;}
