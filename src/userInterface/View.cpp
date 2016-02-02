//
// Created by simon on 2016-02-02.
//

#include <UIImage.h>
#include "View.h"

int View::getWidth(){ return width; }
int View::getHeight(){ return height; }
int View::getXPosition(){ return xPosition; }
int View::getYPosition(){ return yPosition; }
UIImage* View::getImage(){ return image; }

View::View(int width, int height, int xPosition, int yPosition, UIImage* image){
    this->width = width;
    this->height = height;
    this->xPosition = xPosition;
    this->yPosition = yPosition;
    this->image = image;
}

int View::getNumVertices() {
    if(image != nullptr)
        return 6;
    else
        return 0;
}

vector<float> View::getVertices() {

}