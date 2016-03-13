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
    void update(float dt);
    void afterCollision();
    void duringCollision();
    void beforeCollision();
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
