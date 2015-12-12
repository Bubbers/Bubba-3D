//
// Created by johan on 2015-11-29.
//

#include <GameObject.h>
#include <linmath/Quaternion.h>
#include <Logger.h>
#include <timer.h>
#include <sstream>
#include "CarMoveComponent.h"
#include "float3x3.h"
#include "Utils.h"

CarMoveComponent::CarMoveComponent(){

}

CarMoveComponent::CarMoveComponent(bool keysDown[], Car* car, float* cameraThetaLocation, GameObject* carObject, Collider* collisionHandler) {
    this->keysDown = keysDown;
    this->car = car;
    this->cameraThetaLocation = cameraThetaLocation;
    this->carObject = carObject;
    this->collisionHandler = collisionHandler;
}

void CarMoveComponent::update(float dt) {
    checkKeyPresses();
    if(hasChanged) {
        updateCarObject();
        alignCarTowardsSurface();
        hasChanged = false;
    }
}

void CarMoveComponent::onCollision() {
    Logger::logInfo("Collision");
}


void CarMoveComponent::checkKeyPresses() {
    if (keysDown[(int) 'w']) {
        float3 term = car->frontDir * car->moveSpeed;
        car->location += term;
        hasChanged = true;

    }
    if (keysDown[(int) 's']) {
        float3 term = car->frontDir * car->moveSpeed;
        car->location -= term;
        hasChanged = true;

    }
    if (keysDown[(int) 'a'] && (keysDown[(int) 'w'] || keysDown[(int) 's'])) {
        car->angley += car->rotationSpeed;
        car->frontDir = make_rotation_y<float3x3>(car->rotationSpeed) * car->frontDir;
        *cameraThetaLocation += car->rotationSpeed;
        hasChanged = true;

    }
    if (keysDown[(int) 'd'] && (keysDown[(int) 'w'] || keysDown[(int) 's'])) {
        car->angley -= car->rotationSpeed;
        car->frontDir = make_rotation_y<float3x3>(-car->rotationSpeed) * car->frontDir;
        *cameraThetaLocation -= car->rotationSpeed;
        hasChanged = true;
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

void CarMoveComponent::alignCarTowardsSurface() {
    float3 upVec = make_vector(0.0f, 1.0f, 0.0f);

    //Calculate intersections
    float3x3 rot = make_rotation_y<float3x3>(car->angley);
    utils::Timer timer;
    timer.start();
    float a = collisionHandler->rayIntersection(car->location + rot * car->wheel1, -upVec);
    float b = collisionHandler->rayIntersection(car->location + rot * car->wheel2, -upVec);
    float c = collisionHandler->rayIntersection(car->location + rot * car->wheel3, -upVec);
    float d = collisionHandler->rayIntersection(car->location + rot * car->wheel4, -upVec);
    timer.stop();

    stringstream timeMessage;
    timeMessage << "Tested 4 ray/aabb intersections in " << timer.getElapsedTime() << " ms";
    //Logger::logDebug(timeMessage.str());
    if (a == 0 && b == 0 && c == 0 && d == 0) {
        return;
    }
    //Calculate 3d points of intersection
    float3 af = car->wheel1 - (upVec * a);
    float3 bf = car->wheel2 - (upVec * b);
    float3 cf = car->wheel3 - (upVec * c);
    float3 df = car->wheel4 - (upVec * d);

    //Calculate new up vector
    float3 vABa = af - bf;
    float3 vCBa = cf - bf;
    float3 newUpa = cross(vABa, vCBa);

    float3 vABb = bf - cf;
    float3 vCBb = df - cf;
    float3 newUpb = cross(vABb, vCBb);

    float3 halfVector = normalize(newUpa - newUpb);
    car->upDir = -(rot * halfVector);

    //Change wheel locations
    car->wheel1 += upVec * a;
    car->wheel2 += upVec * b;
    car->wheel3 += upVec * c;
    car->wheel4 += upVec * d;

    float3 frontDir = normalize(car->frontDir);
    float3 rightDir = normalize(cross(frontDir, upVec));

    float anglex = -(degreeToRad(90.0f) - acosf(dot(normalize(car->upDir), frontDir)));
    float anglez = (degreeToRad(90.0f) - acosf(dot(normalize(car->upDir), rightDir)));

    Quaternion qatX = make_quaternion_axis_angle(rightDir, anglex);
    Quaternion qatZ = make_quaternion_axis_angle(make_rotation_y<float3x3>(-car->angley) * frontDir, anglez);

    float4 newLoc = makematrix(qatX) * makematrix(qatZ) * make_vector4(car->wheel1, 1.0f);

    car->location += make_vector(0.0f, car->wheel1.y + (car->wheel1.y - newLoc.y), 0.0f);
}