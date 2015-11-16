#include <glutil/glutil.h>
#include <Logger.h>
#include "Texture.h"
#include "FreeImage.h"

void Texture::bind(GLenum textureUnit){
    glActiveTexture(textureUnit);
//    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::loadTexture(std::string fileName)
{
    int width, height;
    FIBITMAP *image32Bit = Texture::LoadImageIntoMemory(fileName);
    width = FreeImage_GetWidth(image32Bit);
    height = FreeImage_GetHeight(image32Bit);
    GLubyte *textureData = FreeImage_GetBits(image32Bit);


    GLuint texid;
    glGenTextures(1, &texid);
    glActiveTexture(GL_TEXTURE0);
    CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texid);
    CHECK_GL_ERROR();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA_EXT, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, textureData);
    CHECK_GL_ERROR();
    glGenerateMipmap(GL_TEXTURE_2D);
    CHECK_GL_ERROR();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
    CHECK_GL_ERROR();

    glBindTexture(GL_TEXTURE_2D, 0);
    CHECK_GL_ERROR();
    textureID = texid;

    FreeImage_Unload(image32Bit);
}

FIBITMAP* Texture::LoadImageIntoMemory(std::string fileName) {
    Logger::logInfo("Loading texture: " + fileName);


    FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType(fileName.c_str(), 0 );
    if(imageFormat == -1) {
        Logger::logSevere("Couldn't find texture file " + fileName);
        exit(0);
    }

    if (imageFormat == FIF_UNKNOWN)
    {
        imageFormat = FreeImage_GetFIFFromFilename(fileName.c_str());

        if ( !FreeImage_FIFSupportsReading(imageFormat) )
        {
            Logger::logSevere("Couldn't load texture format " + fileName);
            exit(-1);
        }
    }

    FIBITMAP *image = FreeImage_Load(imageFormat, fileName.c_str());

    if(imageFormat == FIF_JPEG || imageFormat == FIF_PNG) {
        FreeImage_FlipVertical(image);
        //FreeImage_FlipHorizontal(image);
    }

    FIBITMAP *image32Bit;
    if(FreeImage_GetBPP(image) == 32) {
        image32Bit = image;
    } else {
        image32Bit = FreeImage_ConvertTo32Bits(image);
        FreeImage_Unload(image);
    }

    return image32Bit;
}
