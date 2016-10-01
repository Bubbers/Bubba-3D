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

#ifndef SUPER_BUBBA_AWESOME_SPACE_DIMENTION_H
#define SUPER_BUBBA_AWESOME_SPACE_DIMENTION_H

/**
 * A collection of units used to represent distance and size in
 * the user interface library
 */
class Dimension {
public:
    enum DimensionUnit{PIXELS,PERCENTAGE,FILL,WRAP,PERCENTAGE_PLUS_PIXELS};

    /**
     * Retrieves the size of this Dimension based on the
     * size of the container. only works for Dimension::DimensionUnit::PIXELS,
     * Dimension::DimensionUnit::PERCENTAGE and Dimension::DimensionUnit::PERCENTAGE_PLUS_PIXELS
     */
    float getSize(float room);

    /**
     * Create a dimension based an a percentage of the size of the container
     */
    static Dimension fromPercentage(float percentage);

    /**
     * A constant size dimension which is always \p pixels wide.
     */
    static Dimension fromPixels(int pixels);

    /**
     * Allows contstruction of a unit which is based on a percentage
     * of the size of the container plus a constant number of pixels.
     */
    static Dimension fromPercentagePlusPixels(float percentage, int pixels);

    /**
     * A unit that uses up the unused space of the container.
     * Alias for `fill(1)`
     */
    static Dimension fill();

    /**
     * A unit that uses up the weighted unused space of the container.
     * If the container contains one fill element with weight 1 and
     * one fill element with weight 2 then the first will take up
     * a third of the space and the second element will take up
     * two thirds of the space.
     *
     * Fill element always take up the space not used by elements
     * which are not fill elements.
     */
    static Dimension fill(unsigned int weight);

    /**
     * Mostly used with sizes of elements. Will be the minimum size
     * possible size without cutting any content.
     */
    static Dimension wrap();

    /**
     * Returns the unit of this dimension
     */
    DimensionUnit getUnit();

    /**
     * If this is a fill element, return the weight of this Dimension
     */
    int getWeight();

    /**
     * If this is a pixel element, return the number of pixels of this element.
     */
    int getPixels();

    /**
     * If this is a percentage element, return the percentage
     */
    float getPercentage();

    /**
     * Use the static functions to create dimensions
     */
    Dimension();

private:
    Dimension(DimensionUnit unit);
    DimensionUnit unit;
    int pixels = 0;
    int weight = 0;
    float percentage = 0.0f;

};


#endif //SUPER_BUBBA_AWESOME_SPACE_DIMENTION_H
