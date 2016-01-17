#ifndef BUBBA_3D_KEYBOARDMOVECOMPONENT_H
#define BUBBA_3D_KEYBOARDMOVECOMPONENT_H

#include "IComponent.h"
#include "float3.h"
#include "GameObject.h"
#include "HudRenderer.h"
#include <Quaternion.h>

using namespace chag;




class MoveComponent : public IComponent {
public:
    MoveComponent();
    MoveComponent(GameObject* meshObject);
    MoveComponent(GameObject* meshObject,Quaternion axis, float rotationSpeed, float3 velocity,
                  float3 location, float3 acceleration, float3 scale, float3 scaleSpeed);
    void update(float dt);
    void afterCollision();
    void duringCollision();
    void beforeCollision();
    float3 getVelocity();
    float3 getAcceleration();
    Quaternion getRotation();
    float3 getLocation();
    float getRotationSpeed();
    float3 getScale();
    float3 getScaleSpeed();
    void setVelocity(float3 v);
    void setAcceleration(float3 a);
    void setRotation(Quaternion r);
    void setLocation(float3 l);
    void setRotationSpeed(float rs);
    void updateRotation(Quaternion r);
    void setScale(float3 s);
    void setScaleSpeed(float3 ss);
protected:

    GameObject* meshObject;

private:
    float3 velocity = make_vector(0.0f, 0.0f, 0.0f);
    float3 acceleration = make_vector(0.0f, 0.0f, 0.0f);
    Quaternion rotation = make_quaternion_axis_angle(make_vector(0.0f,0.0f,0.0f),0.0f);
    bool hasRotation = false;
    float3 location = make_vector(0.0f,0.0f,0.0f);
    float rotationSpeed = 0.0f;
    float3 scale = make_vector(1.0f,1.0f,1.0f);
    float3 scaleSpeed = make_vector(0.0f,0.0f,0.0f);
};


#endif //BUBBA_3D_KEYBOARDMOVECOMPONENT_H
