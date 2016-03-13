//
// Created by simon on 2016-01-10.
//

#include <Globals.h>

int Globals::get(Key key) {
    return masterFunc(key,0,true);
}

void Globals::set(Key key, int value) {
    masterFunc(key,value,false);
}

int Globals::masterFunc(Key key, int value, bool getElseSet) {
    static int vals[] = {0,0,0,0};
    if(getElseSet){
        return vals[key];
    } else {
        vals[key] = value;
        return 0;
    }
}