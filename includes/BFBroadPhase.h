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

    virtual void add(GameObject* GameObject) ;
    virtual void update() ;
    virtual CollisionPairList computeCollisionPairs() ;

private:
    std::vector<GameObject*> GameObjectList;


};

#endif //BUBBA_3D_BRUTEFORCEBROADPHASE_H
