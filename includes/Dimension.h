//
// Created by simon on 2016-02-06.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_DIMENTION_H
#define SUPER_BUBBA_AWESOME_SPACE_DIMENTION_H

class Dimension {
public:
    enum DimensionUnit{PIXELS,PERCENTAGE,FILL,WRAP,PERCENTAGE_PLUS_PIXELS};

    float getSize(float room);
    static Dimension fromPercentage(float percentage);
    static Dimension fromPixels(int pixels);
    static Dimension fromPercentagePlusPixels(float percentage, int pixels);
    static Dimension fill();
    static Dimension fill(unsigned int weight);
    static Dimension wrap();

    DimensionUnit getUnit();

    int getWeight();
    int getPixels();
    float getPercentage();
    Dimension();

private:
    Dimension(DimensionUnit unit);
    DimensionUnit unit;
    int pixels = 0;
    int weight = 0;
    float percentage = 0.0f;

};


#endif //SUPER_BUBBA_AWESOME_SPACE_DIMENTION_H
