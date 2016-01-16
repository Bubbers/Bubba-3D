//
// Created by johan on 2015-11-29.
//

#include <GameObject.h>
#include "MoveComponent.h"
#include "float3x3.h"

MoveComponent::MoveComponent(){

}

MoveComponent::MoveComponent(GameObject* meshObject) {
    this->meshObject = meshObject;
}

MoveComponent::MoveComponent(GameObject* meshObject, float rotation, float3 rotationAxis, float rotationSpeed,
                             float3 velocity, float3 location, float3 acceleration,
                             float3 scale, float3 scaleSpeed) : MoveComponent (meshObject){
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->rotation = rotation;
    this->rotationSpeed = rotationSpeed;
    this->rotationAxis = rotationAxis;
    this->location = location;
    this->scale = scale;
    this->scaleSpeed = scaleSpeed;
}

void MoveComponent::update(float dt) {
    updateMeshObject(dt,make_identity<float4x4>(),make_identity<float4x4>(),make_identity<float4x4>(),
                     make_identity<float4x4>(),false);
}



void MoveComponent::updateWithExtras(float dt, float4x4 afterTranslate, float4x4 beforeTranslate,
                                     float4x4 beforeRotation, float4x4 beforeScale) {
    updateMeshObject(dt,afterTranslate,beforeTranslate,beforeRotation,beforeScale,true);
}

void MoveComponent::afterCollision() {
}

void MoveComponent::duringCollision() {
}

void MoveComponent::beforeCollision() {
}


void MoveComponent::updateMeshObject(float dt, float4x4 afterTranslate,float4x4 beforeTranslate, float4x4 beforeRotation,
                                     float4x4 beforeScale, bool extraMatrices){

    velocity += acceleration*dt;
    location += velocity*dt;
    rotation += rotationSpeed*dt;
    scale += scaleSpeed*dt;

    float4x4 transform;
    if(extraMatrices)
        transform = afterTranslate*make_translation(location);
    else
        transform = make_translation(location);
    if(extraMatrices)
        transform = transform*beforeTranslate;
    if(rotationAxis != make_vector(0.0f,0.0f,0.0f))
        transform = transform*make_rotation<float4x4>(rotationAxis,rotation);
    if(extraMatrices)
        transform = transform*beforeRotation;
    transform = transform*make_scale<float4x4>(scale);
    if(extraMatrices)
        transform = transform*beforeScale;
    meshObject->move(transform);


}

float3 MoveComponent::getVelocity(){ return velocity; }
float3 MoveComponent::getAcceleration(){ return acceleration; }
float MoveComponent::getRotation() { return rotation; }
float3 MoveComponent::getLocation(){ return location; }
float MoveComponent::getRotationSpeed(){ return rotationSpeed; }
float3 MoveComponent::getRotationAxis() { return rotationAxis; }
float3 MoveComponent::getScale(){ return scale; }
float3 MoveComponent::getScaleSpeed(){ return scaleSpeed; }


void MoveComponent::setVelocity(float3 v){velocity = v;}
void MoveComponent::setAcceleration(float3 a){acceleration = a;}
void MoveComponent::setRotation(float r){rotation = r;}
void MoveComponent::setLocation(float3 l){location = l;}
void MoveComponent::setRotationSpeed(float rs){rotationSpeed = rs;}
void MoveComponent::setRotationAxis(float3 axis) { rotationAxis = normalize(axis); }
void MoveComponent::setScale(float3 s){scale = s;}
void MoveComponent::setScaleSpeed(float3 ss){scaleSpeed = ss;}
