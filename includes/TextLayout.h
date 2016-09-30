#pragma once

#include <Layout.h>
#include <Font.h>

/**
 * A layout used to present text.
 */
class TextLayout : public Layout{

public:

    /**
     * Creates a text layout. Create a font using FontManager::loadAndFetchFont()
     */
    TextLayout(std::string text, Font* font, Dimension width, Dimension height);


    ~TextLayout() = default;

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
     * Changes the text of the TextLayout. If you want to change the text of
     * an already drawn text you should use TextObject::setText() instead.
     * You can get a hold of the TextObject by first setting an ID on this
     * object before adding it to the HUD using TextLayout::setTextId(). Then
     * call HudRenderer::getHudDrawableById() and typecast the result to a TextObject.
     */
    void setText(std::string text);

    /**
     * Sets the id of the to-be-rendered TextObject. This object can be accessed by
     * HudRenderer::getHudDrawableById().
     */
    TextLayout* setTextId(std::string id);

    TextLayout* setPadding(int pixels);
    TextLayout* setPadding(int topAndBottom, int rightAndLeft);
    TextLayout* setPadding(int top, int right, int bot, int left);

private:
    std::string text;
    Font* font;
    Dimension width;
    Dimension height;
    std::string textId = "";
    int padding[4] = {0,0,0,0};

};
