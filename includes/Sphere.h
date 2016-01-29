//
// Created by simon on 2016-01-29.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_SPHERE_H
#define SUPER_BUBBA_AWESOME_SPACE_SPHERE_H

using namespace chag;

class Sphere{

public:
    Sphere(){}
    Sphere(float3 position,float radius) : pos(position), radius(radius){}
    float3 getPosition(){ return pos; }
    float getRadius(){return radius;}
    void setPosition(float3 position){ pos = position; }
    void setRadius(float radius){ this->radius = radius;}

private:
    float3 pos;
    float radius;

};

#endif //SUPER_BUBBA_AWESOME_SPACE_SPHERE_H
