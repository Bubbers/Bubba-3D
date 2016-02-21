//
// Created by simon on 2016-02-21.
//

#include <stdexcept>
#include "PositioningLayout.h"
#include <vector>
#include <map>

using namespace std;

void PositioningLayout::addChild(Layout *child) {
    checkChildCompatibility(child);
    addChild(child,Dimension(),Dimension());
}

void PositioningLayout::addChild(Layout *child, Dimension x, Dimension y) {
    children.push_back(new PositionedLayout(child,x,y));
}

Dimension PositioningLayout::getWidth() {
    return width;
}

Dimension PositioningLayout::getHeight() {
    return height;
}

PositioningLayout::PositioningLayout(Dimension width, Dimension height) : width(width), height(height) {
    if(width.getUnit() == Dimension::WRAP || height.getUnit() == Dimension::WRAP)
        throw new invalid_argument("The height or width of a PositioningLayout cannot be wrap.");
}

void PositioningLayout::getGLSquares(float layoutXPos, float layoutYPos, float layoutWidth, float layoutHeight,
                                     map<string,GLSquare*> *list) {

    Layout::getGLSquares(layoutXPos,layoutYPos,layoutWidth,layoutHeight,list);

    for(PositionedLayout* child : children){
        float x = layoutXPos + child->x.getSize(layoutWidth);
        float y = layoutYPos + child->y.getSize(layoutHeight);
        float width = child->child->getWidth().getSize(layoutWidth);
        float height = child->child->getHeight().getSize(layoutHeight);
        child->child->getGLSquares(x,y,width,height,list);
    }
}

void PositioningLayout::checkChildCompatibility(Layout *child) {
    if(child->getHeight().getUnit() == Dimension::FILL || child->getWidth().getUnit() == Dimension::FILL)
        throw new invalid_argument("The width or height of a child of a PositioningLayout cannot be FILL.");
}