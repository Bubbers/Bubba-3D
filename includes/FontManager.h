//
// Created by simon on 2016-03-11.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_FONTMANAGER_H
#define SUPER_BUBBA_AWESOME_SPACE_FONTMANAGER_H

#include <unordered_map>

using namespace std;

class FT_GlyphSlot;

class FontManager {

public:
    Font* loadAndFetchFont(string fontFace, int pixelSize);
    FontManager();

protected:
    virtual void loadFont(string fontFace, int pixelSize);

private:
    struct FontDefinition{
        string face;
        int pixelSize;
        FontDefinition(string face, int pixelSize);
        bool operator == (FontDefinition fd);
        size_t hash();
    };

    void initTexture(unsigned int width, unsigned int height);

    typedef unordered_map<FontDefinition,Font*,[](FontDefinition fd) -> {return fd.hash();}> fontMap;

    vector<FontDefinition>* getPreviouslyLoadedDefinitionsAnd(FontDefinition fd);
    void iterateGlyphs(vector<FontDefinition>* defs, function<void (FT_GlyphSlot)> forEachGlyph, function<void ()> betweenFonts);
    vector<vector<FT_GlyphSlot>>* getBitmaps(vector<FontDefinition>* defs);
    fontMap loadedFonts;
    FT_Library* ft_library;
};


#endif //SUPER_BUBBA_AWESOME_SPACE_FONTMANAGER_H
