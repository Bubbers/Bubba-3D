//
// Created by johan on 2015-12-05.
//

#ifndef BUBBA_3D_BRUTEFORCEBROADPHASE_H
#define BUBBA_3D_BRUTEFORCEBROADPHASE_H

#include "BroadPhase.h"
#include "GameObject.h"


class BFBroadPhase : BroadPhase
{
public:
    BFBroadPhase();

    virtual void addGameObject(GameObject* GameObject);
    virtual CollisionPairList computeCollisionPairs() ;
    virtual void updateCollision();
private:
    bool isPossiblyColliding(GameObject* gameObject1, GameObject* gameObject2);
    void computeExactCollision(CollisionPairList possibleCollision);
    void triggerObjectEvent(GameObject* object1, GameObject* object2, EventType eventType);

    void forgetCollidingPair(GameObject* object1, GameObject* object2);
    void rememberCollidingPair(GameObject* object1, GameObject* object2);
    bool wasCollidingPreviously(GameObject* object1, GameObject* object2);

    int getMaxId(GameObject* object1, GameObject* object2);
    int getMinId(GameObject* object1, GameObject* object2);

    std::vector<GameObject*> GameObjectList;
    std::vector<std::pair<int, int>> collidingList;
    void removeDirty();

};

#endif //BUBBA_3D_BRUTEFORCEBROADPHASE_H
