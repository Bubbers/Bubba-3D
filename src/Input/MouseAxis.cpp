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
//
// Created by simon on 2016-01-01.
//

#include <MouseAxis.h>
#include <ControlStatus.h>
#include "Input.h"
#include <MousePosition.h>
#include <algorithm>

MouseAxis::MouseAxis(Axis axis, float accelerator) : Input(true) {
    this->axis = axis;
    this->accelerator = accelerator;
}

ControlStatus MouseAxis::getStatus() {
    float value = axis == X ? MousePosition().getXMidOffset() : MousePosition().getYMidOffset();
    return ControlStatus(ControlStatus::Activator::MOUSE,clip(value*accelerator));
}

ControlStatus::Activator MouseAxis::getActivator() {
    return ControlStatus::MOUSE;
}

float MouseAxis::clip(float n) {
    return std::max(-100.0f, std::min(n, 100.0f));
}