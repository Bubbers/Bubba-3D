//
// Created by simon on 2015-12-20.
//

#ifndef BUBBA_3D_MOUSEWARP_H
#define BUBBA_3D_MOUSEWARP_H


class MouseWarp {
private:
    bool warped;
    int x = 0,y = 0;
public:
    MouseWarp();
    MouseWarp(int x, int y);
    static MouseWarp noWarp();
    static MouseWarp warp(int x, int y);
    bool isWarped();
    int getX();
    int getY();

};


#endif //BUBBA_3D_MOUSEWARP_H
