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
// Created by simon on 2016-03-11.
//

#include <ft2build.h>
#include <stdexcept>
#include FT_FREETYPE_H

#include "Font.h"

Font::GlyphData::GlyphData(int offsetX, FT_GlyphSlot ft_glyphSlot):
    advanceX(ft_glyphSlot->advance.x),
    advanceY(ft_glyphSlot->advance.y),
    bitmapWidth(ft_glyphSlot->bitmap.width),
    bitmapHeight(ft_glyphSlot->bitmap.rows),
    bitmapLeft(ft_glyphSlot->bitmap_left),
    bitmapTop(ft_glyphSlot->bitmap_top),
    offsetX(offsetX)
{
}

Font::GlyphData Font::getCharacter(unsigned char character) {
    checkInvalidCharacter(character);
    return glyphs[character-32];
}

void Font::checkInvalidCharacter(unsigned char character) {
    if(character < 32 || character >= 128) {
        throw std::invalid_argument("The font only support characters with"
                               " ascii value from 32 inclusive to 128 exclusive.");
    }
}

void Font::addGlyph(FT_GlyphSlot glyph, int offsetX, unsigned char character) {
    checkInvalidCharacter(character);
    glyphs[character-32] = GlyphData(offsetX,glyph);
}

int Font::getPixelSize() {
    return pixelSize;
}

Font::Font(int pixelSize) : pixelSize(pixelSize) {
}
