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
//
// Created by simon on 2016-02-21.
//

#include <stdexcept>
#include "PositioningLayout.h"
#include <vector>
#include <map>
#include <IHudDrawable.h>
#include <iostream>
#include "InsideChecker.h"

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
                                     map<string,IHudDrawable*> *list) {

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

Layout* PositioningLayout::findById(string id) {
    if(this->id == id)
        return this;
    for(auto it : children){
        Layout* res = it->child->findById(id);
        if(res != nullptr)
            return res;
    }
    return nullptr;
}

void PositioningLayout::invokeListenersInternal(int x, int y, ListenerType listenerType, bool mayBeHit) {
    bool *wasActive = listenerType == Layout::ON_CLICK_LISTENER ? &mouseWasDown : &hoveredBackground;
    if(insideChecker != nullptr && mayBeHit && insideChecker->isInside(x, y)){
        if(!*wasActive)
            callListeners(x,y,listenerType,true);
        *wasActive = true;
        for (PositionedLayout *child : children) {
            child->child->invokeListenersInternal(x, y, listenerType, true);
        }
    }else if(*wasActive){
        if(listenerType == Layout::ON_HOVER_LISTENER || insideChecker->isInside(x,y))
            callListeners(x,y,listenerType,false);
        *wasActive = false;
        for (PositionedLayout *child : children) {
            child->child->invokeListenersInternal(x, y, listenerType, false);
        }

    }
}