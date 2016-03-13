//
// Created by simon on 2016-01-15.
//

#ifndef BUBBA_3D_IJOYSTICKTRANSLATION_H
#define BUBBA_3D_IJOYSTICKTRANSLATION_H

class IJoystickTranslation{
public:
    enum Button : int {A,B,X,Y,LB,RB,START,BACK,GUIDE,LEFT_THUMBSTICK_PUSH,RIGHT_THUMBSTICK_PUSH};
    enum Axis : int {LEFT_THUMBSTICK_X,LEFT_THUMBSTICK_Y,RIGHT_THUMBSTICK_X,RIGHT_THUMBSTICK_Y,DPAD_X,DPAD_Y,LT,RT};

    virtual float getAxisValue(Axis axis) = 0;
    virtual float getButtonValue(Button button) = 0;
    virtual bool isDefaultMapping() = 0;

};

#endif //BUBBA_3D_IJOYSTICKTRANSLATION_H
