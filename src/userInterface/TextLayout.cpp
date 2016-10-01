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
#include <TextLayout.h>
#include <IHudDrawable.h>
#include <TextObject.h>

TextLayout::TextLayout(std::string text, Font* font, Dimension width, Dimension height): 
    text(text),
    font(font),
    width(width),
    height(height) {
}

Dimension TextLayout::getWidth() {
    return width;
}

Dimension TextLayout::getHeight() {
    return height;
}

void TextLayout::setText(std::string text) {
    this->text = text;
}

TextLayout* TextLayout::setTextId(std::string id) {
    this->textId = id;
    return this;
}

TextLayout* TextLayout::setPadding(int pixels) {
    setPadding(pixels,pixels);
    return this;
}

TextLayout* TextLayout::setPadding(int topAndBottom, int rightAndLeft) {
    setPadding(topAndBottom,rightAndLeft,topAndBottom,rightAndLeft);
    return this;
}

TextLayout* TextLayout::setPadding(int top, int right, int bot, int left) {
    padding[0] = top;
    padding[1] = right;
    padding[2] = bot;
    padding[3] = left;
    return this;
}

void TextLayout::getGLSquares(float layoutXPos, float layoutYPos, float layoutWidth, float layoutHeight,
                              std::map<std::string, IHudDrawable *> *map) {

    Layout::getGLSquares(layoutXPos, layoutYPos, layoutWidth, layoutHeight, map);

    TextObject* textDrawer = new TextObject(text,font,layoutWidth-(padding[1]+padding[3]),
                                                      layoutHeight-(padding[0]+padding[2]),
                                                      layoutXPos + padding[3],padding[0]+layoutYPos);

    map->insert(std::pair<std::string, IHudDrawable*>(textId == "" ? getNextRandId() : textId,textDrawer));
}
