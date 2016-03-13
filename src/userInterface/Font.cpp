//
// Created by simon on 2016-03-11.
//

#include <ft2build.h>
#include <stdexcept>
#include FT_FREETYPE_H

#include "Font.h"

Font::GlyphData::GlyphData(int offsetX, FT_GlyphSlot ft_glyphSlot) {
    this->offsetX = offsetX;
    this->advanceX = ft_glyphSlot->advance.x;
    this->advanceY = ft_glyphSlot->advance.y;
    this->bitmapHeight = ft_glyphSlot->bitmap.rows;
    this->bitmapWidth = ft_glyphSlot->bitmap.width;
    this->bitmapLeft = ft_glyphSlot->bitmap_left;
    this->bitmapTop = ft_glyphSlot->bitmap_top;
}

Font::GlyphData Font::getCharacter(unsigned char character) {
    checkInvalidCharacter(character);
    return glyphs[character-32];
}

void Font::checkInvalidCharacter(unsigned char character) {
    if(character < 32 || character >= 128)
        throw invalid_argument("The font only support characters with ascii value from 32 inclusive to 128 exclusive.");
}

void Font::addGlyph(FT_GlyphSlot glyph, int offsetX, unsigned char character) {
    checkInvalidCharacter(character);
    glyphs[character-32] = GlyphData(offsetX,glyph);
}

int Font::getPixelSize() { return pixelSize; }

Font::Font(int pixelSize) : pixelSize(pixelSize) {}
