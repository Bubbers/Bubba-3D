//
// Created by simon on 2016-02-06.
//

#include "Dimension.h"

float Dimension::getSize(float room){
    if(unit == PIXELS)
        return (float)pixels;
    if(unit == PERCENTAGE)
        return room*percentage/100.0f;
    if(unit == PERCENTAGE_PLUS_PIXELS)
        return room*percentage/100.0f + (float)pixels;
    throw "undecidable";
}

Dimension Dimension::fromPercentage(float percentage){
    Dimension dim = Dimension(PERCENTAGE);
    dim.percentage = percentage;
    return dim;
}

Dimension Dimension::fromPixels(int pixels){
    Dimension dim = Dimension(PIXELS);
    dim.pixels = pixels;
    return dim;
}

Dimension Dimension::fromPercentagePlusPixels(float percentage, int pixels) {
    Dimension dim = Dimension(PERCENTAGE_PLUS_PIXELS);
    dim.percentage = percentage;
    dim.pixels = pixels;
    return dim;
}

Dimension Dimension::fill(){
    return fill(1);
}

Dimension Dimension::fill(unsigned int weight){
    Dimension dim = Dimension(FILL);
    dim.weight = weight;
    return dim;
}

Dimension Dimension::wrap(){
    return Dimension(WRAP);
}

Dimension::DimensionUnit Dimension::getUnit() {
    return unit;
}

int Dimension::getWeight() {
    return weight;
}

int Dimension::getPixels(){
    return pixels;
}

float Dimension::getPercentage(){
    return percentage;
}

Dimension::Dimension(DimensionUnit unit) {
    this->unit = unit;
}

Dimension::Dimension() : Dimension(PIXELS) {
    pixels = 0;
}