/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */

#include <Globals.h>
#include "RelativeIHudDrawable.h"
#include "Camera.h"
#include <memory>

RelativeIHudDrawable::RelativeIHudDrawable(Camera* worldCamera, std::shared_ptr<GameObject> relativeTo, IHudDrawable *toDraw) :
        relativeTo(relativeTo), toDraw(toDraw), worldCamera(worldCamera) {}

void RelativeIHudDrawable::render(std::shared_ptr<ShaderProgram> shaderProgram, chag::float4x4 *projectionMatrix) {

    chag::float3 drawableRelativePosition = toDraw->relativePosition;
    chag::float4x4 screenPos = worldCamera->getProjectionMatrix()
                             * worldCamera->getViewMatrix()
                             * relativeTo->getModelMatrix();

    int w = Globals::get(Globals::WINDOW_WIDTH);
    int h = Globals::get(Globals::WINDOW_HEIGHT);

    float x =     (screenPos.c4.x / screenPos.c4.w + 1.0f) * w / 2.0f;
    float y = h - (screenPos.c4.y / screenPos.c4.w + 1.0f) * h / 2.0f;

    chag::float3 tempRelativeVector = drawableRelativePosition + chag::make_vector(roundf(x), -roundf(y), 0.0f);

    toDraw->relativePosition = tempRelativeVector;
    toDraw->render(shaderProgram, projectionMatrix);
    toDraw->relativePosition = drawableRelativePosition;

}
