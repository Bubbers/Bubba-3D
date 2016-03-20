//
// Created by simon on 2016-03-12.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_IHUDDRAWABLE_H
#define SUPER_BUBBA_AWESOME_SPACE_IHUDDRAWABLE_H

#include <linmath/float3.h>
#include <linmath/float4x4.h>

using namespace chag;

class ShaderProgram;

/**
 * An interface for low-level objects used by HudRenderer to render the HUD.
 * Mainly produced by different implementations of Layout
 */
class IHudDrawable {
public:
    virtual void render(ShaderProgram* shaderProgram, float4x4* projectionMatrix) = 0;

    /**
     * Sets a position relative to the original position calculated
     * by Layout implementations.
     */
    virtual void setRelativePosition(float3 position);
    /**
     * Sets the rotation center offset. The original center is the actual center
     * of the drawn object.
     */
    virtual void setCenterOffset(float3 offset);
    virtual void setRotation(float rotation);

protected:
    float3 relativePosition = make_vector(0.0f, 0.0f, 0.0f), center = make_vector(0.0f, 0.0f, 0.0f);
    float rotation = 0.0f;
    float3 originalPosition = make_vector(0.0f, 0.0f, 0.0f);

};


#endif //SUPER_BUBBA_AWESOME_SPACE_IHUDDRAWABLE_H
