//
// Created by simon on 2016-02-14.
//

#include <ListLayout.h>
#include <Dimension.h>
#include <GLSquare.h>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>

using namespace std;

ListLayout::ListLayout(Orientation orientation, Dimension width, Dimension height) : width(width), height(height){
    this->orientation = orientation;
}

void ListLayout::getGLSquares(float layoutXPos,float layoutYPos, float layoutWidth,
                                       float layoutHeight, map<string,GLSquare*>* list){

    Layout::getGLSquares(layoutXPos,layoutYPos,layoutWidth,layoutHeight,list);

    int weights = 0;
    float unitsLeft = orientation == HORIZONTAL ? layoutWidth : layoutHeight;
    for(Layout* child : children) {
        Dimension dimension = orientation == HORIZONTAL ? child->getWidth() : child->getHeight();
        if (dimension.getUnit() == Dimension::FILL)
            weights += dimension.getWeight();
        else if(dimension.getUnit() == Dimension::PERCENTAGE || dimension.getUnit() == Dimension::PIXELS)
            unitsLeft -= dimension.getSize(orientation == HORIZONTAL ? layoutWidth : layoutHeight);
    }

    float xpos = layoutXPos, ypos = layoutYPos, width,height, calculatedPos;
    for(Layout* child : children){
        Dimension dimension = orientation == HORIZONTAL ? child->getWidth() : child->getHeight();
        if (dimension.getUnit() == Dimension::FILL)
            calculatedPos = unitsLeft/(float)weights*(float)dimension.getWeight();
        else
            calculatedPos = dimension.getSize(orientation == HORIZONTAL ? layoutWidth : layoutHeight);

        if(orientation == HORIZONTAL){
            width = calculatedPos;
            Dimension heightDim = child->getHeight();
            height = heightDim.getUnit() == Dimension::FILL ? layoutHeight : heightDim.getSize(layoutHeight);
        }else{
            Dimension widthDim = child->getWidth();
            width = widthDim.getUnit() == Dimension::FILL ? layoutWidth : widthDim.getSize(layoutHeight);
            height = calculatedPos;
        }
        child->getGLSquares(xpos,ypos,width,height,list);
        if(orientation == HORIZONTAL){
            xpos += width;
        }else{
            ypos += height;
        }
    }
}

Dimension ListLayout::getWidth() {
    if(width.getUnit() != Dimension::WRAP)
        return width;
    else
        return Dimension::fromPixels(wrapSize(HORIZONTAL));
}

Dimension ListLayout::getHeight() {
    if(height.getUnit() != Dimension::WRAP)
        return height;
    else
        return Dimension::fromPixels(wrapSize(VERTICAL));
}

unsigned int ListLayout::wrapSize(Orientation parentOrientation) {
    unsigned int wrapSize = 0;
    for(Layout* child : children){
        Dimension childDim = parentOrientation == HORIZONTAL ? child->getWidth() : child->getHeight();
        // We know that the child dimension cannot be percentage or fill (checkChildCompatibility)
        // and as getWidth() and getHeight() can never return a wrapping dimension, it has to be pixels
        if(parentOrientation == orientation)
            wrapSize += childDim.getPixels();
        else
            wrapSize = max(wrapSize,(unsigned int)childDim.getPixels());
    }
    return wrapSize;
}

void ListLayout::addChild(Layout *child) {
    checkChildCompatibility(child);
    Layout::addChild(child);
}

void ListLayout::checkChildCompatibility(Layout *child) {
    Dimension thisDim = orientation == HORIZONTAL ? width : height;
    Dimension childDim = orientation == HORIZONTAL ? child->getWidth() : child->getHeight();
    if(thisDim.getUnit() == Dimension::WRAP &&
       !(childDim.getUnit() == Dimension::WRAP || childDim.getUnit() == Dimension::PIXELS)){
        string err = string("The child of a wrapping layout can only use Dimension::WRAP or Dimension::PIXELS ") +
                     "in the same orientation as the parent as these aren't dependent on the parent size.";
        throw invalid_argument(err.c_str());
    }

}