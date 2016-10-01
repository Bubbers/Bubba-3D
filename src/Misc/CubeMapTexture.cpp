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
#include "CubeMapTexture.h"
#include "glutil/glutil.h"
#include <Texture.h>
#include <stb_image.h>
#include <Logger.h>


CubeMapTexture::CubeMapTexture(const string& posXFilename, const string& negXFilename, const string& posYFilename, const string& negYFilename, const string& posZFilename, const string& negZFilename) {

	//************************************************
	//	Creating a texture ID for the OpenGL texture
	//************************************************
	GLuint textureID;
	glGenTextures(1, &textureID);

	//************************************************
	//	 Load the faces into the cube map texture
	//************************************************
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	loadCubeMapFace(posXFilename, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	loadCubeMapFace(negXFilename, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	loadCubeMapFace(posYFilename, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	loadCubeMapFace(negYFilename, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	loadCubeMapFace(posZFilename, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	loadCubeMapFace(negZFilename, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	//************************************************
	//			Set filtering parameters
	//************************************************

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Sets the type of mipmap interpolation to be used on magnifying and 
	// minifying the active texture. 
	// For cube maps, filtering across faces causes artifacts - so disable filtering
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);

	// In case you want filtering anyway, try this below instead
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Use tri-linear mip map filtering
	//glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);			  // or replace trilinear mipmap filtering with nicest anisotropic filtering


	CHECK_GL_ERROR();
	m_texture = textureID;

}



void CubeMapTexture::loadCubeMapFace(std::string filename, GLenum face)
{
	// TODO(Bubbad) LET TEXTURE HANDLE ALL THIS
	stbi_set_flip_vertically_on_load(true);

    int width, height;
    int comp;
    unsigned char* image = stbi_load(filename.c_str(), &width, &height, &comp, 0);

    if(image == nullptr) {
        Logger::logError("Couldnt load image "+ filename);
    }

	GLenum format = comp == 3 ? GL_RGB : GL_RGBA;

	glTexImage2D(face, 0, GL_SRGB_ALPHA_EXT, width, height, 0, format, GL_UNSIGNED_BYTE, image);

	stbi_image_free(image);
}

void CubeMapTexture::bind(GLenum textureUnit) {
	glActiveTexture(textureUnit);
//	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}



CubeMapTexture::~CubeMapTexture()
{
}
