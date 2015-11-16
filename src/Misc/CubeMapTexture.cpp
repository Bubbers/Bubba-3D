#include "CubeMapTexture.h"
#include <glutil/glutil.h>
#include <FreeImage.h>
#include <Texture.h>


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

	//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Sets the type of mipmap interpolation to be used on magnifying and 
	// minifying the active texture. 
	// For cube maps, filtering across faces causes artifacts - so disable filtering
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// In case you want filtering anyway, try this below instead
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Use tri-linear mip map filtering
	//glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);			  // or replace trilinear mipmap filtering with nicest anisotropic filtering


	CHECK_GL_ERROR();
	m_texture = textureID;

}



void CubeMapTexture::loadCubeMapFace(std::string filename, GLenum face)
{
	FIBITMAP *image32Bit = Texture::LoadImageIntoMemory(filename);
	int width, height;
	width = FreeImage_GetWidth(image32Bit);
	height = FreeImage_GetHeight(image32Bit);
	GLubyte *textureData = FreeImage_GetBits(image32Bit);
	glTexImage2D(face, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
}

void CubeMapTexture::bind(GLenum textureUnit) {
	glActiveTexture(textureUnit);
//	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}



CubeMapTexture::~CubeMapTexture()
{
}
