#pragma once

#include <linmath/float3.h>

class Wind {
public:
    Wind();
    Wind(const chag::float3 &force);

    const chag::float3 &getForce() const;

private:
    chag::float3 force;

};



