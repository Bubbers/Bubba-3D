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
#include <StdOutLogHandler.h>
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"


void Texture::bind(GLenum textureUnit){
    glActiveTexture(textureUnit);
    CHECK_GL_ERROR()
    glBindTexture(GL_TEXTURE_2D, textureID);
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
