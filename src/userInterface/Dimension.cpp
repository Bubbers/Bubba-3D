/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */
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