#include "Utils.h"
#include "ShaderProgram.h"
#include "GameObject.h"

float degreeToRad(const float degree) {
    return (float) (degree * M_PI / 180);
}

float radToDegree(const float rad) {
    return (float) (rad * 180 / M_PI);
}

float getRand(const float min, const float max) {
    //srand(time(NULL));
    const float range = max - min;
    return (((float) rand() / (float) RAND_MAX) * range) + min;
}

template <typename T, unsigned S>
T getRandomElem(const T (&ts)[S])
{
    if (S > 0) {
        int ix = (int) getRand(0.0f, S - 0.1f);
        return ts[ix];
    } else {
        return NULL;
    }
}

void updateMinAndMax(const float x, const float y, const float z, chag::float3* minV, chag::float3* maxV) {
    if (x < minV->x) { minV->x = x; };
    if (y < minV->y) { minV->y = y; };
    if (z < minV->z) { minV->z = z; };
    if (x > maxV->x) { maxV->x = x; };
    if (y > maxV->y) { maxV->y = y; };
    if (z > maxV->z) { maxV->z = z; };
}

float3 createRandomVector(const float minValue, const float maxValue){
    return make_vector(getRand(minValue,maxValue), getRand(minValue,maxValue), getRand(minValue,maxValue));
}
