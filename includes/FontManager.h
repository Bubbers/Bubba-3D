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

#pragma once

#include <unordered_map>
#include <GL/glew.h>
#include <functional>

#include <ft2build.h>
#include FT_FREETYPE_H


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
    Font* loadAndFetchFont(std::string fontFace, int pixelSize);

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
    virtual void loadFont(std::string fontFace, int pixelSize);

private:
    FontManager();
    ~FontManager();

    /**
     * An identifier for a font. Contains the file path to the font-face and
     * a font size.
     */
    struct FontDefinition{
        std::string face;
        int pixelSize;
        FontDefinition(std::string face, int pixelSize);
        FontDefinition(){}
        bool operator == (FontDefinition fd) const;
        size_t hash();
        std::string getFace() const;
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
    typedef std::unordered_map<FontDefinition,Font*,FontDefHash> fontMap;

    void iterateGlyphs(FontDefinition def, unsigned int* width, unsigned int* height);
    void drawGlyphs();
    void initTexture();
    GLuint* getTex(bool force) ;

    fontMap loadedFonts;
    FT_Library* ft_library;
    unsigned int atlasWidth = 0, atlasHeight = 0;
    bool initiated = false;

};
