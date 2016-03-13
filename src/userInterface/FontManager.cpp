//
// Created by simon on 2016-03-11.
//

#include <string>
#include <vector>
#include <cmath>

#include <FontManager.h>
#include <stdexcept>
#include <Logger.h>
#include <GL/glew.h>
#include <functional>
#include <Font.h>

#include <ft2build.h>
#include <glutil/glutil.h>
#include <Globals.h>
#include FT_FREETYPE_H


using namespace std;

size_t FontManager::FontDefinition::hash() {
    return std::hash<string>()(face) + pixelSize;
}

GLuint* FontManager::getTex() {
    static GLuint h = 0;
    return &h;
}

bool FontManager::FontDefinition::operator== (FontDefinition fd) const {
    return fd.face == face && fd.pixelSize == pixelSize;
}

string FontManager::FontDefinition::getFace() const {
    return face;
}

int FontManager::FontDefinition::getPixelSize() const {
    return pixelSize;
}

FontManager::FontDefinition::FontDefinition(string face, int pixelSize) : face(face), pixelSize(pixelSize){ }

Font* FontManager::loadAndFetchFont(string fontFace, int pixelSize) {
    FontDefinition locate = FontDefinition(fontFace,pixelSize);
    auto it = loadedFonts.find(locate);
    if(it == loadedFonts.end()) {
        loadFont(fontFace, pixelSize);
        return loadedFonts.find(locate)->second;
    }else
        return it->second;
}

void FontManager::loadFont(string fontFace, int pixelSize) {

    loadedFonts = fontMap();

    vector<FontDefinition>* fontDefs = getPreviouslyLoadedDefinitionsAnd(FontDefinition(fontFace,pixelSize));
    unsigned int *width = (unsigned int*)calloc(sizeof(unsigned int),1);
    unsigned int *height = (unsigned int*)calloc(sizeof(unsigned int),1);

    iterateGlyphs(fontDefs,width,height);

    initTexture(*width,*height);

    drawGlyphs(fontDefs);

    Globals::set(Globals::FONT_TEXTURE_WIDTH,*width);
    Globals::set(Globals::FONT_TEXTURE_HEIGHT,*height);

}

void FontManager::initTexture(unsigned int width, unsigned int height) {
    GLuint* tex = getTex();
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, tex);
    glBindTexture(GL_TEXTURE_2D, *tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexStorage2D(GL_TEXTURE_2D,1,GL_R8,width,height);
}

void FontManager::iterateGlyphs(vector<FontDefinition> *defs, unsigned int* width, unsigned int* height) {

    vector<Font::GlyphData>* allGlyphs = new vector<Font::GlyphData>;
    FT_Face face = (FT_Face)malloc(sizeof(FT_Face));
    for(unsigned int i = 0; i< defs->size(); i++){


        if(int error = FT_New_Face(*ft_library,(*defs)[i].face.c_str(),0,&face)){
            Logger::logError("Failed loading face '" + (*defs)[i].face + "'. Error code: " + to_string(error));
            continue;
        }
        FT_Set_Pixel_Sizes(face,0,(*defs)[i].pixelSize);
        FT_GlyphSlot glyph = face->glyph;

        for(unsigned char c = 32; c < 128; c++){

            if(int error = FT_Load_Char(face,c,FT_LOAD_RENDER)){
                Logger::logError("Failed loading char '" + to_string(c) + "'. Error code: " + to_string(error));
                continue;
            }

            allGlyphs->push_back(Font::GlyphData(*width,glyph));
            *height = max(*height,glyph->bitmap.rows);
            *width += glyph->bitmap.width;

        }

    }


}

void FontManager::drawGlyphs(vector<FontDefinition> *defs) {

    FT_Face face;
    int x = 0;
    for(unsigned int i = 0; i< defs->size(); i++){

        if(int error = FT_New_Face(*ft_library,(*defs)[i].face.c_str(),0,&face)){
            Logger::logError("Failed loading face '" + (*defs)[i].face + "'. Error code: " + to_string(error));
            continue;
        }
        FT_Set_Pixel_Sizes(face,0,(*defs)[i].pixelSize);
        FT_GlyphSlot glyph = face->glyph;
        Font* font = new Font((*defs)[i].pixelSize);

        for(unsigned char c = 32; c < 128; c++){

            if(int error = FT_Load_Char(face,c,FT_LOAD_RENDER)){
                Logger::logError("Failed loading char '" + to_string(c) + "'. Error code: " + to_string(error));
                continue;
            }

            glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, glyph->bitmap.width, glyph->bitmap.rows,
                            GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
            CHECK_GL_ERROR();
            font->addGlyph(glyph,x,c);
            x += glyph->bitmap.width;

        }

        loadedFonts.insert(std::pair<FontDefinition,Font*>((*defs)[i], font));

    }


}

vector<FontManager::FontDefinition>* FontManager::getPreviouslyLoadedDefinitionsAnd(FontDefinition fd) {
    vector<FontManager::FontDefinition>* list = new vector<FontManager::FontDefinition>();
    for(auto it : loadedFonts)
        list->push_back(it.first);
    list->push_back(fd);
    return list;
}

FontManager::FontManager() {

    int error;
    ft_library = (FT_Library*)malloc(sizeof(FT_Library));
    if(error = FT_Init_FreeType(ft_library)){
        throw new runtime_error("Could not initiate FreeType. Error code: " + error);
    }

}