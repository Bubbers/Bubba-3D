//
// Created by simon on 2016-02-06.
//

#include <GLSquare.h>
#include <map>
#include <IHudDrawable.h>
#include "Layout.h"

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
    if(graphic != nullptr) {
        string idd = id == "" ? getNextRandId() : id;
        list->insert(list->end(),pair<string,GLSquare*>(idd,new GLSquare(layoutXPos, layoutYPos, layoutWidth, layoutHeight, graphic)));
    }
}

Layout* Layout::setId(string id) {
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