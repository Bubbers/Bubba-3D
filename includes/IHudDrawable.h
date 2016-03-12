//
// Created by simon on 2016-03-12.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_IHUDDRAWABLE_H
#define SUPER_BUBBA_AWESOME_SPACE_IHUDDRAWABLE_H

#include <linmath/float3.h>
#include <linmath/float4x4.h>

using namespace chag;

class ShaderProgram;

class IHudDrawable {
public:
    virtual void render(ShaderProgram* shaderProgram, float4x4* projectionMatrix) = 0;

    virtual void setRelativePosition(float3 position);
    virtual void setCenterOffset(float3 offset);
    virtual void setRotation(float rotation);

protected:
    float3 relativePosition = make_vector(0.0f, 0.0f, 0.0f), center = make_vector(0.0f, 0.0f, 0.0f);
    float rotation = 0.0f;
    float3 originalPosition = make_vector(0.0f, 0.0f, 0.0f);

};


#endif //SUPER_BUBBA_AWESOME_SPACE_IHUDDRAWABLE_H
