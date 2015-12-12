#ifndef BUBBA_3D_KEYBOARDMOVECOMPONENT_H
#define BUBBA_3D_KEYBOARDMOVECOMPONENT_H

#include "IComponent.h"
#include "float3.h"
#include "Car.h"
#include "GameObject.h"

using namespace chag;




class CarMoveComponent : public IComponent {
public:
    CarMoveComponent();
    CarMoveComponent(Car* car, float* cameraThetaLocation, GameObject* carObject);
    void update(float dt);
    void afterCollision();
    void duringCollision();
    void beforeCollision();
private:
    float* cameraThetaLocation;
    Car* car;
    GameObject* carObject;
    bool hasChanged = true;

    void checkKeyPresses();
    void updateCarObject();
    void alignCarTowardsSurface();
};


#endif //BUBBA_3D_KEYBOARDMOVECOMPONENT_H
