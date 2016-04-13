//
// Created by johan on 2016-04-13.
//

#include <cstddef>
#include <timer.h>
#include <sstream>
#include "TwoPhaseCollider.h"

TwoPhaseCollider::TwoPhaseCollider(BroadPhaseCollider *broadPhaseCollider, ExactPhaseCollider *exactPhaseCollider) {
    this->broadPhaseCollider = broadPhaseCollider;
    this->exactPhaseCollider = exactPhaseCollider;
}

void TwoPhaseCollider::updateCollision(Scene *scene) {
    utils::Timer timer;
    timer.start();
    CollisionPairList possibleCollision = broadPhaseCollider->computeCollisionPairs(scene);
    exactPhaseCollider->computeExactCollision(possibleCollision);

    timer.stop();
    stringstream timeMessage;
    timeMessage << "Tested " << possibleCollision.size() <<" possible collision in : " << timer.getElapsedTime() << " ms";
    Logger::logDebug(timeMessage.str());
}