//
// Created by simon on 2016-02-26.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_TEXTLAYOUT_H
#define SUPER_BUBBA_AWESOME_SPACE_TEXTLAYOUT_H

#include <Layout.h>
#include <Font.h>

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
                              float layoutHeight, map<string,IHudDrawable*>* list);

    TextLayout(string text, Font* font, Dimension width, Dimension height);

    void setText(string text);

private:
    Dimension width, height;
    string text;
    Font* font;

};


#endif //SUPER_BUBBA_AWESOME_SPACE_TEXTLAYOUT_H
