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
                              map<string, IHudDrawable *> *list) {

    Layout::getGLSquares(layoutXPos,layoutYPos,layoutWidth,layoutHeight,list);

    TextObject* textDrawer = new TextObject(text,font,layoutWidth-(padding[1]+padding[3]),
                                                      layoutHeight-(padding[0]+padding[2]),
                                                      layoutXPos + padding[3],padding[0]+layoutYPos);
    list->insert(pair<string,IHudDrawable*>(textId == "" ? getNextRandId() : textId,textDrawer));

}
