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

GLuint* FontManager::getTex() {
    return getTex(false);
}

GLuint* FontManager::getTex(bool force) {
    if(!force && !initiated)
        throw logic_error("Bad state: FontManager hasn't loaded any fonts yet.");

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

FontManager* FontManager::getInstance() {

    static FontManager instance; // Guaranteed to be destroyed.
    // Instantiated on first use.
    return &instance;

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

    FontDefinition newFont = FontDefinition(fontFace,pixelSize);
    loadedFonts.insert(std::pair<FontDefinition,Font*>(newFont,new Font(pixelSize)));

    unsigned int *width = (unsigned int*)calloc(sizeof(unsigned int),1);
    unsigned int *height = (unsigned int*)calloc(sizeof(unsigned int),1);
    *width = atlasWidth;
    *height = atlasHeight;

    iterateGlyphs(newFont,width,height);

    atlasWidth = *width;
    atlasHeight = *height;

    initTexture();

    drawGlyphs();

    Globals::set(Globals::FONT_TEXTURE_WIDTH,*width);
    Globals::set(Globals::FONT_TEXTURE_HEIGHT,*height);

}

void FontManager::initTexture() {
    GLuint* tex = getTex(true);

    if(initiated)
        glDeleteTextures(1,tex);
    else
        initiated = true;

    glActiveTexture(GL_TEXTURE4);
    glGenTextures(1, tex);
    glBindTexture(GL_TEXTURE_2D, *tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexStorage2D(GL_TEXTURE_2D,1,GL_R8,atlasWidth,atlasHeight);
}

void FontManager::iterateGlyphs(FontDefinition def, unsigned int* width, unsigned int* height) {

    FT_Face face = (FT_Face)malloc(sizeof(FT_Face));

    if(int error = FT_New_Face(*ft_library,def.face.c_str(),0,&face)){
        Logger::logError("Failed loading face '" + def.face + "'. Error code: " + to_string(error));
        return;
    }
    FT_Set_Pixel_Sizes(face,0,def.pixelSize);
    FT_GlyphSlot glyph = face->glyph;

    for(unsigned char c = 32; c < 128; c++){

        if(int error = FT_Load_Char(face,c,FT_LOAD_RENDER)){
            Logger::logError("Failed loading char '" + to_string(c) + "'. Error code: " + to_string(error));
            continue;
        }

        *height = max(*height,glyph->bitmap.rows);
        *width += glyph->bitmap.width;
    }

}

void FontManager::drawGlyphs() {

    FT_Face face;
    int x = 0;
    for(auto fontIt : loadedFonts){

        Font* font = fontIt.second;
        FontDefinition fDef = fontIt.first;
        if(int error = FT_New_Face(*ft_library,fDef.face.c_str(),0,&face)){
            Logger::logError("Failed loading face '" + fDef.face + "'. Error code: " + to_string(error));
            continue;
        }
        FT_Set_Pixel_Sizes(face,0,fDef.pixelSize);
        FT_GlyphSlot glyph = face->glyph;

        for(unsigned char c = 32; c < 128; c++){

            if(int error = FT_Load_Char(face,c,FT_LOAD_RENDER)){
                Logger::logError("Failed loading char '" + to_string(c) + "'. Error code: " + to_string(error));
                continue;
            }

			if (glyph->bitmap.width > 0 && glyph->bitmap.rows > 0) {
				glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, glyph->bitmap.width, glyph->bitmap.rows,
					GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
				CHECK_GL_ERROR();
			}
            
            font->addGlyph(glyph,x,c);
            x += glyph->bitmap.width;

        }

    }


}

FontManager::FontManager() {

    int error;
    ft_library = (FT_Library*)malloc(sizeof(FT_Library));
    if(error = FT_Init_FreeType(ft_library)){
        throw new runtime_error("Could not initiate FreeType. Error code: " + error);
    }

}