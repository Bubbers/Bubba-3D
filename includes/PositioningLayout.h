//
// Created by simon on 2016-02-21.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_POSITIONINGLAYOUT_H
#define SUPER_BUBBA_AWESOME_SPACE_POSITIONINGLAYOUT_H

#include <Layout.h>

using namespace std;

class PositioningLayout : public Layout {
public:
    virtual void addChild(Layout* child);

    virtual void addChild(Layout* child, Dimension x, Dimension y);

    /**
     * \warn Never allowed to return a wrapping dimension
     */
    virtual Dimension getWidth();
    /**
     * \warn Never allowed to return a wrapping dimension
     */
    virtual Dimension getHeight();

    virtual void getGLSquares(float layoutXPos,float layoutYPos, float layoutWidth,
                              float layoutHeight, map<string,IHudDrawable*>* list);

    PositioningLayout(Dimension width, Dimension height);

    void checkChildCompatibility(Layout* child);

    virtual Layout* findById(string id);

protected:
    struct PositionedLayout {
        Layout* child;
        Dimension x;
        Dimension y;
        PositionedLayout(Layout* child, Dimension x, Dimension y) : child(child),x(x),y(y){}
    };

    Dimension width,height;
    vector<PositionedLayout*> children;
};


#endif //SUPER_BUBBA_AWESOME_SPACE_POSITIONINGLAYOUT_H
