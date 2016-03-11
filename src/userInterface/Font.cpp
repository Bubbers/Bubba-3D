//
// Created by simon on 2016-03-11.
//

#include <ft2build.h>
#include <stdexcept>
#include FT_FREETYPE_H

#include "Font.h"

Font::GlyphData::GlyphData(int offsetX, int offsetY, FT_GlyphSlotRec *ft_glyphSlot) {
    this->offsetX = offsetX;
    this->offsetY = offsetY;
    this->advanceX = ft_glyphSlot->advance.x;
    this->advanceY = ft_glyphSlot->advance.y;
    this->bitmapHeight = ft_glyphSlot->bitmap.rows;
    this->bitmapWidth = ft_glyphSlot->bitmap.width;
    this->bitmapLeft = ft_glyphSlot->bitmap_left;
    this->bitmapTop = ft_glyphSlot->bitmap_top;
}

GlyphData Font::getCharacter(char character) {
    int ch = character;
    if(ch < 32 || ch >= 128)
        throw invalid_argument("The font only support characters with ascii value from 32 inclusive to 128 exclusive.");
    return glyphs[character-32];
}

Font::Font(GlyphData glyphs[128-32]) : glyphs(glyphs){}
