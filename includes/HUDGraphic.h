//
// Created by simon on 2016-02-21.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_HUDGRAPHIC_H
#define SUPER_BUBBA_AWESOME_SPACE_HUDGRAPHIC_H

#include <Dimension.h>

using namespace chag;

/**
 * A class used to specify the background graphics of Layout implementations.
 */
class HUDGraphic {

public:

    /**
     * Represents a color
     */
    class Color{
    public:
        /**
         * Create a color from its hex value.
         *
         * \param hexString May or may not star with '#'. The size of the string
         * excluding the '#' sign should be 3 or 6.
         */
        Color(string* hexString);
        /**
         * Create a color from its hex value and opacity.
         *
         * \param hexString May or may not star with '#'. The size of the string
         * excluding the '#' sign should be 3 or 6.
         * \param opacity A value between 0.0 and 1.0 specifying the opacity of the color
         */
        Color(string* hexString, float opacity);

        /**
         * Create a color from its color values. The values are expected to be
         * in the range 0-255 or 0x00 to 0xFF.
         */
        Color(int red, int green, int blue);

        /**
         * Create a color from its color values. The values are expected to be
         * in the range 0-255 or 0x00 to 0xFF except the opacity. The opacity
         * should be a value between 0.0 and 1.0
         */
        Color(int red, int green, int blue, float opacity);

        /**
         * Create the color from a raw vector. All values should be in the range 0.0-1.0
         */
        Color(float4 rawColor);

        /**
         * Get the calculated raw color.
         */
        float4 getColor();

    private:
        float4 color;
        void hexStringToFloat(string* hexString,unsigned int pos, unsigned int len, float* target);
    };

    template <typename T>
    struct TexturePosition{
        T botLeftX, botLeftY,topRightX,topRightY;
        bool empty = true;
        TexturePosition(T botLeftX, T botLeftY, T topRightX, T topRightY)
                : botLeftX(botLeftX), botLeftY(botLeftY), topRightX(topRightX), topRightY(topRightY), empty(false){}
        TexturePosition(){}
        bool isEmpty();
    };

    HUDGraphic(Texture* texture);
    HUDGraphic(Color color);

    HUDGraphic* setCenterOffset(Dimension offsetX, Dimension offsetY);
    HUDGraphic* setTexturePosition(TexturePosition<int> texturePosition);

    TexturePosition<float> getTexturePosition();
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
