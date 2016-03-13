//
// Created by simon on 2016-02-14.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_LISTLAYOUT_H
#define SUPER_BUBBA_AWESOME_SPACE_LISTLAYOUT_H

#include <Layout.h>

using namespace std;

/**
 * A layout to create a list of other layouts.
 */
class ListLayout : public Layout {
public:
    enum Orientation {HORIZONTAL,VERTICAL};

    /**
     * The child of a wrapping layout can only use
     * Dimension::WRAP or Dimension::PIXELS
     */
    virtual void addChild(Layout* child);

    virtual Dimension getWidth();
    virtual Dimension getHeight();

    /**
     * Creates a list where the children are placed after
     * each other in the specified \p orientation
     */
    ListLayout(Orientation orientation, Dimension width, Dimension height);
    virtual ~ListLayout(){}
    virtual void getGLSquares(float layoutXPos,float layoutYPos, float layoutWidth,
                              float layoutHeight, map<string,IHudDrawable*> *list);

protected:
    Orientation orientation;
    Dimension width,height;

    unsigned int wrapSize(Orientation orientation);
    void checkChildCompatibility(Layout* child);
};


#endif //SUPER_BUBBA_AWESOME_SPACE_LISTLAYOUT_H
