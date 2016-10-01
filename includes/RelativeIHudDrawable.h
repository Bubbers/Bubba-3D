//
// Created by simon on 2016-09-30.
//

#ifndef BUBBAROGUEFORT_RELATIVEIHUDDRAWABLE_H
#define BUBBAROGUEFORT_RELATIVEIHUDDRAWABLE_H

#include "IHudDrawable.h"

class GameObject;
class Camera;

class RelativeIHudDrawable : public IHudDrawable{
public:

    RelativeIHudDrawable(Camera* worldCamera, GameObject* relativeTo, IHudDrawable* toDraw);
    virtual void render(ShaderProgram* shaderProgram, float4x4* projectionMatrix);

protected:
    GameObject* relativeTo;
    IHudDrawable* toDraw;
    Camera* worldCamera;

};


#endif //BUBBAROGUEFORT_RELATIVEIHUDDRAWABLE_H
