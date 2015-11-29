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
    CarMoveComponent(bool keysDown[],bool* hasChangedLocation, Car* car, float* cameraThetaLocation, GameObject* carObject);
    void update(float dt);
private:
    bool *keysDown;
    bool *hasChangedLocation;
    float* cameraThetaLocation;
    Car* car;
    GameObject* carObject;

    void checkKeyPresses();
    void updateCarObject();
};


#endif //BUBBA_3D_KEYBOARDMOVECOMPONENT_H
