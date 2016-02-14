//
// Created by simon on 2016-02-06.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_LAYOUT_H
#define SUPER_BUBBA_AWESOME_SPACE_LAYOUT_H

using namespace std;

class Layout {

    enum Orientation {HORIZONTAL,VERTICAL};
    enum Position {LOWER,MIDDLE,HIGHER};

    virtual void addChild(Layout* child);
    virtual vector<GLSquare*> getGLSquares(unsigned int layoutXPos,unsigned int layoutYPos, unsigned int layoutWidth,
                                   unsigned int layoutHeight,unsigned int windowWidth, unsigned int windowHeight);

    virtual Layout(Orientation orientation, Position position, Dimension width, Dimension height);
    virtual ~Layout();

private:
    void checkChildCompatibility(Layout* child);
    int getTotalFillWeight();
    int wrapSize(Orientation parentOrientation);
    Orientation orientation;
    Position position;
    Dimension width,height;
    vector<Layout*> children;

};


#endif //SUPER_BUBBA_AWESOME_SPACE_LAYOUT_H
