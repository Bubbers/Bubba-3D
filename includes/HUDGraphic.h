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
#pragma once

#include <Dimension.h>
#include <linmath/float2.h>
#include <linmath/float3.h>
#include <linmath/float4.h>

class Texture;

/**
 * A class used to specify the background graphics of Layout implementations.
 */
class HUDGraphic {

public:

    /**
     * Represents a color
     */
    class Color {
    public:
        /**
         * Create a color from its hex value.
         *
         * \param hexString May or may not star with '#'. The size of the string
         * excluding the '#' sign should be 3 or 6.
         */
        Color(std::string hexString);
        /**
         * Create a color from its hex value and opacity.
         *
         * \param hexString May or may not star with '#'. The size of the string
         * excluding the '#' sign should be 3 or 6.
         * \param opacity A value between 0.0 and 1.0 specifying the opacity of the color
         */
        Color(std::string hexString, float opacity);

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
        Color(chag::float4 rawColor);

        /**
         * Get the calculated raw color.
         */
        chag::float4 getColor();

    private:
        chag::float4 color;
        void hexStringToFloat(std::string hexString,unsigned int pos, unsigned int len, float* target);
    };

    template <typename T>
    struct TexturePosition{
        T botLeftX, botLeftY,topRightX,topRightY;
        bool empty = true;
        TexturePosition(T botLeftX, T botLeftY, T topRightX, T topRightY)
                : botLeftX(botLeftX), botLeftY(botLeftY), topRightX(topRightX), topRightY(topRightY), empty(false) {}
        TexturePosition() {}
        bool isEmpty();
    };

    /**
     * Creates a graphic that has a texture as a background. If
     * a portion of the texture is required, use HUDGraphic::setTexturePosition()
     */
    HUDGraphic(Texture* texture);

    /**
     * Creates a graphic with a solid color as a background.
     */
    HUDGraphic(Color color);

    /**
     * Set the offset from the actual center that the rotational center should be.
     */
    HUDGraphic* setCenterOffset(Dimension offsetX, Dimension offsetY);

    /**
     * Sets the portion of the texture that should be used.
     */
    HUDGraphic* setTexturePosition(TexturePosition<int> texturePosition);

    /**
     * \internal
     */
    TexturePosition<float> getTexturePosition();

    /**
     * \internal
     */
    chag::float3 getCenterOffset(float width, float height);

    /**
     * Returns the texture of this graphic, or throws a runtime_error
     * if it only contains a color.
     */
    Texture* getTexture();

    /**
     * Returns the color of this graphic, or throws a runtime_error
     * if it only contains a texture.
     */
    chag::float4 getColor();

    /**
     * Returns true if this contains a texture
     */
    bool isTextureElseColor();

    /**
     * Returns an array of size four. The sizes of the rounded corners
     * are int he following order: top left, top right, bot right, bot left.
     */
    int* getRoundedCorners();

    /**
     * Returns an array of size four. The sizes of the borders are in
     * the following order: top, right, bot, left
     */
    int* getBorders();

    /**
     * Returns the raw color of the border in RGBA
     */
    chag::float4 getBorderColor();

    HUDGraphic* setRoundedCorners(int pixels);
    HUDGraphic* setRoundedCorners(int topLeft, int topRight, int botRight, int botLeft);
    HUDGraphic* setBorder(int pixels, Color borderColor);
    HUDGraphic* setBorder(int botAndTop, int leftAndRight, Color borderColor);
    HUDGraphic* setBorder(int top, int right, int bot, int left, Color borderColor);
    HUDGraphic* setBorderSize(int top, int right, int bot, int left);
    HUDGraphic* setBorderColor(Color borderColor);
    HUDGraphic* setBackground(Color color);
    HUDGraphic* setBackground(Texture* texture);

protected:

    Dimension offsetX, offsetY;
    Texture* texture = nullptr;
    chag::float4 color;
    TexturePosition<int> texturePosition;
    bool textureElseColor;
    int roundedCorners[4] = {0, 0, 0, 0};
    int borders[4] = {0, 0, 0, 0};
    chag::float4 borderColor;
};
