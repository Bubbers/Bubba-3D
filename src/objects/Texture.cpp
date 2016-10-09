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
#include "glutil/glutil.h"
#include <Logger.h>
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

void Texture::bind(GLenum textureUnit){
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::loadTexture(std::string fileName)
{
    stbi_set_flip_vertically_on_load(true);

    int comp;
    unsigned char* image = stbi_load(fileName.c_str(), &width, &height, &comp, 0);

    if(image == nullptr) {
        Logger::logError("Couldnt load image "+ fileName);
    }

    GLuint texid;
    glGenTextures(1, &texid);
    glActiveTexture(GL_TEXTURE0);
    CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texid);
    CHECK_GL_ERROR();


    GLenum format = comp == 3 ? GL_RGB : GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA_EXT,
                 width, height, 0, format, GL_UNSIGNED_BYTE, image);

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

    stbi_image_free(image);
    printf("Loaded texture: %s\n", fileName.c_str()); // TODO Use logger instead
}

int Texture::getHeight() {
    return height;
}

int Texture::getWidth() {
    return width;
}

GLuint Texture::getID() {
    return textureID;
};
