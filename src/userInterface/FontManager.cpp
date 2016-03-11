//
// Created by simon on 2016-03-11.
//

#include <string>
#include <vector>
#include <cmath>

#include <ft2build.h>
#include <FontManager.h>
#include <stdexcept>
#include <Logger.h>
#include <GL/glew.h>
#include <functional>
#include FT_FREETYPE_H


using namespace std;

size_t FontManager::FontDefinition::hash() {
    return std::hash<string>()(face) + pixelSize;
}

bool FontManager::FontDefinition::operator== (FontDefinition fd) {
    return fd.face == face && fd.pixelSize == pixelSize;
}

FontManager::FontDefinition::FontDefinition(string face, int pixelSize) : face(face), pixelSize(pixelSize){ }

void FontManager::loadFont(string fontFace, int pixelSize) {

    vector<FontDefinition>* fontDefs = getPreviouslyLoadedDefinitionsAnd(FontDefinition(fontFace,pixelSize));
    int width = 0, maxHeight = 0;

    iterateGlyphs(fontDefs,
                  [width,maxHeight](FT_GlyphSlot glyph) -> {
                      maxHeight = max(glyph->bitmap.rows,maxHeight);
                      width += glyph->bitmap.width;
                  },
                  []() -> {}
    );

    initTexture(maxHeight,width);

    int x = 0;
    fontMap fonts;
    iterateGlyphs(fontDefs,
                  [](FT_GlyphSlot glyph) -> {
                      glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, glyph->bitmap.width, glyph->bitmap.rows,
                                      GL_ALPHA, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
                      x += glyph->bitmap.width;
                  },
                  []() -> {

                  });

}

void FontManager::initTexture(unsigned int width, unsigned int height) {
    GLuint tex;
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
}

void FontManager::iterateGlyphs(vector<FontDefinition> *defs, function<void (FT_GlyphSlot)> forEachGlyph,
                                function<void ()> betweenFonts) {

    FT_Face face;
    for(int i = 0; i< defs->size(); i++){

        FT_New_Face(*ft_library,(*defs)[i].face.c_str(),0,&face);
        FT_GlyphSlot glyph = face->glyph;

        for(int c = 32; c < 128; c++){

            if(int error = FT_Load_Char(face,c,FT_LOAD_RENDER)){
                Logger::logError("Failed loading char '" + to_string(c) + "'. Error code: " + to_string(error));
                continue;
            }

            forEachGlyph(glyph);

        }

        betweenFonts();

    }


}

vector<FontDefinition>* FontManager::getPreviouslyLoadedDefinitionsAnd(FontDefinition fd) {
    vector<FontDefinition>* list = new vector<FontDefinition>(loadedFonts.size()+1);
    for(auto it : loadedFonts)
        list->push_back(it.first);
    list->push_back(fd);
    return list;
}

FontManager::FontManager() {

    if(int error = FT_Init_FreeType(ft_library)){
        throw new runtime_error("Could not initiate FreeType. Error code: " + error);
    }

}