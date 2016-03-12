//
// Created by simon on 2016-02-26.
//

#include <TextLayout.h>
#include <IHudDrawable.h>
#include <TextObject.h>

TextLayout::TextLayout(string text, Font* font, Dimension width, Dimension height) {
    this->text = text;
    this->font = font;
    this->width = width;
    this->height = height;
}

Dimension TextLayout::getWidth() {
    return width;
}

Dimension TextLayout::getHeight() {
    return height;
}

void TextLayout::setText(string text) {
    this->text = text;
}

void TextLayout::getGLSquares(float layoutXPos, float layoutYPos, float layoutWidth, float layoutHeight,
                              map<string, IHudDrawable *> *list) {

    Layout::getGLSquares(layoutXPos,layoutYPos,layoutWidth,layoutHeight,list);

    float w = width.getUnit() == Dimension::FILL ? layoutWidth : width.getSize(layoutWidth);
    float h = height.getUnit() == Dimension::FILL ? layoutHeight : height.getSize(layoutHeight);
    TextObject* textDrawer = new TextObject(text,font,w,h, layoutXPos,layoutYPos);
    list->insert(pair<string,IHudDrawable*>(getNextRandId(),textDrawer));

}