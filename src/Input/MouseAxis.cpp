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