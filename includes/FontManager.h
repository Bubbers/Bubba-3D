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

/**
 * Used to load and perform initialization on fonts
 */
class FontManager {

public:

    /**
     * If the font has been loaded before it is just returned. Otherwise
     * the font is loaded and the characters are cached to the graphics card.
     *
     * \warning Never dereference the returned font or copy it as the font
     * may change when other fonts are added.
     */
    Font* loadAndFetchFont(string fontFace, int pixelSize);

    /**
     * Returns the texture id used to store the font glyphs in.
     *
     * \warning The texture id might change when new fonts are loaded.
     *
     * \throws logic_error If the method is called before any fonts have been loaded.
     */
    GLuint* getTex() ;

    /**
     * Returns the singleton instance of the FontManager
     */
    static FontManager* getInstance();

protected:
    /**
     * Loads a font and caches it to the graphics card
     */
    virtual void loadFont(string fontFace, int pixelSize);

private:
    FontManager();

    /**
     * An identifier for a font. Contains the file path to the font-face and
     * a font size.
     */
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

    /**
     * Hash function for FontDefinition
     */
    struct FontDefHash
    {
        std::size_t operator()(FontDefinition const& fd) const
        {
            std::size_t h1 = std::hash<std::string>()(fd.getFace());
            std::size_t h2 = std::hash<int>()(fd.getPixelSize());
            return h1 ^ (h2 << 1); // or use boost::hash_combine
        }
    };

    /**
     * An unordered map mapping a font definition to a font
     */
    typedef unordered_map<FontDefinition,Font*,FontDefHash> fontMap;

    void iterateGlyphs(FontDefinition def, unsigned int* width, unsigned int* height);
    void drawGlyphs();
    void initTexture();
    GLuint* getTex(bool force) ;

    fontMap loadedFonts;
    FT_Library* ft_library;
    unsigned int atlasWidth = 0, atlasHeight = 0;
    bool initiated = false;

};


#endif //SUPER_BUBBA_AWESOME_SPACE_FONTMANAGER_H
