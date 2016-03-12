//
// Created by simon on 2016-02-21.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_HUDGRAPHIC_H
#define SUPER_BUBBA_AWESOME_SPACE_HUDGRAPHIC_H

#include <Dimension.h>

using namespace chag;

class HUDGraphic {

public:

    class Color{
    public:
        Color(string* hexString);
        Color(string* hexString, float opacity);
        Color(int red, int green, int blue);
        Color(int red, int green, int blue, float opacity);
        Color(float4 rawColor);
        float4 getColor();

    private:
        float4 color;
        void hexStringToFloat(string* hexString,unsigned int pos, unsigned int len, float* target);
    };

    template <typename T>
    struct TexturePosition{
        T topLeftX,topLeftY,botRightX,botRightY;
        TexturePosition(T topLeftX, T topLeftY, T botRightX, T botRightY);
        TexturePosition(){}
    };

    HUDGraphic(Texture* texture);
    HUDGraphic(Color color);

    HUDGraphic* setCenterOffset(Dimension offsetX, Dimension offsetY);
    HUDGraphic* setTexturePosition(TexturePosition<int> texturePosition);

    TexturePosition<float> getTexturePosition(float width, float height);
    float3 getCenterOffset(float width, float height);
    Texture* getTexture();
    float4 getColor();
    bool isTextureElseColor();

protected:

    Dimension offsetX, offsetY;
    Texture* texture;
    float4 color;
    TexturePosition<int> texturePosition;
    bool textureElseColor;

private:
};


#endif //SUPER_BUBBA_AWESOME_SPACE_HUDGRAPHIC_H
