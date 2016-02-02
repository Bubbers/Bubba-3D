//
// Created by simon on 2016-02-02.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_VIEW_H
#define SUPER_BUBBA_AWESOME_SPACE_VIEW_H

using namespace std;

class View {

public:
    int getWidth();
    int getHeight();
    int getXPosition();
    int getYPosition();
    UIImage* getImage();
    int getNumVertices();
    vector<float> getVertices();

    View(int width, int height, int xPosition, int yPosition, UIImage* image);

private:
    int width;
    int height;
    int xPosition;
    int yPosition;
    UIImage* image;
    void getVertices(vector<float>* list);

};


#endif //SUPER_BUBBA_AWESOME_SPACE_VIEW_H
