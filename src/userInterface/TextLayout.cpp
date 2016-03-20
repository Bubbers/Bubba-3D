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

TextLayout* TextLayout::setTextId(string id) {
    this->textId = id;
    return this;
}

void TextLayout::getGLSquares(float layoutXPos, float layoutYPos, float layoutWidth, float layoutHeight,
                              map<string, IHudDrawable *> *list) {

    Layout::getGLSquares(layoutXPos,layoutYPos,layoutWidth,layoutHeight,list);

    TextObject* textDrawer = new TextObject(text,font,layoutWidth,layoutHeight, layoutXPos,layoutYPos);
    list->insert(pair<string,IHudDrawable*>(textId == "" ? getNextRandId() : textId,textDrawer));

}