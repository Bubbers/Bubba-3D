//
// Created by simon on 2016-02-06.
//

#include <cmath>
#include "Dimension.h"

float Dimension::getSize(int room){
    if(unit == PIXELS)
        return (float)pixels;
    if(unit == PERCENTAGE)
        return std::round(((float)room)*percentage/100.0f);
    throw "undecidable";
}
static Dimension fromPercentage(float percentage);
static Dimension fromPixels(int pixels);
static Dimension fill();
static Dimension fill(unsigned int weight);
static Dimension wrap();

DimensionUnit getUnit();

int getWeight();
int getPixels();
float getPercentage();

private:
Dimension(DimensionUnit unit);