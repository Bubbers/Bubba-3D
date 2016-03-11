//
// Created by simon on 2016-02-26.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_TEXTLAYOUT_H
#define SUPER_BUBBA_AWESOME_SPACE_TEXTLAYOUT_H

#include <Layout.h>

using namespace std;

class TextLayout : public Layout{

public:

    /**
     * \warn Never allowed to return a wrapping dimension
     */
    virtual Dimension getWidth();
    /**
     * \warn Never allowed to return a wrapping dimension
     */
    virtual Dimension getHeight();

    virtual void getGLSquares(float layoutXPos,float layoutYPos, float layoutWidth,
                              float layoutHeight, map<string,GLSquare*>* list);

    TextLayout(string text, Font font, Dimension width, Dimension height);

private:
    Dimension width, height;
    string text;
    Font font;

};


#endif //SUPER_BUBBA_AWESOME_SPACE_TEXTLAYOUT_H
