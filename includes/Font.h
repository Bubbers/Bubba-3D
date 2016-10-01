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
#include <ft2build.h>
#include FT_FREETYPE_H


/**
 * A class that contains data about the characters of a font.
 * Load a font using FontManager. TextObject can render text
 * using a Font.
 */
class Font {
public:

    struct GlyphData{
        int advanceX; // advance.x
        int advanceY; // advance.y

        int bitmapWidth; // bitmap.width;
        int bitmapHeight; // bitmap.rows;

        int bitmapLeft; // bitmap_left;
        int bitmapTop; // bitmap_top;

        int offsetX; // x offset of glyph in texture coordinates
        GlyphData(int offsetX, FT_GlyphSlot ft_glyphSlot);
        GlyphData(){}
    };

    GlyphData getCharacter(unsigned char character);
    Font(int pixelSize);
    void addGlyph(FT_GlyphSlot glyph, int offsetX, unsigned char character);

    int getPixelSize();

private:
    void checkInvalidCharacter(unsigned char character);
    GlyphData glyphs[128-32];
    int pixelSize = 0;

};
