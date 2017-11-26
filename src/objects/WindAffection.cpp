#include "WindAffection.h"


bool WindAffection::getIsAffectedByWind() const {
    return isAffectedByWind;
}

void WindAffection::setAffectedByWind(bool isAffectedByWind) {
    this->isAffectedByWind = isAffectedByWind;
}

float WindAffection::getMainBendiness() const {
    return mainBendiness;
}

void WindAffection::setMainBendiness(float mainBendiness) {
    this->mainBendiness = mainBendiness;
}

float WindAffection::getBranchAmplitude() const {
    return branchAmplitude;
}

void WindAffection::setBranchAmplitude(float branchAmplitude) {
    WindAffection::branchAmplitude = branchAmplitude;
}

float WindAffection::getLeafAmplitude() const {
    return leafAmplitude;
}

void WindAffection::setLeafAmplitude(float leafAmplitude) {
    WindAffection::leafAmplitude = leafAmplitude;
}
