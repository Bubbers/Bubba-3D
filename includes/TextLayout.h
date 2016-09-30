//
// Created by simon on 2016-02-26.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_TEXTLAYOUT_H
#define SUPER_BUBBA_AWESOME_SPACE_TEXTLAYOUT_H

#include <Layout.h>
#include <Font.h>

using namespace std;

/**
 * A layout used to present text.
 */
class TextLayout : public Layout{

public:

    /**
     * \internal
     * \warning Never allowed to return a wrapping dimension
     */
    virtual Dimension getWidth();

    /**
     * \internal
     * \warning Never allowed to return a wrapping dimension
     */
    virtual Dimension getHeight();

    /**
     * \internal
     */
    virtual void getGLSquares(float layoutXPos,float layoutYPos, float layoutWidth,
                              float layoutHeight, map<string,IHudDrawable*>* list);

    /**
     * Creates a text layout. Create a font using FontManager::loadAndFetchFont()
     */
    TextLayout(string text, Font* font, Dimension width, Dimension height);

    /**
     * Changes the text of the TextLayout. If you want to change the text of
     * an already drawn text you should use TextObject::setText() instead.
     * You can get a hold of the TextObject by first setting an ID on this
     * object before adding it to the HUD using TextLayout::setTextId(). Then
     * call HudRenderer::getHudDrawableById() and typecast the result to a TextObject.
     */
    void setText(string text);

    /**
     * Sets the id of the to-be-rendered TextObject. This object can be accessed by
     * HudRenderer::getHudDrawableById().
     */
    TextLayout* setTextId(string id);

    TextLayout* setPadding(int pixels);
    TextLayout* setPadding(int topAndBottom, int rightAndLeft);
    TextLayout* setPadding(int top, int right, int bot, int left);

private:
    Dimension width, height;
    string text;
    Font* font;
    string textId = "";
    int padding[4] = {0,0,0,0};

};


#endif //SUPER_BUBBA_AWESOME_SPACE_TEXTLAYOUT_H
