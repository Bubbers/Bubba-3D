//
// Created by johan on 2015-11-29.
//

#ifndef BUBBA_3D_COMPONENT_H
#define BUBBA_3D_COMPONENT_H

#include <GameObjectType.h>

class IComponent {
public:
    virtual void update(float dt) = 0;

    virtual void beforeCollision(GameObjectType collider) {};
    virtual void duringCollision(GameObjectType collider) {};
    virtual void afterCollision(GameObjectType collider) {};
    virtual void onDeath() {};
private:
};

#endif //BUBBA_3D_COMPONENT_H
