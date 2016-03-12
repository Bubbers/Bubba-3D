//
// Created by johan on 2015-11-29.
//

#ifndef BUBBA_3D_COMPONENT_H
#define BUBBA_3D_COMPONENT_H

class GameObject;

class IComponent {
public:
    virtual void update(float dt) = 0;
    void bind(GameObject *owner) { this->owner = owner; };

    virtual void beforeCollision(GameObject* collider) {};
    virtual void duringCollision(GameObject* collider) {};
    virtual void afterCollision(GameObject* collider) {};
    virtual void onDeath() {};
protected:
    GameObject *owner;
};

#endif //BUBBA_3D_COMPONENT_H
