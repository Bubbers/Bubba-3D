#include <il.h>
#include <glutil/glutil.h>
#include <Logger.h>
#include <ilu.h>
#include "Texture.h"


void Texture::bind(GLenum textureUnit){
    glActiveTexture(textureUnit);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
}


void Texture::loadTexture(std::string fileName)
{
    Logger::logInfo("Loading texture: " + fileName);
    ILuint image = ilGenImage();
    ilBindImage(image);
    CHECK_GL_ERROR();

    if (ilLoadImage(fileName.c_str()) == IL_FALSE)
    {
        Logger::logSevere("Error to load texture " + fileName);
        ILenum Error;
        while ((Error = ilGetError()) != IL_NO_ERROR)
        {
            printf("  %d: %s\n", Error, iluErrorString(Error));
        }
        ilDeleteImage(image);
    }
    CHECK_GL_ERROR();
    // why not?
    if (ilTypeFromExt(fileName.c_str()) == IL_PNG || ilTypeFromExt(fileName.c_str()) == IL_JPG)
    {
        iluFlipImage();
    }
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    GLuint texid;
    glGenTextures(1, &texid);
    glActiveTexture(GL_TEXTURE0);
    CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texid);
    CHECK_GL_ERROR();
    int width = ilGetInteger(IL_IMAGE_WIDTH);
    int height = ilGetInteger(IL_IMAGE_HEIGHT);
    // Note: now with SRGB
    ILubyte* b = ilGetData();
    ILubyte c = *b;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, b);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
    CHECK_GL_ERROR();
    ilDeleteImage(image);
    CHECK_GL_ERROR();
    //GLuint texid = ilutGLLoadImage(const_cast<char *>(fileName.c_str()));
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
}
