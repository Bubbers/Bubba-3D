//
// Created by johan on 2015-12-05.
//

#ifndef BUBBA_3D_BRUTEFORCEBROADPHASE_H
#define BUBBA_3D_BRUTEFORCEBROADPHASE_H

#include "BroadPhase.h"

class BFBroadPhase : BroadPhase
{
public:
    BFBroadPhase();

    virtual void addGameObject(GameObject* GameObject);
    virtual CollisionPairList computeCollisionPairs() ;
    virtual void updateCollision();
private:
    std::vector<GameObject*> GameObjectList;
    std::vector<std::pair<int, int>> collidingList;


};

#endif //BUBBA_3D_BRUTEFORCEBROADPHASE_H
