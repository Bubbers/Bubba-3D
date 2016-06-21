//
// Created by simon on 2016-02-06.
//

#include <GLSquare.h>
#include <map>
#include <HUDGraphic.h>
#include <iostream>
#include "Layout.h"
#include "InsideRoundedSquareChecker.h"

using namespace std;

void Layout::addChild(Layout* child){
    children.push_back(child);
}

map<string,IHudDrawable*> Layout::getGLSquares(float layoutXPos, float layoutYPos, float layoutWidth,
                                       float layoutHeight) {
    map<string,IHudDrawable*> list;
    getGLSquares(layoutXPos,layoutYPos,layoutWidth,layoutHeight,&list);
    return list;
}

void Layout::getGLSquares(float layoutXPos, float layoutYPos, float layoutWidth, float layoutHeight,
                          map<string,IHudDrawable*> *list) {

    id = id == "" ? getNextRandId() : id;
    if(graphic != nullptr) {
        GLSquare* renderedSquare = new GLSquare(layoutXPos, layoutYPos, layoutWidth, layoutHeight, graphic);
        list->insert(list->end(),pair<string,GLSquare*>(id,renderedSquare));
        renderedBackground = renderedSquare;
        int* roundedBorders = graphic->getRoundedCorners();
        insideChecker = new InsideRoundedSquareChecker(layoutXPos,layoutYPos,layoutWidth,layoutHeight,roundedBorders[0],roundedBorders[1],roundedBorders[2],roundedBorders[3]);
    }else {
        renderedBackground = nullptr;
        insideChecker = new InsideSquareChecker(layoutXPos, layoutYPos, layoutWidth, layoutHeight);
    }
}

void Layout::invokeListeners(int x, int y, ListenerType listenerType) {

    invokeListenersInternal(x,y,listenerType,true);

}

void Layout::invokeListenersInternal(int x, int y, ListenerType listenerType, bool mayBeHit) {

    bool *wasActive = listenerType == Layout::ON_CLICK_LISTENER ? &mouseWasDown : &hoveredBackground;
    if(insideChecker != nullptr && mayBeHit && insideChecker->isInside(x, y)){
        if(!*wasActive)
            callListeners(x,y,listenerType,true);
        *wasActive = true;
        for (Layout *child : children) {
            child->invokeListenersInternal(x, y, listenerType, true);
        }
    }else if(*wasActive){
        if(listenerType == Layout::ON_HOVER_LISTENER || insideChecker->isInside(x,y))
            callListeners(x,y,listenerType,false);
        *wasActive = false;
        for (Layout *child : children) {
            child->invokeListenersInternal(x, y, listenerType, false);
        }

    }


}

Layout* Layout::setLayoutId(string id) {
    this->id = id;
    return this;
}

Layout* Layout::findById(string id) {
    if(this->id == id)
        return this;
    for(auto it : children){
        Layout* res = it->findById(id);
        if(res != nullptr)
            return res;
    }
    return nullptr;
}

Layout* Layout::setBackground(HUDGraphic *graphic) {
    this->graphic = graphic;
    if(renderedBackground != nullptr){
        renderedBackground->setGraphic(graphic);
    }
    return this;
}

string Layout::getNextRandId() {
    static string prev = "ganwubcky";
    char lastChar = prev.at(prev.length()-1);
    if(lastChar < 'z') {
        prev.pop_back();
        prev.push_back(lastChar+1);
    }else
        prev.push_back('a');
    return prev;
}

void Layout::clearClickListeners() {
    clickListeners->clear();
}

void Layout::clearHoverListeners() {
    hoverListeners->clear();
}

Layout* Layout::addClickListener(EventFunction eventFunction) {
    clickListeners->insert(clickListeners->end(),eventFunction);
    return this;
}

Layout* Layout::addHoverListener(EventFunction eventFunction) {
    hoverListeners->insert(hoverListeners->end(),eventFunction);
    return this;
}

void Layout::callListeners(int x, int y, ListenerType listenerType, bool enteringElseLeaving) {
    vector<EventFunction>* toCall;
    switch (listenerType){
        case ON_HOVER_LISTENER:
            toCall = hoverListeners;
            break;
        case ON_CLICK_LISTENER:
            toCall = clickListeners;
            break;
    }
    for (EventFunction call : *toCall) {
        call(x, y, this,enteringElseLeaving);
    }
}

HUDGraphic* Layout::getGraphic() {
    if(renderedBackground == nullptr)
        return graphic;
    else
        return renderedBackground->getGraphic();
}

void Layout::updateGraphic() {
    if(renderedBackground != nullptr)
        renderedBackground->updateGraphic();
}
