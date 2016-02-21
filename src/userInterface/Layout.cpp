//
// Created by simon on 2016-02-06.
//

#include <GLSquare.h>
#include <map>
#include "Layout.h"

using namespace std;

void Layout::addChild(Layout* child){
    children.push_back(child);
}

map<string,GLSquare*> Layout::getGLSquares(float layoutXPos, float layoutYPos, float layoutWidth,
                                       float layoutHeight) {
    map<string,GLSquare*> list;
    getGLSquares(layoutXPos,layoutYPos,layoutWidth,layoutHeight,&list);
    return list;
}

void Layout::getGLSquares(float layoutXPos, float layoutYPos, float layoutWidth, float layoutHeight,
                          map<string,GLSquare*> *list) {
    if(graphic != nullptr) {
        string idd = id == "" ? getNextRandId() : id;
        list->insert(list->end(),pair<string,GLSquare*>(idd,new GLSquare(layoutXPos, layoutYPos, layoutWidth, layoutHeight, graphic)));
    }
}

void Layout::setId(string id) {
    this->id = id;
}

void Layout::setBackground(HUDGraphic *graphic) {
    this->graphic = graphic;
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