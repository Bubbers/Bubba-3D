//
// Created by simon on 2016-01-09.
//

#ifndef BUBBA_3D_GLOBALS_H
#define BUBBA_3D_GLOBALS_H

class Globals{
public:
    enum Key { WINDOW_HEIGHT=0, WINDOW_WIDTH=1,MOUSE_WINDOW_X=2,MOUSE_WINDOW_Y=3};
    static void set(Key key, int value);
    static int get(Key key);

private:
    static int masterFunc(Key key, int value, bool getElseSet);

};

#endif //BUBBA_3D_GLOBALS_H
