//
// Created by simon on 2016-09-30.
//

#include <Globals.h>
#include "RelativeIHudDrawable.h"
#include "Camera.h"

RelativeIHudDrawable::RelativeIHudDrawable(Camera* worldCamera, GameObject *relativeTo, IHudDrawable *toDraw) :
        relativeTo(relativeTo), toDraw(toDraw), worldCamera(worldCamera) {}

void RelativeIHudDrawable::render(ShaderProgram *shaderProgram, float4x4 *projectionMatrix) {

    float3 drawableRelativePosition = toDraw->relativePosition;
    float4x4 screenPos = worldCamera->getProjectionMatrix()*worldCamera->getViewMatrix()*relativeTo->getModelMatrix();
    int w = Globals::get(Globals::WINDOW_WIDTH);
    int h = Globals::get(Globals::WINDOW_HEIGHT);
    float x = (screenPos.c4.x/screenPos.c4.w+1.0f)*w/2.0f;
    float y = h-(screenPos.c4.y/screenPos.c4.w+1.0f)*h/2.0f;
    float3 tempRelativeVector = drawableRelativePosition + make_vector(x,-y,0.0f);
    toDraw->relativePosition = tempRelativeVector;
    toDraw->render(shaderProgram,projectionMatrix);
    toDraw->relativePosition = drawableRelativePosition;

}