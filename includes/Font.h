//
// Created by simon on 2016-03-11.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_FONT_H
#define SUPER_BUBBA_AWESOME_SPACE_FONT_H


struct FT_GlyphSlotRec;

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
        int offsetY;
        GlyphData(int offsetX,int offsetY, FT_GlyphSlotRec* ft_glyphSlot);
    };

    GlyphData getCharacter(char character);
    Font(GlyphData glyphs[128-32]);

private:
    GlyphData glyphs[128-32];

};


#endif //SUPER_BUBBA_AWESOME_SPACE_FONT_H
