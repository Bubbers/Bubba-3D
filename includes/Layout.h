//
// Created by simon on 2016-02-06.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_LAYOUT_H
#define SUPER_BUBBA_AWESOME_SPACE_LAYOUT_H

#include <vector>
#include <map>
#include <Dimension.h>

using namespace std;

class GLSquare;
class Texture;
class HUDGraphic;

class Layout {

public:

    virtual void addChild(Layout* child);
    virtual map<string,GLSquare*> getGLSquares(float layoutXPos,float layoutYPos, float layoutWidth,
                                           float layoutHeight);

    /**
     * \warn Never allowed to return a wrapping dimension
     */
    virtual Dimension getWidth() = 0;
    /**
     * \warn Never allowed to return a wrapping dimension
     */
    virtual Dimension getHeight() = 0;

    virtual ~Layout(){}
    virtual void getGLSquares(float layoutXPos,float layoutYPos, float layoutWidth,
                              float layoutHeight, map<string,GLSquare*>* list) = 0;

    virtual void setBackground(HUDGraphic* graphic);

    virtual void setId(string id);

protected:
    vector<Layout*> children;
    HUDGraphic* graphic = nullptr;
    string id = "";

    string getNextRandId();

};

#endif //SUPER_BUBBA_AWESOME_SPACE_LAYOUT_H
