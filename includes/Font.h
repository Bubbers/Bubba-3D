//
// Created by simon on 2016-03-11.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_FONT_H
#define SUPER_BUBBA_AWESOME_SPACE_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;

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

    GlyphData getCharacter(char character);
    Font();
    void addGlyph(FT_GlyphSlot glyph, int offsetX, char character);

private:
    void checkInvalidCharacter(char character);
    GlyphData glyphs[128-32];

};


#endif //SUPER_BUBBA_AWESOME_SPACE_FONT_H
