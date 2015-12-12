#ifndef BUBBA_3D_KEYBOARDMOVECOMPONENT_H
#define BUBBA_3D_KEYBOARDMOVECOMPONENT_H

#include "IComponent.h"
#include "float3.h"
#include "GameObject.h"

using namespace chag;




class CarMoveComponent : public IComponent {
public:
    CarMoveComponent();
    CarMoveComponent(float* cameraThetaLocation, GameObject* carObject);
    void update(float dt);
    void afterCollision();
    void duringCollision();
    void beforeCollision();
private:

    float3 frontDir = make_vector(0.0f, 0.0f, 1.0f);
    float3 upDir = make_vector(0.0f, 1.0f, 0.0f);
    float3 velocity = make_vector(0.0f, 0.0f, 0.0f);
    float3 acceleration = make_vector(0.0f, 0.0f, 0.0f);
    float3 location = make_vector(0.0f, 0.0f, 0.f);

    float moveSpeed = (float) (2 * M_PI / 180);
    float rotationSpeed = (float) (2 * M_PI / 180);

    float* cameraThetaLocation;
    GameObject* carObject;
    bool hasChanged = true;
    float angley = 0;
    float anglez = 0;
    float anglex = 0;
    float lengthx = 2;
    float lengthz = 3;

    void checkKeyPresses(float dt);
    void updateCarObject();
    void alignCarTowardsSurface();
};


#endif //BUBBA_3D_KEYBOARDMOVECOMPONENT_H
