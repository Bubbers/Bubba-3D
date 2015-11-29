//
// Created by johan on 2015-11-29.
//

#include <GameObject.h>
#include <linmath/Quaternion.h>
#include "CarMoveComponent.h"
#include "float3x3.h"
#include "Utils.h"

CarMoveComponent::CarMoveComponent(){

}

CarMoveComponent::CarMoveComponent(bool keysDown[], bool* hasChangedLocation, Car* car, float* cameraThetaLocation, GameObject* carObject) {
    this->keysDown = keysDown;
    this->hasChangedLocation = hasChangedLocation;
    this->car = car;
    this->cameraThetaLocation = cameraThetaLocation;
    this->carObject = carObject;
}

void CarMoveComponent::update(float dt) {
    checkKeyPresses();
    updateCarObject();
}


void CarMoveComponent::checkKeyPresses() {
    if (keysDown[(int) 'w']) {
        float3 term = car->frontDir * car->moveSpeed;
        car->location += term;
        *hasChangedLocation = true;

    }
    if (keysDown[(int) 's']) {
        float3 term = car->frontDir * car->moveSpeed;
        car->location -= term;
        *hasChangedLocation = true;

    }
    if (keysDown[(int) 'a'] && (keysDown[(int) 'w'] || keysDown[(int) 's'])) {
        car->angley += car->rotationSpeed;
        car->frontDir = make_rotation_y<float3x3>(car->rotationSpeed) * car->frontDir;
        *cameraThetaLocation += car->rotationSpeed;
        *hasChangedLocation = true;

    }
    if (keysDown[(int) 'd'] && (keysDown[(int) 'w'] || keysDown[(int) 's'])) {
        car->angley -= car->rotationSpeed;
        car->frontDir = make_rotation_y<float3x3>(-car->rotationSpeed) * car->frontDir;
        *cameraThetaLocation -= car->rotationSpeed;
        *hasChangedLocation = true;
    }
}



void CarMoveComponent::updateCarObject(){
    float3 vUp = make_vector(0.0f, 1.0f, 0.0f);

    float3 frontDir = normalize(car->frontDir);
    float3 rightDir = normalize(cross(frontDir, vUp));

    float anglex = -(degreeToRad(90.0f) - acosf(dot(normalize(car->upDir), frontDir)));
    float anglez = (degreeToRad(90.0f) - acosf(dot(normalize(car->upDir), rightDir)));

    Quaternion qatX = make_quaternion_axis_angle(rightDir, anglex);
    Quaternion qatY = make_quaternion_axis_angle(vUp, car->angley);
    Quaternion qatZ = make_quaternion_axis_angle(make_rotation_y<float3x3>(-car->angley) * frontDir, anglez);

    carObject->move(make_translation(car->location));
    carObject->update(makematrix(qatX));
    carObject->update(makematrix(qatY));
    carObject->update(makematrix(qatZ));
}