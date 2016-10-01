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
