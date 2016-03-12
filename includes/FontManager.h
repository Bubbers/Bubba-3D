//
// Created by simon on 2016-03-11.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_FONTMANAGER_H
#define SUPER_BUBBA_AWESOME_SPACE_FONTMANAGER_H

#include <unordered_map>
#include <GL/glew.h>
#include <functional>

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;

class Font;

class FontManager {

public:
    Font* loadAndFetchFont(string fontFace, int pixelSize);
    FontManager();
    static GLuint* getTex();

protected:
    virtual void loadFont(string fontFace, int pixelSize);

private:

    struct FontDefinition{
        string face;
        int pixelSize;
        FontDefinition(string face, int pixelSize);
        FontDefinition(){}
        bool operator == (FontDefinition fd) const;
        size_t hash();
        string getFace() const;
        int getPixelSize() const;
    };

    struct FontDefHash
    {
        std::size_t operator()(FontDefinition const& fd) const
        {
            std::size_t h1 = std::hash<std::string>()(fd.getFace());
            std::size_t h2 = std::hash<int>()(fd.getPixelSize());
            return h1 ^ (h2 << 1); // or use boost::hash_combine
        }
    };

    void initTexture(unsigned int width, unsigned int height);

    typedef unordered_map<FontDefinition,Font*,FontDefHash> fontMap;

    vector<FontDefinition>* getPreviouslyLoadedDefinitionsAnd(FontDefinition fd);
    void iterateGlyphs(vector<FontDefinition>* defs, unsigned int* width, unsigned int* height);
    void drawGlyphs(vector<FontDefinition> *defs);
    fontMap loadedFonts;
    FT_Library* ft_library;

};


#endif //SUPER_BUBBA_AWESOME_SPACE_FONTMANAGER_H
