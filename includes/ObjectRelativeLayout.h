//
// Created by simon on 2016-09-30.
//

#ifndef BUBBAROGUEFORT_OBJECTRELATIVELAYOUT_H
#define BUBBAROGUEFORT_OBJECTRELATIVELAYOUT_H

#include <linmath/float4x4.h>
#include "PositioningLayout.h"


class GameObject;
class Dimension;
class IHudDrawable;
class Camera;

class ObjectRelativeLayout{
public:

    ObjectRelativeLayout(Layout* rootLayout);

    /****/
    virtual void getGLSquares(map<std::string,IHudDrawable*>* list);

private:
    Layout* rootLayout;

};

#endif //BUBBAROGUEFORT_OBJECTRELATIVELAYOUT_H
