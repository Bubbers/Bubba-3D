

#include "Wind.h"

Wind::Wind() {
    force = chag::make_vector(0.0f, 0.0f, 0.0f);
}

Wind::Wind(const chag::float3 &force) : force(force) {

}

const chag::float3 &Wind::getForce() const {
    return force;
}
